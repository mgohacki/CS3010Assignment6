//
// Created by Miro Gohacki on 12/6/23.
//
#include "fileops.amonks-mgohacki.h"
int insertWord(FILE *fp, char *word){
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
    FileHeader *header;
    fseek(fp, 0,SEEK_SET);
    fread(header, sizeof(FileHeader), 1, fp);
    if (header->counts[letter] < 0){
        return 0;
    }
    else{
        return header->counts[letter];
    }
}

char *getWord(FILE *fp, char letter, int index){
//    Suppose I want to read the second word starting with 'n'
//    1. if the file is empty, there are no words starting with 'n'
//    2. if the file is not empty, then read the header and look at header.counts[13]
    FileHeader *header;
    fseek(fp, 0,SEEK_SET);
    fread(header, sizeof(FileHeader), 1, fp);
//    3. if header.counts[13] less than two, then there is no second word starting with 'n'
    if (header->counts[letter] < index){
        return NULL;
    }
//    4. otherwise, look at header.startPositions[13]
    else{

        //    5. suppose that value is p : then fseek() to p , and read the WordRecord structure starting
        //    at this position
        fseek(fp, header->startPositions[letter], SEEK_SET);
        for (int i =0; i< index;i++){
            WordRecord* word;
            //    6. get the nextpos value from the record
            fread(&word, sizeof(WordRecord), 1, fp);
            //    7. seek to nextpos
            fseek(fp, word->nextpos, SEEK_SET);
        }
        //    8. read the record at that position and get the word from that record
        WordRecord* word;
        fread(&word, sizeof(WordRecord), 1, fp);
        return word->word;
    }
}