#ifndef __PROGTEST__
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <openssl/evp.h>
#include <openssl/rand.h>

#endif /* __PROGTEST__ */

int countZero(unsigned char *hash) {
    int cnt = 0;
    unsigned char *byte = (unsigned char*) hash;
    for(int i = 0; i < EVP_MAX_MD_SIZE; i++) {
        if(*byte != 0) break;
        cnt += 8;
        byte++;
    }
    for(int i = 0; i < 8; i++) {
        if((*byte & (1 << (7-i))) != 0) break;
        cnt++;
    }
    return cnt;
}

void freeMe (char ** message, char ** hash) {
    free (*message);
    free (*hash);
}

bool genMessag(unsigned char * message, int len) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < len; ++i) {
        int index = rand() % (int)(sizeof(charset) - 1);
        (message)[i] = charset[index];
    }
    (message)[len] = '\0';
    return true;
}

bool checkBites (EVP_MD_CTX *ctx, const EVP_MD *type, unsigned char *hash, unsigned char *message, int bits,unsigned int *length, int len){

    while (true){
        genMessag(message,len);
        if (!EVP_DigestInit_ex(ctx, type, NULL))
            return false;
        if (!EVP_DigestUpdate(ctx, message, len)) // feed the message in
            return false;
        if (!EVP_DigestFinal_ex(ctx, (unsigned char *)hash,  length)) // get the hash
            return false;
        if (countZero(hash) == bits)
            return true;
     }
    return false;
}



int findHashEx (int bits, char ** message, char ** hash, const char * hashFunction) {


    if (bits < 0 ||  512  < bits ){
        return 0;
    }
    int llen = 16;

    //char hashFunction[] = "sha512";

    unsigned char hashTMP[EVP_MAX_MD_SIZE];
    unsigned char messTmp[17];



    EVP_MD_CTX * ctx;  // struktura kontextu
    const EVP_MD * type; // typ pouzite hashovaci funkce
    unsigned int length;

    OpenSSL_add_all_digests();

    type = EVP_get_digestbyname(hashFunction);


    if (!type) {
       // freeMe(message,hash);
        return 0;
    }

    int hash_size = EVP_MD_size(type);

    // Print the size of the hash
    //printf("Hash size: %d\n", hash_size);

    *hash =    (char*) malloc((hash_size*2 + 1) * sizeof (char));
    *message = (char*) malloc((llen*2+1) * sizeof(char));

    ctx = EVP_MD_CTX_new(); // create context for hashing

    if (ctx == NULL){
        freeMe(message,hash);
        EVP_MD_CTX_free(ctx);
        return 0;
    }
    if (!checkBites(ctx, type, hashTMP, messTmp, bits, &length, llen)){
        freeMe(message,hash);
        EVP_MD_CTX_free(ctx);
        return 0;
    }

    for (int i = 0; i < hash_size; i++) {
        (*hash)[i*2] = '\0';
        (*hash)[i*2+1] = '\0';
    }
    for (int i = 0; i < hash_size; i++)
        sprintf((*hash)+(i*2), "%02x", hashTMP[i]);

    (*hash)[hash_size*2] = 0;

    for (int i = 0; i < llen; i++) {
        (*message)[i*2] = '\0';
        (*message)[i*2+1] = '\0';
    }
    for (int i = 0; i < llen; i++)
        sprintf((*message)+(i*2), "%02x", messTmp[i]);
    (*message)[llen*2] = 0;
    EVP_MD_CTX_free(ctx); // destroy the context
    return 1;

}

int findHash (int bits, char ** message, char ** hash) {
    if (!findHashEx( bits, message, hash, "sha512"))
        return 0;
    return 1;
}


#ifndef __PROGTEST__

char * getBinary(int decimal, int length) {
    char *binary = (char *) malloc((length + 1) * sizeof(char));
    int i;
    for (i = length - 1; i >= 0; i--) {
        binary[i] = (decimal % 2) + '0';
        decimal /= 2;
    }
    binary[length] = '\0';
    return binary;
}


int checkHash(int bits, char * hexString) {
    char *binaryString = (char *) malloc((strlen(hexString) * 4 + 1) * sizeof(char));
    int i;
    for (i = 0; i < strlen(hexString); i++) {
        char hexChar = hexString[i];
        if (hexChar >= '0' && hexChar <= '9') {
            int hexValue = hexChar - '0';
            char * strBinary =  getBinary(hexValue, 4);
            sprintf(&binaryString[i * 4], "%04d", atoi(strBinary));
            free(strBinary);
        } else if (hexChar >= 'a' && hexChar <= 'f') {
            int hexValue = hexChar - 'a' + 10;
            char * strBinary =  getBinary(hexValue, 4);
            sprintf(&binaryString[i * 4], "%04d", atoi(strBinary));
            free(strBinary);
        } else if (hexChar >= 'A' && hexChar <= 'F') {
            int hexValue = hexChar - 'A' + 10;
            char * strBinary =  getBinary(hexValue, 4);
            sprintf(&binaryString[i * 4], "%04d", atoi(strBinary));
            free(strBinary);
        } else {
            free(binaryString);
            return 0;
        }
    }

    for (i = 0; i < bits; i++) {
        if (binaryString[i] != '0') {
            free(binaryString);
            return 0;
        }
    }

    free(binaryString);
    return 1;
}




int howMuch(char **star) {
    int size = 0;
    while (*star++) size++;
    return size;
}

int main (void) {


    char * message , * hash;
    const char sha1 [] = "edfwfwef";

    assert(findHashEx(1, &message, &hash, sha1) == 0);
   // free(message);
   // free(hash);


    assert(findHash(0, &message, &hash) == 1);
    assert(message && hash && checkHash(0, hash));
    free(message);
    free(hash);


    assert(findHash(1, &message, &hash) == 1);
    assert(message && hash && checkHash(1, hash));
    free(message);
    free(hash);

    assert(findHash(2, &message, &hash) == 1);
    assert(message && hash && checkHash(2, hash));
    free(message);
    free(hash);

    assert(findHash(3, &message, &hash) == 1);
    assert(message && hash && checkHash(3, hash));
    free(message);
    free(hash);

   // assert(findHashEx(EVP_MAX_MD_SIZE, &message, &hash, "ewe") == 0);


////////////////

    assert(findHash(4, &message, &hash) == 1);
    assert(message && hash && checkHash(4, hash));
    free(message);
    free(hash);



    assert(findHash(-2, &message, &hash) == 0);

    assert(findHash(513, &message, &hash) == 0);




    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

