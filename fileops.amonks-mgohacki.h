//
// Created by Miro Gohacki on 12/6/23.
//

#ifndef CS3010ASSIGNMENT6_FILEOPS_AMONKS_MGOHACKI_H
#define CS3010ASSIGNMENT6_FILEOPS_AMONKS_MGOHACKI_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define NUMLETTERS 26
typedef struct {
    long counts[NUMLETTERS];
    long startPositions[NUMLETTERS];
} FileHeader;

#define MAXWORDLEN 31
typedef struct {
    char word[1+MAXWORDLEN];
    long nextpos;
} WordRecord;

int insertWord(FILE *fp, char *word);

int countWords(FILE *fp, char letter, int *count);

char *getWord(FILE *fp, char letter, int index);

//--------------------------------------------------------
// return 1 if any chararacters are non-alphabetic;
// otherwise return 0

int checkWord(char *word) {
    int i, len;

    len = strlen(word);
    for (i=0; i<len; ++i) {
        if ( ! isalpha(word[i]) )
            return 1;
    }

    return 0;
}

//--------------------------------------------------------
// convert ach character to its lower-case equivalent;
// leaves characters that are already lower case unchanged;
// returns zero always;
// assumes that convertedWord has sufficient space to hold
// the result

int convertToLower(char *word, char *convertedWord) {
    int i, len;

    strcpy(convertedWord, word);

    len = strlen(word);
    for (i=0; i<len; ++i)
        convertedWord[i] = tolower(word[i]);

    return 0;
}

#endif //CS3010ASSIGNMENT6_FILEOPS_AMONKS_MGOHACKI_H
