#pragma once

#include "Common.h"
#include "ISP.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	// 구현코드
	cv::Mat my_Sobel(Size(src.rows, src.cols), CV_8UC1);
	cv::Mat my_Sobel_dx(Size(src.rows, src.cols), CV_8UC1);
	cv::Mat my_Sobel_dy(Size(src.rows, src.cols), CV_8UC1);

	int hori_filter[] = {
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1
	};

	int ver_filter[] = {
		1, 0, -1,
		2, 0, -2,
		1, 0, -1
	};

	int threshold = 128;
	const int filter_sz = 3;
	int half_filterSize = filter_sz / 2;

	for (size_t row = half_filterSize; row < src.rows - half_filterSize; row++)
	{
		for (size_t col = half_filterSize; col < src.cols - half_filterSize; col++)
		{
			int horizontal_gradient = 0;
			int vertical_gradient = 0;
			for (int f_row = -half_filterSize; f_row <= half_filterSize; f_row++)
			{
				for (int f_col = -half_filterSize; f_col <= half_filterSize; f_col++)
				{
					int index = (row + f_row) * src.cols + (col + f_col);
					int f_index = (f_row + half_filterSize) * filter_sz + (f_col + half_filterSize);
					horizontal_gradient += src.data[index] * hori_filter[f_index];
					vertical_gradient += src.data[index] * ver_filter[f_index];
				}
			}
			int index = (row)*src.cols + (col);
			my_Sobel.data[index] = static_cast<uchar>(horizontal_gradient);
			//sobel_edge.data[index] = (unsigned char)sqrt((double)(grad_x * grad_x + grad_y * grad_y));//td::abs(grad_x) + std::abs(grad_y);
			if (abs(horizontal_gradient) + abs(vertical_gradient) > threshold)
			{
				my_Sobel.data[index] = 255;
			}
			else
			{
				my_Sobel.data[index] = 0;
			}

			my_Sobel_dx.data[index] = vertical_gradient;
			my_Sobel_dy.data[index] = horizontal_gradient;
		}
	}

	// openCV 정답
	Mat dx, dy;
	Sobel(src, dx, CV_32FC1, 1, 0); // x축 방향으로 편미분을 구하여 dx행렬에 저장
	Sobel(src, dy, CV_32FC1, 0, 1); // y축 방향으로 편미분을 구하여 dy행렬에 저장

	Mat float_mag, mag;
	magnitude(dx, dy, float_mag);
	float_mag.convertTo(mag, CV_8UC1);

	return 1;
}
