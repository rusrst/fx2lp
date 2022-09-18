#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define MAX_HEX_STRING_LENGTH  1 + (1 + 2 + 1 + 255 + 1) * 2 + 1
#define IN "-in"
#define OUT "-out"
#define HELP "-help"
#define ADR "-adr"
#define DATA "-data"

struct hexType{
    uint8_t sizeData;
    uint16_t startAdress;
    uint8_t dataType;
    uint8_t* data;
    uint8_t checksum;
};

struct needChangeIndexes{
    size_t sizeIndexes;
    uint8_t* arrayIndexes;
};

bool equalsString(uint8_t* firstString, uint8_t* secondString);
uint8_t charToByte(uint8_t byte);
uint8_t getByte(uint8_t first, uint8_t second);
size_t getStringLength(uint8_t* str);
uint8_t readTwoCharReturnByte(uint8_t* str);
struct hexType getHexTypeStruct(uint8_t* str);
size_t getFileStringCounter(FILE* file);
bool readStringFromFileToPointer(FILE* file, uint8_t** pointer);
uint8_t getCharFromByte(uint8_t byte);
bool getTwoCharFromByte(uint8_t byte, uint8_t* charPointer);
struct hexType updateDataFromAdress(struct hexType hexStruct, uint16_t addr, uint8_t* data, size_t dataSize);
uint8_t* hexTypeStructToString(struct hexType hexTypeStruct);
void printInfo(void);
void printHelp(void);
void printNotInputPath(void);
void printErrorSize(void);
struct needChangeIndexes getChangeIndexes(uint8_t** stringArray, size_t stringSize, uint16_t changeAdress, uint8_t dataSize);

#endif // MAIN_H_INCLUDED