
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "kat.cpp"

void blurWrapper(int, void*);
void threshWrapper(int, void*);
void thickWrapper(int, void*);
void drawWrapper(int, void*);

int blur,thresh,thick,length;
artWork chad; 

int main(int argc, char* argv[])
{
   if(argc <2)
   {
      std::cout<<"no input file" <<std::endl;
      //printHelp()
      return 1;
   }
   
   chad = artWork(argv[1]);

   blur =   chad.getBlur();
   thresh = chad.getThresh();
   thick =  chad.getThick();
   length = chad.getLength();
   
   cv::namedWindow( "Source", CV_WINDOW_NORMAL ) ;
   cv::createTrackbar("blur","Source", &blur, 25, blurWrapper);
   cv::createTrackbar("thresh","Source", &thresh, 255, threshWrapper);
   cv::createTrackbar("Thick","Source", &thick, 20, thickWrapper);
   //cv::createTrackbar("Horizontal","Source", &horz, 100, thresh_callback);
   blurWrapper(0,0);
   cv::waitKey(0);

}


void blurWrapper(int, void*)
{
   cv::Mat blurImage = chad.applyBlur(blur);
   cv::namedWindow( "blurImage", CV_WINDOW_NORMAL );
   cv::imshow( "blurImage", blurImage );
   threshWrapper(0,0);   
 }
void threshWrapper(int, void*)
{  
   cv::Mat threshImage = chad.applyThresh(thresh);
   cv::namedWindow( "ThreshImage", CV_WINDOW_NORMAL );
   cv::imshow( "ThreshImage", threshImage );
   thickWrapper(0,0) ;
}
void thickWrapper(int, void*)
{
   cv::Mat thickImage = chad.applyThick(thick);
   cv::namedWindow( "thickImage", CV_WINDOW_NORMAL );
   cv::imshow( "thickImage", thickImage ); 
   drawWrapper(0,0);
}
void drawWrapper(int, void*)
{
   cv::Mat draw = chad.drawMarks();
   cv::namedWindow( "draw", CV_WINDOW_NORMAL );
   cv::imshow( "draw", draw ); 
}

