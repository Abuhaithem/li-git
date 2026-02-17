#ifndef LGIT_REPO_H
#define LGIT_REPO_H

/* Repository management: create and validate the internal .lgit layout. */

/*
 * Initialize repository structure under the given path (.lgit, objects, refs).
 * `path` may be NULL to default to the current working directory. Creates:
 *   .lgit/objects
 *   .lgit/refs/heads
 *   .lgit/HEAD -> ref: refs/heads/master
 * Returns 0 on success, -1 on failure (no partial success guarantees beyond
 * best-effort directory creation).
 */
int repo_init(const char *path);

#endif /* LGIT_REPO_H */
