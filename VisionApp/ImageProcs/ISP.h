#pragma once

#include "Common.h"

class ISP
{
public:
	ISP();
	~ISP();

	bool Convert_BGR2GRAY(
		uchar* pBGR, // color channel, input ����
		int cols, // input ������ cols
		int rows, // input ������ row
		uchar* pGray // mono channel, output ����
	);
	bool Get_Historgram(
		uchar* pGray,
		int cols, // input ������ cols
		int rows, // input ������ row
		int* pHisto,
		int histoSz
	);
	bool Enhance_HistogramEq(
		uchar* pGray,
		int cols, 
		int rows, 
		uchar* pGrayEq
	);

private:

};
