#include "Common_kenGwon.h"

int main()
{
	/*
		lena 이미지에 잡음 추가
	*/
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	if (src.empty())
	{
		std::cerr << "Image load fail" << std::endl;
		return -1;
	}

	int stddev = 15;
	Mat noise(src.size(), CV_32SC1);
	randn(noise, 0, stddev);

	Mat dst; // 잡음 추가된 lena 이미지
	add(src, noise, dst, Mat(), CV_8UC1);

	/*
		가우시안 노이즈를 가우시안 필터로 지우면 edge 성분들이 흐려지는 문제 발생 -> 양방향 필터 적용
	*/
	Mat noise_lena = dst.clone();

	// 1. 가우시안블러로 노이즈 제거 시도
	Mat fitered1;
	GaussianBlur(noise_lena, fitered1, Size(), stddev);

	// 2. 양방향필터 노이즈 제거 시도(양방향 필터는 가우시안 함수의 곱으로 구성)
	Mat fitered2;
	bilateralFilter(noise_lena, fitered2, -1, 10, stddev);


	return 1;
}