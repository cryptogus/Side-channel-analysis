#ifndef __AES128__
#define __AES128__
unsigned char mix2(unsigned char b);
unsigned char mix3(unsigned char a);
void AES_En(unsigned char cipherText[16] ,unsigned char plainText[16], unsigned char key[16]);
#endif