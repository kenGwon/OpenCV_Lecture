#pragma once

#include "ISP.h"
/*
  요구사항
  1. 다수의 이미지에서 구멍을 찾고
  2. 색칠을 한 뒤 
  3. 해당 위치에 면적, 사이즈, avg, radius 등의 값을 출력하라.
*/

/*
  - 폴더의 다수의 브레이크 영상에 순차적으로 접근한다. for문 돌면서 Mat하나씩 찍어내기.
  - 우선은 하나의 영상에 대해서 작동하는 로직 완성

  - binarization
  - morpholoy 연산 필요한지 검토... 07번 영상 컨트롤 하려면 필요함... MORPH_ELLIPSE
  - 원형으로 닫혀있는 외곽선인지 확인 필요
  - find contour
  - 나머지는 지난 퀴즈와 로직 동일
  - 단, 노란색 그림을 그리려면 3채널로 cvtColor()로 변환된 Mat에 그려야 함.

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

	// 영상 이진화
	Mat src_bin;
	adaptiveThreshold(src_gray, src_bin, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 5, 3);

	// Morphology 연산
	int kernelSz = 7;
	int shape = MorphShapes::MORPH_ELLIPSE;
	cv::Size sz = Size(2 * kernelSz + 1, 2 * kernelSz + 1); // 여기를 세로방향으로 만드는게 포인트
	Mat SE = cv::getStructuringElement(shape, sz);

	Mat temp;
	cv::morphologyEx(src_bin, temp, MorphTypes::MORPH_OPEN, SE);
	Mat src_masked;
	cv::morphologyEx(temp, src_masked, MorphTypes::MORPH_CLOSE, SE);

	// 외곽선 추출
	vector<vector<Point>> contours_temp;
	vector<Vec4i> hierarchy;
	findContours(src_masked, contours_temp, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	// 폐곡선인 contours만 선별
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

	// 외곽선을 순환하면서 1. 색칠 2. 정보출력
	Mat src_result;
	cvtColor(src_gray, src_result, CV_GRAY2BGR); // 3채널짜리 영상에다 그려야 노란색 그리기 가능
	for (size_t i = 0; i < contours.size(); i++)
	{
		// 빨간색 영역을 감싸는 원 그리기
		Point2f target_center;
		float target_radius;
		minEnclosingCircle(contours[i], target_center, target_radius);
		circle(src_result, target_center, static_cast<int>(target_radius), Scalar(0, 255, 255), -1);
		drawContours(src_result, contours, i, Scalar(0, 255, 255), -1);

		// 각종 값들 산출
		double area = contourArea(contours[i]);
		double arcLen = arcLength(contours[i], true);

		// putText 작업
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
