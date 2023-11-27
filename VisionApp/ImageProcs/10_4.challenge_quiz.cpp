#pragma once

#include "ISP.h"
/*
  �䱸����
  1. "���� ���ġ", "���� ���ġ", "���� ��ġ" ���� �׸���
  2. ���� ���ġ�� ���� ���ġ range�ȿ� ������ "ok" 
     else "NG" return
*/

/*
  < ��� ��������� �������� ã�Ƴ��� �������� �˰��� >
  ���� ���󿡼� ���������� ��� ������ �������� ���� Ž���ϸ鼭, 
  ȸ������ ������� ���ڱ� ���� �ö󰡴� ������ ����Ʈ�� �� �ű⼭ �������� �ö󰡸�
  �ٷ� ���� ��׿��� ROI������ ��ġ ���� �ٷ� �� �� �ִ�.

  ������ ������ �ݺ��Ǵ� ���󿡼� �������� ã�Ƴ��� �˰����� �ٽ���,
  ���� ��Ŀ��� ������ �ݺ��Ǵ� ������ �ִ� ����� �� ������ Ž���ϸ鼭, 
  ���� 0���� 255�� ���ڱ� �ٴ� ������ point�� ��� ���̴�.
  �׸��� �� ������point�� �������� ROI�� ������ �� �ְ� �Ǵ� ���̴�.
  
  (���� ��� findContour()�� Ȱ���� �˰����� ������ ������ �ٲ��, 
  ���� �����ϴ� �ܰ����� ���� ���� �ٽ� ����������ϴ� ������ �߻��Ѵ�.
  ������ �������� �˰����� ���� ������ �ٸ� ������ ���͵�,
  ������ ������ ��Ģ�� ������ �������� ��Ƽ� ROI�� ������ �� �ְԵǴ� ���̴�.)
*/

int main()
{
	std::string fileName = "../KCCImageNet/images/ampoules/ampoules_07.png";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	// ampoules�� ������ �������� ��� ���� ���� ����ȭ
	Mat src_bin;
	threshold(src_gray, src_bin, 210, 255, THRESH_BINARY);

	// Morphology �������� ���� ����
	int kernelSz = 6;
	int shape = MorphShapes::MORPH_ELLIPSE;
	cv::Size sz = Size(2 * kernelSz + 1, 2 * kernelSz + 1);
	Mat SE = cv::getStructuringElement(shape, sz);

	cv::morphologyEx(src_bin, src_bin, MorphTypes::MORPH_OPEN, SE);
	cv::morphologyEx(src_bin, src_bin, MorphTypes::MORPH_CLOSE, SE);

	// �ܰ����� ã��, ������ 3000 �ʰ��� �ܰ����鸸 ���͸�
	vector<vector<Point>> contours_temp;
	vector<Vec4i> hierarchy;
	findContours(src_bin, contours_temp, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	vector<vector<Point>> contours;
	for (size_t i = 0; i < contours_temp.size(); i++)
	{
		if (static_cast<int> (contourArea(contours_temp[i])) > 3000)
		{
			contours.push_back(contours_temp[i]);
			printf("%d\n", (int)contourArea(contours_temp[i]));
		}
	}

	// �ܰ����� ��ȸ�ϸ�, �ܰ������� �����߽� ���ϱ�
	vector<Point> centers;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int sum_x = 0;
		int sum_y = 0;
		for (size_t j = 0; j < contours[i].size(); j++)
		{
			sum_x += contours[i].at(j).x;
			sum_y += contours[i].at(j).y;
		}
		centers.push_back(Point(sum_x / contours[i].size(), sum_y / contours[i].size()));
	}

	// �����߽��� �������� �� ampoules�� ������ �簢���� ������� �ٶ� �� �ֵ���, �κ���� ����
	vector<Point> liquid_line_points;
	for (size_t i = 0; i < centers.size(); i++)
	{
		if ((int)centers[i].x - 60 < 0) continue; // ����ó��

		Point topLeft = Point(centers[i].x - 60, centers[i].y - 200);
		Mat ROI = src_gray(Rect(topLeft.x, topLeft.y, 40, 150)).clone(); // deep copy�ؾ� �Ʒ� �������� �ùٸ� �ε��� ���� ����
		Mat temp = src_gray(Rect(topLeft.x, topLeft.y, 40, 150)); // ������ ���� shallow copy �ð��ڷ�

		// ��� ������ ���� ������׷� ��Ȱȭ
		equalizeHist(ROI, ROI); 
		equalizeHist(temp, temp); // ������ ���� shallow copy �ð��ڷ�

		// projection? 
		vector<int> projection;
		for (size_t row = 0; row < ROI.rows; row++)
		{
			int sum_of_row = 0;
			for (size_t col = 0; col < ROI.cols; col++)
			{
				sum_of_row += ROI.data[row * ROI.cols + col];
			}
			projection.push_back(sum_of_row);
		}

		// ���� ���� �ް��ϰ� ���ϴ� ��輱 �ε��� ��ġ ����
		vector<int> vector_dy;
		for (size_t k = 0; k < projection.size() - 1; k++)
		{
			int dy = abs(projection[k] - projection[k + 1]);
			vector_dy.push_back(dy);
		}
		int maxIdx = max_element(vector_dy.begin(), vector_dy.end()) - vector_dy.begin();
		Point liquid_point = Point(topLeft.x, topLeft.y + maxIdx);

		liquid_line_points.push_back(liquid_point);
	}

	// �� �׸���
	cvtColor(src_gray, src_gray, CV_GRAY2BGR);
	int min_y = 90, max_y = 120;
	line(src_gray, Point(0, min_y), Point(src_gray.cols, min_y), Scalar(0, 255, 255), 2); // ����ġ ����
	line(src_gray, Point(0, max_y), Point(src_gray.cols, max_y), Scalar(0, 255, 255), 2); // ����ġ ����

	for (size_t i = 0; i < liquid_line_points.size(); i++)
	{
		if (liquid_line_points[i].y > min_y && liquid_line_points[i].y < max_y) // OK
		{
			line(src_gray, 
				liquid_line_points[i], Point(liquid_line_points[i].x + 40, liquid_line_points[i].y),
				Scalar(255, 100, 0), 3);
			putText(src_gray, "OK", liquid_line_points[i], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 0), 2, 8);
		}
		else // NG
		{
			line(src_gray,
				liquid_line_points[i], Point(liquid_line_points[i].x + 40, liquid_line_points[i].y),
				Scalar(0, 0, 255), 3);
			putText(src_gray, "NG", liquid_line_points[i], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 2, 8);
		}
	}


	return 1;
}
