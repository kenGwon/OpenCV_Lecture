#pragma once
#include "Common.h"

int main()
{
	std::string fileName = "../KCCImageNet/stinkbug.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);
	cv::resize(src, src, cv::Size(src.cols * 2, src.rows * 2));
	int datas[10] = { 5,3,1,4,6,4,8,4,3 };
	int* pDatas = datas;//data array

	uchar* pData = src.data;//data array...start[0,1,2,3,4,5]
	int length = src.total();//data length
	int channels = src.channels();
	//length *= channels;




	const int histoSz = 256;//sizeof(uchar);
	int histo[histoSz] = { 0, };
	//int* pHisto = new int[length];

	for (size_t i = 0; i < length; i++)
	{
		histo[pData[i]]++;
	}

	uchar* data = src.data;
	int size = src.rows * src.cols;
	auto result = std::minmax_element(pData, pData + length);
	double mean_brightness = cv::mean(src)[0];
	auto _minmax = std::minmax_element(histo, histo + histoSz);
	int _min_value = *_minmax.first;
	int _max_value = *_minmax.second;


	std::string strLine;
	for (size_t row = 0; row < histoSz; row++)
	{
		strLine = "";
		strLine += to_string(row);
		strLine += ("   ||");

		int scaled_cols = (histo[row] * 1.0 / _max_value) * 100;
		for (size_t col = 0; col < scaled_cols; col++)
		{
			strLine += ("=");
		}
		std::cout << strLine << std::endl;
	}

	cv::Mat draw = src.clone();
	cvtColor(draw, draw, ColorConversionCodes::COLOR_GRAY2BGR);
	for (size_t row = 0; row < histoSz; row++)
	{
		int scaled_cols = (histo[row] * 1.0 / _max_value) * src.cols;
		cv::Point st = Point(0, row * 2.7);
		cv::Point ed = Point(scaled_cols, row * 2.7);
		cv::line(draw, st, ed, CV_RGB(255, 255, 0));
	}

	int a = 0;
	


#if 0
	//std::string fileName = "../KCCImageNet/stinkbug.png";
	std::string fileName = "../KCCImageNet/stop_img.png";
	//cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_COLOR);
	
	uchar* pData = src.data; // data array는 "픽셀 갯수 * 채널 갯수" 크기의 배열이고, 그 안에는 0~255 범위 값들이 들어가 있다.
	int length = src.total(); // total()함수는 픽셀의 갯수를 리턴하는 함수이다.

	int channel = src.channels();
	length *= channel;


	// historgram을 구하시오. (src 이미지 버퍼)
	const int RGB_range = 256;
	int counts_R[RGB_range] = { 0, };
	int counts_G[RGB_range] = { 0, };
	int counts_B[RGB_range] = { 0, };

	for (size_t i = 0; i < length; i++)
	{
		if (i % 3 == 0) counts_R[*(pData + i)]++;
		if (i % 3 == 1) counts_G[*(pData + i)]++;
		if (i % 3 == 2) counts_B[*(pData + i)]++;
	}


	for (size_t i = 0; i < RGB_range; i++)
	{
		std::cout << "R ";
		std::cout.width(3);
		std::cout << i << " | ";
		for (size_t j = 0; j < counts_R[i]; j++) std::cout << "=";
		std::cout << counts_R[i] << std::endl;

	}

	for (size_t i = 0; i < RGB_range; i++)
	{
		std::cout << "G ";
		std::cout.width(3);
		std::cout << i << " | ";
		for (size_t j = 0; j < counts_G[i]; j++) std::cout << "=";
		std::cout << counts_G[i] << std::endl;
	}

	for (size_t i = 0; i < RGB_range; i++)
	{
		std::cout << "B ";
		std::cout.width(3);
		std::cout << i << " | ";
		for (size_t j = 0; j < counts_B[i]; j++) std::cout << "=";
		std::cout << counts_B[i] << std::endl;
	}
#endif

#if 0
	// Quiz:: 가장 많은/적은 빈도수의 값을 출력하시오
	// 가장 많은 빈도 값: 4... 3개
	// 가장 적은 빈도 값: 1 ... 1개
	//                   5 ... 1개
	//                   8 ... 1개
	const int length = 3 * 3;

	int datas[length] = { 5, 3, 1, 4, 6, 4, 8, 4, 3 };
	int counts[10] = { 0 };

	for (size_t i = 0; i < length; i++)
	{
		counts[datas[i]]++;
	}

	int size = sizeof(counts) / sizeof(counts[0]);
	int max = *std::max_element(counts, counts + size);
	int min = *std::min_element(counts, counts + size) + 1;
	bool flag = false;

	std::cout << "가장 많은 빈도 값: ";
	for (size_t i = 0; i < size; i++)
	{
		if (counts[i] == max)
		{
			if (flag)
			{
				std::cout << std::endl << "\t\t   ";
			}
			std::cout << i << " ... " << max << "개";
			flag = true;
		}
	}
	std::cout << std::endl;

	flag = false;
	std::cout << "가장 적은 빈도 값: ";
	for (size_t i = 0; i < size; i++)
	{
		if (counts[i] == min)
		{
			if (flag)
			{
				std::cout << std::endl << "\t\t   ";
			}
			std::cout << i << " ... " << min << "개";
			flag = true;
		}
	}
	std::cout << std::endl;
#endif
#if 0
	const int length = 3 * 3;
	const int cnt_length = 3 * 3;

	int datas[length] = { 5, 3, 1, 4, 6, 4, 8, 4, 3 };
	int counts[cnt_length] = { 0 };

	int _max_count_val = -1;
	int _min_count_val = 1000;

	for (size_t i = 0; i < length; i++)
	{
		counts[datas[i]]++;

		if (_max_count_val < counts[datas[i]])_max_count_val = counts[datas[i]];
		if (_min_count_val > counts[datas[i]])_min_count_val = counts[datas[i]];
	}
#endif

	return 1; 
}
