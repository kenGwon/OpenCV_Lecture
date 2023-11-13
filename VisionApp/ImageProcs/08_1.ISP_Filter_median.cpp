#pragma once

#include "Common.h"
#include "ISP.h"

int main()
{
	std::string fileName = "../KCCImageNet/lena_gray_saltpepper.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	// 구현코드
	cv::Mat my_salt_pepper(Size(src.rows, src.cols), CV_8UC1);

	const int filter_sz = 3;
	vector<uchar> kernel;
	const int kernel_median_idx = 4;

	for (size_t row = 0; row < src.rows; row++)
	{
		for (size_t col = 0; col < src.cols; col++)
		{
			for (size_t k_row = 0; k_row < filter_sz; k_row++)
			{
				for (size_t k_col = 0; k_col < filter_sz; k_col++)
				{
					int index = (row + k_row) * src.cols + (col + k_col);
					kernel.push_back(src.data[index]);
				}
			}
			sort(kernel.begin(), kernel.end());

			int index = (row)*src.cols + (col);
			my_salt_pepper.data[index] = kernel[kernel_median_idx];

			kernel.clear();
		}
	}

	// 좌상으로 커널 사이즈만큼 이동하는 문제 해결하려면...
	// 커널에서 픽셀 인덱스를 참조할때 시작점에서 0,1,2로 볼게 아니라 -1,0,1로 보도록 해야함. 
	// 그걸 할라고 하프사이즈 구해서 "-"붙이고 한것.
	int half_filter_sz = filter_sz / 2; // 1.5에서 1로 타입캐스팅
	Mat my_salt_pepper_fix(Size(src.rows, src.cols), CV_8UC1);

	for (size_t row = half_filter_sz; row < src.rows - half_filter_sz; row++)
	{
		for (size_t col = half_filter_sz; col < src.cols - half_filter_sz; col++)
		{
			for (int k_row = -half_filter_sz; k_row <= half_filter_sz; k_row++) // -1, 0, 1
			{
				for (int k_col = -half_filter_sz; k_col < half_filter_sz; k_col++) // -1, 0, 1
				{
					int index = (row + k_row) * src.cols + (col + k_col);
					kernel.push_back(src.data[index]);
				}
			}
			sort(kernel.begin(), kernel.end());

			int index = (row)*src.cols + (col);
			my_salt_pepper_fix.data[index] = kernel[kernel_median_idx];

			kernel.clear();
		}
	}

	// openCV 정답
	Mat opencv_salt_pepper;
	medianBlur(src, opencv_salt_pepper, 3);

	return 1;
}