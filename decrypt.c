#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define BLOC 16

void cbc(unsigned char* key, unsigned char* iv, unsigned char* data, int size, unsigned char* res){
    EVP_CIPHER_CTX *ctx;
    int len;
    assert(ctx = EVP_CIPHER_CTX_new());
    assert(EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv));
    assert(EVP_DecryptUpdate(ctx, res, &len, data, size));
}

void ecb(unsigned char* key, unsigned char* data, unsigned char* res){
    EVP_CIPHER_CTX *ctx;
    int len;
    assert(ctx = EVP_CIPHER_CTX_new());
    assert(EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL));
    assert(EVP_DecryptUpdate(ctx, res, &len, data, BLOC));
}

int main(int argc, char **argv){
    int len;
    unsigned char key[BLOC];
    unsigned char iv[BLOC];
    EVP_CIPHER_CTX *ctx;
    unsigned char res;

    FILE *fi = fopen(argv[1], "rb");
    FILE *fo = fopen(argv[2], "wb");

    fseek(fi, 0, SEEK_END);
    long fsize = ftell(fi);
    long dsize = fsize - 2*BLOC;
    fseek(fi, 0, SEEK_SET);

    fread(key, sizeof(char), BLOC, fi);
    fread(iv, sizeof(char), BLOC, fi);

    unsigned char data[dsize];
    fread(data, sizeof(char), dsize, fi);

    if((dsize % BLOC) > 0){
        int pad = BLOC - (dsize % BLOC);
        unsigned char secondtolast[BLOC];
        unsigned char d_secondtolast[BLOC];
        unsigned char datapad[dsize+pad];

        memcpy(&secondtolast, &data[dsize-2*BLOC+pad], BLOC);

        ecb(key, secondtolast, d_secondtolast);

        memcpy(&datapad, &data, dsize-2*BLOC+pad);
        memcpy(&datapad[dsize-2*BLOC+pad], &data[dsize-BLOC+pad], BLOC-pad);
        memcpy(&datapad[dsize-BLOC], &d_secondtolast[BLOC-pad], pad);
        memcpy(&datapad[dsize-BLOC+pad], &data[dsize-2*BLOC+pad], BLOC);

        unsigned char res[dsize+pad];
        cbc(key,iv, datapad, dsize+pad, res);
        fwrite(res, sizeof(char), dsize+pad, fo);
    }
    else{
        unsigned char res[dsize];
        cbc(key,iv, data, dsize, res);
        fwrite(res, sizeof(char), dsize, fo);
    }

    fclose(fi);
    fclose(fo);
}
