#ifndef LGIT_TREE_H
#define LGIT_TREE_H

/* Tree handling: build directory listings into tree objects and pretty-print them. */

/* Walk working directory (excluding .lgit), emit a tree object, and return root hash. */
int lgit_write_tree(const char *repo_path, const char *workdir, char *hex_out, size_t hex_out_len);

#endif /* LGIT_TREE_H */
