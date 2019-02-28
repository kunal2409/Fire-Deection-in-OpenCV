#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <bits/stdc++.h>
#include <opencv2/features2d/features2d.hpp>
using namespace std;
using namespace cv;
#include <vector>
#include <sstream>




int main(int argc , char** argv)
{
	cv::VideoCapture video(argv[2]);
	cv::Ptr<cv::BackgroundSubtractor> pMOG2;
	pMOG2 = cv::createBackgroundSubtractorMOG2();
	
	if (!video.isOpened()) {
		std::cout << "Can not open video" << std::endl;
		return 0;
	}

	
	cv::Mat originalFrame;
	cv::Mat resultFrame;
	Mat initial;
	
	
	cv::namedWindow("OriginalVideo", cv::WINDOW_NORMAL);
	cv::namedWindow("ResultVideo", cv::WINDOW_NORMAL);
	
	std::vector<cv::Mat> channels;
	stringstream num(argv[1]);
	int counter;
	num >> counter;

	while (1) {

		vector <Point> points;
		video >> originalFrame;
		video >> originalFrame;
		video >> originalFrame;
		video >> originalFrame;
		video >> originalFrame;
		
		 if (originalFrame.empty())
     		 break;
		
		
		
		
		cv::imshow("OriginalVideo", originalFrame);
		
		ostringstream count;
		count << counter;
		string s = "SmokeFrames/Image";
		s += count.str() + ".jpg";
		
		counter++;
    	cv::imwrite(s,originalFrame);
		

		
	if (cv::waitKey(1) == 27) {
			break;
		}
}
	cout<<counter<<endl;

	

	
	originalFrame.release();
	resultFrame.release();
	pMOG2.release();

	video.release();
	destroyAllWindows();


	return 0;
}
