#include <stdio.h>
#include <stdlib.h>
#define  _BLOCK_SIZE_ 8

char XnorDotProduct(char a, char b) {
	char XNOR = 0;
	char s = 0;
	XNOR = ~(a ^ b); //XNOR
	s = -8;
	s += 2 * ((XNOR & 1) + ((XNOR >> 1) & 1) + ((XNOR >> 2) & 1) + ((XNOR >> 3) & 1) + ((XNOR >> 4) & 1) + ((XNOR >> 5) & 1) + ((XNOR >> 6) & 1) + ((XNOR >> 7) & 1));

	return s;
}

char sign(char input) {

	if (input < 0) {
		return -1;
	}
	else {
		return 1;
	}
}

char sign1(char input) {

	if (input < 0) {
		return 0;
	}
	else {
		return 1;
	}
}

int main() {
	char flag[32][8] = {
		{-2, -2, -4, -2,  2, -4,  2,  2 },
	{0,  5, -1, -1,  1,  3, -2, -1}	  ,
	{1, -5,  2, -4,  5,  4, -5, -1}	  ,
	{1,  5, -4, -2,  0, -3,  4,  2}  ,
	{4, -3,  4, -3,  4,  1, -3,  5}  ,
	{-1, -2, -5,  4, -2,  0,  4, -2}  ,
	{-2, -2, -3, -1,  3, -5,  5,  5}  ,
	{1,  1, -1,  1, -2,  1, -5, -3}  ,
	{2,  0,  0, -5,  3,  3,  2, -3}  ,
	{-3, -4, -1, -1,  4, -2,  0, -4}  ,
	{-2, -5,  3, -2,  5, -4, -1,  0}  ,
	{-4,  0,  0, -3, -2, -5, -3,  5}  ,
	{-3, -2,  3, -5, -3,  5,  2,  5}  ,
	{5,  0,  2, -3, -2, -4,  0, -3}  ,
	{4, -4, -3, -5, -4, -2,  0,  0}  ,
	{2, -4,  4,  1,  4,  0,  4,  4}  ,
	{5, -5, -5,  1,  3,  5,  3,  2}  ,
	{-4,  4,  5, -2, -1, -2,  3, -5}  ,
	{1, -4,  1, -2, -1, -5,  4,  4}  ,
	{1,  2,  5, -2,  2, -5, -2,  5}  ,
	{3, -1,  4, -2,  3,  1,  1,  2}  ,
	{5, -4, -2, -5, -2, -5,  5,  0}  ,
	{1,  1, -2, -2,  5,  0,  5,  2}  ,
	{0, -2, -4, -1,  1,  3,  1,  3}  ,
	{4, -5,  5, -4,  0, -4, -1, -2}  ,
	{2, -2, -1, -5,  5,  5,  4, -1}  ,
	{2, -3, -5, -3,  0,  1,  3,  3}  ,
	{5, -4, -1,  0, -2, -1,  0,  1}  ,
	{4, -4, -1, -3,  0,  1,  0,  2}  ,
	{-3,  0,  5,  0, -5, -2,  3, -2}  ,
	{-5,  3,  0,  1, -1,  3, -1,  5}  ,
	{-3, -5, -4, -3,  3, -2, -2, -1}  
	};
	char sum1 = 0;
	char sum[8] = {0,};
	char sum3[8] = {0,};
	char sum2 = 0;
	char state3 = 0;
	for (int j = 0; j < 32; j++) {
		for (int i = 0; i < 8; i++) {
			if (((0x66 >> (7- i)) & 1) == 0) {
				sum1 = -1;
			}
			else {
				sum1 = 1;
			}
			sum2 += sum1 * flag[j][i];
		}
		sum3[0] = sign1(sum2);
		sum2 = 0;
		for (int i = 0; i < 8; i++) {
			if (((0x36 >> (7 - i)) & 1) == 0) {
				sum1 = -1;
			}
			else {
				sum1 = 1;
			}
			sum2 += sum1 * flag[j][i];
		}
		sum3[1] = sign1(sum2);
		sum2 = 0;
		for (int i = 0; i < 8; i++) {
			if (((0x8e >> (7 - i)) & 1) == 0) {
				sum1 = -1;
			}
			else {
				sum1 = 1;
			}
			sum2 += sum1 * flag[j][i];
		}
		sum3[2] = sign1(sum2);
		sum2 = 0;
		for (int i = 0; i < 8; i++) {
			if (((0xd2 >> (7 - i)) & 1) == 0) {
				sum1 = -1;
			}
			else {
				sum1 = 1;
			}
			sum2 += sum1 * flag[j][i];
		}
		sum3[3] = sign1(sum2);
		sum2 = 0;
		for (int i = 0; i < 8; i++) {
			if (((0x27 >> (7 - i)) & 1) == 0) {
				sum1 = -1;
			}
			else {
				sum1 = 1;
			}
			sum2 += sum1 * flag[j][i];
		}
		sum3[4] = sign1(sum2);
		sum2 = 0;

		for (int i = 0; i < 8; i++) {
			if (((0x4b >> (7 - i)) & 1) == 0) {
				sum1 = -1;
			}
			else {
				sum1 = 1;
			}
			sum2 += sum1 * flag[j][i];
		}
		sum3[5] = sign1(sum2);
		sum2 = 0;

		for (int i = 0; i < 8; i++) {
			if (((0x2e >> (7 - i)) & 1) == 0) {
				sum1 = -1;
			}
			else {
				sum1 = 1;
			}
			sum2 += sum1 * flag[j][i];
		}
		sum3[6] = sign1(sum2);
		sum2 = 0;
		for (int i = 0; i < 8; i++) {
			if (((0x55 >> (7 - i)) & 1) == 0) {
				sum1 = -1;
			}
			else {
				sum1 = 1;
			}
			sum2 += sum1 * flag[j][i];
		}
		sum3[7] = sign1(sum2);
		sum2 = 0;

		state3 = 0;
		for (int i = 0; i < 8; i++) {

			state3 |= (sum3[i] & 0x1);
			if (i == 7)
				break;
			state3 <<= 1;
		}
		sum[0] = sign1(XnorDotProduct(state3, 0xd2));
		sum[1] = sign1(XnorDotProduct(state3, 0x9a));
		sum[2] = sign1(XnorDotProduct(state3, 0x8d));
		sum[3] = sign1(XnorDotProduct(state3, 0x14));
		printf("%d", sum[0]);
		printf("%d", sum[1]);
		printf("%d", sum[2]);
		printf("%d", sum[3]);
		if (j % 2 == 1)
			printf("\n");
	}

}