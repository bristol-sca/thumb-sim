/*
 * AES-128 Encryption in CBC Mode - Main File
 */
#include "stdint.h"
//#include "stdio.h" //printf
#include "masked_combined.h"
#include "rand.h"
//#include "elmo-funcs.h"

// Fixed 16 Byte Key:
uint8_t key[16] = {0xb6, 0x10, 0x49, 0xfc, 0x9b, 0x73, 0xa0, 0xf5, 0x58, 0xcd, 0x6b, 0x38, 0xac, 0x29, 0xed, 0x00};

// Initializing lfsr 
unsigned int rngSeed1 = 0x12FD01;
unsigned int rngSeed2 = 0xF1AC14;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {

    init_lfsrs(get_rand(), get_rand());
    uint8_t ciphertext[16];
    uint8_t plaintext[16]; 


// Generating a random plaintext 
    for(int i=0; i < 16; i++){
        plaintext[i]= getRand();
    }
    add_to_trace(plaintext,16);
    Encrypt(ciphertext, plaintext, key);
    add_to_trace(plaintext,16);    
    
}

