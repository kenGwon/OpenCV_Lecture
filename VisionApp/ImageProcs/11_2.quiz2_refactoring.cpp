/*
  끊어진 부분 찾기 - 탬플릿 매칭
*/

#include "Common.h"

int main()
{
	std::string fileName = "../KCCImageNet/solar_cell/solar_cell_06.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);

	Mat color_channels[3];
	split(src, color_channels); // 어떤 채널이 가장 잘 구분이 되는지 .. bgr 컬러를 bgr로 분리를 한다.

	Mat src_bin;
	threshold(color_channels[2], src_bin, 180, 255, ThresholdTypes::THRESH_BINARY_INV); // 레드채널이 가장 대비가 잘나와서 레드채널 영상으로 이진화 실시

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	vector<Point> abnormal_points;

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
				ROI = ~ROI;

				rectangle(src, Rect(Point(fiducial_mark.x, fiducial_mark.y), Size(width, height)), Scalar(0, 0, 255), 2);

				// 제대로 잡는지 확인용
				Mat testt = src_bin(Rect(Point(fiducial_mark.x, fiducial_mark.y), Size(width, height)));
				testt = ~testt;

				vector<int> projection;
				for (size_t col = 0; col < ROI.cols; col++)
				{
					int sum = 0;
					for (size_t row = 10; row < ROI.rows - 10; row++)
					{
						sum += ROI.data[row * ROI.cols + col];
					}
					projection.push_back(sum);
				}

				bool flag = false;
				for (size_t i = 0; i < projection.size(); i++)
				{
					if (projection[i] == 0)
					{
						if (!flag)
						{
							flag = true;
							abnormal_points.push_back(
								Point(fiducial_mark.x + i, fiducial_mark.y + (height / 2))
							);
						}
					}
					else
					{
						if (flag)
						{
							flag = false;
							abnormal_points.push_back(
								Point(fiducial_mark.x + i, fiducial_mark.y + (height / 2))
							);
						}
					}
				}
			}
		}
	}

	// 끊어진 곳 원 그리기
	for (size_t i = 0; i < abnormal_points.size(); i += 2)
	{
		int radius = (abnormal_points[i + 1].x - abnormal_points[i].x) / 2;
		Point center = Point(abnormal_points[i].x + radius, abnormal_points[i].y);

		circle(src, center, radius + 7, Scalar(0, 255, 255), 2);
	}


	return 1;
}

/*
* deflate : ROI를 땄는데, 상하좌우 가얘부분을 쬐금씩 덜어내서 불필요한 데이터들을 덜어내는 행위
* 
* 내가 레이블링으로 문제를 풀려고 할 때 본능적으로 min_x, max_x, min_y, max_y에다가 3씩을 더하고 빼줬던 게,
* 3이라는 deflate값을 준 것이다.
*/
