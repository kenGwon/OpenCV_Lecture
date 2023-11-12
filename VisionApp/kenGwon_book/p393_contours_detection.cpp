#include "Common_kenGwon.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/detect_blob.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	if (src.empty())
	{
		std::cerr << "Image load fail" << std::endl;
		return -1;
	}

	// 계층구조를 사용하지 않는 외곽선 검출
	vector<vector<Point>> contours; // 외곽선은 점들의 집합이기 때문에 이와 같은 구조로 외곽선 vector 변수 선언
	findContours(src, contours, RETR_LIST, CHAIN_APPROX_NONE);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	for (int i = 0; i < contours.size(); i++)
	{
		Scalar random_color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, i, random_color, 2);
	}

	// 계층구조를 사용하는 외곽선 검출
	vector<vector<Point>> contours_h;
	vector<Vec4i> hierarchy;
	findContours(src, contours_h, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE); // 오버로딩

	Mat dst2;
	cvtColor(src, dst2, COLOR_GRAY2BGR);

	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
	{
		Scalar c(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst2, contours, idx, c, -1, LINE_8, hierarchy);
	}
	
	return 1;
}
