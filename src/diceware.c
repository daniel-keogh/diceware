#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "diceware.h"

extern char _binary_lists_diceware_wordlist_txt_start[];

word_t* readWordList(const char* filename)
{
    FILE* pWordList;
    char word[MAX_WORD_LEN];
    word_t* dict = malloc(sizeof(word_t) * DICT_LEN);

    if (filename != NULL) {
        if ((pWordList = fopen(filename, "r")) == NULL) {
            fprintf(stderr, "%s", "[Error] Failed to open the wordlist file.\n");
            exit(EXIT_FAILURE);
        }
    } else {
        size_t len = strlen(_binary_lists_diceware_wordlist_txt_start);
        pWordList = fmemopen(_binary_lists_diceware_wordlist_txt_start, len, "r");
    }

    for (size_t i = 0; !feof(pWordList); i++) {
        if (fscanf(pWordList, "%5d %128s", &dict[i].key, word) == NUM_COLS) {
            if (dict[i].key > MAX_ROLL || dict[i].key < MIN_ROLL) {
                fprintf(stderr, "%s", "[Error] The wordlist file contains invalid entries.\n");
                exit(EXIT_FAILURE);
            }

            dict[i].word = malloc(sizeof(char) * (strlen(word) + 1));
            strcpy(dict[i].word, word);
        } else {
            fprintf(stderr, "%s", "[Error] The wordlist file contains invalid entries.\n");
            exit(EXIT_FAILURE);
        }
    }
    fclose(pWordList);

    return dict;
}

word_t* generatePhrase(const word_t* dict, const int length, const bool verbose)
{
    word_t* passphrase = malloc(sizeof(word_t) * length);

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
