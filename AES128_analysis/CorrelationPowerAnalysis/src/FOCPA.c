#include "../include/parameter.h"

//Hamming Weight
unsigned int HW(unsigned long long x)
{
	const unsigned long long m1 = 0x5555555555555555;
	const unsigned long long m2 = 0x3333333333333333;
	const unsigned long long m4 = 0x0f0f0f0f0f0f0f0f;
	x -= (x >> 1) & m1;             //put count of each 2 bits into those 2 bits
	x = (x & m2) + ((x >> 2) & m2); //put count of each 4 bits into those 4 bits 
	x = (x + (x >> 4)) & m4;        //put count of each 8 bits into those 8 bits 
	x += x >> 8;  //put count of each 16 bits into their lowest 8 bits
	x += x >> 16;  //put count of each 32 bits into their lowest 8 bits
	x += x >> 32;  //put count of each 64 bits into their lowest 8 bits
	return x & 0x7f;
}
//int HW[256] = { //AES SBox기준 해밍웨이트 테이블
//  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2,
//  2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3,
//  2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4,
//  4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4,
//  2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4,
//  4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5,
//  4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6,
//  6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
//  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3,
//  3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5,
//  4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4,
//  4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6,
//  4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5,
//  5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7,
//  6, 7, 7, 8 };




void CPA() {
	
	FILE* trace = fopen(trace_path, "rb");
	FILE* plain = fopen(plain_path, "r");
	
	//동적할당을 위한 변수
	unsigned int byte = last_byte - start_byte + 1; 
	unsigned long long point_num = _END_POINT_ - _START_POINT_ + 1;
	
	trace_HEADER header;
	fread(&header, sizeof(header), 1, trace);
	printf(" ---------------------------Trace Information---------------------------\n");
	printf("|	Number of Trace	:	%d				|\n", header.trace_no);
	printf("|	Number of Point	:	%d				|\n", header.point_no);
	printf(" -----------------------------------------------------------------------\n");

	
	double* po_trace = (double*)calloc(sizeof(double), point_num);
	
	unsigned char* plaintext = (unsigned char*)calloc(sizeof(unsigned char), byte);

	unsigned int* middle = (unsigned int*)calloc(sizeof(unsigned int), 256);
	
	unsigned int* index = (unsigned int*)calloc(sizeof(unsigned int), byte);

	double* keymaxCorrs = (double*)calloc(sizeof(double), 256);

	double* MaxCorrs = (double*)calloc(sizeof(double), byte);
	
	double* ratio = (double*)calloc(sizeof(double), byte);

	double* sum_x = (double*)calloc(sizeof(double), point_num);
	
	double* sum_y = (double*)calloc(sizeof(double), 256);
	
	double** sum_xy = (double**)calloc(sizeof(double*), 256);
	
    for (int i = 0; i < 256; i++) {
		sum_xy[i] = (double*)calloc(sizeof(double), point_num);
	}

	double* sum_x2 = (double*)calloc(sizeof(double), point_num);
	
	double* sum_y2 = (double*)calloc(sizeof(double), 256);

	unsigned int* second_key = (unsigned int*)calloc(sizeof(unsigned int), byte);
	
	unsigned int point;
	float pointer;
	double under_Corr = 0;
	double up_Corr = 0;
	double Corr = 0;
	
    for (unsigned int i = 0; i < byte; i++) {
		
		//Corr = 0;
		for (int a = 0; a < 256; a++)
		{	
			keymaxCorrs[a] = 0;
		}
		fseek(plain, 3 * (start_byte - 1), SEEK_SET); //plain.txt 파일은 띄어쓰기까지 총 3번 건너뛰어야함 ex)읽어 온 후 캐럿의 위치 다음에 오는게 0xff라면 "ff "만큼 건너 뛰어야함
 		fseek(trace, 32  + sizeof(float) * (_START_POINT_ - 1), SEEK_SET);
		for (int i = 0; i < 256; i++) {
			sum_y[i] = 0;
			sum_y2[i] = 0;
			for (point = 0; point < point_num; point++) {
				sum_xy[i][point] = 0;
			}
		}
		for (point = 0; point < point_num; point++) {
			sum_x[point] = 0;
			sum_x2[point] = 0;
		}
		for (unsigned int j = 0; j < _TRACE_NUM_; j++) {
			
			for (unsigned int j = 0; j < byte; j++) {
				fscanf(plain, "%hhx", &plaintext[j]);
                printf("%02x ",plaintext[j]);
			}
            printf("\n");
			fseek(plain, 3 * (Block_Size - byte + 1) - 1, SEEK_CUR);
			
			
			for (unsigned long long pi = 0; pi < point_num; pi++) {
				fread(&pointer, sizeof(float), 1, trace);//파형정보 불러오기
				po_trace[pi] = (double)pointer;
			}
																																
			fseek(trace, sizeof(float)*(header.point_no - point_num), SEEK_CUR);
																																  
			for (point = 0; point < point_num; point++) {

				sum_x[point] = sum_x[point] + po_trace[point]; //상관계수 X변수 값들의 합
				sum_x2[point] = sum_x2[point] + po_trace[point] * po_trace[point]; //제곱
			}

			for (unsigned int key = 0; key < 0x100; key++) {

				middle[key] = HW(S_Box[plaintext[i] ^ key]); //subbytes HW
				sum_y[key] = sum_y[key] + middle[key];
				for (point = 0; point < point_num; point++) {
					sum_xy[key][point] = sum_xy[key][point] + po_trace[point] * middle[key];
				}

				sum_y2[key] = sum_y2[key] + (double)middle[key] * middle[key];
			}
		}
		for (unsigned int key = 0; key < 0x100; key++) {
			
			for (point = 0; point < point_num; point++) {
				under_Corr = ((_TRACE_NUM_ * sum_x2[point]) - (sum_x[point] * sum_x[point]))* ((_TRACE_NUM_ * sum_y2[key]) - (sum_y[key] * sum_y[key]));
				up_Corr = (_TRACE_NUM_ * sum_xy[key][point]) - (sum_x[point] * sum_y[key]);
				//Corr = fabs(((_TRACE_NUM_ *sum_xy[key][point]) - (sum_x[point] * sum_y[key]))/ sqrt(((_TRACE_NUM_ *sum_x2[point]) - (sum_x[point] * sum_x[point])) * ((_TRACE_NUM_ * sum_y2[key]) - (sum_y[key] * sum_y[key]))));// 상관관계 절댓값
				if (under_Corr <= (double)0) {
					under_Corr = (double)0;
				}
				else {
					Corr = up_Corr / sqrt(under_Corr);
					Corr = fabs(Corr);
				}
				//키 값 구하기
				if (MaxCorrs[i] < Corr) {
					MaxCorrs[i] = Corr;
					index[i] = key;
				}
				//키 후보별 최대 상관계수
				if (keymaxCorrs[key] < Corr) {
					keymaxCorrs[key] = Corr;
				}
			}

		}
		for (unsigned int a = 0; a < 256; a++)
		{
			if ((keymaxCorrs[a] > ratio[i]) && (a != index[i]))
			{
				second_key[i] = a;
				ratio[i] = keymaxCorrs[a];
			}
		}
		ratio[i] = MaxCorrs[i] / ratio[i];

	}
	printf("Master Key: ");
	for (unsigned int i = 0; i < byte; i++) {
		printf("%02x ", index[i]);
	}
	printf("\n\n");
	for (unsigned int i = 0; i < byte; i++) {
		printf("%02dst byte maximum correlation coefficient is %g \n", start_byte + i ,MaxCorrs[i]);
	}
	printf("\n\n");
	for (unsigned int i = 0; i < byte; i++) {
		printf("%02dst byte ratio %g   The second candidate: 0x%02x\n", start_byte + i, ratio[i], second_key[i]);
	}
	free(sum_x), free(sum_y), free(sum_x2), free(sum_y2);
	for (unsigned long long i = 0; i < 256; i++) {
		free(sum_xy[i]);
	}
	free(sum_xy);
	free(ratio);
	free(MaxCorrs);
	free(keymaxCorrs);
	free(po_trace);
	free(plaintext);
	free(middle);
	free(index);
	free(second_key);

	fclose(plain);
	fclose(trace);

    printf("asd\n");
}