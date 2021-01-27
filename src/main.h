#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define OPTS            "d:l:f:vh"
#define DEF_DELIMITER   ' '
#define DEF_FILENAME    NULL
#define DEF_LEN         6
#define DEF_VERBOSITY   false

typedef struct {
    char delimiter;
    char* filename;
    int length;
    bool verbose;
} options_t;

void parseOpts(int argc, char* argv[], options_t* options);
void outputPassphrase(const word_t* passphrase, options_t options);
void outputHelp(const char*);

#endif