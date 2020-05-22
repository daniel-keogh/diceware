#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

#include "diceware.h"

#define OPTS "d:l:f:vh"

int main(int argc, char* argv[])
{
    char* filename = DEF_FILE_NAME;
    int phraseLen = DEF_LEN;
    char delimiter = DEF_DELIMITER;
    bool verbose = false;

    srand(time(NULL));

    int opt;
    while ((opt = getopt(argc, argv, OPTS)) != -1) {
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
                delimiter = optarg[0];
                break;
            case 'l':
                phraseLen = atoi(optarg);
                if (phraseLen > DICT_LEN || phraseLen <= 0) {
                    fprintf(stderr, "[Error] Length must be an number between 0 and %d.\n", DICT_LEN);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'f':
                if (access(optarg, R_OK) != -1) {
                    filename = (char*) malloc(sizeof(char) * (strlen(optarg) + 1));
                    strcpy(filename, optarg);
                } else {
                    fprintf(stderr, "[Error] File: \"%s\" not found.\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
                outputHelp(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                exit(EXIT_FAILURE);
        }
    }

    word_t* dict = readWordList(filename);
    word_t* passphrase = generatePhrase(dict, phraseLen, verbose);
    
    if (verbose) {
        printf("\n%s", "Your passphrase is: ");
    }
    outputPassphrase(passphrase, delimiter, phraseLen);
}

word_t* readWordList(const char* filename)
{
    FILE* pWordList;
    char word[MAX_WORD_LEN];
    bool readerr = false;
    word_t* dict = (word_t*) malloc(sizeof(word_t) * DICT_LEN);

    if ((pWordList = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "%s", "[Error] Failed to open the wordlist file.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; !feof(pWordList); i++) {
        if (fscanf(pWordList, "%5d %128s", &dict[i].key, word) == NUM_COLS) {
            if (dict[i].key > MAX_ROLL || dict[i].key < MIN_ROLL) {
                readerr = true;
                break;
            }

            dict[i].word = (char*) malloc(sizeof(char) * (strlen(word) + 1));
            strcpy(dict[i].word, word);
        } else {
            readerr = true;
            break;
        }
    }
    fclose(pWordList);

    if (readerr) {
        fprintf(stderr, "%s", "[Error] The wordlist file contains invalid entries.\n");
        exit(EXIT_FAILURE);
    }

    return dict;
}

word_t* generatePhrase(const word_t* dict, const int length, const bool verbose)
{
    word_t* passphrase = (word_t*) malloc(sizeof(word_t) * length);

    for (size_t i = 0; i < length; i++) {
        int roll = rollDice();
        size_t word = findWord(dict, roll);  // find the word that corresponds to roll

        if (word == -1) {
            exit(EXIT_FAILURE);
        }

        passphrase[i] = dict[word];

        if (verbose) {
            printf("Roll %-6lu%d\t%s\n", i + 1, passphrase[i].key, passphrase[i].word);
        }
    }

    return passphrase;
}

int rollDice(void)
{
    int roll[NUM_ROLLS];
    int concat = 0;

    for (size_t i = 0; i < NUM_ROLLS; i++) {
        roll[i] = rand() % DICE_SIDES + 1;   // Generate a number between one and six
        concat = (concat * 10) + roll[i];    // Join together each number in roll[]
    }

    return concat;
}

size_t findWord(const word_t* dict, const int key)
{
    int low = 0;
    int high = DICT_LEN;

    while (low <= high) {
        int middle = (low + high) / 2;

        if (key == dict[middle].key) {
            return middle;
        } else if (key < dict[middle].key) {
            high = middle - 1;
        } else { 
            low = middle + 1;
        }
    }

    return -1;
}

void outputPassphrase(const word_t* passphrase, const char delimiter, const int length)
{    
    for (int i = 0; i < length - 1; i++) {
        printf("%s%c", passphrase[i].word, delimiter);
    }
    printf("%s\n", passphrase[length - 1].word);
}

void outputHelp(const char* exec)
{
    printf("Usage: %s [options]\n\n", exec);
    printf("%s", "Options:\n");
    printf("%s", "  -h\t\t\tPrints some help text.\n");
    printf("%s", "  -d <delimiter>\tCharacter to separate the words with.\n");
    printf("%s", "  -l <length>\t\tThe length of the passphrase.\n");
    printf("%s", "  -f <file>\t\tPath to the input file.\n");
    printf("%s", "  -v\t\t\tPrints additional information.\n");
}
