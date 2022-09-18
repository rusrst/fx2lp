#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <malloc.h>
#include <stdbool.h>
#include "main.h"

int main(int argc, char **argv)
{
    printf("START\n");
    const char* inputFile = NULL;
    const char* outputFile = NULL;
    uint8_t adr = NULL;
    uint8_t newData[8] = {0};

    if (argc == 1){
       printInfo();
       return -1;
    }

    for(size_t i = 1; i < argc; i++) {
        if(equalsString((uint8_t *)HELP, (uint8_t *)argv[i])){
            printHelp();
            return -1;
        }
        if (equalsString((uint8_t *)IN, (uint8_t *)argv[i]) && i+1 < argc){
            inputFile = (const char*) argv[i + 1];// *(argv + i)
        }
        if (equalsString((uint8_t *)OUT, (uint8_t *)argv[i]) && i+1 < argc) {
            outputFile = (const char*) argv[i + 1];// *(argv + i)
        }
        if (equalsString((uint8_t *)ADR, (uint8_t *) argv[i]) && i+1 < argc){
            size_t j = i+1;
           if((*(*(argv + j))) == '0' && (*(*(argv + j) + 1)) == 'x' && getStringLength((uint8_t *) argv[j]) == 4){
                adr = readTwoCharReturnByte((uint8_t *) argv[j] + 2);
            }
        }
        if (equalsString((uint8_t *) DATA, (uint8_t*) argv[i]) && i+1 < argc) {
            uint8_t* dataString = (uint8_t*) argv[i+1];
            if(*dataString == '0' && *(dataString + 1) == 'x'){
                size_t dataLength = getStringLength(dataString + 2);
                if (dataLength > 8 * 2) {
                    printErrorSize();
                    return -1;
                }
                uint8_t rest = dataLength%2;
                size_t start = 0;
                if (rest > 0) {
                    uint8_t byte = getByte('0', *(dataString + 2));
                    newData[start] = byte;
                    start = 1;
                }
                size_t j = start;
                for(; start < dataLength; start+=2, j++){
                    uint8_t byte = readTwoCharReturnByte(dataString + 2 + start);
                    newData[j] = byte;
                }
            }
        }
    }

    if(inputFile == NULL){
        printNotInputPath();
        printInfo();
        return -1;
    }

    if(outputFile == NULL)
        outputFile = inputFile;
    if(adr == NULL) {
       printInfo();
        return -1;
    }
    if (argc > 0){
        FILE* hexFile = fopen(inputFile, "r");
        char ch;
    if (hexFile != NULL) {
        size_t stringsCounter = getFileStringCounter(hexFile);
        uint8_t** stringsArray = (uint8_t**) calloc(stringsCounter, sizeof(uint8_t*));
        bool goodRead = true;
        fclose(hexFile);
        hexFile = fopen(inputFile, "r");
        if(stringsArray != NULL && hexFile != NULL) {
            for (size_t i = 0; i < stringsCounter; i++) {
                goodRead = readStringFromFileToPointer(hexFile, stringsArray + i);
                if (!goodRead) {
                        stringsCounter = i -1;
                        break;
                }
            }
            struct needChangeIndexes changeIndexes = getChangeIndexes(stringsArray, stringsCounter, adr, sizeof(newData));
            if (changeIndexes.sizeIndexes == 0) {
                printf("NOT DATA FOR CHANGE");
                return 1;
            }

            if (changeIndexes.arrayIndexes == NULL) {
                printf("ERROR MEMORY MALLOC");
                return 1;
            }

            struct hexType structHexType = {0, 0, 0, NULL, 0};
            if (changeIndexes.sizeIndexes == 1) {
                structHexType = getHexTypeStruct(*(stringsArray + *changeIndexes.arrayIndexes));
                struct hexType data = updateDataFromAdress(structHexType, adr, newData, sizeof(newData));
                uint8_t* string = hexTypeStructToString(data);
                    if(string != NULL) {
                        printf("%s", string);
                        stringsArray[*changeIndexes.arrayIndexes] = string;
                    }
                free(changeIndexes.arrayIndexes);
                free(data.data);
            } else {
                printf("NEED TO DO !!!");
            }
            fclose(hexFile);
            hexFile = fopen(outputFile, "w");
            if (hexFile != NULL) {
                for(size_t i = 0; i < stringsCounter; i++) {
                    fprintf(hexFile, "%s\n", stringsArray[i]);
                }
                 fclose(hexFile);
            }
            free(stringsArray);
        }
    }
    }
    return 0;
}

bool equalsString(uint8_t* firstString, uint8_t* secondString) {
    while(*firstString != '\0' || *secondString != '\0') {
        if (*firstString != *secondString)
            return false;
        firstString++;
        secondString++;
    }
    if(*firstString != *secondString)
        return false;
    return true;
}

uint8_t charToByte(uint8_t byte) {
    if(byte >= '0' && byte <= '9')
        return (byte - '0');

    if(byte >= 'A' && byte <= 'F')
        return (byte - 'A' + 10);

    if(byte >= 'a' && byte <= 'f')
        return (byte - 'a' + 10);

    if (byte == ':')
        return byte;

    return 0xff;
}

uint8_t getByte(uint8_t first, uint8_t second){
    return (charToByte(first) << 4) | charToByte(second);
}

size_t getStringLength(uint8_t* str) {
    size_t length = 0;
    for(size_t size = 0; *(str + size) != '\0'; length = size, size++);
    return length > 0 ? ++length : length;
}

uint8_t readTwoCharReturnByte(uint8_t* str) {
    return getByte(*str, *(str + 1));
}

struct hexType getHexTypeStruct(uint8_t* str) {
    if(*str != ':')
        return (struct hexType) {};

    str++;
    size_t stringLength = getStringLength(str);
    if (stringLength%2 != 0)
        return (struct hexType) {};


    struct hexType ht;
    ht.sizeData = readTwoCharReturnByte(str);
    str += 2;
    if (*str == '\0')
        return (struct hexType) {};

    uint8_t startAdrH = readTwoCharReturnByte(str);
    str += 2;
    if (*str == '\0')
        return (struct hexType) {};

    uint8_t startAdrL = readTwoCharReturnByte(str);
    str += 2;
    if (*str == '\0')
        return (struct hexType) {};

    ht.startAdress = (startAdrH << 8) | startAdrL;
    ht.dataType = getByte(*str, *(str + 1));
    str += 2;
    if (*str == '\0')
        return (struct hexType) {};

    uint8_t* stuctString = (uint8_t*) malloc(sizeof(uint8_t) * ht.sizeData);
    if(stuctString == NULL)
        return (struct hexType) {0, 0, 0, NULL, 0};

    for(uint8_t i = 0; i < ht.sizeData * 2; i+=2) {
        *(stuctString + i/2) = getByte(*(str + i), *(str + i + 1));
    }
    ht.data = stuctString;
    str+=ht.sizeData * 2;
    if(*str == '\0')
        return (struct hexType) {0, 0, 0, ht.data, 0};

    ht.checksum = getByte(*str, *(str+1));
    str += 2;
    return (*str == '\0') ? ht : (struct hexType) {0, 0, 0, ht.data, 0};
}

size_t getFileStringCounter(FILE* file) {
    char ch = fgetc(file);
    size_t counterStrings = 0;
    if (ch != EOF){
        counterStrings++;
        while(ch != EOF){
            if(ch == '\n') counterStrings++;
            ch = fgetc(file);
        }
    }
    return counterStrings;
}

bool readStringFromFileToPointer(FILE* file, uint8_t** pointer) {
    uint8_t* stringPointer = (uint8_t*) calloc(MAX_HEX_STRING_LENGTH, sizeof(uint8_t));
    if(stringPointer == NULL) return false;
    *pointer = stringPointer;
    char ch = fgetc(file);
    while(ch != EOF && ch != '\n'){
        *stringPointer = ch;
        stringPointer++;
        ch = fgetc(file);
    }
    return true;
}

struct hexType updateDataFromAdress(struct hexType hexStruct, uint16_t addr, uint8_t* data, size_t dataSize){
    if(hexStruct.startAdress < addr)
        return (struct hexType) {};
    
    uint16_t shift = addr - hexStruct.startAdress;
    if(shift > 0xff || shift>=hexStruct.sizeData || shift + dataSize > 0xff)
        return (struct hexType) {};

    if (dataSize + shift > 0xff) 
        return (struct hexType) {};

    if (dataSize + shift > hexStruct.sizeData){
        uint8_t* newPointer = (uint8_t*) malloc((dataSize + shift) * sizeof(uint8_t));
        for (size_t i = 0; i < shift; i++) {
            newPointer[i] = hexStruct.data[i];
        }
        free(hexStruct.data);
        hexStruct.data = newPointer;
        hexStruct.sizeData = dataSize + shift;
    }
    
    for(uint8_t i = 0; i < dataSize; i++) {
        hexStruct.data[shift + i] = data[i];
    }
    uint8_t sum = hexStruct.sizeData + (hexStruct.startAdress >> 8) + hexStruct.startAdress + hexStruct.dataType;
    uint8_t sumData = 0;
    for (uint8_t i = 0; i < hexStruct.sizeData; i++){
        sumData+=hexStruct.data[i];
    }
    hexStruct.checksum = 0 - sum - sumData;
    return hexStruct;
}

uint8_t getCharFromByte(uint8_t byte) {
    uint8_t returnByte;
    if(byte >= 10) {
        returnByte = byte - 10 + 'A';
    }
    else {
        returnByte = byte + '0';
    }
    return (returnByte > 'F') ? 0xFF : returnByte;
}

bool getTwoCharFromByte(uint8_t byte, uint8_t* charPointer) {
    uint8_t first = byte >> 4;
    uint8_t second = byte & 0x0F;
    first = getCharFromByte(first);
    second = getCharFromByte(second);
    if (first > 'F' || second > 'F')
        return false;

    *charPointer = first;
    charPointer++;
    *charPointer = second;
    return true;
}

uint8_t* hexTypeStructToString(struct hexType hexTypeStruct) {
    uint8_t* pointer = (uint8_t*) malloc(MAX_HEX_STRING_LENGTH *sizeof(uint8_t));
    if(pointer == NULL)
        return NULL;

    uint8_t* currentPointer = pointer;
    *currentPointer = ':';
    currentPointer++;
    if (!getTwoCharFromByte(hexTypeStruct.sizeData, currentPointer)) {
        free(pointer);
        return NULL;
    }

    currentPointer = currentPointer+2;
    uint8_t startAdress = hexTypeStruct.startAdress >> 8;
    if (!getTwoCharFromByte(startAdress, currentPointer)) {
        free(pointer);
        return NULL;
    }

    currentPointer = currentPointer+2;
    startAdress = (uint8_t) hexTypeStruct.startAdress;
    if (!getTwoCharFromByte(startAdress, currentPointer)) {
        free(pointer);
        return NULL;
    }

    currentPointer = currentPointer+2;
    if (!getTwoCharFromByte(hexTypeStruct.dataType, currentPointer)) {
        free(pointer);
        return NULL;
    }

    currentPointer = currentPointer+2;
    for(size_t i = 0; i < hexTypeStruct.sizeData; i++) {
        if (!getTwoCharFromByte(hexTypeStruct.data[i], currentPointer)) {
            free(pointer);
            return NULL;
        }

        currentPointer = currentPointer+2;
    }
    if (!getTwoCharFromByte(hexTypeStruct.checksum, currentPointer)) {
        free(pointer);
        return NULL;
    }
    currentPointer = currentPointer + 2;
    *currentPointer = '\0';
    return pointer;
}

void printInfo(void) {
    printf("\nThe necessary parameters were not found, for reference, enter "HELP);
}

void printHelp(void) {
    printf("Input file "IN" path\n");
    printf("Output file "OUT" path (not required, -in is used by default)\n");
    printf("Interrupt address "ADR" 0xXX (0x43 or 0x53)\n");
    printf("Data in interrupt table "DATA" 0xXXXXXX (max - 8 byte) \n");
}

void printNotInputPath(void) {
    printf("\nNOT INPUT PATH");
}

void printErrorSize(void) {
    printf("\ndata size exceeded (maximum size 8 bytes)");
}

struct needChangeIndexes getChangeIndexes(uint8_t** stringArray, size_t stringSize, uint16_t changeAdress, uint8_t dataSize) {
    struct needChangeIndexes changeIndexes = {0, NULL};
    changeIndexes.arrayIndexes = (uint8_t*) malloc(stringSize * sizeof(uint8_t));
    if (changeIndexes.arrayIndexes == NULL)
    return changeIndexes;

    for (size_t i = 0; i < stringSize; i++) {
        struct hexType currentHexType = getHexTypeStruct(stringArray[i]);
        if ((currentHexType.startAdress < changeAdress || currentHexType.startAdress < changeAdress + dataSize) 
        && (currentHexType.startAdress + currentHexType.sizeData > changeAdress))
        {
            changeIndexes.arrayIndexes[changeIndexes.sizeIndexes] = i;
            changeIndexes.sizeIndexes++;
        }
        free(currentHexType.data);
    }
    return changeIndexes;
}