//
// Created by Miro Gohacki on 12/6/23.
//

#ifndef CS3010ASSIGNMENT6_FILEOPS_AMONKS_MGOHACKI_H
#define CS3010ASSIGNMENT6_FILEOPS_AMONKS_MGOHACKI_H
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
#endif //CS3010ASSIGNMENT6_FILEOPS_AMONKS_MGOHACKI_H
