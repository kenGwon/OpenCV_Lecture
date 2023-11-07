#pragma once

#include "Common.h"

class ISP
{
public:
	ISP();
	~ISP();

	bool Convert_BGR2GRAY(
		uchar* pBGR, // color channel, input 영상
		int cols, // input 영상의 cols
		int rows, // input 영상의 row
		uchar* pGray // mono channel, output 영상
	);
	bool Get_Historgram(
		uchar* pGray,
		int cols, // input 영상의 cols
		int rows, // input 영상의 row
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
