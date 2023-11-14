#pragma once

#include "Common.h"
#include "ISP.h"
#include <numeric>

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	cv::Mat src_gray_blur = src_gray.clone();
	src_gray_blur = 0;
	
	const int kernel_sz = 5;
	int half_kernelSize = kernel_sz / 2;

	const float gaussian_sigma = 1;
	vector<float> kernel;
	for (int x = -half_kernelSize; x <= half_kernelSize; x++)
	{
		for (int y = -half_kernelSize; y <= half_kernelSize; y++)
		{
			kernel.push_back(
				1 / (2 * CV_PI * gaussian_sigma * gaussian_sigma) * (exp(-(x * x + y * y) / (2 * gaussian_sigma * gaussian_sigma)))
			);
		}
	}

	for (size_t row = half_kernelSize; row < src_gray.rows - half_kernelSize; row++)
	{
		for (size_t col = half_kernelSize; col < src_gray.cols - half_kernelSize; col++)
		{
			int sum = 0;
			for (int k_row = -half_kernelSize; k_row <= half_kernelSize; k_row++)
			{
				for (int k_col = -half_kernelSize; k_col <= half_kernelSize; k_col++)
				{
					int index = (row + k_row) * src_gray.cols + (col + k_col);
					int f_index = (k_row + half_kernelSize) * kernel_sz + (k_col + half_kernelSize);
					sum += src_gray.data[index] * kernel[f_index];
				}
			}
			int index = (row)*src_gray.cols + (col);
			src_gray_blur.data[index] = cv::saturate_cast<uchar> (static_cast<uchar>(sum));
		}
	}

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


	return 1;
}

