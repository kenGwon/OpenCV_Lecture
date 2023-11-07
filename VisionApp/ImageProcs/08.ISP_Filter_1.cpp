#pragma once

#include "Common.h"
#include "ISP.h"
#include <numeric>

int main()
{
	int datas[] = { 6, 4, 8, 9, 4, 4, 8, 64, 4, 6, 4, 8, 6, 4, 11, 1, 3, 1134, 5, 64, 5, 64 };

	/*
		노이즈가 없는 대표 신호 값 하나를 정하시오.

		1. outlier detection (how?)
		  - 중앙값과의 차이가 평균값를 넘어가면 outlier로 판정

		2. 대표 값을 정한다. (how?)
		  - 중앙값?
		  - 최다 빈출 값?
		  - 평균값?
	*/
	
	int length = sizeof(datas) / sizeof(datas[0]);
	std::sort(datas, datas + length);
	int median = datas[length / 2];	

	int sum = 0;
	for (size_t i = 0; i < length; i++) sum += datas[i];
	int mean = sum / length - 1; // 우연의 일친지 모르겠는데 그냥 계산하면 평균값이 64.xxxx로 나와서 뒤에서 64가 안걸러짐. 그래서 1 빼줌.

	vector<int> new_datas;
	for (size_t i = 0; i < length; i++)
	{
		if (datas[i] - median > mean) continue;
		else new_datas.push_back(datas[i]);
	}
	
	double sum_vector = accumulate(new_datas.begin(), new_datas.end(), 0);
	double result = sum_vector / new_datas.size();
	

	///////////////////////////////
	// 필터를 이용한 blur 자작코드 // 
	///////////////////////////////

	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	// 아 이런식으로도 동일한 크기의 행렬을 만들어서 초기화 해줄 수 있구나
	cv::Mat src_gray_blur = src_gray.clone();
	src_gray_blur = 0;

	//filter 3x3 ... 1/9
	const int filter_sz = 3;
	float blur[] = {
		1.0 / 9, 1.0 / 9, 1.0 / 9,
		1.0 / 9, 1.0 / 9, 1.0 / 9,
		1.0 / 9, 1.0 / 9, 1.0 / 9,
	};

	//for (size_t row = 0; row < src_gray.rows - filter_sz; row++)
	for (size_t row = 0; row < src_gray.rows; row++)
	{
		//for (size_t col = 0; col < src_gray.cols - filter_sz; col++)
		for (size_t col = 0; col < src_gray.cols; col++)
		{
			int sum = 0;
			for (size_t f_row = 0; f_row < filter_sz; f_row++)
			{
				for (size_t f_col = 0; f_col < filter_sz; f_col++)
				{
					int index = (row + f_row) * src_gray.cols + (col + f_col);
					int f_index = (f_row)*filter_sz + (f_col);
					sum += src_gray.data[index] * blur[f_index];
				}
			}
			int index = (row)*src_gray.cols + (col);
			src_gray_blur.data[index] = static_cast<uchar>(sum);
		}
	}

	
	return 1;
}
