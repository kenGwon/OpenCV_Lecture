#include "Common_kenGwon.h"

int main()
{
	/*
		lena �̹����� ���� �߰�
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

	Mat dst; // ���� �߰��� lena �̹���
	add(src, noise, dst, Mat(), CV_8UC1);

	/*
		����þ� ����� ����þ� ���ͷ� ����� edge ���е��� ������� ���� �߻� -> ����� ���� ����
	*/
	Mat noise_lena = dst.clone();

	// 1. ����þȺ��� ������ ���� �õ�
	Mat fitered1;
	GaussianBlur(noise_lena, fitered1, Size(), stddev);

	// 2. ��������� ������ ���� �õ�(����� ���ʹ� ����þ� �Լ��� ������ ����)
	Mat fitered2;
	bilateralFilter(noise_lena, fitered2, -1, 10, stddev);


	return 1;
}