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

	/*
		소블 엣지
	*/
	Mat dx, dy;
	Sobel(src, dx, CV_32FC1, 1, 0); // x축 방향으로 편미분을 구하여 dx행렬에 저장
	Sobel(src, dy, CV_32FC1, 0, 1); // y축 방향으로 편미분을 구하여 dy행렬에 저장

	Mat float_mag, mag;
	magnitude(dx, dy, float_mag);
	float_mag.convertTo(mag, CV_8UC1);

	Mat edge = mag > 150; // Mat클래스 연산자 재정의 ... 임계값 150으로 설정

	/*
		케니 엣지
	*/
	Mat dst_canny1;
	Mat dst_canny2;
	Canny(src, dst_canny1, 50, 100);
	Canny(src, dst_canny2, 50, 150);

	return 1;
}
