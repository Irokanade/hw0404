//
//  hw0404.c
//  hw0404
//
//  Created by michaelleong on 16/05/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct _zipHeader {
    //char signature[4]; //4 bytes signature
    uint16_t version; //2 bytes version
    uint16_t flag; //2 bytes flag
    uint16_t compression; //2 bytes compression
    uint16_t modTime; //2 bytes mod time
    uint16_t moderate; //2 bytes moderate
    uint16_t crc32_1; //check sum
    uint16_t crc32_2; //check sum
    uint32_t compressedSize; //4 bytes compressed size
    uint32_t uncompressedSize; //4 bytes uncompressed size
    uint16_t fileNameLen; //2 bytes file name length
    uint16_t extraFieldLen; //2 bytes extra field length
} __attribute__((packed)) zipHeader;

//if bit 3 of the flag is set then there is a data descriptor
typedef struct _dataDescriptor {
    char crc32[4]; //check sum
    uint32_t compressedSize; //4 bytes compressed size
    uint32_t uncompressedSize; //4 bytes uncompressed size
} dataDescriptor;

int noSlashPrint(char *str);
int printHelp(char *option);

int main(int argc, char *argv[]) {
    FILE *zipFile = NULL;
    
    if(printHelp(argv[1])) {
        return 1;
    }
    
    if(argc != 2) {
        printf("invalid option\n");
        printf("use '--help' for help\n");
        return 1;
    }
    
    //printf("argv[1]: %s\n", argv[1]);
    
    //printf("extension: %s\n", argv[1]+strlen(argv[1])-4);
    if(strncmp(argv[1]+strlen(argv[1])-4, ".zip", 4) != 0) {
        printf("must be .zip file\n");
        printf("use '--help' for help\n");
        return 1;
    }
    
    if((zipFile = fopen(argv[1], "rb")) == NULL) {
        printf("file open error\n");
        printf("enter '--help' for help\n");
        return 1;
    }
    
    char signature[4] = {0x50, 0x4b, 0x03, 0x04};
    int sigCount = 0;
    
    printf(".\n");
    
    while(!feof(zipFile)) {
        if(sigCount == 4) {
            zipHeader zipHeader1;
            fread(&zipHeader1, sizeof(zipHeader1), 1, zipFile);
            char *filePath = calloc(zipHeader1.fileNameLen+1, sizeof(char));
            fread(filePath, zipHeader1.fileNameLen, 1, zipFile);
            
            //count the number of '/' char except the last one
            //print tab char for each cd dir
            char *fileName = NULL;
            for(size_t i = 0; i < zipHeader1.fileNameLen-1; i++) {
                if(filePath[i] == '/') {
                    printf("    ");
                    //print from the second last '/'
                    fileName = &filePath[i];
                }
            }
            printf("+-- ");
            if(fileName == NULL) {
                noSlashPrint(filePath);
                //printf("%s\n", filePath);
            } else {
                noSlashPrint(fileName);
                //printf("%s\n", fileName);
            }
            
            free(filePath);
        }
        
        uint8_t readByte = fgetc(zipFile);
        if(readByte == signature[sigCount]) {
            sigCount++; //search for signature
        } else {
            sigCount = 0;
        }
        
    }
    
    return 0;
}

int noSlashPrint(char *str) {
    if(str[strlen(str)-1] == '/') {
        str[strlen(str)-1] = 0;
    }
    
    if(str[0] == '/') {
        return printf("%s\n", str+1);
    }
    
    return printf("%s\n", str);
}

int printHelp(char *option) {
    if(strncmp(option, "--help", 6) == 0) {
        printf("use './hw0404 [filename]' to read header of .zip file\n ");
        return 1;
    }
    
    return 0;
}
