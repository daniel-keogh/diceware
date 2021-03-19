#ifndef DICEWARE_H_INCLUDED
#define DICEWARE_H_INCLUDED

#define DICE_SIDES      6
#define DICT_LEN        7776
#define NUM_COLS        2
#define NUM_ROLLS       5
#define MIN_ROLL        11111
#define MAX_ROLL        66666
#define MAX_WORD_LEN    128

typedef struct {
    int key;
    char* word;
} word_t;

word_t* readWordList(const char* filename);
word_t* generatePhrase(const word_t* dict, const int length);
int rollDice(void);
size_t findWord(const word_t* dict, const int key);

#endif