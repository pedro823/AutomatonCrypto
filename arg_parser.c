#include "arg_parser.h"
#include "error_handler.h"
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

/* Auxiliar function for string equality */
bool equal(char* first, char* second) {
    return strcmp(first, second) == 0;
}

options parse_options(int argc, char** argv) {
    options new_options = {5, NULL, 3, 30, false};
    static struct option
}

void show_usage() {
    char* text = "AutomatonCrypto - The automaton-based hasher for files.\n"
                 "A project made by Pedro Pereira (github.com/pedro823).\n"
                 "Purpose: use this program to calculate integrity hashes for files.\n"
                 "NOT PRODUCTION READY. DO NOT USE THIS IN A LIVE ENVIRONMENT.\n"
                 "\n"
                 "Usage: ./automaton -f FILE [-r RULE] [-i INFLUENCE] [-d DEBUG_LEVEL]\n"
                 "";
    printf("%s\n", text);
}
