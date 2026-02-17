#include <stdio.h>
#include <string.h>

#include "repo.h"

/*
 * CLI entry point. Currently supports only `init`, delegating to repo_init.
 * Future subcommands (hash-object, cat-file, write-tree, commit-tree, etc.)
 * will extend this dispatcher. Returns non-zero on errors for shell scripting
 * friendliness.
 */
int main(int argc, char **argv) {
    if (argc >= 2 && strcmp(argv[1], "init") == 0) {
        if (repo_init(".") == 0) {
            puts("Initialized empty lgit repository in .lgit");
            return 0;
        }
        fputs("lgit: init failed\n", stderr);
        return 1;
    }

    puts("usage: lgit <command> [options]\n\ncommands:\n  init    initialize repository");
    return 0;
}
