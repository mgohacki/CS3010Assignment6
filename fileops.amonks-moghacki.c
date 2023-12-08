//
// Created by Miro Gohacki on 12/6/23.
//

#include "fileops.amonks-mgohacki.h"

// Function to insert a word into the file
int insertWord(FILE *fp, char *word) {
    // Check if the word is valid
    if (word == NULL || word[0] == '\0') {
        return 1; // Invalid word
    }

    // Check if the word contains only alphabetic characters
    if (checkWord(word) != 0) {
        return 1; // Invalid word
    }

    // Convert the word to lowercase
    char lcword[MAXWORDLEN + 1];
    convertToLower(word, lcword);

    // Get the current file position and read the file header
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    FileHeader header;

    if (filesize == 0) { // File is empty, write a new header
        // Initialize the counts and start positions in the header
        for (int i = 0; i < NUMLETTERS; i++) {
            header.counts[i] = 0;
            header.startPositions[i] = 0;
        }

        // Update the counts and start position for the first letter of the word
        header.counts[lcword[0] - 'a'] = 1;
        header.startPositions[lcword[0] - 'a'] = sizeof(FileHeader);

        // Write the header to the file
        fseek(fp, 0, SEEK_SET);
        fwrite(&header, sizeof(FileHeader), 1, fp);

        // Write the word record to the file
        WordRecord record;
        strcpy(record.word, lcword);
        record.nextpos = 0;
        fseek(fp, sizeof(FileHeader), SEEK_SET);
        fwrite(&record, sizeof(WordRecord), 1, fp);

        return 0; // Successful insertion
    }

    // File is not empty
    fseek(fp, 0, SEEK_SET);
    fread(&header, sizeof(FileHeader), 1, fp);
    int index = lcword[0] - 'a'; // Index of the first character

    // Update counts for the first letter of the word
    header.counts[index]++;

    if (header.counts[index] == 1) { // If this is the first word for this letter
        // Update the start position for the first letter of the word
        header.startPositions[index] = filesize;

        // Write the header to the file
        fseek(fp, 0, SEEK_SET);
        fwrite(&header, sizeof(FileHeader), 1, fp);

        // Write the word record to the file
        WordRecord record;
        strcpy(record.word, lcword);
        record.nextpos = 0;
        fseek(fp, 0, SEEK_END);
        fwrite(&record, sizeof(WordRecord), 1, fp);

    } else { // Not the first word for the letter
        // Write the header back to the file
        fseek(fp, 0, SEEK_SET);
        fwrite(&header, sizeof(FileHeader), 1, fp);

        // Move to the start position for the current letter and read the current word record
        fseek(fp, header.startPositions[index], SEEK_SET);
        WordRecord currentRecord;
        fread(&currentRecord, sizeof(WordRecord), 1, fp);

        // Iterate to the end of the linked list for the current letter
        while (currentRecord.nextpos != 0) {
            fseek(fp, currentRecord.nextpos, SEEK_SET);
            fread(&currentRecord, sizeof(WordRecord), 1, fp);
        }

        // Write the new word record to the file
        WordRecord newRecord;
        strcpy(newRecord.word, lcword);
        newRecord.nextpos = 0;
        fseek(fp, 0, SEEK_END);
        long currentPosition = ftell(fp);
        fwrite(&newRecord, sizeof(WordRecord), 1, fp);

        // Update the next position of the second to last record in the linked list to the position of the last record
        currentRecord.nextpos = currentPosition;
        printf("%d",currentPosition);
        printf("\n");
        fseek(fp, header.startPositions[index], SEEK_SET);
        fwrite(&currentRecord, sizeof(WordRecord), 1, fp);



    }

    return 0; // Successful insertion
}

// Function to count words starting with a specific letter
int countWords(FILE *fp, char letter, int *count) {
    FileHeader header;

    // Read the file header
    fseek(fp, 0, SEEK_SET);
    fread(&header, sizeof(FileHeader), 1, fp);

    // Check if the input letter is valid
    if (!checkWord(&letter)) {
        return 1; // Invalid letter
    }

    // Get the index corresponding to the input letter
    int index = letter - 'a';

    // Update the count variable with the count of words starting with the given letter
    *count = header.counts[index];

    return 0; // Successful count
}

// Function to get a word at a specific index starting with a specific letter
char *getWord(FILE *fp, char letter, int index) {
    FileHeader header;

    // Read the file header
    fseek(fp, 0, SEEK_SET);
    fread(&header, sizeof(FileHeader), 1, fp);

    // Get the index corresponding to the input letter
    int letterIndex = letter - 'a';

    // Check if the requested index is within bounds
    if (header.counts[letterIndex] < index + 1) {
        return NULL; // Index out of bounds
    } else {
        WordRecord word;

        // Move to the start position for the current letter
        fseek(fp, header.startPositions[letterIndex], SEEK_SET);

        // Iterate to the desired index in the linked list
        for (int i = 0; i < index; i++) {
            fread(&word, sizeof(WordRecord), 1, fp);
            fseek(fp, word.nextpos, SEEK_SET);
        }

        // Read the word record at the desired index
        fread(&word, sizeof(WordRecord), 1, fp);

        // Allocate memory for the final word and copy the word to it
        char *finalWord = malloc(strlen(word.word) + 1);
        strcpy(finalWord, word.word);

        return finalWord; // Return the final word
    }
}

//--------------------------------------------------------
// Return 1 if any characters are non-alphabetic;
// otherwise return 0
int checkWord(char *word) {
    int i, len;

    len = strlen(word);

    // Iterate through each character in the word
    for (i = 0; i < len; ++i) {
        // Check if the character is not alphabetic
        if (!isalpha(word[i]))
            return 1; // Non-alphabetic character found
    }

    return 0; // All characters are alphabetic
}

//--------------------------------------------------------
// Convert each character to its lower-case equivalent;
// leaves characters that are already lowercase unchanged;
// returns zero always;
// assumes that convertedWord has sufficient space to hold
// the result
int convertToLower(char *word, char *convertedWord) {
    int i, len;

    // Copy the original word to the converted word
    strcpy(convertedWord, word);

    len = strlen(word);

    // Iterate through each character in the word and convert it to lowercase
    for (i = 0; i < len; ++i)
        convertedWord[i] = tolower(word[i]);

    return 0; // Successful conversion
}
