#ifndef __ARGPARSER_H__
#define __ARGPARSER_H__


#define ll long long
#define bool char
#define ull unsigned long long

typedef struct {
    bool verbose;

} options;


options parse_options(int argc, char** argv);

void show_usage();



#endif
