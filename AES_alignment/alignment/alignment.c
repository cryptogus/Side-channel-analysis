#include "alignment.h"

int main()
{
    FILE* fp = NULL;
	
	FILE* fpt = NULL;


	__int64			index = 0; //가장 큰 상관계수를 가질 때 시작 point
	
	__int64			tn = 0;		// for (TN)
	__int64			pi = 0;		// for (PI)
	__int64			TN = 0;		// Trace Number
	__int64			PI = 0;		// Point Interval
	__int64			PI2 = 0;		// Point Interval

	float			F_Temp = 0.;

	double* Temp_Points = NULL;
	double* Temp_Points2 = NULL;
	


	double* MaxPeak = NULL;

	double			Correlation = 0.;
	double			Correlation_L = 0.;
	double			Correlation_R = 0.;
	double			Max = 0.;
	double			Max_Sec = 0.;

	double			Ratio = 0.;
	fpt = fopen("../AES-ALIGN.trace","rb");
	fp = fopen("../index.txt","w");
	// 분석할 파형 및 포인트 수 계산
	TN = (__int64)_TRACE_NUM_;
	PI = (__int64)_END_POINT_ - (__int64)_START_POINT_ + (__int64)1;
	PI2 = (__int64)_END_POINT_2 - (__int64)_START_POINT_2 + (__int64)1;

	/************************************************************************************/
	/*                                   변수 동적할당                                   /
	/************************************************************************************/
	
	// 분석 포인트 저장
	Temp_Points = (double*)malloc((unsigned int)PI * sizeof(double));

	Temp_Points2 = (double*)malloc((unsigned int)PI2 * sizeof(double));

	//파형1 측정값 E[X] 저장
	double H_S = 0;

	//파형1 측정값 E[X^2] 저장
	double H_SS = 0;

	// 파형2 측정값 E[Y] 저장
	double W_CS = 0;

	// 파형2 측정값 E[Y^2] 저장 
	double W_CSS = 0;

	// E[XY] 저장
	double W_HS = 0;
	fprintf(fp, "0\n", index);
	for (int k = 0; k < _TRACE_NUM_; k++) {//k에 의해 다음 파형으로 넘어감
		//새로운 파형에서의 값을 구하기위해 초기화
		Correlation = 0;
		index = 0;
		for (int j = 0; j < _POINT_; j++) {
			
			//값 초기화
			for (pi = 0; pi < PI; pi++) {

				Temp_Points[pi] = 0;
				Temp_Points2[pi] = 0;

			}
			W_CS = 0;
			W_CSS = 0;
			W_HS = 0;
			H_S = 0;
			H_SS = 0;

			// 파형 1 불러오기
			fseeko(fpt, 32 + ((__int64)_POINT_NUM_ * (_TARGET_TRACE_1 - 1) + (__int64)_START_POINT_ - (__int64)1) * (__int64)4, SEEK_SET);
			for (pi = 0; pi < PI; pi++) {
				fread(&F_Temp, sizeof(float), 1, fpt);
				//printf("%f ", F_Temp);
				Temp_Points[pi] = (double)F_Temp;
				//printf("%g ", Temp_Points[pi]);
			}
			//printf("\n");
			// 파형 2 불러오기 -> 다음 포인트 구간 잡기 (__int64)_START_POINT_2 + j - (__int64)1), _TARGET_TRACE_2 - 1 + k -> 파형 하나하나 비교
			fseeko(fpt, 32 + ((__int64)_POINT_NUM_ * (_TARGET_TRACE_2 - 1 + k) + (__int64)_START_POINT_2 + j - (__int64)1) * (__int64)4, SEEK_SET);
			for (pi = 0; pi < PI2; pi++) {
				fread(&F_Temp, sizeof(float), 1, fpt);
				//printf("%f ", F_Temp);
				Temp_Points2[pi] = (double)F_Temp;
			}

			// E[X], E[X^2] 계산
			for (pi = 0; pi < PI; pi++) {
				W_CS += Temp_Points[pi];
				W_CSS += Temp_Points[pi] * Temp_Points[pi];
			}


			// E[Y], E[Y^2] 계산
			for (pi = 0; pi < PI2; pi++) {
				H_S += Temp_Points2[pi];
				H_SS += Temp_Points2[pi] * Temp_Points2[pi];
			}


			// E[XY] 계산
			for (pi = 0; pi < PI; pi++) {

				W_HS += Temp_Points[pi] * Temp_Points2[pi];

			}

			Max = (PI * W_HS - W_CS * H_S) / sqrt((PI * W_CSS - W_CS * W_CS) * (PI * H_SS - H_S * H_S));
			if (fabs(Max) > fabs(Correlation)) {
				Correlation = fabs(Max);
				index = _START_POINT_2 + j;
			}

		}
		fprintf(fp, "%lld\n", index);
	}
    fclose(fpt);
	fclose(fp);

	free(Temp_Points);
	free(Temp_Points2);
}