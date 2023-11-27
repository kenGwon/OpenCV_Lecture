/*
  ������ �κ� ã�� - ���ø� ��Ī
*/

#include "Common.h"

int main()
{
	std::string fileName = "../KCCImageNet/solar_cell/solar_cell_06.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);

	Mat color_channels[3];
	split(src, color_channels); // � ä���� ���� �� ������ �Ǵ��� .. bgr �÷��� bgr�� �и��� �Ѵ�.

	Mat src_bin;
	threshold(color_channels[2], src_bin, 180, 255, ThresholdTypes::THRESH_BINARY_INV); // ����ä���� ���� ��� �߳��ͼ� ����ä�� �������� ����ȭ �ǽ�

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

			if (height > 30) // ���� �κ��� �ִ� �κ��� ���� ROI: �Ӱ��� ���� 30 �ȼ�
			{
				Point fiducial_mark = Point(min_x, min_y);
				Mat ROI = src_bin(Rect(Point(fiducial_mark.x, fiducial_mark.y), Size(width, height))).clone();
				ROI = ~ROI;

				rectangle(src, Rect(Point(fiducial_mark.x, fiducial_mark.y), Size(width, height)), Scalar(0, 0, 255), 2);

				// ����� ����� Ȯ�ο�
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

	// ������ �� �� �׸���
	for (size_t i = 0; i < abnormal_points.size(); i += 2)
	{
		int radius = (abnormal_points[i + 1].x - abnormal_points[i].x) / 2;
		Point center = Point(abnormal_points[i].x + radius, abnormal_points[i].y);

		circle(src, center, radius + 7, Scalar(0, 255, 255), 2);
	}


	return 1;
}

/*
* deflate : ROI�� ���µ�, �����¿� ����κ��� �رݾ� ����� ���ʿ��� �����͵��� ����� ����
* 
* ���� ���̺����� ������ Ǯ���� �� �� ���������� min_x, max_x, min_y, max_y���ٰ� 3���� ���ϰ� ����� ��,
* 3�̶�� deflate���� �� ���̴�.
*/
