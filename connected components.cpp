//channels.clear();
		//erode(bin,bin,100);
		//dilate(bin,bin,1000);
		
		/*imshow("dilate",bin);
		Mat labelImage(resultFrame.size(), CV_32S);
		Mat stat,centroid;
		int nLabels= cv::connectedComponentsWithStats(bin, labelImage, stat, centroid, 8);
		//imshow("die",stat);
		std::vector<Vec3b> colors(nLabels);
      	colors[0] = Vec3b(0, 0, 0);//background
   		for (int label = 1; label < nLabels; ++label) {
 		colors[label] = Vec3b((rand() & 255), (rand() & 255), (rand() & 255));}
 		Mat dst(resultFrame.size(), CV_8UC3);
 for (int r = 0; r < dst.rows; ++r) {
     for (int c = 0; c < dst.cols; ++c) {
         int label = labelImage.at<int>(r, c);
         Vec3b &pixel = dst.at<Vec3b>(r, c);

         pixel = colors[label];
     	}
      

}
     vector<Rect> rec;
	
    for (int i = 1;i < nLabels;i++) {
        int x = stat.at<int>(i, CC_STAT_LEFT);
        int y = stat.at<int>(i, CC_STAT_TOP);
        int w = stat.at<int>(i, CC_STAT_WIDTH) ;
        int h = stat.at<int>(i, CC_STAT_HEIGHT);
        //rectangle(resultFrame, Rect(x,y,w,h) , Scalar(0, 255, 255));

         rec.push_back(Rect(x,y,w,h));
        
    }
    int size = rec.size();
	/*for( int i = 0; i < size; i++ )
	{
    	rec.push_back(Rect(rec[i]));
	}*/
	/*double a = rec[0].area();
	Rect largest = rec[0];
	cv::groupRectangles(rec, 200, 1000000);
	for( int i = 1; i < size; i++ )
	{
    	//rectangle(resultFrame, rec[i] , Scalar(0, 255, 255));
    	if(rec[i].area()>a){
    		largest = rec[i];
    		a = rec[i].area();
    	}
	}
	largest += Size(10,10);

	rectangle(resultFrame, largest , Scalar(0, 255, 255));