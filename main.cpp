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
	//Reading the video
	cv::VideoCapture video(argv[1]);
	//Creating the Background Subtractor object
	cv::Ptr<cv::BackgroundSubtractor> pMOG2;
	pMOG2 = cv::createBackgroundSubtractorMOG2();
	
	if (!video.isOpened()) {
		std::cout << "Can not open video" << std::endl;
		return 0;
	}

	
	cv::Mat originalFrame;
	cv::Mat resultFrame;
	Mat initial;
	//Reading the initial frame 
	video >> initial;
	//Reading in Grayscale
	cvtColor(initial, initial , COLOR_BGR2GRAY);
	//Making a binary image
	cv::Mat test(initial.size(), CV_8UC1); 
		test = Scalar(0);
	
	cv::namedWindow("OriginalVideo", cv::WINDOW_NORMAL);
	cv::namedWindow("ResultVideo", cv::WINDOW_NORMAL);
	
	std::vector<cv::Mat> channels;
	int frame_width = video.get(CV_CAP_PROP_FRAME_WIDTH); 
    int frame_height = video.get(CV_CAP_PROP_FRAME_HEIGHT);
    //Videowriter object to write the resulting video
	VideoWriter video1(argv[2],CV_FOURCC('H','2','6','4'),24,Size(frame_width,frame_height));

	while (1) {
		//this vector contains the coordinates of the fire pixels 
		vector <Point> points;
		video >> originalFrame;
		
		
		 if (originalFrame.empty())
     		 break;
		
		
		//Blurring the frame
		cv::medianBlur(originalFrame, resultFrame, 3);

		//Splitting into channels
		cv::split(resultFrame, channels);
		//Creating a binary image
		cv::Mat bin(resultFrame.size(), CV_8UC1); 
		bin = Scalar(0);

		Mat redChannel,bluechannel,greenchannel;
		redChannel = channels[2];
		bluechannel = channels[0];
		greenchannel = channels[1];

		


		Mat temp(redChannel);
		Mat ycbr,hsv;
		float r=0, g=0, b=0;
		
		//Applying the background subtractor to the frame
		pMOG2->apply(temp, temp);

		//RGB to YCbCr colorspace
		cvtColor(resultFrame,ycbr,CV_BGR2YCrCb);
		
		float Y=0, cr=0, cb=0;
		int k=0;

		//Calculating the mean luminance value of the all the pixels
		for(int j=0;j<temp.rows;j++){
			for(int i=0;i<temp.cols;i++){
				
				Vec3b color = resultFrame.at<Vec3b>(Point(i,j));
				Vec3b channel = ycbr.at<Vec3b>(Point(i,j));
				
				Y += channel[0];
				cr += channel[1];
				cb += channel[2];
				k++;
			}
		}

		float ym = Y/k, crm=cr/k,cbm = cb/k;
		
        
       




		//Detecting fire pixel 
		for(int j=0;j<temp.rows;j++){
			for(int i=0;i<temp.cols;i++){
				
				Vec3b color = resultFrame.at<Vec3b>(Point(i,j));
				Vec3b channel = ycbr.at<Vec3b>(Point(i,j));
			
				//if pixel value is 1 in the Foreground Image
				if(temp.at<uchar>(j,i)){
					
						
					//Color Rule
					if( channel[0] > channel[2] && channel[1]>channel[2]){
						if(channel[0]>ym && channel[1]>crm && channel[2]<cbm){
								
							if(color[2]>190 && color[1]>100 && color[0]<140){
									
								test.at<uchar>(j,i) += 1;
								if(test.at<uchar>(j,i)>15){
									points.push_back(Point(i,j));
									bin.at<uchar>(j,i) = 255;
								}
									
							}
						}



					}

				}

			}

				
		}

		
			
		

		

	
		//imshow("binary", bin);
		Mat frame,res;
		cvtColor(resultFrame, frame, COLOR_BGR2GRAY);
		vector <Point> fire;
		// Calculating the optical flow
		calcOpticalFlowFarneback(initial, frame, res , 0.5, 1, 10, 2, 5, 1.1, 0);
		 for(int x=0;x<points.size();x+=2)
        {
            // get the flow from y, x position * 3 for better visibility
            const Point2f flowatxy = res.at<Point2f>(points[x].y, points[x].x);
            //Optical Flow thresholds
            if((abs(flowatxy.x) > 0.2 && abs(flowatxy.y) > 0.2) && abs(atan(flowatxy.y/flowatxy.x))>0.3){
            	
            // draw line at flow direction
            //line(resultFrame, points[x], Point(cvRound(points[x].x + flowatxy.x), cvRound(points[x].y + flowatxy.y)), Scalar(255, 0, 0));
            
            fire.push_back(points[x]);
        	}
                
        }

        //Draw the Bounding Rectangle around the Detected points
    	cv::Rect rt = cv::boundingRect(fire);
        Scalar col = Scalar(0,255,0);
        rt.height += 10;
		rt.width += 10;
		if(rt.area() >=100){

    	rectangle(resultFrame,rt,col,4);
		}        

		//Show the inputs and outputs on the separate windows
		cv::resizeWindow("OriginalVideo", 600,600);
		cv::resizeWindow("ResultVideo", 600,600);
		cv::imshow("OriginalVideo", originalFrame);
		cv::imshow("ResultVideo", resultFrame);
		video1.write(resultFrame);
		

		
	if (cv::waitKey(1) == 27) {
			break;
		}
}
	

	

	
	originalFrame.release();
	resultFrame.release();
	pMOG2.release();

	video.release();
	video1.release();
	destroyAllWindows();


	return 0;
}