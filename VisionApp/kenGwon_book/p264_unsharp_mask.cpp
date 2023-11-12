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


	for (int sigma = 1; sigma <= 5; sigma++)
	{
		cv::Mat blurred;
		cv::GaussianBlur(src, blurred, Size(), static_cast<double>(sigma));

		float alpha = 1.f; // 날카로운 성분에 대한 가중치
		Mat dst = (1 + alpha) * src - alpha * blurred;
	}


	return 1;
}