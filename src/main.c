#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

#include "diceware.h"
#include "main.h"

bool verbose = false;

int main(int argc, char* argv[])
{
    options_t options = {
        .delimiter = DEF_DELIMITER,
        .filename = DEF_FILENAME,
        .length = DEF_LEN
    };

    srand(time(NULL));

    parseOpts(argc, argv, &options);

    word_t* dict = readWordList(options.filename);
    word_t* passphrase = generatePhrase(dict, options.length);

    outputPassphrase(passphrase, options);
}

void parseOpts(int argc, char* argv[], options_t* options)
{
    static struct option cli_options[] = {
        {"help", no_argument, 0, 'h'},
        {"verbose", no_argument, 0, 'v'},
        {"file", required_argument, 0, 'f'},
        {"length", required_argument, 0, 'l'},
        {"delimiter", required_argument, 0, 'd'},
        {0, 0, 0, 0}
    };

    int opt;
    int optIndex = 0;

    while ((opt = getopt_long(argc, argv, OPTS, cli_options, &optIndex)) != -1) {
        switch (opt) {
            case 'd':
                if (strlen(optarg) > 1) {
                    fprintf(stderr, "%s", "[Error] Provide only a single delimitation character.\n");
                    exit(EXIT_FAILURE);
                }
                if (!isprint(optarg[0])) {
                    fprintf(stderr, "%s", "[Error] Invalid delimiter.\n");
                    exit(EXIT_FAILURE);
                }
                options->delimiter = optarg[0];
                break;
            case 'l':
                options->length = atoi(optarg);
                if (options->length > DICT_LEN || options->length <= 0) {
                    fprintf(stderr, "[Error] Length must be an number between 1 and %d.\n", DICT_LEN);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'f':
                if (access(optarg, R_OK) != -1) {
                    options->filename = malloc(sizeof(char) * (strlen(optarg) + 1));
                    strcpy(options->filename, optarg);
                }
                else {
                    fprintf(stderr, "[Error] File: \"%s\" not found.\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
                usage(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                exit(EXIT_FAILURE);
        }
    }
}

void outputPassphrase(const word_t* passphrase, const options_t options)
{
    if (verbose) {
        printf("\n%s", "Your passphrase is: ");
    }

    for (int i = 0; i < options.length - 1; i++) {
        printf("%s%c", passphrase[i].word, options.delimiter);
    }
    printf("%s\n", passphrase[options.length - 1].word);
}

void usage(const char* exec)
{
    printf("Usage: %s [options]\n\n", exec);
    printf("%s", "Options:\n");
    printf("%s", "  -h, --help\t\t\tPrints some help text.\n");
    printf("%s", "  -d, --delimiter <delimiter>\tCharacter to separate the words with.\n");
    printf("%s", "  -l, --length <length>\t\tThe length of the passphrase.\n");
    printf("%s", "  -f, --file <file>\t\tPath to the input file.\n");
    printf("%s", "  -v, --verbose\t\t\tPrints additional information.\n");
}
