#pragma once

#include "ISP.h"
/*
  �䱸����
  
  1. "���� ���ġ", "���� ���ġ", "���� ��ġ" ���� �׸���
  2. ���� ���ġ�� ���� ���ġ range�ȿ� ������ "ok" 
     else "NG" return
*/

/*
  - ������׷� ��Ȱȭ�� �Ͽ� ������ ��� ������ �켱 �а� �Ѵ�.
  - ��׵� ��ü �ϳ��ϳ��� ���� ����ȭ �ؼ� ���� �ִ� �˰��� �ʿ�
  - �� ���� �ȿ��� ����� ������ ��� ������ ������ 105 ~ 125

  - ���ϱ⸦ ���༭ ��� �ִ�ȭ ���ְ�
  - �� ���� ������ ���� ��� ���ؼ� ���ϸ� �ǰڴ�. ���������� ���ϸ� �ǰڴ�.
  
  - �߰��� �� ������ findContour()�� ã�ƿͼ� �� �����߽��� ã�� 
    �� �����߽��� �������� ������ ©�� ���鼭 
  - �̺��� ����ؼ� �߰� �������� ���ϸ� �ǰڴ�. 

  
*/


int main()
{
	std::string fileName = "../KCCImageNet/images/ampoules/ampoules_07.png";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	// ���� ����ȭ
	Mat src_bin;
	threshold(src_gray, src_bin, 210, 255, THRESH_BINARY);

	// Morphology ����
	int kernelSz = 4;
	int shape = MorphShapes::MORPH_ELLIPSE;
	cv::Size sz = Size(2 * kernelSz + 1, 2 * kernelSz + 1); // ���⸦ ���ι������� ����°� ����Ʈ
	Mat SE = cv::getStructuringElement(shape, sz);

	Mat temp;
	cv::morphologyEx(src_bin, temp, MorphTypes::MORPH_OPEN, SE);
	Mat src_masked;
	cv::morphologyEx(temp, src_masked, MorphTypes::MORPH_CLOSE, SE);

	// findContour�ϰ� area�� 300�̸��� �ܰ������� ���δ� 0���� ó��
	vector<vector<Point>> contours_temp;
	vector<Vec4i> hierarchy;
	findContours(src_masked, contours_temp, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	vector<vector<Point>> contours;
	for (size_t i = 0; i < contours_temp.size(); i++)
	{
		if (static_cast<int> (contourArea(contours_temp[i])) > 300)
		{
			contours.push_back(contours_temp[i]);
		}
	}

	// �ܰ����� ��ȸ�ϸ�, �ܰ������� �����߽� vector ���ϱ�
	vector<Point> centers;
	for (size_t i = 0; i < contours.size(); i++)
	{
		// ������ ���� ��ȸ�Ͽ�, �� �ܰ����� �߽��� ã��
		int sum_x = 0;
		int sum_y = 0;
		for (size_t j = 0; j < contours[i].size(); j++)
		{
			sum_x += contours[i].at(j).x;
			sum_y += contours[i].at(j).y;
		}
		centers.push_back(Point(sum_x / contours[i].size(), sum_y / contours[i].size()));
	}

	// �����߽��� �������� ������ ������ �簢���� ������� �ٶ� �� �ֵ���, �κ���� ����
	vector<Mat> ROIs;
	for (size_t i = 0; i < centers.size(); i++)
	{
		Point topLeft = Point(centers[i].x - 60, centers[i].y - 200);
		ROIs.push_back(src_gray(Rect(topLeft.x, topLeft.y, 40, 150)));
	}

	// �κ���ĵ��� projection�Ͽ� ��輱 point ����
	vector<int> liquid_lines;
	for (size_t i = 0; i < ROIs.size(); i++)
	{
		// ������׷� ��Ȱȭ
		equalizeHist(ROIs[i], ROIs[i]);
		// projection
		vector<int> projection;
		for (size_t row = 0; row < ROIs[i].rows; row++)
		{
			int sum_of_row = 0;
			for (size_t col = 0; col < ROIs[i].cols; col++)
			{
				sum_of_row += ROIs[i].data[row * ROIs[i].cols + col];
			}
			projection.push_back(sum_of_row);
		}
		
		// ���� ���� �ް��ϰ� ���ϴ� ��輱 �ε��� ��ġ ����
		int liquid_line = 0;
		for (size_t i = 0; i < projection.size() - 1; i++)
		{
			if (projection[i] - projection[i + 1] > liquid_line)
			{
				liquid_line = projection[i] - projection[i + 1];
			}
			else
			{
				continue;
			}
		}

		liquid_lines.push_back(liquid_line);
	}


	return 1;
}
