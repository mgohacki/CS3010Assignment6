//
// Created by Miro Gohacki on 12/6/23.
//
#include "fileops.amonks-mgohacki.h"

int insertWord(FILE *fp, char *word){
    if (word == NULL || word[0] == '\0') {
        return 1;
    }
    if (checkWord(word) != 0) {
        return 1;
    }

    char lcword[MAXWORDLEN + 1];
    convertToLower(word, lcword);
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    FileHeader header;
    if (filesize == 0) { // File is empty, write a new header
        for (int i = 0; i < NUMLETTERS; i++) {
            header.counts[i] = 0;
            header.startPositions[i] = 0;
        }
        header.counts[lcword[0] - 'a'] = 1;
        header.startPositions[lcword[0] - 'a'] = sizeof(FileHeader);
        fseek(fp, 0, SEEK_SET);
        fwrite(&header, sizeof(FileHeader), 1, fp);
        WordRecord record;
        strcpy(record.word, lcword);
        record.nextpos = 0;
        fseek(fp, sizeof(FileHeader), SEEK_SET);
        fwrite(&record, sizeof(WordRecord), 1, fp);
        return 0;
    }
    // File is not empty
    fseek(fp, 0, SEEK_SET);
    fread(&header, sizeof(FileHeader), 1, fp);
    int index = lcword[0] - 'a'; //index of first char
    header.counts[index]++;
    if (header.counts[index] == 1) {// If this is the first word for this letter
        header.startPositions[index] = filesize;
        fseek(fp, 0, SEEK_SET);
        fwrite(&header, sizeof(FileHeader), 1, fp);
        WordRecord record;
        strcpy(record.word, lcword);
        record.nextpos = 0;
        fseek(fp, 0, SEEK_END);
        fwrite(&record, sizeof(WordRecord), 1, fp);

    } else { //Not first word for the letter
        long currentPosition = sizeof(FileHeader);
        fseek(fp, header.startPositions[index], SEEK_SET);
        WordRecord currentRecord;
        fread(&currentRecord, sizeof(WordRecord), 1, fp);
        while (currentRecord.nextpos != 0) { //read all records for given letter
            currentPosition = currentRecord.nextpos;
            fseek(fp, currentRecord.nextpos, SEEK_SET);
            fread(&currentRecord, sizeof(WordRecord), 1, fp);
        }
        currentRecord.nextpos = filesize;
        fseek(fp, currentPosition, SEEK_SET);
        fwrite(&currentRecord, sizeof(WordRecord), 1, fp);
        WordRecord newRecord;
        strcpy(newRecord.word, lcword);
        newRecord.nextpos = 0;
        fseek(fp, 0, SEEK_END);
        fwrite(&newRecord, sizeof(WordRecord), 1, fp);
    }
    return 0;

//    Suppose I want to write the word "kerfuffle" to the file
//    1. if the file is empty
//      1. write a new header with header.counts[10] = 1 and header.startPositions = sizeof(Header)
//      2. fseek() to position sizeof(Header) and write a record with ("kerfuffle", 0)
//    2. otherwise
//      1. read the header
//      2. increment header.counts[10]
//      3. if header.counts[10] is now 1 then
//          1. set header.startPositions[10] to the current size of the file (must seek to file start and write the updated header)
//          2. fseek() to the end of the file and write a record with ("kerfuffle", 0)
//      4. else
//          1. successively read the records for words starting with 'k'
//          2. update the nextpos last record for words starting with 'k' to the current file size (must read this record, update it, and write it)
//          3. fseek() to the end of the file and write a record with ("kerfuffle", 0

}

int countWords(FILE *fp, char letter, int *count){
    FileHeader header;
    fseek(fp, 0,SEEK_SET);
    fread(&header, sizeof(FileHeader), 1, fp);
    int index = letter - 'a';
    if (header.counts[index] < 0){
        printf("ERROR: header.counts[] less than 0");
        return 0;
    }
    else{
        *count = header.counts[index];
        return 0;
    }
}

char *getWord(FILE *fp, char letter, int index){
//    Suppose I want to read the second word starting with 'n'
//    1. if the file is empty, there are no words starting with 'n'
//    2. if the file is not empty, then read the header and look at header.counts[13]
    FileHeader header;
    fseek(fp, 0,SEEK_SET);
    fread(&header, sizeof(FileHeader), 1, fp);
//    3. if header.counts[13] less than two, then there is no second word starting with 'n'
    int letterIndex = letter - 'a';
    printf("\n%d\n", index);
    if (header.counts[letterIndex] < index + 1){
        return NULL;
    }
//    4. otherwise, look at header.startPositions[13]
    else{
        //    5. suppose that value is p : then fseek() to p , and read the WordRecord structure starting
        //    at this position
        WordRecord word;
        fseek(fp, header.startPositions[letterIndex], SEEK_SET);
        for (int i = 0; i < index;i++){

            //    6. get the nextpos value from the record
            fread(&word, sizeof(WordRecord), 1, fp);
            //    7. seek to nextpos
            fseek(fp, word.nextpos, SEEK_SET);
        }
        //    8. read the record at that position and get the word from that record
        fread(&word, sizeof(WordRecord), 1, fp);
        char *finalWord = malloc(strlen(word.word) + 1);
        strcpy(finalWord, word.word);
        printf("Word: %s\n", finalWord);
        return finalWord;
    }
}


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