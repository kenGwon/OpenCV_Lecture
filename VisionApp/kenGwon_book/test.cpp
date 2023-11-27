#include <iostream>
#include "Common_kenGwon.h"

int main()
{
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cerr << "Camera open failed!" << endl;
		return -1;
	}

	CascadeClassifier classifier("../thirdparty/opencv_480/build/etc/haarcascades/haarcascade_frontalface_default.xml");
	if (classifier.empty())
	{
		cerr << "SML load failed!" << endl;
		return -2;
	}

	Mat frame, inversed;
	while (true)
	{
		cap >> frame;
		if (frame.empty())
		{
			break;
		}


		vector<Rect> faces;
		classifier.detectMultiScale(frame, faces);

		for (Rect rc : faces)
		{
			rectangle(frame, rc, Scalar(255, 0, 255), 2);
		}

		imshow("frame", frame);


		if (waitKey(10) == 27) // ESC key
		{
			break;
		}
	}

	destroyAllWindows();

	return 1;
}
