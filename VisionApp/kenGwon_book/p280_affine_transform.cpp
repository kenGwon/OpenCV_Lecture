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

	// ������ ��ȯ(affine transform)
	Point2f srcPts[3], dstPts[3];
	srcPts[0] = Point2f(0, 0);
	srcPts[1] = Point2f(src.cols - 1, 0);
	srcPts[2] = Point2f(src.cols - 1, src.rows - 1);
	dstPts[0] = Point2f(50, 50);
	dstPts[1] = Point2f(src.cols - 100, 100);
	dstPts[2] = Point2f(src.cols - 50, src.rows - 50);

	Mat M = getAffineTransform(srcPts, dstPts); // ������ ��ȯ ��� ���ϱ�

	Mat dst;
	warpAffine(src, dst, M, Size());

	
	// �̵� ��ȯ(translation transform)
	Mat M2 = Mat_<double>({ 2, 3 }, {1, 0, 150, 0, 1, 100}); // ���η� 150�ȼ���ŭ, ���η� 100�ȼ���ŭ ������ ������ �̵�
	Mat dts2;
	warpAffine(src, dts2, M2, Size());


	// ���� ��ȯ(shear transform)
	double mx = 0.3;
	Mat M3 = Mat_<double>({ 2, 3 }, {1, mx, 0, 0, 1, 0}); // ���ι��������� �и������� -0.3���� ����

	Mat dst3;
	warpAffine(src, dst3, M3, Size(cvRound(src.cols + src.rows * mx), src.rows));


	// ũ�� ��ȯ(scale transform)
	/*
	    ũ�� ��ȯ�� ������ó�� affine ��ȯ ����� ���ϰ� warpAffine�� ���ؼ� ũ�� ��ȯ�� �� �� ������,
		������ ũ�� ��ȯ�� ����ó�� �ý��ۿ��� �ſ� ����ϰ� ���Ǳ� ������, openCV���� �׳� resize()��� �Լ��� ��������.(���� ������ ����)
	*/
	Mat dst4;
	resize(src, dst4, Size(1920, 1280), 0, 0, INTER_LINEAR);


	// ȸ�� ��ȯ(rotation transform)
	Point2f cp(src.cols / 2.f, src.rows / 2.f); // ȸ�� �߽��� ����
	Mat M5 = getRotationMatrix2D(cp, 20, 1); // �ݽð� �������� 20����ŭ ȸ��

	Mat dst5;
	warpAffine(src, dst5, M5, Size());


	// ��Ī ��ȯ(filp transform)
	Mat dst6, dst7, dst8;
	vector<int>flip_code = {-1, 0, 1};
	flip(src, dst6, flip_code[0]);
	flip(src, dst7, flip_code[1]);
	flip(src, dst8, flip_code[2]);


	return 1;
}