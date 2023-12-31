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
		  - 중앙값과의 차이가 평균값을 넘어가면 outlier로 판정

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
	int mean = sum / length - 10; // 우연의 일친지 모르겠는데 그냥 계산하면 평균값이 64.xxxx로 나와서 뒤에서 64가 안걸러짐. 그래서 1 빼줌.

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


	//float blur[] = {
	//	1.0 / 9, 1.0 / 9, 1.0 / 9,
	//	1.0 / 9, 1.0 / 9, 1.0 / 9,
	//	1.0 / 9, 1.0 / 9, 1.0 / 9,
	//};

	//float blur[] = {
	//	1.0 / 16, 2.0 / 16, 1.0 / 16,
	//	2.0 / 16, 4.0 / 16, 2.0 / 16,
	//	1.0 / 16, 2.0 / 16, 1.0 / 16,
	//};

	float blur[] = {
		2.0 / 25, 3.0 / 25, 2.0 / 25,
		3.0 / 25, 5.0 / 25, 3.0 / 25,
		2.0 / 25, 3.0 / 25, 2.0 / 25,
	};


#if 0 // 이미지가 필터 사이즈만큼 좌상으로 이동해 있는 문제가 있는 코드
	for (size_t row = 0; row < src_gray.rows - filter_sz; row++)
	{
		for (size_t col = 0; col < src_gray.cols - filter_sz; col++)
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
#else // 문제가 해결된 교수님 코드 + 하지만 여전히 필터링을 하다보니 맨 끝에 연산되지 않은 픽셀이 존재함... interpolation(보간법) 필요!
	int half_kernelSize = filter_sz / 2;
	for (size_t row = half_kernelSize; row < src_gray.rows - half_kernelSize; row++)
	{
		for (size_t col = half_kernelSize; col < src_gray.cols - half_kernelSize; col++)
		{
			int sum = 0;
			for (int f_row = -half_kernelSize; f_row <= half_kernelSize; f_row++)
			{
				for (int f_col = -half_kernelSize; f_col <= half_kernelSize; f_col++)
				{
					int index = (row + f_row) * src_gray.cols + (col + f_col);
					int f_index = (f_row + half_kernelSize) * filter_sz + (f_col + half_kernelSize);
					sum += src_gray.data[index] * blur[f_index];
				}
			}
			int index = (row)*src_gray.cols + (col);
			src_gray_blur.data[index] = static_cast<uchar>(sum);
		}
	}
#endif

	// 최근접 보간법(Nearest Neighbor interpolation) 끝픽셀 채우기: 인접한 셀의 값을 그대로 대입
	for (size_t row = 1; row < src_gray_blur.rows - 1; row++)
	{
		// copy row+1 to row
		src_gray_blur.data[src_gray_blur.cols * row] 
			= src_gray_blur.data[src_gray_blur.cols * row + 1];
		// copy row to row+1
		src_gray_blur.data[src_gray_blur.cols * row + src_gray_blur.rows - 1] 
			= src_gray_blur.data[src_gray_blur.cols * row + src_gray_blur.rows - 2];
	}
	for (size_t col = 1; col < src_gray_blur.cols - 1; col++)
	{
		// copy col to col+1
		src_gray_blur.data[col] 
			= src_gray_blur.data[src_gray_blur.rows + col];
		// copy col+1 to col 
		src_gray_blur.data[src_gray_blur.rows * (src_gray_blur.cols - 1) + col]
			= src_gray_blur.data[src_gray_blur.rows * (src_gray_blur.cols - 2) + col];
	}

	// 대각 코너 4개 점은 인접한 두 픽셀의 평균값을 취하기
	// 좌상
	src_gray_blur.data[0]
		= (src_gray_blur.data[0 + 1] + src_gray_blur.data[0 + src_gray_blur.cols]) / 2;
	// 우상
	src_gray_blur.data[src_gray_blur.rows - 1]
		= (src_gray_blur.data[src_gray_blur.rows - 1 - 1] + src_gray_blur.data[src_gray_blur.rows * 2 - 1]) / 2;
	// 좌하
	src_gray_blur.data[src_gray_blur.cols * (src_gray_blur.rows - 1)]
		= (src_gray_blur.data[src_gray_blur.cols * (src_gray_blur.rows - 2)] + src_gray_blur.data[src_gray_blur.cols * (src_gray_blur.rows - 1) + 1]) / 2;
	// 우하
	src_gray_blur.data[src_gray_blur.cols * src_gray_blur.rows - 1]
		= (src_gray_blur.data[src_gray_blur.cols * src_gray_blur.rows - 1 - 1] + src_gray_blur.data[src_gray_blur.cols * (src_gray_blur.rows - 1 - 1) + src_gray_blur.rows - 1]) / 2;


	return 1;
}

