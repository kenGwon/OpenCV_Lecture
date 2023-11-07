#pragma concept

#include "Common.h"
#include "ISP.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);

	ISP _isp;

	/*
	  1. 색영역 변환 color converting
	*/

	// 자작 함수
	cv::Mat gray = cv::Mat(src.rows, src.cols, CV_8UC1);
	_isp.Convert_BGR2GRAY(src.data, src.cols, src.rows, gray.data);

	{
		// openCV 공식 함수
		cv::Mat gray_opencv = cv::Mat(src.rows, src.cols, CV_8UC1);
		cv::cvtColor(src, gray_opencv, ColorConversionCodes::COLOR_BGR2GRAY);

		// 자작함수와 공식함수에 차이가 있는지 검증
		cv::Mat cc_diff = cv::Mat(src.rows, src.cols, CV_8UC1);
		cc_diff = gray - gray_opencv;
		double error = cv::sum(cc_diff)[0];
	}

	/*
	  2. 히스토그램 평활화 histogram equalization
	*/
	
	// 자작 함수
	cv::Mat gray_eq = cv::Mat(src.rows, src.cols, CV_8UC1);
	_isp.Enhance_HistogramEq(gray.data, gray.cols, gray.rows, gray_eq.data);

	{
		// openCV 공식 함수
		cv::Mat gray_eq_opencv = cv::Mat(src.rows, src.cols, CV_8UC1);
		cv::equalizeHist(gray, gray_eq_opencv);

		// 자작함수와 공식함수에 차이가 있는지 검증
		cv::Mat he_diff = cv::Mat(src.rows, src.cols, CV_8UC1);
		he_diff = gray_eq - gray_eq_opencv;
		Scalar sum = cv::sum(he_diff);
	}
	
	// 결과는 똑같지만, opencv 라이브러리의 함수는 퍼포먼스가 훨씬 뛰어나다.

	return 1;
}