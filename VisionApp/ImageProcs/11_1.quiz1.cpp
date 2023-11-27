/*
  윌리 찾기 - 탬플릿 매칭
*/

#include "Common.h"

int main()
{
	std::string fileName = "../KCCImageNet/Willie/Willie.jpg";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);
	std::string template_fileName = "../KCCImageNet/Willie/me.jpg";
	cv::Mat src_template = cv::imread(template_fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	Mat match_position = Mat::zeros(Size(src_gray.cols - src_template.cols + 1, src_gray.rows - src_template.rows + 1), CV_32FC1);

	for (size_t row = 0; row < src_gray.rows - src_template.rows + 1; row++)
	{
		for (size_t col = 0; col < src_gray.cols - src_template.cols + 1; col++)
		{
			double TM_CCORR = 0.0;
			double TM_CCORR_NORMED = 0.0;
			double operand1 = 0.0;
			double operand2 = 0.0;
			for (size_t trow = 0; trow < src_template.rows; trow++)
			{
				for (size_t tcol = 0; tcol < src_template.cols; tcol++)
				{
					double corr = src_template.data[trow * src_template.cols + tcol]
						* src_gray.data[(row + trow) * src_gray.cols + (col + tcol)];
					TM_CCORR += corr;
					operand1 += src_template.data[trow * src_template.cols + tcol]
						* src_template.data[trow * src_template.cols + tcol];
					operand2 += src_gray.data[(row + trow) * src_gray.cols + (col + tcol)]
						* src_gray.data[(row + trow) * src_gray.cols + (col + tcol)];
				}
			}
			TM_CCORR_NORMED = TM_CCORR / sqrt(operand1 * operand2);
			match_position.at<float>(row, col) = TM_CCORR_NORMED;
		}
	}

	Mat match_position_bin;
	threshold(match_position, match_position_bin, 0.97, 0.99, ThresholdTypes::THRESH_BINARY);
	match_position_bin *= 255; // 외곽선 따기 위해 8비트 레인지로 업스케일
	match_position_bin.convertTo(match_position_bin, CV_8UC1);
	
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(match_position_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	vector<Point> matching_points;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int sum_x = 0;
		int sum_y = 0;
		for (size_t k = 0; k < contours[i].size(); k++)
		{
			sum_x += contours[i].at(k).x;
			sum_y += contours[i].at(k).y;
		}
		int center_x = sum_x / contours[i].size();
		int center_y = sum_y / contours[i].size();
		matching_points.push_back(Point(center_x, center_y));
	}

	Mat src_3channels;
	cvtColor(src_gray, src_3channels, CV_GRAY2BGR);
	
	for (size_t i = 0; i < matching_points.size(); i++)
	{
		rectangle(src_3channels,
			matching_points[i], Point(matching_points[i].x + src_template.cols, matching_points[i].y + src_template.rows),
			Scalar(0, 0, 255), 2);
	}

	return 1;
}
