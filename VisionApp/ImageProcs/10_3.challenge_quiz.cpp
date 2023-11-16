#pragma once

#include "ISP.h"
/*
  �䱸����
  1. �ټ��� �̹������� ������ ã��
  2. ��ĥ�� �� �� 
  3. �ش� ��ġ�� ����, ������, avg, radius ���� ���� ����϶�.
*/

/*
  - ������ �ټ��� �극��ũ ���� ���������� �����Ѵ�. for�� ���鼭 Mat�ϳ��� ����.
  - �켱�� �ϳ��� ���� ���ؼ� �۵��ϴ� ���� �ϼ�

  - binarization
  - morpholoy ���� �ʿ����� ����... 07�� ���� ��Ʈ�� �Ϸ��� �ʿ���... MORPH_ELLIPSE
  - �������� �����ִ� �ܰ������� Ȯ�� �ʿ�
  - find contour
  - �������� ���� ����� ���� ����
  - ��, ����� �׸��� �׸����� 3ä�η� cvtColor()�� ��ȯ�� Mat�� �׷��� ��.

*/
int main()
{
	/*vector<std::string> fileNames = {
		"../KCCImageNet/brake_disk/brake_disk_part_01.png",
		"../KCCImageNet/brake_disk/brake_disk_part_02.png",
		"../KCCImageNet/brake_disk/brake_disk_part_03.png",
		"../KCCImageNet/brake_disk/brake_disk_part_04.png",
		"../KCCImageNet/brake_disk/brake_disk_part_05.png",
		"../KCCImageNet/brake_disk/brake_disk_part_06.png",
		"../KCCImageNet/brake_disk/brake_disk_part_07.png",
		"../KCCImageNet/brake_disk/brake_disk_part_08.png",
		"../KCCImageNet/brake_disk/brake_disk_part_09.png",
	};
	
	for (int i = 1; i <= 9; i++)
	{
		std::string fileName = "../KCCImageNet/brake_disk/brake_disk_part_00.png";

		fileName.replace(fileName.size() - 5, 1, to_string(i));
		printf("%s\n", fileName);

	}*/

	std::string fileName = "../KCCImageNet/brake_disk/brake_disk_part_07.png";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	// ���� ����ȭ
	Mat src_bin;
	adaptiveThreshold(src_gray, src_bin, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 5, 3);

	// Morphology ����
	int kernelSz = 7;
	int shape = MorphShapes::MORPH_ELLIPSE;
	cv::Size sz = Size(2 * kernelSz + 1, 2 * kernelSz + 1); // ���⸦ ���ι������� ����°� ����Ʈ
	Mat SE = cv::getStructuringElement(shape, sz);

	Mat temp;
	cv::morphologyEx(src_bin, temp, MorphTypes::MORPH_OPEN, SE);
	Mat src_masked;
	cv::morphologyEx(temp, src_masked, MorphTypes::MORPH_CLOSE, SE);

	// �ܰ��� ����
	vector<vector<Point>> contours_temp;
	vector<Vec4i> hierarchy;
	findContours(src_masked, contours_temp, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	// ���� contours�� ����
	vector<vector<Point>> contours;
	for (size_t i = 0; i < contours_temp.size(); i++)
	{
		Point first_point = contours_temp[i].at(0);
		Point last_point = contours_temp[i].at(contours_temp[i].size() - 1);
		
		bool closed = false;
		for (int ROI_x = -1; ROI_x <= 1; ROI_x++) // -1, 0, 1
		{
			for (int ROI_y = -1; ROI_y <= 1; ROI_y++) // -1, 0, 1
			{
				bool condition = ((first_point.x + ROI_x == last_point.x) && (first_point.y + ROI_y == last_point.y));
				if (condition)
				{
					closed = true;
					break;
				}
			}
			if (closed) break;
		}
		
		if (closed)
			contours.push_back(contours_temp[i]);
		else
			continue;
	}

	// �ܰ����� ��ȯ�ϸ鼭 1. ��ĥ 2. �������
	Mat src_result;
	cvtColor(src_gray, src_result, CV_GRAY2BGR); // 3ä��¥�� ���󿡴� �׷��� ����� �׸��� ����
	for (size_t i = 0; i < contours.size(); i++)
	{
		// ������ ������ ���δ� �� �׸���
		Point2f target_center;
		float target_radius;
		minEnclosingCircle(contours[i], target_center, target_radius);
		circle(src_result, target_center, static_cast<int>(target_radius), Scalar(0, 255, 255), -1);
		drawContours(src_result, contours, i, Scalar(0, 255, 255), -1);

		// ���� ���� ����
		double area = contourArea(contours[i]);
		double arcLen = arcLength(contours[i], true);

		// putText �۾�
		Point ptTxt = Point(target_center.x, target_center.y);

		String msg;
		msg = std::format("area = {:.1f}", area);
		putText(src_result, msg, Point(ptTxt.x, ptTxt.y + 30 * 0), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(0, 0, 0), 1, 8);
		msg = std::format("length = {:.1f}", arcLen);
		putText(src_result, msg, Point(ptTxt.x, ptTxt.y + 30 * 1), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(0, 0, 0), 1, 8);
		msg = std::format("x,y = {:.1f}, {:.1f}", target_center.x, target_center.y);
		putText(src_result, msg, Point(ptTxt.x, ptTxt.y + 30 * 2), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(0, 0, 0), 1, 8);
		msg = std::format("Radius = {:.1f}", target_radius);
		putText(src_result, msg, Point(ptTxt.x, ptTxt.y + 30 * 3), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(0, 0, 0), 1, 8);
	
	}

	return 1;
}
