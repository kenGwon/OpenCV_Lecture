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

	// ���������� ������� �ʴ� �ܰ��� ����
	vector<vector<Point>> contours; // �ܰ����� ������ �����̱� ������ �̿� ���� ������ �ܰ��� vector ���� ����
	findContours(src, contours, RETR_LIST, CHAIN_APPROX_NONE);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	for (int i = 0; i < contours.size(); i++)
	{
		Scalar random_color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, i, random_color, 2);
	}

	// ���������� ����ϴ� �ܰ��� ����
	vector<vector<Point>> contours_h;
	vector<Vec4i> hierarchy;
	findContours(src, contours_h, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE); // �����ε�

	Mat dst2;
	cvtColor(src, dst2, COLOR_GRAY2BGR);

	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
	{
		Scalar c(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst2, contours, idx, c, -1, LINE_8, hierarchy);
	}
	
	return 1;
}
