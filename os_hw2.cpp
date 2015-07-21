// 운영체제_과제2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "mmio.h"
#include "FileIoHelperClass.h"
#include "StopWatch.h"
#include<stdint.h>

int _tmain(int argc, _TCHAR* argv[])
{
	uint32_t uSize = 5000; // 4gb 이상 가능
	LARGE_INTEGER Size;    // 

	DWORD bufSize = 2048;  // 32bit가 4gb 인데 실질적으로 2gb 사용 가능
	// 그러니 최대인 2048M 씩 할당
	LARGE_INTEGER offset;  // 크기
	offset.QuadPart = (LONGLONG)0;

	StopWatch sw1, sw2;



	Size.QuadPart = (LONGLONG)(1024 * 1024) * (LONGLONG)uSize;;

	_ASSERTE(create_very_big_file(L"big.txt", uSize));

	sw1.Start();
	_ASSERTE(file_copy_using_read_write(L"big.txt", L"big2.txt"));
	sw1.Stop();
	print("info] time elapsed = %f", sw1.GetDurationSecond());


	PUCHAR buf = (PUCHAR)malloc(bufSize);


	sw2.Start();

	FileIoHelper Helper;
	Helper.FIOpenForRead(L"big.txt");
	Helper.FIOCreateFile(L"big3.txt", Size);

	while (offset.QuadPart < Size.QuadPart)
	{
		if ((Size.QuadPart - offset.QuadPart) >(LONGLONG)bufSize)
			bufSize = 2048;
		else
			bufSize = (DWORD)(Size.QuadPart - offset.QuadPart);

		Helper.FIOReadFromFile(offset, bufSize, buf);
		Helper.FIOWriteToFile(offset, bufSize, buf);

		offset.QuadPart += (LONGLONG)bufSize;
	}
	sw2.Stop();
	print("info] time elapsed = %f", sw2.GetDurationSecond());


	return 0;
}