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

	// 어파인 변환(affine transform)
	Point2f srcPts[3], dstPts[3];
	srcPts[0] = Point2f(0, 0);
	srcPts[1] = Point2f(src.cols - 1, 0);
	srcPts[2] = Point2f(src.cols - 1, src.rows - 1);
	dstPts[0] = Point2f(50, 50);
	dstPts[1] = Point2f(src.cols - 100, 100);
	dstPts[2] = Point2f(src.cols - 50, src.rows - 50);

	Mat M = getAffineTransform(srcPts, dstPts); // 어파인 변환 행렬 구하기

	Mat dst;
	warpAffine(src, dst, M, Size());

	
	// 이동 변환(translation transform)
	Mat M2 = Mat_<double>({ 2, 3 }, {1, 0, 150, 0, 1, 100}); // 가로로 150픽셀만큼, 세로로 100픽셀만큼 떨어진 곳으로 이동
	Mat dts2;
	warpAffine(src, dts2, M2, Size());


	// 전단 변환(shear transform)
	double mx = 0.3;
	Mat M3 = Mat_<double>({ 2, 3 }, {1, mx, 0, 0, 1, 0}); // 가로방향으로의 밀림정도를 -0.3으로 설정

	Mat dst3;
	warpAffine(src, dst3, M3, Size(cvRound(src.cols + src.rows * mx), src.rows));


	// 크기 변환(scale transform)
	/*
	    크기 변환도 위에서처럼 affine 변환 행렬을 구하고 warpAffine을 통해서 크기 변환을 할 수 있지만,
		영상의 크기 변환은 영상처리 시스템에서 매우 빈번하게 사용되기 때문에, openCV에서 그냥 resize()라는 함수를 만들어버림.(동작 원리는 동일)
	*/
	Mat dst4;
	resize(src, dst4, Size(1920, 1280), 0, 0, INTER_LINEAR);


	// 회전 변환(rotation transform)
	Point2f cp(src.cols / 2.f, src.rows / 2.f); // 회전 중심점 설정
	Mat M5 = getRotationMatrix2D(cp, 20, 1); // 반시계 방향으로 20도만큼 회전

	Mat dst5;
	warpAffine(src, dst5, M5, Size());


	// 대칭 변환(filp transform)
	Mat dst6, dst7, dst8;
	vector<int>flip_code = {-1, 0, 1};
	flip(src, dst6, flip_code[0]);
	flip(src, dst7, flip_code[1]);
	flip(src, dst8, flip_code[2]);


	return 1;
}