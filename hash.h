#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include <stdlib.h>
#include <string.h>

void hash_f (char* text){
    int text_len = strlen(text);
    text[text_len++] = 1 << 7;
    for(int i = text_len; i < 63; i++){
        text[i] = ' ';
    }
    text[63] = text_len;
    unsigned int h[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};
    unsigned int k[64] =
            {0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
             0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
             0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
             0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
             0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
             0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
             0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
             0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};
    unsigned int m[64];
    for(int i = 0; i < 16; i++){
        m[i] = (text[i*4] << 24) + (text[i*4 + 1] << 16) + (text[i*4 + 2] << 8) + text[i*4 + 3];
    }
    for(int i = 16; i < 64; i++){
        unsigned int s0 = ((m[i-15] << (32-7)) + (m[i-15] >> 7)) ^ ((m[i-15] << (32-18)) + (m[i-15] >> 18)) ^ (m[i-15] >> 3);
        unsigned int s1 = ((m[i-2] << (32-17)) + (m[i-2] >> 17)) ^ ((m[i-2] << (32-19)) + (m[i-2] >> 19)) ^ (m[i-2] >> 10);
        m[i] = m[i-16] + s0 + m[i-7] + s1;
    }
    unsigned int a = h[0];
    unsigned int b = h[1];
    unsigned int c = h[2];
    unsigned int d = h[3];
    unsigned int e = h[4];
    unsigned int f = h[5];
    unsigned int g = h[6];
    unsigned int hh = h[7];
    for(int i = 0; i < 64; i++){
        unsigned int S1 = ((e << (32-6)) + (e >> 6)) ^ ((e << (32-11)) + (e >> 11)) ^ ((e << (32-25)) + (e >> 25));
        unsigned int ch = (e & f) ^ ((!e) & g);
        unsigned int temp1 = hh + S1 + ch + k[i] + m[i];
        unsigned int S0 = ((a << (32-2)) + (a >> 2)) ^ ((a << (32-13)) + (a >> 13)) ^ ((a << (32-22)) + (a >> 22));
        unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
        unsigned int temp2 = S0 + maj;
        hh = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }
    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
    h[5] += f;
    h[6] += g;
    h[7] += hh;
    for(int j = 0; j < 8; j++){
        for(int i = 8*(j+1) - 1; i >= 8*j; i--){
            text[i] = h[j] % ('~' - '!') + '!';
            h[j] >>= 4;
        }
    }
}

void vigenere (char *text, char* master_pass){
    for(int i = strlen(text); i < 32; i++){
        text[i] = '%';
    }
    char *key = (char*)malloc(100000);
    while(key[31] == '\0'){
        strcat(key, master_pass);
    }
    for(int i = 0; i < 32; i++){
        text[i] = ((text[i] + key[i] - 2 * '!') % ('~' - ' ')) + '!';
    }
}

void vigenere_r (char *text, char* master_pass){
    char *key = (char*)malloc(100000);
    while(key[31] == '\0'){
        strcat(key, master_pass);
    }
    for(int i = 0; i < 32; i++){
        text[i] = (text[i] + ('~' - ' ') - key[i]) % ('~' - ' ') + '!';
        if(text[i] == '%')
            text[i] = '\0';
    }
}

#endif // HASH_H_INCLUDED
