
// main.c
//

#include "Analysis.h"

int main()
{
	time_t			timer;

	__int64			a, b;
	struct			tm t;

	unsigned int	POINTS		= 0;
	unsigned int	TRACE_NUM	= 0;
	unsigned int	Judgement	= 0;
	
	timer = time(NULL);
	localtime_s(&t, &timer);

	// ���� ��� Ȯ��
	if (POINT_VERIFY(&POINTS, &TRACE_NUM) == _FAIL_)
		return _FAIL_;

	// �ð� ���� ����
	a = time(NULL);

	// FOCPA
	Judgement = First_Order_CPA(&t, POINTS, TRACE_NUM);

	// �ð� ���� ��
	b = time(NULL);

	// CPA ���� ���� ���� ����Ʈ
	if (Judgement == _FAIL_) {
		printf(" -----------------------------------------------------------------------\n");
		printf("|                        First Order CPA Failure                        |\n");
		printf(" -----------------------------------------------------------------------\n");
		return _FAIL_;
	}
	else {
		LOG(&t, a, b);
		printf(" -----------------------------------------------------------------------\n");
		printf("|	Total Analysis Time = %I64d Hours %I64d Min %I64d Sec			|\n", (b - a) / 3600, ((b - a) / 60) % 60, (b - a) % 60);
		printf(" -----------------------------------------------------------------------\n");
	}

	return _PASS_;
}
