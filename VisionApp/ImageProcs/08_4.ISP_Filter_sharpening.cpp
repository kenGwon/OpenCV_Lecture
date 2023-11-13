#pragma once

#include "Common.h"
#include "ISP.h"
#include <numeric>

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	// �� �̷������ε� ������ ũ���� ����� ���� �ʱ�ȭ ���� �� �ֱ���
	cv::Mat src_gray_blur = src_gray.clone();
	src_gray_blur = 0;

	const int filter_sz = 3;

	float blur[] = {
		1.0 / 9, 1.0 / 9, 1.0 / 9,
		1.0 / 9, 1.0 / 9, 1.0 / 9,
		1.0 / 9, 1.0 / 9, 1.0 / 9,
	};


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

	// �ֱ��� ������(Nearest Neighbor interpolation) ���ȼ� ä���: ������ ���� ���� �״�� ����
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

	// �밢 �ڳ� 4�� ���� ������ �� �ȼ��� ��հ��� ���ϱ�
	// �»�
	src_gray_blur.data[0]
		= (src_gray_blur.data[0 + 1] + src_gray_blur.data[0 + src_gray_blur.cols]) / 2;
	// ���
	src_gray_blur.data[src_gray_blur.rows - 1]
		= (src_gray_blur.data[src_gray_blur.rows - 1 - 1] + src_gray_blur.data[src_gray_blur.rows * 2 - 1]) / 2;
	// ����
	src_gray_blur.data[src_gray_blur.cols * (src_gray_blur.rows - 1)]
		= (src_gray_blur.data[src_gray_blur.cols * (src_gray_blur.rows - 2)] + src_gray_blur.data[src_gray_blur.cols * (src_gray_blur.rows - 1) + 1]) / 2;
	// ����
	src_gray_blur.data[src_gray_blur.cols * src_gray_blur.rows - 1]
		= (src_gray_blur.data[src_gray_blur.cols * src_gray_blur.rows - 1 - 1] + src_gray_blur.data[src_gray_blur.cols * (src_gray_blur.rows - 1 - 1) + src_gray_blur.rows - 1]) / 2;


	// ���� sharpening
	Mat src_gray_sharp(Size(src_gray_blur.rows, src_gray_blur.cols), CV_8UC1);
	Mat src_gray_sharp2(Size(src_gray_blur.rows, src_gray_blur.cols), CV_8UC1);
	Mat src_gray_sharp3(Size(src_gray_blur.rows, src_gray_blur.cols), CV_8UC1);

	const float alpha = 2.f;
	src_gray_sharp = (1 + alpha) * src_gray - alpha * src_gray_blur;
	src_gray_sharp2 = src_gray + (alpha * (src_gray - src_gray_blur));
	src_gray_sharp3 = src_gray + (alpha * src_gray) - (alpha * src_gray_blur);
	
	Scalar a = mean(src_gray_sharp);
	Scalar b = mean(src_gray_sharp2);
	Scalar c = mean(src_gray_sharp3);


	return 1;
}

