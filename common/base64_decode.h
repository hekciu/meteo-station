#ifndef BASE64_DECODE_H
#define BASE64_DECODE_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "power.h"

size_t base64_decode(char * input, char ** output) {
    const char BASE64_ALPHABET[64] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
    const char PAD = '=';

    int inputNPad = 0;
    size_t inputLen = strlen(input); 

    for (int i = 0; i < inputLen; ++i) {
        if (*(input + i) == PAD) {
            inputNPad++;
        }
    }
     
    int numBits = inputLen * 8;
    int numPadBits = inputNPad * 8;

    int outputBits = ((numBits - numPadBits) / 24) * 24;
    *output = malloc(outputBits / 8);

    int numOutBit = 0;
    char currOutChar = 0;

    for (int nChar = 0; nChar < inputLen; ++nChar) {
        char currChar = *(input + nChar);
        if (currChar == PAD) {
            break;
        }
        int currCharIndex = -1;

        for (int i = 0; i < 64; ++i) {
            if (BASE64_ALPHABET[i] == currChar) {
                currCharIndex = i;
                break;
            }
        }

        if (currCharIndex == -1) {
            fprintf(stderr, "got incorrect char as base64_decode input: %c\n", currChar);
            return -1; 
        }

        for (int n = 2; n < 8; ++n) {
            int currOutCharIndex = numOutBit / 8;            

            uint8_t bitValue = ((uint8_t)currCharIndex >> (7 - n)) & (uint8_t)1;
            currOutChar += bitValue * power(2, 7 - numOutBit % 8); 

            if ((numOutBit + 1) % 8 == 0) {
                *(*output + currOutCharIndex) = currOutChar;
                currOutChar = 0;   
            }
            numOutBit++;
        }
    }
}

#endif
