#include <stdio.h>

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#define FRAME_NUMBER 6
#define FRAME_DETECT_NUMBER 4

using namespace std;
using namespace cv;
void detectAndDisplay(Mat frame);
Mat computeMatXGradient(const Mat &mat);

String face_cascade_name, left_eye_cascade_name, right_eyes_cascade_name, eyes_cascade_name;

CascadeClassifier face_cascade;
CascadeClassifier left_eye_cascade;
CascadeClassifier right_eye_cascade;
CascadeClassifier eye_cascade;

int detect_number = 0;
bool detect_frame[FRAME_NUMBER];
int frame_index = 0;

int main(int argc, const char** argv)
{
	face_cascade_name = "haarcascades/haarcascade_frontalface_alt.xml";
	left_eye_cascade_name = "haarcascades/haarcascade_lefteye_2splits.xml";
	right_eyes_cascade_name = "haarcascades/haarcascade_righteye_2splits.xml";
	eyes_cascade_name = "haarcascades/haarcascade_eye_tree_eyeglasses.xml";
	for (int i = 0; i < FRAME_NUMBER; i++)
	{
		detect_frame[i] = false;
	}

	VideoCapture capture;
	Mat frame;
	Mat frame_test;
	//-- 1. Load the cascades
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); return -1; };
	if (!left_eye_cascade.load(left_eye_cascade_name)) { printf("--(!)Error loading eyes cascade\n"); return -1; };
	if (!right_eye_cascade.load(right_eyes_cascade_name)) { printf("--(!)Error loading eyes cascade\n"); return -1; };
	if (!eye_cascade.load(eyes_cascade_name)) { printf("--(!)Error loading eyes cascade\n"); return -1; };

	//-- 2. Read the video stream
	capture.open(0);
	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }

	capture.set(CV_CAP_PROP_FPS, 0.5);

	while (capture.read(frame))
	{
		resize( frame, frame, cv::Size( frame.cols/2, frame.rows/2 ), 0, 0, CV_INTER_NN);

		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}

		detectAndDisplay(frame);

		frame_index++;

		if (FRAME_NUMBER <= frame_index)
		{
			frame_index = 0;
			//printf("detect_number: %d", detect_number);
			if (FRAME_DETECT_NUMBER < detect_number)
			{
				printf("1");
			}
			else
			{
				printf("0");
			}
			printf("\n");
		}

		if (waitKey(10) == 27) { break; } // escape
	}
	return 0;
}


void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	bool face_check = false;
	bool left_eye_check = false;
	bool left_open_eye_check = false;
	bool right_eye_check = false;
	bool right_open_eye_check = false;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(60, 60));
	for (size_t i = 0; i < faces.size(); i++)
	{
		face_check = true;

		Point right_p(faces[i].x + faces[i].width / 16,
			(int)(faces[i].y + (faces[i].height / 5)));
		Size right_s((faces[i].width - 2 * faces[i].width / 16) / 2,
			(int)(faces[i].height / 3.0));

		Rect eyearea_right(right_p, right_s);

		Point left_p(faces[i].x + faces[i].width / 16 + (faces[i].width - 2 * faces[i].width / 16) / 2,
			(int)(faces[i].y + (faces[i].height / 5)));
		Size left_s((faces[i].width - 2 * faces[i].width / 16) / 2,
			(int)(faces[i].height / 3.0));

		Rect eyearea_left(left_p, left_s);

		Mat rightROI = frame_gray(eyearea_right);
		Mat leftROI = frame_gray(eyearea_left);

		std::vector<Rect> eyes;
		//-- In each face, detect eyes
		left_eye_cascade.detectMultiScale(leftROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(20, 10), Size(120, 60));
		if (0 < eyes.size())
		{
			left_eye_check = true;
		}
		eyes.clear();
		eye_cascade.detectMultiScale(leftROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(20, 10), Size(120, 60));
		if (0 < eyes.size())
		{
			left_open_eye_check = true;
		}

		eyes.clear();
		right_eye_cascade.detectMultiScale(rightROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(20, 10), Size(120, 60));
		if (0 < eyes.size())
		{
			right_eye_check = true;
		}
		eyes.clear();
		eye_cascade.detectMultiScale(rightROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(20, 10), Size(120, 60));
		if (0 < eyes.size())
		{
			right_open_eye_check = true;
		}

		if (right_eye_check && right_open_eye_check && left_eye_check && left_open_eye_check)
		{
			break;
		}
	}

	if ((left_eye_check && left_open_eye_check) || (right_eye_check && right_open_eye_check))
	{
		if (!detect_frame[frame_index])
		{
			detect_frame[frame_index] = true;
			detect_number++;
		}
	}
	else
	{
		if (detect_frame[frame_index])
		{
			detect_frame[frame_index] = false;
			detect_number--;
		}
	}

	//printf("tick:  %d %d %d %d %d \n", face_check, left_eye_check, left_open_eye_check, right_eye_check, right_open_eye_check);
}

