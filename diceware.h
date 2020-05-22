#ifndef DICEWARE_H_INCLUDED
#define DICEWARE_H_INCLUDED

#define DEF_FILE_NAME   "lists/diceware_wordlist.txt"
#define DEF_DELIMITER   ' '
#define DEF_LEN         6
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

word_t* readWordList(const char*);
word_t* generatePhrase(const word_t*, const int, const bool);
int rollDice(void);
size_t findWord(const word_t*, int);
void outputPassphrase(const word_t*, const char, const int);
void outputHelp(const char*);

#endif