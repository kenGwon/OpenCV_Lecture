#include "Common_kenGwon.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	if (src.empty())
	{
		std::cerr << "Image load fail" << std::endl;
		return -1;
	}

	cv::Mat dst;	

	for (int sigma = 1; sigma <= 5; sigma++)
	{
		cv::GaussianBlur(src, dst, Size(), static_cast<double>(sigma));
	}


	return 1;
}