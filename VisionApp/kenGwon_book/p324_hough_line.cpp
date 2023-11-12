#include "Common_kenGwon.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/building.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	if (src.empty())
	{
		std::cerr << "Image load fail" << std::endl;
		return -1;
	}

	Mat edge;
	Canny(src, edge, 50, 150);

	vector<Vec2f> lines;
	HoughLines(edge, lines, 1, CV_PI / 180, 250);

	Mat dst;
	cvtColor(edge, dst, COLOR_GRAY2BGR);

	for (size_t i = 0; i < lines.size(); i++) // HoughLines()에 의해 구해진 직선의 갯수만큼 반복문 돌기
	{
		float r = lines[i][0], t = lines[i][1]; // 직선의 방정식 파라미터 중에서 p값을 변수 r에 setha값을 변수 t에 저장
		double cos_t = cos(t), sin_t = sin(t); 
		double x0 = r * cos_t, y0 = r * sin_t; // x0과 y0은 우너점에서 직선에 수선을 내렸을 대 만나는 점의 좌표
		double alpha = 1000;
		
		// pt1, pt2에는 (x0, y0)에서 충분히 멀리 떨어져 있는 직선 상의 두 점 좌표가 저장됨
		Point pt1(cvRound(x0 + alpha * (-sin_t)), cvRound(y0 + alpha * cos_t));
		Point pt2(cvRound(x0 - alpha * (-sin_t)), cvRound(y0 - alpha * cos_t));
		
		line(dst, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
	}

	/*
		확률적 허프 변환
		- 일반적인 허프 변환이 직선의 방정식 파라미터 p와 setha를 반환했다면,
		- 확률적 허프 변환은 직선의 시작점과 끝점 좌표를 반환한다. 즉, 선분을 찾는 것이다. 
	*/
	vector<Vec4i> lines_p;
	HoughLinesP(edge, lines_p, 1, CV_PI / 180, 160, 50, 5);

	Mat dst_p;
	cvtColor(edge, dst_p, COLOR_GRAY2BGR);

	for (auto l : lines_p)
	{
		line(dst_p, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, LINE_AA); // l[0], l[1] = (x1, y1) // l[2], l[3] = (x2, y2)
	}

	/*
		허프 변환 원 검출
	*/
	
	std::string fileName2 = "../thirdparty/opencv_480/sources/samples/data/smarties.png";
	cv::Mat src2 = cv::imread(fileName2, cv::ImreadModes::IMREAD_GRAYSCALE);

	if (src2.empty())
	{
		std::cerr << "Image load fail" << std::endl;
		return -1;
	}
	
	vector<Vec3f> circles;
	HoughCircles(src2, circles, HOUGH_GRADIENT, 1, 50, 150, 30);

	Mat dst_circle;
	cvtColor(src2, dst_circle, COLOR_GRAY2BGR);

	for (auto c : circles)
	{
		Point center(cvRound(c[0]), cvRound(c[1]));
		int radius = cvRound(c[2]);
		circle(dst_circle, center, radius, Scalar(0, 0, 255), 2, LINE_AA);
	}
	
	
	return 1;
}
