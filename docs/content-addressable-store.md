# Building a Git-Style Content-Addressable Store in C

This guide outlines how to recreate the core of early Git (content-addressable storage) in C. It covers object types, serialization formats, suggested code structure, and setup steps.

## Object Model
- **Blob**: Raw file data. Stored as-is (after a header) and hashed.
- **Tree**: Directory listing. Each entry maps a name to either a Blob or another Tree, along with its UNIX mode (e.g., `100644` for files, `40000` for directories).
- **Commit**: Snapshot metadata. Points to a single root Tree, references zero or more parent commits, and records author/committer info, timestamp, and message.

## Storage Layout
- Create a repository directory (e.g., `.lgit/`).
- Store objects under `.lgit/objects/XX/YY...` where `XX` is the first two hex chars of the object ID and `YY...` is the remainder (mirrors Git). This keeps directories shallow.
- Object IDs are cryptographic hashes (SHA-1 in classic Git; SHA-256 is also fine). Hash = `SHA1(header + content)`.

## Serialization Formats (Git-Compatible)
- **Blob**: `"blob <size>\0<bytes>"`
- **Tree entry**: `<mode> <name>\0<20-byte raw hash>`; a Tree object is the concatenation of entries, wrapped with a header: `"tree <len>\0<entries>"`.
- **Commit** (text):
  ```
  tree <tree-hash>
  parent <parent-hash>    # repeat for merges
  author Name <email> <unix-seconds> <tz>
  committer Name <email> <unix-seconds> <tz>
  
  <commit message>
  ```
  The full payload is prefixed with `"commit <size>\0"` before hashing.

## Minimal Command Set to Build
1. `init`: create `.lgit/objects/` and `.lgit/refs/` (empty is fine initially), plus `HEAD` pointing to `refs/heads/master`.
2. `hash-object -w <file>`: read a file, build a blob header, hash, then write the object file (optionally zlib-compress before writing, like Git).
3. `cat-file -p <hash>`: read and print a blob (raw), tree (as a listing), or commit (as text).
4. `write-tree`: walk the working directory recursively, build Tree objects from the filesystem, and return the root tree hash.
5. `commit-tree <tree-hash> [-p <parent>] -m <msg>`: build a commit object referencing the tree (and optional parent), write it, and print its hash.

## Suggested Code Structure
```
project/
  src/
    main.c              # CLI argument dispatch
    repo.c/.h           # init repo paths, ensure directories
    hash.c/.h           # SHA-1 or SHA-256 wrapper
    object_store.c/.h   # write/read objects to .lgit/objects
    blob.c/.h           # serialize/parse blobs
    tree.c/.h           # build tree entries from filesystem
    commit.c/.h         # format commit text, parse fields
    util.c/.h           # small helpers (path join, I/O helpers)
  include/
    ... matching headers ...
  Makefile
```
Notes:
- Keep headers small; expose only what each module needs.
- Use `size_t` for lengths, and validate all I/O return values.
- For hashing, you can either: (a) use OpenSSL/LibreSSL `SHA1()` or `SHA256()`, or (b) embed a tiny public-domain SHA-1 implementation.

## Building
Example `Makefile` skeleton (link with `-lcrypto` if you use OpenSSL):
```
CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -pedantic -O2
LDFLAGS = -lcrypto      # drop if you use a bundled SHA-1
SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:.c=.o)

all: lgit

lgit: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) lgit
```

## Key Routines (Pseudocode)
- **Write object (blob/tree/commit)**:
  1. Build payload bytes (already includes headers for tree/commit; for blobs, prepend `"blob <size>\0"`).
  2. `hash = SHA1(payload)`.
  3. Path = `.lgit/objects/` + `hash[0:2]` + `/` + `hash[2:]`.
  4. `mkdir -p` for the directory, then write the payload (optionally zlib-compress; you can skip compression initially for simplicity).
- **Read object**:
  1. Open by hash path.
  2. (If compressed) decompress.
  3. Split at the first `\0` to separate header from body. Header format is `<type> <size>`.

## Implementation Order
1. Repo init (`init`).
2. Hash and write blobs (`hash-object -w`, `cat-file -p`).
3. Build trees from the working directory (`write-tree`).
4. Emit commits (`commit-tree`).
5. Add ref handling (`HEAD`, `refs/heads/master`) and a simple `update-ref` to move branch tips.

## Testing Ideas
- Round-trip a file: `hash-object -w file`, then `cat-file -p <hash>` and compare.
- Build a tree from a tiny directory; inspect with `cat-file -p <tree-hash>`.
- Create a commit and ensure the recorded tree hash matches `write-tree` output.
- Compare your object file sizes against `git hash-object -w` output for the same content to validate format compatibility.

## Extra Notes on the Three Object Types
- Blob: No metadata besides the size in the header; content is opaque.
- Tree: Sorted lexicographically by filename; directories use mode `40000`. The 20-byte hash stored per entry is the raw binary hash, not hex.
- Commit: The canonical text format must end with a newline after the message. Multiple parents are allowed (merges). Timestamps are UNIX seconds with `+/-HHMM` timezone offsets.

With these pieces you can produce a functional, minimal, git-compatible content-addressable store in C. Start without compression or refs if you prefer; once core read/write paths work, add zlib and refs to match Git behavior more closely.
