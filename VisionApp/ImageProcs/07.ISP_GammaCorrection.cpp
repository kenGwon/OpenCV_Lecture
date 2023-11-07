#pragma once

#include "Common.h"
#include "ISP.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	ISP _isp;

	uchar* pixels = src.data;
	int length = src.total();

	double gamma = 2.5;
	cv::Mat src_gamma = cv::Mat(src.rows, src.cols, CV_8UC1);

	for (size_t i = 0; i < length; i++)
	{
		//double normalized_value = (double)pixels[i] / 255.0;
		//double corrected_value = pow(normalized_value, gamma) * 255.0;
		//*(src_gamma.data + i) = (uchar)corrected_value;
		src_gamma.data[i] = static_cast<uchar>(std::pow(pixels[i] / 255.0, gamma) * 255.0); // 위에 3줄을 단 한줄로
	}
	
	return 1;
}
