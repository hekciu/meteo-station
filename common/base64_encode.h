#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "power.h"
#include <stdint.h>

size_t base64_encode(char * input, char ** output) {
    const char BASE64_ALPHABET[64] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
    const char PAD = '=';

    int numBits = strlen(input) * 8;
    int outputBits = numBits + (numBits % 24 == 0 ? 0 : 24 - numBits % 24);
    *output = malloc(outputBits / 8);

    int numCurrBit = 0;
    int numCurrOutChar = 0;
    int currCharIndex = 0;
    for (int n = 0; n < numBits; ++n) {
        int numCurrInChar = n / 8;
        if (numCurrBit == 6) {
            *(*output + numCurrOutChar) = BASE64_ALPHABET[currCharIndex];
            numCurrBit = 0;
            numCurrOutChar++;
            currCharIndex = 0;
        } 
        
        int numCurrInBit = n % 8;
        uint8_t currentChar = (uint8_t)(*(input + numCurrInChar));
        uint8_t currentMask = (uint8_t)1 << numCurrInBit;
        uint8_t bitValue = (currentChar >> (7 - numCurrInBit)) & (uint8_t)1;
        currCharIndex += bitValue * power(2, 5 - numCurrBit); 

        numCurrBit++;
    } 
    
    while (numCurrBit != 0) {
        printf("dupa, tutaj sie cos psuje\n");
        if (numCurrBit == 6) {
            *(*output + numCurrOutChar) = BASE64_ALPHABET[currCharIndex];
            break;
        } 

        currCharIndex += 0; 

        numCurrBit++;
    }

    while((numCurrOutChar + 1) % 4 != 0) {
        *(* output + numCurrOutChar) = PAD;
        numCurrOutChar++;
    }
}
