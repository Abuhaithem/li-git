#ifndef LGIT_COMMIT_H
#define LGIT_COMMIT_H

/* Commit handling: format commit objects, including tree, parent(s), author/committer, and message. */

/* Create a commit object pointing to tree and optional parent; message must end with newline. */
int lgit_commit_create(const char *repo_path,
					  const char *tree_hex,
					  const char *parent_hex,
					  const char *author,
					  const char *committer,
					  const char *message,
					  char *hex_out,
					  size_t hex_out_len);

#endif /* LGIT_COMMIT_H */
