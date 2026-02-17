#ifndef LGIT_HASH_H
#define LGIT_HASH_H

/* Hash utilities: wraps SHA-1/SHA-256 calculation and hex helpers. */

/* Compute digest over buffer. Caller provides output buffer sized for the chosen algorithm. */
int lgit_hash_buffer(const void *buf, size_t len, unsigned char *out, size_t out_len);

/* Convert binary digest to lowercase hex string. dst must be at least 2*out_len+1 bytes. */
int lgit_hash_hex(const unsigned char *digest, size_t digest_len, char *dst, size_t dst_len);

#endif /* LGIT_HASH_H */
