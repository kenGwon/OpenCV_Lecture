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
	setMouseCallback("src", on_mouse); // 함수명도 포인터다.(배열명처럼)

	imshow("src", src);
	waitKey();

	return 1;
}

/*
	마우스로 점을 찍는 순서가 올바르게 지켜져야 올바른 투시변환 결과가 나온다.
	좌상, 우상, 우하, 좌하 순서대로 점을 찍어야 올바른 투시변환 결과가 나온다.
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