#include "Common_kenGwon.h"

int main()
{
	std::string fileName = "../KCCImageNet/circuit.bmp";
	cv::Mat img = cv::imread(fileName, cv::ImreadModes::IMREAD_COLOR);
	fileName = "../KCCImageNet/crystal.bmp";
	cv::Mat templ = cv::imread(fileName, cv::ImreadModes::IMREAD_COLOR);

	if (img.empty() || templ.empty())
	{
		std::cerr << "Image load fail" << std::endl;
		return -1;
	}

	img = img + Scalar(50, 50, 50);
	
	Mat noise(img.size(), CV_32SC3);
	randn(noise, 0, 10);
	add(img, noise, img, Mat(), CV_8UC3);

	Mat res, res_norm;
	matchTemplate(img, templ, res, CV_TM_CCOEFF_NORMED);
	normalize(res, res_norm, 0, 255, NORM_MINMAX, CV_8U);

	double maxv;
	Point maxloc;
	minMaxLoc(res, 0, &maxv, 0, &maxloc);
	std::cout << "maxv: " << maxv << std::endl;

	rectangle(img, Rect(maxloc.x, maxloc.y, templ.cols, templ.rows), Scalar(0, 0, 255), 2);

	return 1;
}
