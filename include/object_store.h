#ifndef LGIT_OBJECT_STORE_H
#define LGIT_OBJECT_STORE_H

/* Object storage: persist and retrieve raw git-style objects under .lgit/objects. */

/* Write full object payload (already includes header). Returns 0 on success. */
int lgit_object_write(const char *repo_path, const void *data, size_t len, char *hex_out, size_t hex_out_len);

/* Read object by hex id into buffer; caller owns buffer sizing. Returns bytes read or -1. */
ssize_t lgit_object_read(const char *repo_path, const char *hex_id, void *buf, size_t buf_len);

#endif /* LGIT_OBJECT_STORE_H */
