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
		�Һ� ����
	*/
	Mat dx, dy;
	Sobel(src, dx, CV_32FC1, 1, 0); // x�� �������� ��̺��� ���Ͽ� dx��Ŀ� ����
	Sobel(src, dy, CV_32FC1, 0, 1); // y�� �������� ��̺��� ���Ͽ� dy��Ŀ� ����

	Mat float_mag, mag;
	magnitude(dx, dy, float_mag);
	float_mag.convertTo(mag, CV_8UC1);

	Mat edge = mag > 150; // MatŬ���� ������ ������ ... �Ӱ谪 150���� ����

	/*
		�ɴ� ����
	*/
	Mat dst_canny1;
	Mat dst_canny2;
	Canny(src, dst_canny1, 50, 100);
	Canny(src, dst_canny2, 50, 150);

	return 1;
}
