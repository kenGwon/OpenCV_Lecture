#include "Common_kenGwon.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/vtest.avi";
	VideoCapture cap(fileName);
	
	if (!cap.isOpened())
	{
		std::cerr << "Video open failed!" << std::endl;
		return -1;
	}

	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	Mat frame;
	while (true)
	{
		cap >> frame;
		if (frame.empty()) break;

		vector<Rect> detected;
		hog.detectMultiScale(frame, detected);

		for (auto r : detected)
		{
			Scalar c = Scalar(rand() % 256, rand() % 256, rand() % 256);
			rectangle(frame, r, c, 3);
		}

		imshow("frame", frame);

		if (waitKey(10) == 27) break;
	}


	return 1;
}