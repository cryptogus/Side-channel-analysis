#ifndef _PARAM_
#define _PARAM_
// If you want to know the weights of the hidden nodes, erase this micro.
#define first

// If you want to open and use a large file larger than 2GB, you should define it as follows.
#define __USE_LARGEFILE64 //fseeko64() so on..
// https://www.ibm.com/docs/ko/i/7.3?topic=functions-fopen-open-files
// https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=programsite&logNo=150013719920
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
typedef unsigned long long __int64;
/***
 *BNN 가중치 range you want to find
*/
#define _BYTE_					1
#define	_BLOCK_SIZE_				8			//number of nodes - input: 8, output: 4
#define _BLOCK_SIZE_2				4

/***
 * File path 
*/
#define trace_path	"../BNN.trace"
#define input_path	"../input.txt"
#define output_path	"../output.txt"

/***
 * Number of waveforms
*/
#define _TRACE_NUM_				1000
/***
 * Interval to start analysis
*/
#define _START_POINT_			1
#define _END_POINT_				1120
/***
 * BNN input has a block size of 1 bytes (8 nodes).
 * BNN output has a block size of 0.5 bytes(4 nodes).
*/

#define _GUESS_KEY_START_			0
#define _GUESS_KEY_NUM_				256

typedef struct TRACE_HEADER {

	unsigned char strID[20];
	unsigned int trace_no;
	unsigned int point_no;
	unsigned char strEND[4];
}trace_HEADER;

#endif