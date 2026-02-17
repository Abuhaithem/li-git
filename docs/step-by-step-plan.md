# Step-by-Step Plan (Minimal, Observable Tasks)

Goal: build a tiny Git-style content-addressable store in C and push it to GitHub, learning each piece in isolation. Keep commits small and frequent.

## 0) Prereqs
- Install toolchain: `cc`/`gcc`, `make`, `git`, `pkg-config`, `zlib1g-dev` (if compressing) and `libssl-dev` (if using OpenSSL SHA-1/SHA-256).
- Create a new GitHub repo (empty) to push later.

## 1) Local repo setup
- Make project dir: `mkdir li-git && cd li-git`.
- `git init` (or `git init -b main`).
- Create folders: `mkdir -p src include docs`. Add a skeletal `README.md`.
- Commit: "chore: scaffold repo".

## 2) Build system + headers
- Add `Makefile` with `CC`, `CFLAGS`, `SRC`, and target `lgit`.
- Add placeholder headers in `include/` for: `repo.h`, `hash.h`, `object_store.h`, `blob.h`, `tree.h`, `commit.h`, `util.h`.
- Add empty `main.c` that prints usage.
- `make` should build (even if program does little).
- Commit: "build: add makefile and headers".

## 3) Repo initialization logic
- Implement `repo.c` to create `.lgit/objects`, `.lgit/refs/heads`, and `HEAD` pointing to `refs/heads/master`.
- Wire `init` subcommand in `main.c` to call repo init.
- Manual test: run `./lgit init`; inspect `.lgit/` layout.
- Commit: "feat: init repository layout".

## 4) Hashing utility
- Implement `hash.c` to wrap SHA-1 (or SHA-256). Expose `compute_hash(const void *buf, size_t len, unsigned char out[20])` (or 32 for SHA-256) and hex helpers.
- Add a tiny self-test in code or a dev-only function.
- Commit: "feat: add sha hashing helper".

## 5) Object write/read (core storage)
- Implement `object_store.c`: functions to write raw object payloads to `.lgit/objects/aa/bb...` and read them back. Payload already includes header (`type size\0`).
- Support optional zlib compression later; start uncompressed for clarity.
- Add `cat-file -p <hash>` plumbing command to dump payload for manual inspection.
- Manual test: write a dummy buffer, read it back.
- Commit: "feat: add object store and cat-file".

## 6) Blob support
- Implement `blob.c`: given a file path, read bytes, build header `"blob <size>\0" + data`, hash, and write via object store; return hash.
- Add CLI `hash-object -w <file>` to write and print hash.
- Manual test: compare with `git hash-object -w <file>` and `git cat-file -p <hash>` for the same file.
- Commit: "feat: support blob write and cat".

## 7) Tree support
- Implement tree serialization: entries as `<mode> <name>\0<20-byte-hash>` concatenated; header `"tree <len>\0" + entries`.
- Implement `write-tree` command to walk working dir (skip `.lgit`), create blobs for files, trees for dirs, and return root tree hash. Ensure entries sorted lexicographically.
- Add a pretty-printer for trees in `cat-file -p`.
- Manual test: small dir with a couple files and subdirs; inspect with `cat-file -p`.
- Commit: "feat: add trees and write-tree".

## 8) Commit support
- Implement commit formatting:
  - Text body with `tree <hash>`, optional `parent <hash>`, `author`/`committer` lines, blank line, then message ending with newline.
  - Header: `"commit <size>\0" + text` before hashing.
- Add `commit-tree <tree> [-p <parent>] -m <msg>` command.
- Manual test: create a tree, commit it, then `cat-file -p <commit>`.
- Commit: "feat: add commits and commit-tree".

## 9) Refs and HEAD plumbing
- Implement reading/writing refs under `.lgit/refs/heads/<branch>` and a simple `update-ref <ref> <hash>`.
- Make `HEAD` symbolic to `refs/heads/master` (default); allow detached when `HEAD` stores a hash.
- Add a minimal `commit` porcelain: calls `write-tree`, picks parent from `HEAD`, writes commit, updates current branch ref.
- Commit: "feat: basic refs and commit command".

## 10) Polish and verification
- Add `cat-file -t` (type) and `cat-file -s` (size) helpers.
- Add input validation and error messages.
- Add a few unit-ish tests (even shell scripts) for blob round-trip, tree determinism (sorted entries), commit formatting.
- Document usage in `README.md` and link to design doc.
- Commit: "docs: usage and design notes" and "test: add basic scripts".

## 11) Compare with Git (optional but educational)
- For a sample directory, run both your tool and `git` to hash the same files and trees. Verify hashes match when using SHA-1 and zlib enabled. If skipping zlib, contents differ on disk but your header/payload should match pre-compression.

## 12) GitHub push
- `git status` to ensure clean.
- `git remote add origin <your-repo-url>`.
- `git push -u origin main`.

## Ongoing workflow tips
- Keep each task minimal and observable; test manually after every step.
- Use small commits with clear messages; push when a slice is complete.
- If stuck, diff against `git cat-file -p` outputs from real Git for the same inputs.

## Quick command reference (you will add these)
- `./lgit init`
- `./lgit hash-object -w <file>`
- `./lgit cat-file -p <hash>`
- `./lgit write-tree`
- `./lgit commit-tree <tree> [-p <parent>] -m "msg"`
- `./lgit commit -m "msg"` (porcelain once you add refs)
- `./lgit update-ref <ref> <hash>`
