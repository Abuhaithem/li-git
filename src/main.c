#include <stdio.h>
#include <string.h>

#include "repo.h"

/* Entry point placeholder: will dispatch subcommands as they are implemented. */
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
