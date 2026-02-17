#include "repo.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define LGIT_PATH_MAX 4096

/*
 * ensure_dir
 * Creates a directory with the requested mode. Treats an existing directory
 * as success to keep init idempotent. Returns 0 on success, -1 on failure.
 */
static int ensure_dir(const char *path, mode_t mode) {
    if (mkdir(path, mode) == 0) {
        return 0;
    }
    if (errno == EEXIST) {
        return 0;
    }
    return -1;
}

/*
 * join_path
 * Safe path join helper using snprintf. Returns 0 on success, -1 on truncation
 * or formatting error. Callers pass in fixed-size buffers to avoid heap use.
 */
static int join_path(char *dst, size_t dst_len, const char *base, const char *suffix) {
    int n = snprintf(dst, dst_len, "%s/%s", base, suffix);
    if (n < 0 || (size_t)n >= dst_len) {
        return -1;
    }
    return 0;
}

/*
 * repo_init
 * Create the minimal repository layout under `path` (or "." when NULL):
 *   .lgit/
 *     objects/
 *     refs/heads/
 *     HEAD -> ref: refs/heads/master
 * Uses defensive path joins and fails fast on any error. Returns 0 on success,
 * -1 on failure without partially written HEAD when possible.
 */
int repo_init(const char *path) {
    char lgit[LGIT_PATH_MAX];
    char objects[LGIT_PATH_MAX];
    char refs[LGIT_PATH_MAX];
    char heads[LGIT_PATH_MAX];
    char head_file[LGIT_PATH_MAX];

    const char *root = path ? path : ".";

    int n = snprintf(lgit, sizeof lgit, "%s/.lgit", root);

    if (n < 0 || (size_t)n >= sizeof lgit) {
        return -1;
    }

    if (ensure_dir(lgit, 0755) < 0) {
        return -1;
    }
    if (join_path(objects, sizeof objects, lgit, "objects") < 0 || ensure_dir(objects, 0755) < 0) {
        return -1;
    }
    if (join_path(refs, sizeof refs, lgit, "refs") < 0 || ensure_dir(refs, 0755) < 0) {
        return -1;
    }
    if (join_path(heads, sizeof heads, refs, "heads") < 0 || ensure_dir(heads, 0755) < 0) {
        return -1;
    }

    if (join_path(head_file, sizeof head_file, lgit, "HEAD") < 0) {
        return -1;
    }

    FILE *f = fopen(head_file, "w");
    if (!f) {
        return -1;
    }
    if (fputs("ref: refs/heads/master\n", f) < 0) {
        fclose(f);
        return -1;
    }
    fclose(f);

    return 0;
}
