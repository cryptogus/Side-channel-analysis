
// First_Order_CPA.c
//

#include "Analysis.h"
#include "Table.h"

int First_Order_CPA(struct tm* TIME, unsigned int POINTS, unsigned int TRACE_NUM)
{
	/************************************************************************************/
	/*                                     변수 선언                                    */
	/************************************************************************************/
	FILE* fp = NULL;
	FILE* fp2 = NULL;
	FILE* fpp = NULL;
	FILE* fpt = NULL;

	char			FOLD_MERGE[_FILE_NAME_SIZE_] = "";
	char			FILE_MERGE[_FILE_NAME_SIZE_] = "";

	unsigned char* Plaintext = NULL;

	unsigned int	i = 0;		// for (_BLOCK_SIZE, _CANDIDATES_)
	unsigned int	B = 0;		// Byte
	unsigned int	Guess_Key = 0;
	unsigned int	Key = 0;
	unsigned int	Key_HW = 0;
	unsigned int	R_Key = 0;
	unsigned int	Right_Key = 0;
	unsigned int	Index = 0;
	unsigned int	Percent = 0;
	unsigned int	Check = 0;

	__int64* H_S = NULL;
	__int64* H_SS = NULL;
	__int64			tn = 0;		// for (TN)
	__int64			pi = 0;		// for (PI)
	__int64			TN = 0;		// Trace Number
	__int64			PI = 0;		// Point Interval

	float			F_Temp = 0.;

	double* Temp_Points = NULL;
	double* MaxPeak = NULL;
	double* W_CS = NULL;
	double* W_CSS = NULL;
	double** W_HS = NULL;
	double			Correlation = 0.;
	double			Correlation_L = 0.;
	double			Correlation_R = 0.;
	double			Max = 0.;
	double			Max_Sec = 0.;
	double			Ratio = 0.;
	
	unsigned char rk1[16] = { 0xCB, 0xd3, 0x16 ,0xA7 ,0x91 ,0xAA ,0x4d, 0x47 ,0x4D ,0xA2 ,0xD8, 0x76, 0xce, 0x2B, 0xC8 ,0x83 };
	unsigned char rk2[16] = { 0xba, 0xe3, 0x64, 0x1f, 0x9e, 0x99, 0x65, 0xfc, 0x3d, 0xed, 0x67, 0xea, 0x8d, 0x51, 0x5e, 0xa7 };
	unsigned char rk3[16] = { 0x3F, 0x16, 0xf4, 0xe9, 0xdc, 0x1d, 0x98, 0x32, 0xe0, 0xe3, 0x64, 0x7A, 0x13, 0xe1, 0x18, 0xfe };
	unsigned char rk4[16] = { 0x10, 0xa0, 0xdb, 0xa0, 0x09, 0xee, 0x84, 0xdd, 0xd0, 0xcd, 0x60, 0x61, 0x33, 0x22, 0x62, 0x66 };
	unsigned char rk5[16] = { 0x25, 0x27, 0x59, 0x81, 0x2f, 0x6c, 0x7c, 0xca, 0x81, 0x97, 0x92, 0x33, 0x08, 0x8a, 0x28, 0xcd };
	// 분석할 파형 및 포인트 수 계산
	TN = (__int64)_TRACE_NUM_;
	PI = (__int64)_END_POINT_ - (__int64)_START_POINT_ + (__int64)1;

	// 분석할 바이트 위치 계산
	B = _BYTE_ - 1;

	/************************************************************************************/
	/*                              변수 설정 값 오류 검사                              */
	/************************************************************************************/
	if (TRACE_NUM < TN) {
		printf(" -----------------------------------------------------------------------\n");
		printf("|                    분석 파형 수가 적절치 않습니다.                    |\n");
		printf(" -----------------------------------------------------------------------\n");
		return _FAIL_;
	}

	if (_START_POINT_ < 1 || _START_POINT_ > _END_POINT_) {
		printf(" -----------------------------------------------------------------------\n");
		printf("|                분석 범위의 시작 지점이 적절치 않습니다.               |\n");
		printf(" -----------------------------------------------------------------------\n");
		return _FAIL_;
	}

	if (_END_POINT_ > POINTS) {
		printf(" -----------------------------------------------------------------------\n");
		printf("|                 분석 범위의 끝 지점이 적절치 않습니다.                |\n");
		printf(" -----------------------------------------------------------------------\n");
		return _FAIL_;
	}

	/************************************************************************************/
	/*                                   변수 동적할당                                   /
	/************************************************************************************/
	// 평문 저장
	Plaintext = (unsigned char*)malloc(_BLOCK_SIZE_ * sizeof(unsigned char));

	// 분석 포인트 저장
	Temp_Points = (double*)malloc((unsigned int)PI * sizeof(double));

	// 중간값 E[X] 저장
	H_S = (__int64*)malloc(_GUESS_KEY_NUM_ * sizeof(__int64));

	// 중간값 E[X^2] 저장
	H_SS = (__int64*)malloc(_GUESS_KEY_NUM_ * sizeof(__int64));

	// 파형 측정값 E[Y] 저장
	W_CS = (double*)malloc((unsigned int)PI * sizeof(double));

	// 파형 측정값 E[Y^2] 저장 
	W_CSS = (double*)malloc((unsigned int)PI * sizeof(double));

	// E[XY] 저장
	W_HS = (double**)malloc(_GUESS_KEY_NUM_ * sizeof(double*));
	for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
		W_HS[Guess_Key] = (double*)malloc((unsigned int)PI * sizeof(double));
	}
	unsigned char state[16] = { 0, };
	// 각 키 후보군 별 MAXPEAK 저장
	MaxPeak = (double*)malloc(_GUESS_KEY_NUM_ * sizeof(double));
	
	/************************************************************************************/
	/*                               First Order CPA 시작                               */
	/************************************************************************************/
	// 평문 파일 열기 (읽기 모드)
	sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%s.txt", _FOLD_, _PLAIN_FILE_);
	fopen_s(&fpp, FILE_MERGE, "r");
	if (fpp == NULL) {
		printf(" -----------------------------------------------------------------------\n");
		printf("|                        Failed To Read Plaintext                       |\n");
		printf(" -----------------------------------------------------------------------\n");
		return _FAIL_;
	}

	// 파형 파일 열기 (읽기 모드)
	sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%s.trace", _FOLD_, _TRACE_FILE_);
	fopen_s(&fpt, FILE_MERGE, "rb");
	if (fpt == NULL) {
		printf(" -----------------------------------------------------------------------\n");
		printf("|                          Failed To Read Trace                         |\n");
		printf(" -----------------------------------------------------------------------\n");
		return _FAIL_;
	}

	// 결과 저장할 폴더 생성
	sprintf_s(FOLD_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%04d_%02d_%02d_%02d_%02d_%02d", _FOLD_, TIME->tm_year + 1900, TIME->tm_mon + 1, TIME->tm_mday, TIME->tm_hour, TIME->tm_min, TIME->tm_sec);
	_mkdir(FOLD_MERGE);

	// 값 초기화
	for (pi = 0; pi < PI; pi++) {
		W_CS[pi] = 0.0;
		W_CSS[pi] = 0.0;
		for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
			W_HS[Guess_Key][pi] = 0.0;
		}
	}
	for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
		MaxPeak[Guess_Key] = 0;
		H_S[Guess_Key] = 0;
		H_SS[Guess_Key] = 0;
	}
	unsigned char state2[16] = { 0, };
	unsigned char state3[16] = { 0, };
	unsigned char state4[16] = { 0, };
	unsigned char state5[16] = { 0, };
	printf(" -----------------------------------------------------------------------\n");
	printf("|                            Loading Trace...                           |\n");
	printf(" -----------------------------------------------------------------------\n");

	for (tn = 0; tn < TN; tn++) {
		// 진행률 표시
		Percent = (unsigned int)((double)tn / (double)TN * 100);
		if (Percent % 10 == 0 && Percent != 0 && Check != Percent) {
			printf("%d%%\t", Percent);
			if (Percent == 90) {
				printf("\n");
			}
			Check = Percent;
		}

		// 평문 불러오기
		//_fseeki64(fpp, ((__int64)_BLOCK_SIZE_ * (__int64)3 + (__int64)2) * (__int64)tn, SEEK_SET);
		for (i = 0; i < _BLOCK_SIZE_; i++) {
			fscanf_s(fpp, "%hhx", &Plaintext[i]);
			//printf("%02x ", Plaintext[i]);
		}

		// 포인트 값 불러오기
		_fseeki64(fpt, 32 + ((__int64)POINTS * (__int64)tn + (__int64)_START_POINT_ - (__int64)1) * (__int64)4, SEEK_SET);
		for (pi = 0; pi < PI; pi++) {
			fread(&F_Temp, sizeof(float), 1, fpt);
			//printf("%f ", F_Temp);
			Temp_Points[pi] = (double)F_Temp;
		}

		// E[Y], E[Y^2] 계산
		for (pi = 0; pi < PI; pi++) {
			W_CS[pi] += Temp_Points[pi];
			W_CSS[pi] += Temp_Points[pi] * Temp_Points[pi];
		}
		
		for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
			// 분석 위치 (Plaintext Load : 0, 1R Sbox : 1)
#if _TARGET_ == 0
			
			
			for (int j = 0; j < 16; j++) {
				state[j] = Plaintext[j];
				state2[j] = 0;
				state3[j] = 0;
				state4[j] = 0;
				state5[j] = 0;
				//printf("%02x ", state[j]);
			}
			//1라운드 키에디션
			for (int k = 0; k < 16; k++) {
				state[k] ^= rk1[k];
				//printf("%02x ", state[k]);
			}
			//Key = invSBox2[state[1] ^ Guess_Key];
			// _GUESS_KEY_NUM_ 65536으로 두고 행렬 A의 특징을 이용하여 두개의 키를 한꺼번에 추출
			state[0] = SBox1[state[0]];
			state[1] = SBox2[state[1]/*^ (Guess_Key & 0xff)*/];
			state[2] = invSBox1[state[2]];
			state[3] = invSBox2[state[3]];
			state[4] = SBox1[state[4]];
			state[5] = SBox2[state[5]];
			state[6] = invSBox1[state[6]/* ^ (Guess_Key & 0xff)*/];
			state[7] = invSBox2[state[7]];
			state[8] = SBox1[state[8]];
			state[9] = SBox2[state[9]];
			state[10] = invSBox1[state[10]];
			state[11] = invSBox2[state[11]/* ^ (Guess_Key & 0xff)*/];
			state[12] = SBox1[state[12] /* ^ (Guess_Key & 0xff)*/];
			state[13] = SBox2[state[13]];
			state[14] = invSBox1[state[14]];
			state[15] = invSBox2[state[15]];
			
			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
					state2[i] ^= A[i][j] * state[j];
				}
			}
			
			//state[1] = SBox2[state[1]/*^ (Guess_Key & 0xff)*/]; 사용해야함 위로 올라가셈
			
			//Key = invSBox2[state2[5] ^ ((Guess_Key >> 8) & 0xff)]; ->1라운드 2번째 키 ???
			//Key = invSBox1[state2[8] ^ ((Guess_Key >> 8) & 0xff)]; -> 1라운드 2번째 키가 D3 라는 추측이 세워짐
			//Key = SBox2[state2[15] ^ ((Guess_Key >> 8) & 0xff)]; -> 1라운드 2번째 키가 D3라는 확신을 줌
			// 
			//Key = invSBox1[state2[0] ^ ((Guess_Key >> 8) & 0xff)]; -> 1라운드 7번째 키 후보 4D
			//Key = SBox1[state2[10] ^ ((Guess_Key >> 8) & 0xff)]; -> 1라운드 7번째 키 후보 4D 확신
			//Key = invSBox2[state2[13] ^ ((Guess_Key >> 8) & 0xff)];// -> 1라운드 7번째 키 후보 4D 확신
			// 
			//Key = SBox2[state2[3] ^ ((Guess_Key >> 8) & 0xff)];// -> 1라운드 12번째 키 후보 76 추측
			//Key = invSBox1[state2[4] ^ ((Guess_Key >> 8) & 0xff)];// -> 1라운드 12번째 키 후보 76 확신
			//Key = SBox1[state2[14] ^ ((Guess_Key >> 8) & 0xff)];// -> 1라운드 12번째 키 후보 76 확신
			// 
			//Key = invSBox2[state2[1] ^ ((Guess_Key >> 8) & 0xff)];// -> 1라운드 13번째 키 후보 c7 ??
			//Key = SBox1[state2[6] ^ ((Guess_Key >> 8) & 0xff)];// -> 1라운드 13번째 키 후보 CE 확신
			//Key = SBox2[state2[11] ^ ((Guess_Key >> 8) & 0xff)];// -> 1라운드 13번째 키 후보 CE 확신
			
			//2라운드 키 추측 //여기부터는  _GUESS_KEY_NUM_ 65536->256 으로 바꾸기 
			//Key = invSBox1[state2[0] ^ Guess_Key]; ->0xBA
			//Key = invSBox2[state2[1] ^ Guess_Key]; //->0xE3 ??
			//Key = SBox1[state2[2] ^ Guess_Key]; //->0x64
			//Key = SBox2[state2[3] ^ Guess_Key]; //->0x1F
			//Key = invSBox1[state2[4] ^ Guess_Key]; //->0x9e
			//Key = invSBox2[state2[5] ^ Guess_Key]; //->0x99
			//Key = SBox1[state2[6] ^ Guess_Key]; //->0x65
			//Key = SBox2[state2[7] ^ Guess_Key]; //->0xFC
			//Key = invSBox1[state2[8] ^ Guess_Key]; //->0x3d
			//Key = invSBox2[state2[9] ^ Guess_Key]; //->0xED
			//Key = SBox1[state2[10] ^ Guess_Key]; //->0x67
			//Key = SBox2[state2[11] ^ Guess_Key]; //->0xEA
			//Key = invSBox1[state2[12] ^ Guess_Key]; //->0x8d
			//Key = invSBox2[state2[13] ^ Guess_Key]; //->0x51
			//Key = SBox1[state2[14] ^ Guess_Key]; //->0x5E
			//Key = SBox2[state2[15] ^ Guess_Key]; //->0xa7
			
			for (int j = 0; j < 16; j++) {
				state2[j] ^= rk2[j];
			
			}
			state2[0] = invSBox1[state2[0]];
			state2[1] = invSBox2[state2[1]];
			state2[2] = SBox1[state2[2]];
			state2[3] = SBox2[state2[3]];
			state2[4] = invSBox1[state2[4]];
			state2[5] = invSBox2[state2[5]];
			state2[6] = SBox1[state2[6]];
			state2[7] = SBox2[state2[7]];
			state2[8] = invSBox1[state2[8]];
			state2[9] = invSBox2[state2[9]];
			state2[10] = SBox1[state2[10]];
			state2[11] = SBox2[state2[11]];
			state2[12] = invSBox1[state2[12]];
			state2[13] = invSBox2[state2[13]];
			state2[14] = SBox1[state2[14]];
			state2[15] = SBox2[state2[15]];
			
			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
					state3[i] ^= A[i][j] * state2[j];
				}
			}
			
			//Key = SBox1[state3[0] ^ (Guess_Key)];
			//Key = SBox2[state3[1]^ Guess_Key];
			//Key = invSBox1[state3[2] ^ Guess_Key];
			//Key = invSBox2[state3[3] ^ Guess_Key];
			//Key = SBox1[state3[4] ^ Guess_Key];
			//Key = SBox2[state3[5] ^ Guess_Key];
			//Key = invSBox1[state3[6] ^ Guess_Key];
			//Key = invSBox2[state3[7] ^ Guess_Key];
			//Key = SBox1[state3[8] ^ Guess_Key];
			//Key = SBox2[state3[9] ^ Guess_Key];
			//Key = invSBox1[state3[10]^ Guess_Key];
			//Key = invSBox2[state3[11]^ Guess_Key];
			//Key = SBox1[state3[12] ^ (Guess_Key & 0xff)];
			//Key = SBox2[state3[13]^ (Guess_Key)];
			//Key = invSBox1[state3[14]^ (Guess_Key)];
			//Key = invSBox2[state3[15] ^ (Guess_Key)];
			for (int j = 0; j < 16; j++) {
				state3[j] ^= rk3[j];

			}

			state3[0] = SBox1[state3[0]];
			state3[1] = SBox2[state3[1]];
			state3[2] = invSBox1[state3[2]];
			state3[3] = invSBox2[state3[3]];
			state3[4] = SBox1[state3[4]];
			state3[5] = SBox2[state3[5]];
			state3[6] = invSBox1[state3[6]];
			state3[7] = invSBox2[state3[7]];
			state3[8] = SBox1[state3[8]];
			state3[9] = SBox2[state3[9]];
			state3[10] = invSBox1[state3[10]];
			state3[11] = invSBox2[state3[11]];
			state3[12] = SBox1[state3[12]];
			state3[13] = SBox2[state3[13]];
			state3[14] = invSBox1[state3[14]];
			state3[15] = invSBox2[state3[15]];

			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
					state4[i] ^= A[i][j] * state3[j];
				}
			}
			//rk4[16] = {0x10, 0xa0, 0xdb, 0xa0,0x09,0xee,0x84,0xdd, 0xd0, 0xcd, 0x60, 0x61, 0x33, 0x22, 0x62, 0x66};
			/*int S = 0;
			S = state4[15] ^ Guess_Key;
			Key = SBox2[S];*/
			for (int j = 0; j < 16; j++) {
				state4[j] ^= rk4[j];

			}
			state4[0] = invSBox1[state4[0]];
			state4[1] = invSBox2[state4[1]];
			state4[2] = SBox1[state4[2]];
			state4[3] = SBox2[state4[3]];
			state4[4] = invSBox1[state4[4]];
			state4[5] = invSBox2[state4[5]];
			state4[6] = SBox1[state4[6]];
			state4[7] = SBox2[state4[7]];
			state4[8] = invSBox1[state4[8]];
			state4[9] = invSBox2[state4[9]];
			state4[10] = SBox1[state4[10]];
			state4[11] = SBox2[state4[11]];
			state4[12] = invSBox1[state4[12]];
			state4[13] = invSBox2[state4[13]];
			state4[14] = SBox1[state4[14]];
			state4[15] = SBox2[state4[15]];

			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
					state5[i] ^= A[i][j] * state4[j];
				}
			}
			
			/*int S = 0;
			S = state5[15] ^ Guess_Key;
			Key = invSBox2[S];*/
#elif _TARGET_ == 1
			Key = AES_S[Plaintext[B] ^ (Guess_Key + _GUESS_KEY_START_)];
#else
			printf(" -----------------------------------------------------------------------\n");
			printf("|                          Failed to Set Target                         |\n");
			printf(" -----------------------------------------------------------------------\n");
			return _FAIL_;
#endif

			// Hamming Weight 계산
			Key_HW = (Key & 1) + ((Key >> 1) & 1) + ((Key >> 2) & 1) + ((Key >> 3) & 1) + ((Key >> 4) & 1) + ((Key >> 5) & 1) + ((Key >> 6) & 1) + ((Key >> 7) & 1);

			// E[X], E[X^2] 계산
			H_S[Guess_Key] += (__int64)Key_HW;
			H_SS[Guess_Key] += (__int64)(Key_HW * Key_HW);

			// E[XY] 계산
			for (pi = 0; pi < PI; pi++) {
				W_HS[Guess_Key][pi] += (double)Key_HW * Temp_Points[pi];
			}
		}

#if _MAX_PEAK_TRACE_
		// Max Peak Trace 저장
		if (!((tn + 1) % _TRACE_UNIT_)) {
			// Max Peak Trace 저장할 파일 열기
			sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%d_MAX_PEAK_UNIT_%d.txt", FOLD_MERGE, B + 1, _TRACE_UNIT_);
			fopen_s(&fp, FILE_MERGE, "a");

			for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
				for (pi = 0; pi < PI; pi++) {
					Correlation_L = (double)(tn + 1) * W_HS[Guess_Key][pi] - W_CS[pi] * (double)H_S[Guess_Key];
					Correlation_R = ((double)(tn + 1) * (double)H_SS[Guess_Key] - (double)H_S[Guess_Key] * (double)H_S[Guess_Key]) * ((double)(tn + 1) * W_CSS[pi] - W_CS[pi] * W_CS[pi]);

					if (Correlation_R <= (double)0) {
						Correlation = (double)0;
					}
					else {
						Correlation = Correlation_L / sqrt(Correlation_R);
						Correlation = fabs(Correlation);
					}

					if (MaxPeak[Guess_Key] < Correlation) {
						MaxPeak[Guess_Key] = Correlation;
					}
				}

				fprintf_s(fp, "%f ", MaxPeak[Guess_Key]);
			}

			fprintf_s(fp, "\n");

			fclose(fp);

			// 값 초기화
			for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
				MaxPeak[Guess_Key] = 0;
			}
		}
#endif
	}

	printf(" -----------------------------------------------------------------------\n");
	printf("|              Correlation Power Analysis On Key Candidates             |\n");
	printf(" -----------------------------------------------------------------------\n");

#if _CORRELATION_TRACE_
	sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%d", FOLD_MERGE, B + 1);
	_mkdir(FILE_MERGE);
#endif

	// 키에 대한 상관계수 계산 및 결과 값 저장
	for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
#if _CORRELATION_TRACE_
		sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%d\\%03d(0x%02X).txt", FOLD_MERGE, B + 1, Guess_Key + _GUESS_KEY_START_, Guess_Key + _GUESS_KEY_START_);
		fopen_s(&fp, FILE_MERGE, "w");
#endif

		for (pi = 0; pi < PI; pi++) {
			Correlation_L = (double)TN * W_HS[Guess_Key][pi] - W_CS[pi] * (double)H_S[Guess_Key];
			Correlation_R = ((double)TN * (double)H_SS[Guess_Key] - (double)H_S[Guess_Key] * (double)H_S[Guess_Key]) * ((double)TN * W_CSS[pi] - W_CS[pi] * W_CS[pi]);

			if (Correlation_R <= (double)0) {
				Correlation = (double)0;
			}
			else {
				Correlation = Correlation_L / sqrt(Correlation_R);
				Correlation = fabs(Correlation);
			}

#if _CORRELATION_TRACE_
			fprintf_s(fp, "%f\n", Correlation);
#endif

			if (MaxPeak[Guess_Key] < Correlation) {
				MaxPeak[Guess_Key] = Correlation;

#if _GUESS_KEY_NUM_ == 1
				// 최종 분석 결과 저장
				Index = (unsigned int)pi;

				sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%d_Result.txt", FOLD_MERGE, B + 1);
				fopen_s(&fp2, FILE_MERGE, "w");

				fprintf_s(fp2, "Point		: %d\n", Index + _START_POINT_);
				fprintf_s(fp2, "Correlation	: %f", MaxPeak[Guess_Key]);

				fclose(fp2);
#endif
			}
		}

#if _CORRELATION_TRACE_
		fclose(fp);
#endif
	}

#if _GUESS_KEY_NUM_ > 1
	// Guess Key Peak 저장
	sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%d_GUESS_KEY_PEAK.txt", FOLD_MERGE, B + 1);
	fopen_s(&fp, FILE_MERGE, "w");

	// 최종 분석 키 저장
	sprintf_s(FILE_MERGE, _FILE_NAME_SIZE_ * sizeof(char), "%s\\%d_RIGHT_KEY.txt", FOLD_MERGE, B + 1);
	fopen_s(&fp2, FILE_MERGE, "w");

	Max = 0;
	for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
		fprintf_s(fp, "%f\n", MaxPeak[Guess_Key]);

		if (Max < MaxPeak[Guess_Key]) {
			Max = MaxPeak[Guess_Key];
			R_Key = Guess_Key;
		}
	}

	fclose(fp);

	fprintf_s(fp2, "  1st  0x%02X  %f\n", R_Key, Max);

	MaxPeak[R_Key] = 0.0;

	for (i = 1; i < _CANDIDATES_; i++) {
		for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
			if (Max_Sec < MaxPeak[Guess_Key]) {
				Max_Sec = MaxPeak[Guess_Key];
				Right_Key = Guess_Key;
			}
		}

		if (i == 1) {
			fprintf_s(fp2, "  2nd  0x%02X  %f\n", Right_Key, Max_Sec);
			Ratio = Max / Max_Sec;
		}
		else if (i == 2) {
			fprintf_s(fp2, "  3rd  0x%02X  %f\n", Right_Key, Max_Sec);
		}
		else {
			fprintf_s(fp2, "%3dth  0x%02X  %f\n", i + 1, Right_Key, Max_Sec);
		}

		MaxPeak[Right_Key] = 0.0;
		Max_Sec = 0.0;
		Right_Key = 0;
	}

	fprintf_s(fp2, "\nRatio  %f\n", Ratio);

	fclose(fp2);
#endif


	fclose(fpp);
	fclose(fpt);

	/************************************************************************************/
	/*                                  동적 할당 해제                                  */
	/************************************************************************************/
	free(Plaintext);
	free(Temp_Points);
	free(H_S);
	free(H_SS);
	free(W_CS);
	free(W_CSS);
	for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
		free(W_HS[Guess_Key]);
	}
	free(W_HS);
	free(MaxPeak);

	return _PASS_;

}
