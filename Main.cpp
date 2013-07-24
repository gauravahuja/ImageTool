#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "Threshold.h"

int main(int argc, char* argv[])
{
	IplImage* Frame;
	CvCapture* Capture = NULL;
	CvVideoWriter* writer;
	_Threshold ThresholdValues[2];
	
	if(argc == 1)
		Capture = cvCaptureFromCAM(-1);
	else if(argc == 2)
	{
		Capture = cvCaptureFromCAM(-1);
	}
	else if(argc == 3)
	{
		if(!_strcmpi("pic", argv[1]))
		{
			Frame = cvLoadImage(argv[2]);
			GetThreshold(Frame, ThresholdValues);
			return 0;
		}
		else if(!_strcmpi("video", argv[1]))
		{
			Capture = cvCaptureFromFile(argv[2]);
		}
		else
		{
			return -1;
		}
	}
	else
		return -1;
	if(Capture == NULL)
		return -1;

	Frame = cvQueryFrame(Capture);

	if(argc == 2)
		writer = cvCreateVideoWriter("Output.avi", -1,  15, cvGetSize(Frame));

	if(Frame == NULL)
		return -1;
	
	IplImage* FrameCopy = cvCloneImage(Frame);
	char c = 0;
	
	
	cvNamedWindow("Capture");
	while(Frame && c != 27)
	{
		cvCopyImage(Frame, FrameCopy);
				
		cvShowImage("Capture", FrameCopy);
		c = cvWaitKey(35);
		if(c == 't' || c == 'T')
		{
			GetThreshold(FrameCopy, ThresholdValues);
		}
		Frame = cvQueryFrame(Capture);
		if(argc == 2)
		{
			cvWriteFrame(writer, Frame);
		}
	}
	
	if(argc == 2)
		cvReleaseVideoWriter(&writer);
	cvReleaseImage(&FrameCopy);
	cvReleaseCapture(&Capture);
	cvDestroyAllWindows();
	return 0;
}