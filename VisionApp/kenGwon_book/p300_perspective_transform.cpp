#include "Common_kenGwon.h"

Mat src;
Point2f srcQuad[4], dstQuad[4];
void on_mouse(int event, int x, int y, int flags, void* userdata);

int main()
{
	std::string fileName = "../KCCImageNet/find_google_area.png";
	src = cv::imread(fileName);

	if (src.empty())
	{
		std::cerr << "Image load fail" << std::endl;
		return -1;
	}

	namedWindow("src");
	setMouseCallback("src", on_mouse); // �Լ��� �����ʹ�.(�迭��ó��)

	imshow("src", src);
	waitKey();

	return 1;
}

/*
	���콺�� ���� ��� ������ �ùٸ��� �������� �ùٸ� ���ú�ȯ ����� ���´�.
	�»�, ���, ����, ���� ������� ���� ���� �ùٸ� ���ú�ȯ ����� ���´�.
*/
void on_mouse(int event, int x, int y, int flags, void*)
{
	static int cnt = 0;

	if (event == EVENT_LBUTTONDOWN)
	{
		if (cnt < 4)
		{
			srcQuad[cnt++] = Point2f(x, y);

			circle(src, Point(x, y), 5, Scalar(0, 0, 255), -1);
			imshow("src", src);

			if (cnt == 4)
			{
				int w = 700, h = 400;

				dstQuad[0] = Point2f(0, 0);
				dstQuad[1] = Point2f(w - 1, 0);
				dstQuad[2] = Point2f(w - 1, h - 1);
				dstQuad[3] = Point2f(0, h - 1);

				Mat pers = getPerspectiveTransform(srcQuad, dstQuad);

				Mat dst;
				warpPerspective(src, dst, pers, Size(w, h));

				imshow("dst", dst);
			}
		}
	}

}