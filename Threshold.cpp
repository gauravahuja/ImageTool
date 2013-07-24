#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <math.h>
#include "Threshold.h"

int SelectionStarted = 0;
int RightButtonDown = 0;
int SelectionMade = 0;
CvRect Selection;
CvPoint Origin;

char TrackBarWindow[] = "TrackBar Window";

//TrackBarWindow Mouse callback function.
void TrackBarWindowMouseCallBack(int Event, int x, int y, int flags, void* Frame)
{
	if(SelectionStarted)
	{
		Selection.x = MIN(x,Origin.x);
        Selection.y = MIN(y,Origin.y);
		Selection.width = abs(x - Origin.x);
        Selection.height = abs(y - Origin.y);
		if(Selection.height <= 0)
			Selection.height = 1;
		if(Selection.width <= 0)
			Selection.width = 1;
	}
	switch(Event)
	{
	case CV_EVENT_RBUTTONDOWN:
		Origin = cvPoint(x, y);
		RightButtonDown = 1;
		SelectionMade = 0;
		break;
	case CV_EVENT_RBUTTONUP:
		RightButtonDown = 0;
		SelectionMade = 0;
		break;
	case CV_EVENT_LBUTTONDOWN:
		SelectionMade = 0;
		if(SelectionStarted == 0)
		{
			Origin = cvPoint(x, y);
			Selection = cvRect(x, y, 1, 1);
			SelectionStarted = 1;
		}
		break;
	case CV_EVENT_LBUTTONUP:
		SelectionMade = 1;
		SelectionStarted = 0;
		break;
	}
}

void SetTrackBarPostion(CvPoint Point, _Threshold BGR, _Threshold HSV)
{
	CvFont Font;
	cvInitFont(&Font, CV_FONT_HERSHEY_PLAIN, 1, 1);

	char TrackBarStrings[27][80] = 
	{
		"X       :  ",
		"Y       :  ",
		"------------",
		"Bmax   :  ",
		"Gmax   :  ",
		"Rmax   :  ",
		"------------",
		"Bmin    :  ",
		"Gmin    :  ",
		"Rmin    :  ",
		"------------",
		"Bavg    :  ",
		"Gavg    :  ",
		"Ravg    :  ",
		"------------",
		"Hmax   :  ",
		"Smax   :  ",
		"Vmax   :  ",
		"------------",
		"Hmin    :  ",
		"Smin    :  ",
		"Vmin    :  ",
		"------------",
		"Havg    :  ",
		"Savg    :  ",
		"Vavg    :  ",
		"------------",
	};

	char Number[10];

	_itoa(Point.x, Number, 10);
	strcat(TrackBarStrings[X], Number);
	_itoa(Point.y, Number, 10);
	strcat(TrackBarStrings[Y], Number);

	_itoa(cvRound(BGR.Max.val[0]), Number, 10);
	strcat(TrackBarStrings[Bmax], Number);
	_itoa(cvRound(BGR.Max.val[1]), Number, 10);
	strcat(TrackBarStrings[Gmax], Number);
	_itoa(cvRound(BGR.Max.val[2]), Number, 10);
	strcat(TrackBarStrings[Rmax], Number);
	
	_itoa(cvRound(BGR.Min.val[0]), Number, 10);
	strcat(TrackBarStrings[Bmin], Number);
	_itoa(cvRound(BGR.Min.val[1]), Number, 10);
	strcat(TrackBarStrings[Gmin], Number);
	_itoa(cvRound(BGR.Min.val[2]), Number, 10);
	strcat(TrackBarStrings[Rmin], Number);

	_itoa(cvRound(BGR.Avg.val[0]), Number, 10);
	strcat(TrackBarStrings[Bavg], Number);
	_itoa(cvRound(BGR.Avg.val[1]), Number, 10);
	strcat(TrackBarStrings[Gavg], Number);
	_itoa(cvRound(BGR.Avg.val[2]), Number, 10);
	strcat(TrackBarStrings[Ravg], Number);
	
	_itoa(cvRound(HSV.Max.val[0]), Number, 10);
	strcat(TrackBarStrings[Hmax], Number);
	_itoa(cvRound(HSV.Max.val[1]), Number, 10);
	strcat(TrackBarStrings[Smax], Number);
	_itoa(cvRound(HSV.Max.val[2]), Number, 10);
	strcat(TrackBarStrings[Vmax], Number);
	
	_itoa(cvRound(HSV.Min.val[0]), Number, 10);
	strcat(TrackBarStrings[Hmin], Number);
	_itoa(cvRound(HSV.Min.val[1]), Number, 10);
	strcat(TrackBarStrings[Smin], Number);
	_itoa(cvRound(HSV.Min.val[2]), Number, 10);
	strcat(TrackBarStrings[Vmin], Number);

	_itoa(cvRound(HSV.Avg.val[0]), Number, 10);
	strcat(TrackBarStrings[Havg], Number);
	_itoa(cvRound(HSV.Avg.val[1]), Number, 10);
	strcat(TrackBarStrings[Savg], Number);
	_itoa(cvRound(HSV.Avg.val[2]), Number, 10);
	strcat(TrackBarStrings[Vavg], Number);
	
	CvSize Size = cvSize(0, 0);
	CvSize Temp;
	int Baseline = -10;
	int i;
	
	for(i = 0; i < 27; i++)
	{
		cvGetTextSize(TrackBarStrings[i], &Font, &Temp, &Baseline);
		if(Temp.width > Size.width)
			Size.width = Temp.width + 20;
		Size.height += Temp.height + 5;
	}

	IplImage* Text = cvCreateImage(Size, IPL_DEPTH_8U, 1);
	cvSet(Text, cvScalarAll(0));

	for(i = 0; i < 27; i++)
	{
		cvPutText(Text, TrackBarStrings[i], cvPoint(10, Size.height*i/27 +  20), &Font, cvScalarAll(255));
	}

	cvShowImage(TrackBarWindow, Text);	
	cvReleaseImage(&Text);
}

void GetThreshold(IplImage* Frame/*BGR Image*/, _Threshold SelectionThresholds[2])
{
	//_Threshold Array of size 2
	//Element 0 being BGR Threshold
	//Element 1 being HSV Threshold
	
	IplImage* FrameCopy = cvCreateImage(cvGetSize(Frame), IPL_DEPTH_8U, 3);
	IplImage* FrameCopyHSV = cvCreateImage(cvGetSize(Frame), IPL_DEPTH_8U, 3);
	cvCopyImage(Frame, FrameCopy);
	cvCvtColor(FrameCopy, FrameCopyHSV, CV_BGR2HSV);
	IplImage* DisplayImage = cvCreateImage(cvGetSize(Frame), IPL_DEPTH_8U, 3);
	cvCopyImage(Frame, DisplayImage);

	int i;
	double MaxVal, MinVal;

	cvNamedWindow(TrackBarWindow);
		
	cvNamedWindow("Threshold Frame");
	cvShowImage("Threshold Frame", DisplayImage);
	cvSetMouseCallback("Threshold Frame", TrackBarWindowMouseCallBack);

	while(cvWaitKey(10) != 27)
	{
		if(SelectionStarted && Selection.width > 0 && Selection.height > 0)
		{
			cvCopyImage(Frame, DisplayImage);
			cvSetImageROI(DisplayImage, Selection);
			cvXorS(DisplayImage, cvScalarAll(255), DisplayImage);
			cvResetImageROI(DisplayImage);
			cvShowImage("Threshold Frame", DisplayImage);
		}
		if(RightButtonDown)
		{
			SelectionThresholds[0].Max = cvGet2D(FrameCopy, Origin.y, Origin.x);
			SelectionThresholds[0].Min = SelectionThresholds[0].Max;
			SelectionThresholds[0].Avg = SelectionThresholds[0].Max;
			
			SelectionThresholds[1].Max = cvGet2D(FrameCopyHSV, Origin.y, Origin.x);
			SelectionThresholds[1].Min = SelectionThresholds[1].Max;
			SelectionThresholds[1].Avg = SelectionThresholds[1].Max;
			
			SetTrackBarPostion(Origin, SelectionThresholds[0], SelectionThresholds[1]);			
		}
		if(SelectionMade)
		{
			cvSetImageROI(FrameCopy, Selection);
			cvSetImageROI(FrameCopyHSV, Selection);
			
			for(i = 0; i < 3; i++)
			{
				cvSetImageCOI(FrameCopy, i+1);
				cvMinMaxLoc(FrameCopy, &MinVal, &MaxVal);
				SelectionThresholds[0].Max.val[i] = MaxVal;
				SelectionThresholds[0].Min.val[i] = MinVal;
			}
			cvSetImageCOI(FrameCopy, 0);
			SelectionThresholds[0].Avg = cvAvg(FrameCopy);
			
			for(i = 0; i < 3; i++)
			{
				cvSetImageCOI(FrameCopyHSV, i+1);
				cvMinMaxLoc(FrameCopyHSV, &MinVal, &MaxVal);
				SelectionThresholds[1].Max.val[i] = MaxVal;
				SelectionThresholds[1].Min.val[i] = MinVal;
			}
			cvSetImageCOI(FrameCopyHSV, 0);			
			SelectionThresholds[1].Avg = cvAvg(FrameCopy);
			SetTrackBarPostion(cvPoint(Selection.x, Selection.y), SelectionThresholds[0], SelectionThresholds[1]);
			
			cvResetImageROI(FrameCopy);
			cvResetImageROI(FrameCopyHSV);
		}
	}
	cvReleaseImage(&FrameCopy);
	cvReleaseImage(&FrameCopyHSV);
	cvReleaseImage(&DisplayImage);
	cvDestroyWindow("Threshold Frame");
	cvDestroyWindow(TrackBarWindow);
}
