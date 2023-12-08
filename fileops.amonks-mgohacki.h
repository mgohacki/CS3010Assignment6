//
// Created by Miro Gohacki and Adrien Monks on 12/6/23.
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

int checkWord(char *);
int convertToLower(char *, char *);


#endif //CS3010ASSIGNMENT6_FILEOPS_AMONKS_MGOHACKI_H
