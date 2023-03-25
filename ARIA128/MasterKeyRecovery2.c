#include <stdio.h>
unsigned char rk1[16] = { 0xCB, 0xd3, 0x16 ,0xA7 ,0x91 ,0xAA ,0x4d, 0x47 ,0x4D ,0xA2 ,0xD8, 0x76, 0xce, 0x2B, 0xC8 ,0x83 };
unsigned char rk2[16] = { 0xba, 0xe3, 0x64, 0x1f, 0x9e, 0x99, 0x65, 0xfc, 0x3d, 0xed, 0x67, 0xea, 0x8d, 0x51, 0x5e, 0xa7 };
unsigned char rk3[16] = { 0x3F, 0x16, 0xf4, 0xe9, 0xdc, 0x1d, 0x98, 0x32, 0xe0, 0xe3, 0x64, 0x7A, 0x13, 0xe1, 0x18, 0xfe };
unsigned char rk4[16] = { 0x10, 0xa0, 0xdb, 0xa0, 0x09, 0xee, 0x84, 0xdd, 0xd0, 0xcd, 0x60, 0x61, 0x33, 0x22, 0x62, 0x66 };
unsigned char rk5[16] = { 0x25, 0x27, 0x59, 0x81, 0x2f, 0x6c, 0x7c, 0xca, 0x81, 0x97, 0x92, 0x33, 0x08, 0x8a, 0x28, 0xcd };

void rotation(unsigned char* W, unsigned char N, unsigned char m, unsigned char* X) {

	unsigned char Y[16] = { 0, };
	if (N == 12) {
		for (int i = 0; i < 15; i++) {
			X[i] = W[i + 1];
			Y[i] = X[i];
		}
		X[15] = W[0];
		Y[15] = X[15];
		for (int i = 15; i >= 0; i--) {
			X[i] <<= 4;
			Y[i] >>= 4;
		}
		for (int i = 0; i < 15; i++) {
			X[i] = X[i] ^ Y[i + 1];
		}
		X[15] ^= Y[0];
	}
	if (N == 19 && m == '<') {
		/////////////// 19 rotation/////////////

		for (int i = 0; i < 14; i++) {
			X[i] = W[i + 2];
			Y[i] = X[i];
		}
		X[14] = W[0];
		X[15] = W[1];
		Y[14] = X[14];
		Y[15] = X[15];
		for (int i = 15; i >= 0; i--) {
			X[i] <<= 3;
			Y[i] >>= 5;
		}
		for (int i = 0; i < 15; i++) {
			X[i] = X[i] ^ Y[i+1];
		}
		X[15] ^= Y[0];
	}
	else {
		for (int i = 15; i > 1; i--) {
			X[i] = W[i - 2];
			Y[i] = X[i];
		}
		X[0] = W[14];
		X[1] = W[15];
		Y[0] = X[0];
		Y[1] = X[1];
		for (int i = 15; i >= 0; i--) {
			X[i] >>= 3;
			Y[i] <<= 5;
		}
		for (int i = 0; i < 15; i++) {
			X[i + 1] = X[i + 1] ^ Y[i];
		}
		X[0] ^= Y[15];
	}
	if (N == 31 && m == '>') {
		/////////////// 31 >>> rotation/////////////

		for (int i = 15; i > 2; i--) {
			X[i] = W[i - 3];
			Y[i] = X[i];
		}
		X[0] = W[13];
		X[1] = W[14];
		X[2] = W[15];
		Y[0] = X[0];
		Y[1] = X[1];
		Y[2] = X[2];
		for (int i = 15; i >= 0; i--) {
			X[i] >>= 7;
			Y[i] <<= 1;
		}
		for (int i = 0; i < 15; i++) {
			X[i + 1] = X[i + 1] ^ Y[i];
		}
		X[0] ^= Y[15];
	}
	else if (N == 31 && m == '<') {
		/////////////// 31 <<< rotation/////////////

		for (int i = 0; i < 13; i++) {
			X[i] = W[i + 3];
			Y[i] = X[i];
		}
		X[13] = W[0];
		X[14] = W[1];
		X[15] = W[2];
		Y[13] = X[13];
		Y[14] = X[14];
		Y[15] = X[15];
		for (int i = 15; i >= 0; i--) {
			X[i] <<= 7;
			Y[i] >>= 1;
		}
		for (int i = 0; i < 15; i++) {
			X[i] = X[i] ^ Y[i+1];
		}
		X[15] ^= Y[0];
	}
}

int main() {
	
	unsigned char T[16] = { 0, };
	unsigned char T2[16] = { 0, };
	unsigned char T3[16] = { 0, };
	/*unsigned char W1[16] = {10001000
		01000100
		10000100
		00000100
		10001000
		00001100
		10001100
		10001000
		11001100
		01001100
		10001000
		10000100
		11000100
		00000100
		11000000
		10001100};*/
	
	
	
	
	for (int i = 0; i < 16; i++) {
		T[i] = rk1[i] ^ rk5[i];
		//printf("%02x ", T[i]);
	}
	//printf("\n");
	rotation(T, 19, '<', T2); //T2 = W1 ^ W1 <<< 12 (T1 <<< 19 == T2)
	for (int i = 0; i < 16; i++) {
		
		printf("%#02x, ", T2[i]);//->����.py���� ����Ʈ a�� ���ҵ�
		//printf("\n");
	}
	//From MasterKeyRecovery1.py
	unsigned char W1[16] = { 0b00100111
							,0b00010111
							,0b10000111
							,0b01001001
							,0b11111000
							,0b11110001
							,0b11101110
							,0b10110100
							,0b10111001
							,0b01100101
							,0b10100011
							,0b01010101
							,0b00110111
							,0b00100100
							,0b00000101
							,0b11100010 };
	/*rotation(W1, 19, '>', T3);
	for (int i = 0; i < 16; i++) {
		W0[i] = rk1[i] ^ T3[i];
		printf("%02x ", W0[i]);
	}*/


	rotation(W1, 19, '>', T3);
	
	for (int i = 0; i < 16; i++) {
		T3[i] ^= rk1[i];
		printf("%02x ", T3[i]);
	}
	printf("\n");
	for (int i = 0; i < 16; i++) {
		printf("%c", T3[i]);
	}
}