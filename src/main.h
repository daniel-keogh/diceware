#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define OPTS            "d:l:f:vh"
#define DEF_DELIMITER   ' '
#define DEF_FILENAME    NULL
#define DEF_LEN         6

typedef struct {
    char delimiter;
    char* filename;
    int length;
} options_t;

void parseOpts(int argc, char* argv[], options_t* options);
void outputPassphrase(const word_t* passphrase, options_t options);
void usage(const char* exec);

#endif