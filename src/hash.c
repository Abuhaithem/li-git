#include "hash.h"

#include <openssl/sha.h>
#include <stddef.h>
#include <string.h>

/* Compute SHA-1 digest over an arbitrary buffer. */
int lgit_hash_buffer(const void *buf, size_t len, unsigned char *out, size_t out_len) {
    if (!out || out_len < LGIT_SHA1_LEN) {
        return -1;
    }
    if (!buf && len != 0) {
        return -1;
    }
    const unsigned char *data = (const unsigned char *)buf;
    unsigned char *res = SHA1(data, len, out);
    return res ? 0 : -1;
}

/* Convert binary digest to lowercase hex string. */
int lgit_hash_hex(const unsigned char *digest, size_t digest_len, char *dst, size_t dst_len) {
    static const char hex_digits[] = "0123456789abcdef";
    if (!digest || !dst) {
        return -1;
    }
    if (dst_len < digest_len * 2 + 1) {
        return -1;
    }
    for (size_t i = 0; i < digest_len; ++i) {
        dst[2 * i]     = hex_digits[(digest[i] >> 4) & 0xF];
        dst[2 * i + 1] = hex_digits[digest[i] & 0xF];
    }
    dst[digest_len * 2] = '\0';
    return 0;
}

/* Simple self-test using the well-known SHA-1 of "abc". */
int lgit_hash_selftest(void) {
    static const char *msg = "abc";
    static const char *expected = "a9993e364706816aba3e25717850c26c9cd0d89d";
    unsigned char digest[LGIT_SHA1_LEN];
    char hex[LGIT_SHA1_HEX];

    if (lgit_hash_buffer(msg, 3, digest, sizeof digest) < 0) {
        return -1;
    }
    if (lgit_hash_hex(digest, LGIT_SHA1_LEN, hex, sizeof hex) < 0) {
        return -1;
    }
    return (strcmp(hex, expected) == 0) ? 0 : -1;
}
