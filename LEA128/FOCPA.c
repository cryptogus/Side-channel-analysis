#include "parameter.h"

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

void CPA()
{
	/************************************************************************************/
	/*                                     변수 선언                                    */
	/************************************************************************************/
	FILE			*trace								= fopen(trace_path, "rb");
	FILE			*fpp								= fopen(input_path, "r");
	FILE			*fpp2								= fopen(output_path, "r");

	unsigned int	i									= 0;		// for (_BLOCK_SIZE, _CANDIDATES_)
	unsigned int	Guess_Key							= 0;
	unsigned int	Key									= 0;
	unsigned int	Key_HW								= 0;
	unsigned int	R_Key								= 0;
	unsigned int	Right_Key							= 0;


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


	u_int8_t	*input									= NULL;
	u_int8_t	* output 								= NULL;
	
	// 분석할 파형 및 포인트 수 계산
	TN = (__int64)_TRACE_NUM_;
	PI = (__int64)_END_POINT_ - (__int64)_START_POINT_ + (__int64)1;

	
	char state2 = 0;
	char sum[8] = {0,};
	trace_HEADER header;
	fread(&header, sizeof(header), 1, trace);
	printf(" ---------------------------Trace Information---------------------------\n");
	printf("|	Number of Trace	:	%d				|\n", header.trace_no);
	printf("|	Number of Point	:	%d				|\n", header.point_no);
	printf(" -----------------------------------------------------------------------\n");
	/************************************************************************************/
	/*                                   변수 동적할당                                   */
	/************************************************************************************/
	// 평문 저장
	input = (unsigned char *)malloc(_BLOCK_SIZE_ * sizeof(unsigned char));
	output = (unsigned char *)malloc(_BLOCK_SIZE_2 * sizeof(unsigned char));

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
	
	
	printf(" -----------------------------------------------------------------------\n");
	printf("|                            Loading Trace...                           |\n");
	printf(" -----------------------------------------------------------------------\n");
	unsigned int plaintext[4] = { 0, };	
	for (tn = 0; tn < TN; tn++) {
		
		// input or output 불러오기
		//_fseeki64(fpp, ((__int64)_BLOCK_SIZE_ * (__int64)3 + (__int64)2) * (__int64)tn, SEEK_SET);
		for (i = 0; i < _BLOCK_SIZE_; i++) {
			fscanf(fpp, "%hhx", &input[i]);
			//printf("%02x ", input[i]);
		}
		//printf("\n");
		
		// 포인트 값 불러오기
		fseeko(trace, 32 + (tn*header.point_no + _START_POINT_ - 1)*4, SEEK_SET);
		for (pi = 0; pi < PI; pi++) {
			fread(&F_Temp, sizeof(float), 1, trace);
			//printf("%f ", F_Temp);
			Temp_Points[pi] = (double)F_Temp;
			//printf("%g ",Temp_Points[pi]);
		}

		// E[Y], E[Y^2] 계산
		for (pi = 0; pi < PI; pi++) {
			W_CS[pi] += Temp_Points[pi];
			W_CSS[pi] += Temp_Points[pi] * Temp_Points[pi];
			//printf("%g ",W_CS[pi]);
			//printf("%g ",W_CSS[pi]);
		}

		for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
			state2 = 0;
			byte2state(input, plaintext);
			
			//(T[0], T[1], T[2], T[1], T[3], T[1])
			//Key = (plaintext[3] ^ Guess_Key)+(plaintext[2]^0x06)&0xff;
			
			//Key = ((plaintext[2] ^ (Guess_Key << 24)) >> 24) & 0xff;
			/////////////////////////////////////Key = (((plaintext[1] ^ 0xb249ffbf) + (plaintext[0] ^0x52cc3a5a ^ (Guess_Key << 24))) >> 24) & 0xff;
			/////////////////////////////////////Key = (((plaintext[2] ^ 0xb249ffbf) + (plaintext[1]^ 0x4206ed9d^ (Guess_Key << 24))) >> 24) & 0xff;
			/////////////////////////////////////Key = (((plaintext[3]^0xb249ffbf ^ (Guess_Key<<8))+(plaintext[2]^0xc4a14a06))>>8)&0xff;
			Key = (((plaintext[3]^ 0xb249ffbf)) +(plaintext[2]^0x4a06 ^ (Guess_Key << 16)) >> 16) & 0xff;

			// Hamming Weight 계산
			Key_HW = (Key & 1) + ((Key >> 1) & 1) + ((Key >> 2) & 1) + ((Key >> 3) & 1) + ((Key >> 4) & 1) + ((Key >> 5) & 1) + ((Key >> 6) & 1) + ((Key >> 7) & 1);
			//printf("%d ", Key_HW);
			// E[X], E[X^2] 계산
			H_S[Guess_Key]  += Key_HW;
			H_SS[Guess_Key] += (Key_HW * Key_HW);
			//printf("%d ",H_S[Guess_Key]);
			//printf("%d ",H_SS[Guess_Key]);
			// E[XY] 계산
			for (pi = 0; pi < PI; pi++) {
				W_HS[Guess_Key][pi] += (double)Key_HW * Temp_Points[pi];
				//printf("%g ",W_HS[Guess_Key][pi]);
			}
		}
	}
	printf(" -----------------------------------------------------------------------\n");
	printf("|              Correlation Power Analysis On Key Candidates             |\n");
	printf(" -----------------------------------------------------------------------\n");

	// 키에 대한 상관계수 계산 및 결과 값 저장
	for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
		for (pi = 0; pi < PI; pi++) {
			//printf("%g ", ((double)TN * W_HS[Guess_Key][pi]));
			//printf("%g ", (W_CS[pi] * (double)H_S[Guess_Key]);
			Correlation_L = (double)TN * W_HS[Guess_Key][pi] - W_CS[pi] * (double)H_S[Guess_Key];
			Correlation_R = ((double)TN * (double)H_SS[Guess_Key] - (double)H_S[Guess_Key] * (double)H_S[Guess_Key]) * ((double)TN * W_CSS[pi] - W_CS[pi] * W_CS[pi]);
			//printf("%g ", Correlation_L);
			//printf("%g ", Correlation_R);
			if (Correlation_R <= (double)0.0) {
				Correlation = (double)0.0;
			}
			else {
				Correlation = Correlation_L / sqrt(Correlation_R);
				Correlation = fabs(Correlation);
			}
			//printf("%g ", Correlation);
			if (MaxPeak[Guess_Key] < Correlation) {
				MaxPeak[Guess_Key] = Correlation;
			}
		}
		//printf("%d = %g\n",Guess_Key, MaxPeak[Guess_Key]);
	}

	Max = 0;
	for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {

		if (Max < MaxPeak[Guess_Key]) {
			Max = MaxPeak[Guess_Key];
			R_Key = Guess_Key;
		}
	}
	fprintf(stdout,"\nValue of weight when BNN input(8 nodes, Think of each node as 1 bits) multiplied by weight\n\n");
	fprintf(stdout, "  1st  0x%02X  %f\n", R_Key, Max);

	MaxPeak[R_Key] = 0.0;

	for (i = 1; i < 10; i++) {
		for (Guess_Key = 0; Guess_Key < _GUESS_KEY_NUM_; Guess_Key++) {
			if (Max_Sec < MaxPeak[Guess_Key]) {
				Max_Sec = MaxPeak[Guess_Key];
				Right_Key = Guess_Key;
			}
		}
		if (i == 1) {
			fprintf(stdout, "  2nd  0x%02X  %g\n", Right_Key, Max_Sec);
		}
		else if (i == 2) {
			fprintf(stdout, "  3rd  0x%02X  %g\n", Right_Key, Max_Sec);
		}
		else{
			fprintf(stdout, "%3dth  0x%02X  %f\n", i + 1, Right_Key, Max_Sec);
		}

		MaxPeak[Right_Key] = 0.0;
		Max_Sec = 0.0;
		Right_Key = 0;
	}

	fclose(trace);
	fclose(fpp);
	fclose(fpp2);

	/************************************************************************************/
	/*                                  동적 할당 해제                                  */
	/************************************************************************************/
	free(input);
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
}
