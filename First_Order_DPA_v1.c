/**
 * @file First_Order_DPA.c
 * @ref https://github.com/nvietsang/dpa-on-aes/blob/master/main.ipynb
 */
#include <stdio.h>
#include "Analysis.h"
// AES S-Box
static unsigned char AES_S[256] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};
int First_Order_DPA(unsigned int POINTS, unsigned int TRACE_NUM) {
    /************************************************************************************/
	/*                                     변수 선언                                    */
	/************************************************************************************/
	FILE			*fp									= NULL;
	FILE			*fp2								= NULL;
	FILE			*fpp								= NULL;
	FILE			*fpt								= NULL;

	char			FOLD_MERGE[_FILE_NAME_SIZE_]		= "";
	char			FILE_MERGE[_FILE_NAME_SIZE_]		= "";

	unsigned char	*Plaintext							= NULL;

	unsigned int	bits								= 0;        // target bit posion in byte
	unsigned int	B									= 0;		// Byte
	unsigned int	Guess_Key							= 0;
    unsigned int	Key									= 0;
    unsigned int    Target_bit                          = 0;

   	__int64			tn									= 0;		// for (TN)
	__int64			pi									= 0;		// for (PI)
	__int64			TN									= 0;		// Trace Number
	__int64			PI									= 0;		// Point Interval

    float			F_Temp								= 0.;

    double			*Temp_Points						= NULL;
	double			*MaxPeak							= NULL;
    double          delta[_GUESS_KEY_NUM_]              = {0};
    double          *Mean_delta_accu_visualization[_GUESS_KEY_NUM_];
    double          *Accu0                              = NULL;
    double          *Accu1                              = NULL;

    TN = (__int64)_TRACE_NUM_;
	PI = (__int64)_END_POINT_ - (__int64)_START_POINT_ + (__int64)1;

    // 분석할 바이트 위치 계산
	B = _BYTE_ - 1;

    Plaintext = (unsigned char *)malloc(_BLOCK_SIZE_ * sizeof(unsigned char));

    for (int byte_guess = 0; byte_guess < _GUESS_KEY_NUM_; byte_guess++) {
        Mean_delta_accu_visualization[byte_guess] = (double *)malloc(_TRACE_NUM_ * sizeof(double));
    }

    Accu0 = (double *)malloc(sizeof(double) * PI);
    Accu1 = (double *)malloc(sizeof(double) * PI);



	/************************************************************************************/
	/*                               First Order CPA 시작                               */
	/************************************************************************************/
	// plaintext(or ciphertext) 파일 열기 (읽기 모드)
#if defined(_WIN32) || defined(_WIN64)
	sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%s.txt", _FOLD_, _PLAIN_FILE_);
#else
	sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s/%s.txt", _FOLD_, _PLAIN_FILE_);
#endif
	fopen_s(&fpp, FILE_MERGE, "r");
	if (fpp == NULL) {
		printf(" -----------------------------------------------------------------------\n");
		printf("|                        Failed To Read Plaintext                       |\n");
		printf(" -----------------------------------------------------------------------\n");
		return _FAIL_;
	}

	// 파형 파일 열기 (읽기 모드)
#if defined(_WIN32) || defined(_WIN64)
	sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%s.sft", _FOLD_, _TRACE_FILE_);
#else
	sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s/%s.sft", _FOLD_, _TRACE_FILE_);
#endif
	fopen_s(&fpt, FILE_MERGE, "rb");
	if (fpt == NULL) {
		printf(" -----------------------------------------------------------------------\n");
		printf("|                          Failed To Read Trace                         |\n");
		printf(" -----------------------------------------------------------------------\n");
		return _FAIL_;
	}

	// 결과 저장할 폴더 생성
#if defined(_WIN32) || defined(_WIN64)
	sprintf_s(FOLD_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\Result_%02dday_%02dh_%02dm_%02ds", _FOLD_, TIME->tm_mday, TIME->tm_hour, TIME->tm_min, TIME->tm_sec);
#else
	//sprintf_s(FOLD_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s/Result_%02dday_%02dh_%02dm_%02ds", _FOLD_, TIME->tm_mday, TIME->tm_hour, TIME->tm_min, TIME->tm_sec);
#endif
	//mkdir(FOLD_MERGE, 0755);


    // target bit position
    for (bits = 0; bits < 8; bits++) {
        printf("Target bit at position %d\n", bits);
        printf("    Recovered round-1 key: \n");

        for (B = 0; B < 16; B++) {
            
            for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
                
                int count0 = 0;
                int count1 = 0;
                memset(Accu0, 0, PI);
                memset(Accu1, 0, PI);

                _fseeki64(fpp, (__int64)_START_TRACE_ * _TEXT_SIZE_, SEEK_SET);
                for (tn = _START_TRACE_; tn < _END_TRACE_; tn++) {
                    for (int i = 0; i < _BLOCK_SIZE_; i++) {
#if defined(_WIN32) || defined(_WIN64)
			            fscanf_s(fpp, "%hhx", &Plaintext[i]);
#else
			            fscanf(fpp, "%hhx", &Plaintext[i]);
#endif
		            }

                    Key = AES_S[Plaintext[B] ^ (Guess_Key + _GUESS_KEY_START_)];
                    Target_bit = (Key >> bits) & 1;
                    _fseeki64(fpt, 32 + ((__int64)POINTS * (__int64)tn + (__int64)_START_POINT_ - (__int64)1) * (__int64)4, SEEK_SET);

                    if (Target_bit == 0) {
                        for (int pi = 0; pi < PI; pi++) {
                            fread(&F_Temp, sizeof(float), 1, fpt);
                            Accu0[pi] += (double)F_Temp;
                        }
                        count0++;
                    } else {
                        for (int pi = 0; pi < PI; pi++) {
                            fread(&F_Temp, sizeof(float), 1, fpt);
                            Accu1[pi] += (double)F_Temp;
                        }
                        count1++;
                    }
                }
                // count0, count1이 0이 아닌지 확인
                if (count0 > 0 && count1 > 0) {
                    for (int k = 0; k < PI; k++) {
                        Mean_delta_accu_visualization[Guess_Key][k] = fabs(Accu1[k] / count1 - Accu0[k] / count0);
                    }
                    delta[Guess_Key] = Mean_delta_accu_visualization[Guess_Key][0]; // 가장 큰 값 선택
                    for (int k = 1; k < PI; k++) {
                        if (Mean_delta_accu_visualization[Guess_Key][k] > delta[Guess_Key]) {
                            delta[Guess_Key] = Mean_delta_accu_visualization[Guess_Key][k];
                        }
                    }
                } else {
                    delta[Guess_Key] = 0;
                }
            }
            // delta에서 최대값 인덱스 찾기
            int predicted_byte = 0;
            double max_delta = delta[0];
            for (int i = 1; i < _GUESS_KEY_NUM_; i++) {
                if (delta[i] > max_delta) {
                    max_delta = delta[i];
                    predicted_byte = i;
                }
            }

            printf("%0x02X ", predicted_byte);
        }
    }
}

int main() {
    First_Order_DPA(7000, 500);
}