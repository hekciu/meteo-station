#include <stdlib.h>
#include <string.h>

size_t base64_encode(char * input, char ** output) {
    const char BASE64_ALPHABET[64] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
    const char PAD = '=';

    int numBits = strlen(input) * sizeof(char);
    int outputBits = numBits + (numBits % 24 == 0 ? 0 : 24 - numBits % 24);
    *output = malloc(outputBits / sizeof(char));

    int numCurrBit = 0;
    int numCurrInChar = 0;
    int numCurrOutChar = 0;
    int currCharIndex = 0;
    for (int n = 0; n < numBits; ++n) {
        if (numCurrBit == 6) {
            *(*output + numCurrOutChar) = BASE64_ALPHABET[currCharIndex];
            numCurrBit = 0;
            numCurrOutChar++;
            continue;
        } 

        if (numCurrInChar == 8) {
            numCurrInChar = 0;
        }

        currCharIndex += ((*(input + numCurrInChar) >> n) & 1) * (2 ^ numCurrBit); 

        numCurrBit++;
        numCurrInChar++;
    } 
    
    while (numCurrBit != 0) {
        if (numCurrBit == 6) {
            *(*output + numCurrOutChar) = BASE64_ALPHABET[currCharIndex];
            numCurrBit = 0;
            numCurrOutChar++;
            continue;
        } 

        if (numCurrInChar == 8) {
            numCurrInChar = 0;
        }
        currCharIndex += 0; 

        numCurrBit++;
        numCurrInChar++;
    }

    while((numCurrOutChar + 1) % 4 != 0) {
        *(* output + numCurrOutChar) = PAD;
        numCurrOutChar++;
    }
}
