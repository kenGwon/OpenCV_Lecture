#pragma once
#include "Common.h"

int main()
{
#if 0
	string inString = "Kdfjow_Abcd_Abcd_Abcd_dkfjow";
	string inPtrn = "Abcd";
	// ptrn의 개수
	// ptrn의 인덱스
	// 출력하시오

	vector<int> ptrn_start_index;
	for (size_t strIdx = 0; strIdx < inString.length() - inPtrn.length(); strIdx++)
	{
		int ptrn_match = 0;
		for (size_t ptrnIdx = 0; ptrnIdx < inPtrn.length(); ptrnIdx++)
		{
			if (inString[strIdx + ptrnIdx] - inPtrn[ptrnIdx] == 0) // 같은지 여부는 빼기 연산으로도 구현 가능.. 이미지도 이렇게 계산함
			{
				ptrn_match++;
			}
		}
		if (ptrn_match == inPtrn.length())
		{
			ptrn_start_index.push_back(strIdx);
		}
	}
	
	std::cout << "ptrn의 개수: " << ptrn_start_index.size() << std::endl;
	std::cout << "ptrn의 인덱스: " << std::endl;
	for (size_t i = 0; i < ptrn_start_index.size(); i++)
	{
		std::cout << ptrn_start_index[i] << ", ";
	}
#endif

	std::string fileName = "../KCCImageNet/images/ampoules/ampoules_02.png";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);
	std::string template_fileName = "../KCCImageNet/images/ampoules/ampoules_template.png";
	cv::Mat src_template = cv::imread(template_fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	Mat match_position = Mat::zeros(Size(src_gray.cols - src_template.cols + 1, src_gray.rows - src_template.rows + 1), CV_32FC1);
	for (size_t row = 0; row < src_gray.rows - src_template.rows; row++)
	{
		for (size_t col = 0; col < src_gray.cols - src_template.cols; col++)
		{
			int score = 0;
			for (size_t trow = 0; trow < src_template.rows; trow++)
			{
				for (size_t tcol = 0; tcol < src_template.cols; tcol++)
				{
					score += src_template.data[trow * src_template.cols + tcol]
						* src_gray.data[(row + trow) * src_gray.cols + (col + tcol)];
				}
			}
			match_position.at<float>(row, col) = (double)score/pow(10, 8); // TM_CCORR
		}
	}

	
	Mat temp;
	threshold(match_position, temp, 2.4, 2.5, ThresholdTypes::THRESH_BINARY_INV);
	temp *= 255;

	temp.convertTo(temp, CV_8UC1);

	double max_value;
	Point max_location;
	minMaxLoc(match_position, &max_value, 0, &max_location);

	rectangle(src_gray, Rect(max_location.x, max_location.y, src_template.cols, src_template.rows),
		Scalar(0, 0, 255), 2);


	return 1;
}
