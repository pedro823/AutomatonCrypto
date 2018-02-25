#ifndef __ARGPARSER_H__
#define __ARGPARSER_H__


#define ll long long
#define bool char
#define ull unsigned long long
#define false 0
#define true 1

typedef struct {
    /* Sets debug priority */
    int debug_priority;
    /* Sets input file */
    char* file_name;
    /* Sets how many bytes will be looked forward */
    int influence;
    /* Sets which rule will be used */
    bool rule;
    /**/
    bool do_not_continue;
} options;

/* Parses argv and returns user options */
options parse_options(int argc, char** argv);

/* Show the program's usage */
void show_usage();

#endif
