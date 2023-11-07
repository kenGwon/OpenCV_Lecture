#include "ISP.h"

ISP::ISP()
{
	std::cout << "ISP::Ctor" << std::endl;
}

ISP::~ISP()
{
	std::cout << "ISP::Dtor" << std::endl;
}

bool ISP::Convert_BGR2GRAY(uchar* pBGR, int cols, int rows, uchar* pGray)
{
	if (pBGR == nullptr || pGray == nullptr)
	{
		return false;
	}

	Mat src = Mat(rows, cols, CV_8UC3, pBGR);
	Mat gray_cv = Mat(rows, cols, CV_8UC1, pGray);
	cvtColor(src, gray_cv, COLOR_BGR2GRAY);

	return true;

#if 0 // �����ս� ���� opencv ���̺귯�� �Լ��� ����ϱ� ���� ��ũ��ġ �ڵ�� disable
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			int index = row * cols + col;
			int val_Y = 0.299 * pBGR[index * 3 + 2] // R
				+ 0.587 * pBGR[index * 3 + 1] // G
				+ 0.114 * pBGR[index * 3 + 0]; // B
			pGray[index] = (uchar)(val_Y + 0.5); // openCV�����Լ��� �Ȱ��� ����� ���� "�ø�"���� (+ 0.5)
		}
	}

	return true;
#endif
}

bool ISP::Get_Historgram(uchar* pGray, int cols, int rows, int* pHisto, int histoSz)
{
	if (pGray == nullptr || pHisto == nullptr)
	{
		return false;
	}


	for (size_t i = 0; i < cols * rows; i++)
	{
		if (*(pGray + i) >= histoSz) continue; // size 256
		pHisto[*(pGray + i)]++;
	}
	
	return false;
}

/*
  desc: GRAY_SCALE�� pGray������ ������׷� ��Ȱȭ�Ͽ� pGrayEq�� ����ش�.
*/
bool ISP::Enhance_HistogramEq(uchar* pGray, int cols, int rows, uchar* pGrayEq)
{
	if (pGray == nullptr || pGrayEq == nullptr)
	{
		return false;
	}
	
	Mat gray = Mat(rows, cols, CV_8UC1, pGray);
	Mat grayEq = Mat(rows, cols, CV_8UC1, pGrayEq);
	equalizeHist(gray, grayEq);

	return true;

#if 0 // �����ս� ���� opencv ���̺귯�� �Լ��� ����ϱ� ���� ��ũ��ġ �ڵ�� disable
	// ������׷� ����
	const int histoSz = 256;
	int histo[256] = { 0, };
	Get_Historgram(pGray, cols, rows, histo, histoSz);

	// Ȯ���е��Լ�
	int acc[histoSz] = { 0, };
	acc[0] = histo[0];
	for (size_t i = 1; i < histoSz; i++) // 1���� ����
	{
		acc[i] = acc[i - 1] + histo[i]; // �׷��� ������ [i - 1]
	}

	// new Look Up Table ... ������ �� ȭ�� ������ ���ο� ���������� ����
	int length = cols * rows;
	int new_Gray[histoSz] = { 0, };
	for (size_t i = 0; i < histoSz; i++)
	{
		new_Gray[i] = static_cast<int>(1.0 * acc[i] * 255 / length + 0.5); // openCV�����Լ��� �Ȱ��� ����� ���� "�ø�"���� (+ 0.5)
	}

	// assign new Gray value using LookUpTable
	for (size_t i = 0; i < length; i++)
	{
		*(pGrayEq + i) = new_Gray[*(pGray + i)];
	}
	
	return true;
#endif
}
