#ifndef LGIT_HASH_H
#define LGIT_HASH_H

#include <stddef.h>

/* Hash utilities: wraps SHA-1 calculation and hex helpers. */

#define LGIT_SHA1_LEN 20            /* raw bytes */
#define LGIT_SHA1_HEX (LGIT_SHA1_LEN * 2 + 1) /* hex string + NUL */

/* Compute SHA-1 digest over buffer. Caller provides at least LGIT_SHA1_LEN bytes in out. */
int lgit_hash_buffer(const void *buf, size_t len, unsigned char *out, size_t out_len);

/* Convert binary digest to lowercase hex string. dst must be >= digest_len*2 + 1 bytes. */
int lgit_hash_hex(const unsigned char *digest, size_t digest_len, char *dst, size_t dst_len);

/* Development self-test: verifies SHA-1("abc"). Returns 0 on success. */
int lgit_hash_selftest(void);

#endif /* LGIT_HASH_H */
