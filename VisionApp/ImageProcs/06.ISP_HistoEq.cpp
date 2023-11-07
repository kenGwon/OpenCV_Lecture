#pragma concept

#include "Common.h"
#include "ISP.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);

	ISP _isp;

	/*
	  1. ������ ��ȯ color converting
	*/

	// ���� �Լ�
	cv::Mat gray = cv::Mat(src.rows, src.cols, CV_8UC1);
	_isp.Convert_BGR2GRAY(src.data, src.cols, src.rows, gray.data);

	{
		// openCV ���� �Լ�
		cv::Mat gray_opencv = cv::Mat(src.rows, src.cols, CV_8UC1);
		cv::cvtColor(src, gray_opencv, ColorConversionCodes::COLOR_BGR2GRAY);

		// �����Լ��� �����Լ��� ���̰� �ִ��� ����
		cv::Mat cc_diff = cv::Mat(src.rows, src.cols, CV_8UC1);
		cc_diff = gray - gray_opencv;
		double error = cv::sum(cc_diff)[0];
	}

	/*
	  2. ������׷� ��Ȱȭ histogram equalization
	*/
	
	// ���� �Լ�
	cv::Mat gray_eq = cv::Mat(src.rows, src.cols, CV_8UC1);
	_isp.Enhance_HistogramEq(gray.data, gray.cols, gray.rows, gray_eq.data);

	{
		// openCV ���� �Լ�
		cv::Mat gray_eq_opencv = cv::Mat(src.rows, src.cols, CV_8UC1);
		cv::equalizeHist(gray, gray_eq_opencv);

		// �����Լ��� �����Լ��� ���̰� �ִ��� ����
		cv::Mat he_diff = cv::Mat(src.rows, src.cols, CV_8UC1);
		he_diff = gray_eq - gray_eq_opencv;
		Scalar sum = cv::sum(he_diff);
	}
	
	// ����� �Ȱ�����, opencv ���̺귯���� �Լ��� �����ս��� �ξ� �پ��.

	return 1;
}