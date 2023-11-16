#pragma once

#include "ISP.h"

int main()
{
	std::string fileName = "../KCCImageNet/images/circle_plate.png";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	cv::Mat src_bin;
	cv::threshold(src_gray, src_bin, 150, 255, ThresholdTypes::THRESH_BINARY);

	uchar* pData = src_bin.data;
	size_t width = src_gray.cols;
	size_t height = src_gray.rows;

	Mat src_erosion = Mat::zeros(height, width, CV_8UC1);	
	Mat src_dilation = Mat::zeros(height, width, CV_8UC1);	
	Mat src_dilation2 = Mat::zeros(height, width, CV_8UC1);	

#if 0
	const int SE_sz = 3; int SE_half_sz = SE_sz / 2;
	int SE_Cross[SE_sz * SE_sz] = {
		0,   255, 0,
		255, 255, 255,
		0,   255, 0 
	};
	int SE_Rect[SE_sz * SE_sz] = {
		255,255,255,
		255,255,255,
		255,255,255 
	};
#endif
#if 1
	const int SE_sz = 5; int SE_half_sz = SE_sz / 2;
	int SE_Cross[SE_sz * SE_sz] = {
		0,   0,   255, 0,   0,
		0,   0,   255, 0,   0,
		255, 255, 255, 255, 255,
		0,   0,   255, 0,   0,
		0,   0,   255, 0,   0
	};
	int SE_Rect[SE_sz * SE_sz] = {
		255,255,255,255,255,
		255,255,255,255,255,
		255,255,255,255,255,
		255,255,255,255,255,
		255,255,255,255,255,
	};
#endif

	/*
	* ħ��(erosion)
	*/
	uchar* pDst_e = src_erosion.data;
	for (size_t row = SE_half_sz; row < height - SE_half_sz; row++) 
	{
		for (size_t col = SE_half_sz; col < width - SE_half_sz; col++) 
		{
			bool flag_erosion = false;
			for (int h = -SE_half_sz; h <= SE_half_sz; h++) 
			{
				for (int w = -SE_half_sz; w <= SE_half_sz; w++) 
				{
					int index = (row + h) * width + (col + w); // ���������� �ε���
					int index_SE = (h + SE_half_sz) * SE_sz + (w + SE_half_sz); // ��Ʈ���ĸ� ������Ʈ �ε���
					
					// SE�� �ٶ󺸴� ������ ���� ���������� �ȼ��� �ϳ��� ��ġ���� �ʾҴٸ� ħ�� �÷��� on
					//if (src_bin.data[index] == 0 && SE_Rect[index_SE] == 255)
					if (src_bin.data[index] == 0 && SE_Cross[index_SE] == 255)
					{
						flag_erosion = true;
						break;
					}
				}
				if (flag_erosion) break;
			}
			if (flag_erosion)
				pDst_e[row * width + col] = 0; // ħ�� �߻�
			else
				pDst_e[row * width + col] = 255; 
		}
	}

	/*
	* ��â(dilation)
	*/
	uchar* pDst_d = src_dilation.data;

	for (size_t row = SE_half_sz; row < height - SE_half_sz; row++)
	{
		for (size_t col = SE_half_sz; col < width - SE_half_sz; col++) 
		{
			bool flag_dilation = false;
			for (int h = -SE_half_sz; h <= SE_half_sz; h++) 
			{
				for (int w = -SE_half_sz; w <= SE_half_sz; w++) 
				{
					int index = (row + h) * width + (col + w); // ���������� �ε���
					int index_SE = (h + SE_half_sz) * SE_sz + (w + SE_half_sz); // ��Ʈ���ĸ� ������Ʈ �ε���
					
					// SE�� �ٶ󺸴� ������ ���� ���������� ��ȿ�ȼ��� �ϳ��� ���ƴٸ� ��â�÷��� on
					//if (SE_Cross[index_SE] == 255 && src_bin.data[index] == SE_Cross[index_SE])
					if (SE_Rect[index_SE] == 255 && src_bin.data[index] == SE_Rect[index_SE])
					{
						flag_dilation = true;
						break;
					}
				}
				if (flag_dilation) break;
			}
			if (flag_dilation)
			{
				for (int h = -SE_half_sz; h <= SE_half_sz; h++)
				{
					for (int w = -SE_half_sz; w <= SE_half_sz; w++)
					{
						int index = (row + h) * width + (col + w);
						int index_SE = (h + SE_half_sz) * SE_sz + (w + SE_half_sz);

						if (SE_Cross[index_SE] == 255)
						{
							pDst_d[index] = 255;
						}
					}
				}
			}
		}
	}


	/*
	* OpenCV ����
	*/
	{
		int kernelSz = 5;
		int shape = MorphShapes::MORPH_ELLIPSE;
		cv::Size sz = Size(2 * kernelSz + 1, 2 * kernelSz + 1);
		Mat SE = cv::getStructuringElement(shape, sz);

		// ħ�� erode
		// src::src_bin
		Mat src_erode;
		cv::erode(src_bin, src_erode, SE);
		Mat diff_erode = src_bin - src_erode;

		Mat src_dilate;
		cv::dilate(src_bin, src_dilate, SE);
		Mat diff_dilate = src_bin - src_erode;

		// openning
		Mat src_open_ex;
		cv::morphologyEx(src_bin, src_open_ex, MorphTypes::MORPH_OPEN, SE);
		Mat diff_open_ex = src_bin - src_open_ex;

		// closing
		Mat src_close_ex;
		cv::morphologyEx(src_bin, src_close_ex, MorphTypes::MORPH_CLOSE, SE);
		Mat diff_close_ex = src_bin - src_close_ex;

		int a = 0;
	}

	return 1;
}


