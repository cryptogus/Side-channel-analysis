
// First_Order_CPA.c
//

#include "Analysis.h"
#include "Table.h"

typedef unsigned int word;
typedef unsigned char byte;

word ROR(word x, word i) {
	return ((x) >> (i)) | ((x) << (32 - (i)));
}

#define GETU32(pt) (((word)(pt)[3] << 24) ^ ((word)(pt)[2] << 16) ^ ((word)(pt)[1] <<  8) ^ ((word)(pt)[0]))
#define PUTU32(ct, st) { (ct)[3] = (byte)((st) >> 24); (ct)[2] = (byte)((st) >> 16); (ct)[1] = (byte)((st) >>  8); (ct)[0] = (byte)(st); }

//=====
// 바이트 b[16] --> 워드 state[4]
void byte2state(unsigned char b[16], unsigned int state[4]) {
	state[0] = GETU32(b);
	state[1] = GETU32(b + 4);
	state[2] = GETU32(b + 8);
	state[3] = GETU32(b + 12);
}


int First_Order_CPA(struct tm *TIME, unsigned int POINTS, unsigned int TRACE_NUM)
{
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

	unsigned int	i									= 0;		// for (_BLOCK_SIZE, _CANDIDATES_)
	unsigned int	B									= 0;		// Byte
	unsigned int	Guess_Key							= 0;
	unsigned int	Key									= 0;
	unsigned int	Key_HW								= 0;
	unsigned int	R_Key								= 0;
	unsigned int	Right_Key							= 0;
	unsigned int	Index								= 0;
	unsigned int	Percent								= 0;
	unsigned int	Check								= 0;

	__int64			*H_S								= NULL;
	__int64			*H_SS								= NULL;
	__int64			tn									= 0;		// for (TN)
	__int64			pi									= 0;		// for (PI)
	__int64			TN									= 0;		// Trace Number
	__int64			PI									= 0;		// Point Interval

	float			F_Temp								= 0.;

	double			*Temp_Points						= NULL;
	double			*MaxPeak							= NULL;
	double			*W_CS								= NULL;
	double			*W_CSS								= NULL;
	double			**W_HS								= NULL;
	double			Correlation							= 0.;
	double			Correlation_L						= 0.;
	double			Correlation_R						= 0.;
	double			Max									= 0.;
	double			Max_Sec								= 0.;
	double			Ratio								= 0.;

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
	Plaintext = (unsigned char *)malloc(_BLOCK_SIZE_ * sizeof(unsigned char));

	// 분석 포인트 저장
	Temp_Points = (double *)malloc((unsigned int)PI * sizeof(double));

	// 중간값 E[X] 저장
	H_S = (__int64 *)malloc(_GUESS_KEY_NUM_ * sizeof(__int64));

	// 중간값 E[X^2] 저장
	H_SS = (__int64 *)malloc(_GUESS_KEY_NUM_ * sizeof(__int64));

	// 파형 측정값 E[Y] 저장
	W_CS = (double *)malloc((unsigned int)PI * sizeof(double));

	// 파형 측정값 E[Y^2] 저장 
	W_CSS = (double *)malloc((unsigned int)PI * sizeof(double));

	// E[XY] 저장
	W_HS = (double **)malloc(_GUESS_KEY_NUM_ * sizeof(double *));
	for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
		W_HS[Guess_Key] = (double *)malloc((unsigned int)PI * sizeof(double));
	}

	// 각 키 후보군 별 MAXPEAK 저장
	MaxPeak = (double *)malloc(_GUESS_KEY_NUM_ * sizeof(double));

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
		
		for (i = 0; i < _BLOCK_SIZE_; i++) {
			//평문을 잘못 읽어오네여~ 한칸씩 밀림
			fscanf_s(fpp, "%hhx", &Plaintext[i]);
			//printf("%02x ", Plaintext[i]);
		}
		//printf("\n");

		// 포인트 값 불러오기
		_fseeki64(fpt, 32 + ((__int64)POINTS * (__int64)tn + (__int64)_START_POINT_ - (__int64)1) * (__int64)4, SEEK_SET);
		for (pi = 0; pi < PI; pi++) {
			fread(&F_Temp, sizeof(float), 1, fpt);
			Temp_Points[pi] = (double)F_Temp;
		}

		// E[Y], E[Y^2] 계산
		for (pi = 0; pi < PI; pi++) {
			W_CS[pi] += Temp_Points[pi];
			W_CSS[pi] += Temp_Points[pi] * Temp_Points[pi];
		}
		int GUESS = 0;
		for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
			// 분석 위치 (Plaintext Load : 0, 1R Sbox : 1)
			
#if _TARGET_ == 0
			unsigned int plaintext[4] = { 0, };
			byte2state(Plaintext, plaintext);
			
			//(T[0], T[1], T[2], T[1], T[3], T[1])
			//Key = (plaintext[3] ^ Guess_Key)+(plaintext[2]^0x06)&0xff;
			
			//Key = ((plaintext[2] ^ (Guess_Key << 24)) >> 24) & 0xff;
			/////////////////////////////////////Key = (((plaintext[1] ^ 0xb249ffbf) + (plaintext[0] ^0x52cc3a5a ^ (Guess_Key << 24))) >> 24) & 0xff;
			/////////////////////////////////////Key = (((plaintext[2] ^ 0xb249ffbf) + (plaintext[1]^ 0x4206ed9d^ (Guess_Key << 24))) >> 24) & 0xff;
			/////////////////////////////////////Key = (((plaintext[3]^0xb249ffbf ^ (Guess_Key<<8))+(plaintext[2]^0xc4a14a06))>>8)&0xff;
			Key = (((plaintext[3]^ 0xb249ffbf)) +(plaintext[2]^0x4a06 ^ (Guess_Key << 16)) >> 16) & 0xff;
#elif _TARGET_ == 1
			//4a->ca
			unsigned int plaintext[4] = { 0, };
			byte2state(Plaintext, plaintext);
			Key = (plaintext[B] ^ Guess_Key) & 0xff;
#else
			printf(" -----------------------------------------------------------------------\n");
			printf("|                          Failed to Set Target                         |\n");
			printf(" -----------------------------------------------------------------------\n");
			return _FAIL_;
#endif

			// Hamming Weight 계산
			Key_HW = (Key & 1) + ((Key >> 1) & 1) + ((Key >> 2) & 1) + ((Key >> 3) & 1) + ((Key >> 4) & 1) + ((Key >> 5) & 1) + ((Key >> 6) & 1) + ((Key >> 7) & 1);

			// E[X], E[X^2] 계산
			H_S[Guess_Key]  += (__int64)Key_HW;
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
