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

	for (size_t i = 0; i < lines.size(); i++) // HoughLines()�� ���� ������ ������ ������ŭ �ݺ��� ����
	{
		float r = lines[i][0], t = lines[i][1]; // ������ ������ �Ķ���� �߿��� p���� ���� r�� setha���� ���� t�� ����
		double cos_t = cos(t), sin_t = sin(t); 
		double x0 = r * cos_t, y0 = r * sin_t; // x0�� y0�� ��������� ������ ������ ������ �� ������ ���� ��ǥ
		double alpha = 1000;
		
		// pt1, pt2���� (x0, y0)���� ����� �ָ� ������ �ִ� ���� ���� �� �� ��ǥ�� �����
		Point pt1(cvRound(x0 + alpha * (-sin_t)), cvRound(y0 + alpha * cos_t));
		Point pt2(cvRound(x0 - alpha * (-sin_t)), cvRound(y0 - alpha * cos_t));
		
		line(dst, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
	}

	/*
		Ȯ���� ���� ��ȯ
		- �Ϲ����� ���� ��ȯ�� ������ ������ �Ķ���� p�� setha�� ��ȯ�ߴٸ�,
		- Ȯ���� ���� ��ȯ�� ������ �������� ���� ��ǥ�� ��ȯ�Ѵ�. ��, ������ ã�� ���̴�. 
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
		���� ��ȯ �� ����
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
