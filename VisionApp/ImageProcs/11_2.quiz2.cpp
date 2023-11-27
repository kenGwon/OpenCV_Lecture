/*
  끊어진 부분 찾기 - 탬플릿 매칭
*/

#include "Common.h"

int main()
{
	std::string fileName = "../KCCImageNet/solar_cell/solar_cell_04.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);

	Mat color_channels[3];
	split(src, color_channels); // 어떤 채널이 가장 잘 구분이 되는지 .. bgr 컬러를 bgr로 분리를 한다.

	Mat src_bin;
	threshold(color_channels[2], src_bin, 180, 255, ThresholdTypes::THRESH_BINARY_INV); // 레드채널이 가장 대비가 잘나와서 레드채널 영상으로 이진화 실시

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	vector<Point> unnormal_points;

	for (size_t i = 0; i < contours.size(); i++)
	{
		double contour_area = contourArea(contours[i]);

		if ((contour_area > 1000 && contour_area < 20000))
		{
			int min_x = src.cols;
			int min_y = src.rows;
			int max_x = 0;
			int max_y = 0;
			for (size_t k = 0; k < contours[i].size(); k++)
			{
				if (contours[i].at(k).x < min_x) min_x = contours[i].at(k).x;
				if (contours[i].at(k).y < min_y) min_y = contours[i].at(k).y;
				if (contours[i].at(k).x > max_x) max_x = contours[i].at(k).x;
				if (contours[i].at(k).y > max_y) max_y = contours[i].at(k).y;
			}
			int width = max_x - min_x;
			int height = max_y - min_y;

			if (height > 30) // 끊긴 부분이 있는 부분을 잡은 ROI: 임계점 높이 30 픽셀
			{
				Point fiducial_mark = Point(min_x, min_y);
				Mat ROI = src_bin(Rect(Point(fiducial_mark.x, fiducial_mark.y), Size(width, height))).clone();
				
				rectangle(src, Rect(Point(fiducial_mark.x, fiducial_mark.y), Size(width, height)), Scalar(0, 0, 255), 2);

				//// 모폴로지 연산으로 경계선정리
				//int kernelSz = 1;
				//int shape = MorphShapes::MORPH_RECT;
				//cv::Size sz = Size(2 * kernelSz + 1, 2 * kernelSz + 1);
				//Mat SE = cv::getStructuringElement(shape, sz);

				//cv::morphologyEx(ROI, ROI, MorphTypes::MORPH_OPEN, SE);

				Mat testt = src_bin(Rect(Point(fiducial_mark.x, fiducial_mark.y), Size(width, height)));
				testt = ~testt;

				vector<int> projection;
				for (size_t row = 10; row < ROI.rows - 10; row++)
				{
					int sum = 0;
					for (size_t col = 0; col < ROI.cols; col++)
					{
						sum += ROI.data[row * ROI.cols + col];
					}
					projection.push_back(sum);
				}

				auto min_ = min_element(projection.begin(), projection.end());
				int threshold = 25000;
				if (*min_ < threshold)
				{
					printf("빈곳 찾음?\n"); 
					int distance_ = distance(projection.begin(), min_);

					bool flag = false;
					for (size_t tmp_col = 0; tmp_col < ROI.cols; tmp_col++)
					{
						if (ROI.data[(distance_ + 10) * ROI.cols + tmp_col] == 255)
						{
							if (!flag)
							{
								flag = true;
								unnormal_points.push_back(
									Point(fiducial_mark.x + tmp_col, fiducial_mark.y + (distance_ + 10))
								);
							}
						}
						if (ROI.data[(distance_ + 10) * ROI.cols + tmp_col] == 0)
						{
							if (flag)
							{
								flag = false;
								unnormal_points.push_back(
									Point(fiducial_mark.x + tmp_col, fiducial_mark.y + (distance_ + 10))
								);
							}
						}
					}
				}
			}
		}
	}

	// 끊어진 곳 원 그리기
	for (size_t i = 0; i < unnormal_points.size(); i += 2)
	{
		int radius = (unnormal_points[i + 1].x - unnormal_points[i].x) / 2;
		Point center = Point(unnormal_points[i].x + radius, 
			unnormal_points[i].y);

		if (radius < 1) circle(src, center, radius + 7, Scalar(0, 255, 255), 2);
		circle(src, center, radius + 7, Scalar(0, 255, 255), 2);
	}


	return 1;
}
