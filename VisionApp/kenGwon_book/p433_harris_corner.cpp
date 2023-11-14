#include "Common_kenGwon.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/building.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	if (src.empty())
	{
		std::cerr << "Image load fail" << std::endl;
		return -1;
	}

	Mat harris;
	cornerHarris(src, harris, 3, 3, 0.04);

	Mat harris_norm;
	normalize(harris, harris_norm, 0, 255, NORM_MINMAX, CV_8U);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	for (int row = 0; row < harris.rows - 1; row++)
	{
		for (int col = 0; col < harris.cols - 1; col++)
		{
			if (harris_norm.at<uchar>(row, col) > 120)
			{
				if (harris_norm.at<uchar>(row, col) > harris_norm.at<uchar>(row - 1, col) &&
					harris_norm.at<uchar>(row, col) > harris_norm.at<uchar>(row + 1, col) &&
					harris_norm.at<uchar>(row, col) > harris_norm.at<uchar>(row, col - 1) &&
					harris_norm.at<uchar>(row, col) > harris_norm.at<uchar>(row, col + 1))
				{
					circle(dst, Point(col, row), 5, Scalar(0, 0, 255), 2);
				}
			}
		}
	}


	return 1;
}
