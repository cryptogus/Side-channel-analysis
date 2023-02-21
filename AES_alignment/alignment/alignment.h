
/*----------------------------------------------------------------------------------*/
/*  파형 수																			*/
/*  전체 포인트 수																	*/
/*----------------------------------------------------------------------------------*/
#define _TRACE_NUM_					500
#define _POINT_NUM_					17110
/*----------------------------------------------------------------------------------*/
/*  분석할 파형																		*/
/*----------------------------------------------------------------------------------*/
/*  분석할 파형 순서 선택															*/
/*----------------------------------------------------------------------------------*/
#define _TARGET_TRACE_1					1
#define _TARGET_TRACE_2					2

/*----------------------------------------------------------------------------------*/
/*  분석 시작 포인트																*/
/*  분석 끝 포인트																	*/
/*----------------------------------------------------------------------------------*/
#define _START_POINT_				201
#define _END_POINT_					650

#define _START_POINT_2				1
#define _END_POINT_2				450
/*----------------------------------------------------------------------------------*/
/*  분석할 포인트 구간	-> 파형 2의 구간을 움직이는 범위							*/
/*----------------------------------------------------------------------------------*/
#define _POINT_						16600

typedef struct TRACE_HEADER {
	unsigned char strID[20];
	unsigned int trace_no;
	unsigned int point_no;
	unsigned char strEND[4];
} trace_HEADER;
typedef unsigned long long __int64;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>