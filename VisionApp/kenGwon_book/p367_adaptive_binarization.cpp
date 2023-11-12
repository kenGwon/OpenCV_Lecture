#include "Common_kenGwon.h"

void on_trackbar(int pos, void* userdata);

int main()
{
	std::string fileName = "../KCCImageNet/sudoku.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	if (src.empty())
	{
		std::cerr << "Image load fail" << std::endl;
		return -1;
	}

	imshow("src", src);

	namedWindow("dst");
	createTrackbar("Block Size", "dst", 0, 200, on_trackbar, (void*)&src);
	setTrackbarPos("Block Size", "dst", 13);

	waitKey();
	return 1;
}

void on_trackbar(int pos, void* userdata)
{
	Mat src = *static_cast<Mat*> (userdata);

	int bsize = pos;
	if (bsize % 2 == 0) bsize--;
	if (bsize < 3) bsize = 3;

	Mat dst;
	adaptiveThreshold(src, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, bsize, 5);

	imshow("dst", dst);
}
