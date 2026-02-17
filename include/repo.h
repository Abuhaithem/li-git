#ifndef LGIT_REPO_H
#define LGIT_REPO_H

/* Repository management: create and validate the internal .lgit layout. */

/* Initialize repository structure under the given path (.lgit, objects, refs). */
int repo_init(const char *path);

#endif /* LGIT_REPO_H */
