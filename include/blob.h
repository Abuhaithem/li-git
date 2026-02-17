#ifndef LGIT_BLOB_H
#define LGIT_BLOB_H

/* Blob handling: serialize file contents into blob objects and parse blob payloads. */

/* Create a blob object from a file on disk, write it, and return hex id. */
int lgit_blob_write_file(const char *repo_path, const char *path, char *hex_out, size_t hex_out_len);

#endif /* LGIT_BLOB_H */
