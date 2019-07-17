#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define DICT_LEN 7776
#define DICT_FILE_NAME "diceware_wordlist.txt"
#define DEF_DELIMITER ' '
#define DEF_LEN 6
#define NUM_COLS 2
#define NUM_ROLLS 5
#define MIN_ROLL 11111
#define MAX_ROLL 66666
#define MAX_ROW_LEN 256
#define OPTS "d:l:h"

typedef struct {
    int key;
    char* word;
} word_t;

int rollDice(void);
int searchDict(const word_t*, int, int, int);
word_t* generatePhrase(const word_t*, int);
void outputPassphrase(const word_t*, char, int);
void outputHelp(const char*);

int main(int argc, char* argv[]) {
    int opt;
    int phraseLength = DEF_LEN;
    char word[MAX_ROW_LEN];
    char delimiter = DEF_DELIMITER;
    word_t dict[DICT_LEN];
    FILE* pWordList;
    srand(time(NULL));

    while ((opt = getopt(argc, argv, OPTS)) != -1) {
        switch (opt) {
            case 'd':
                if (!isprint(optarg[0])) {
                    fprintf(stderr, "%s", "[Error] Invalid delimiter entered.\n");
                    exit(EXIT_FAILURE);
                }
                if (strlen(optarg) > 1) {
                    fprintf(stderr, "%s", "[Error] Provide only a single delimitation character.\n");
                    exit(EXIT_FAILURE);
                }
                delimiter = optarg[0];
                break;
            case 'l':
                phraseLength = atoi(optarg);
                if (phraseLength > DICT_LEN || phraseLength <= 0) {
                    fprintf(stderr, "[Error] Length must be a positive integer, and less than %d.\n", DICT_LEN + 1);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
                outputHelp(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                outputHelp(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if ((pWordList = fopen(DICT_FILE_NAME, "r")) == NULL) {
        fprintf(stderr, "%s", "[Error] Couldn't open the word list file.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; !feof(pWordList); i++) {
        if (fscanf(pWordList, "%d %s", &dict[i].key, word) == NUM_COLS) {
            if (dict[i].key > MAX_ROLL || dict[i].key < MIN_ROLL) {
                fprintf(stderr, "%s", "[Error] The word list file appears to contain invalid entries.\n");
                exit(EXIT_FAILURE);
            }
            dict[i].word = (char*)malloc(sizeof(char) * (strlen(word) + 1));
            strcpy(dict[i].word, word);
        }
    }
    fclose(pWordList);

    word_t* passphrase = generatePhrase(dict, phraseLength);
    outputPassphrase(passphrase, delimiter, phraseLength);
}

word_t* generatePhrase(const word_t* dict, const int length) {
    int roll, loc;
    word_t* passphrase = (word_t*)malloc(sizeof(word_t) * length);

    for (int i = 0; i < length; i++) {
        roll = rollDice();
        loc = searchDict(dict, roll, 0, DICT_LEN); // find the location of the word that matches roll

        if (loc == -1) exit(EXIT_FAILURE);

        passphrase[i] = dict[loc];
        printf("Roll %-6d%d\t%s\n", i + 1, passphrase[i].key, passphrase[i].word);
    }
    return passphrase;
}

int rollDice(void) {
    int roll[NUM_ROLLS];
    int concatRolls = 0;

    for (int i = 0; i < NUM_ROLLS; i++) {
        roll[i] = rand() % 6 + 1; // Generate a number between one and six
        concatRolls = (10 * concatRolls) + roll[i]; // Join together each number in roll[]
    }
    return concatRolls;
}

// Performs a binary search of the words in the wordlist
int searchDict(const word_t* dict, const int searchKey, int low, int high) {
    int middle; 

    while (low <= high) {
        middle = (low + high) / 2;

        if (searchKey == dict[middle].key) 
            return middle;
        else if (searchKey < dict[middle].key)
            high = middle - 1;
        else 
            low = middle + 1;
    }
    return -1;
}

void outputPassphrase(const word_t* passphrase, const char delimiter, const int length) {
    printf("\n%s", "Your passphrase is: ");
    for (int i = 0; i < length - 1; i++)
        printf("%s%c", passphrase[i].word, delimiter);
    printf("%s\n", passphrase[length - 1].word);
}

void outputHelp(const char* exec) {
    printf("Usage: %s [OPTIONS]\n\n", exec);
    printf("%s", "Options:\n");
    printf("%s", "  -h\t\t\tPrints some help text.\n");
    printf("%s", "  -d <delimiter>\tSpecify a character with which to separate the words.\n");
    printf("%s", "  -l <length>\t\tSpecify the length of the passphrase to generate.\n");
}