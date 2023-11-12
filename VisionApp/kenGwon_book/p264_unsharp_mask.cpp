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
		Mat dst = (1 + alpha) * src - alpha * blurred; // 교재 263페이지의 샤프닝을 하기 위한 지극히 당연한 수식을 수학적으로 정리하여 나온 결과 수식(이 수식을 이해하려 하지 말고 원본 수식을 보면 바로 이해됨)
	}


	return 1;
}