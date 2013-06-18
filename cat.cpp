/**
 * @function findContours_Demo.cpp
 * @brief Demo code to find contours in an image
 * @author OpenCV team
 */


//
// src -> src_gray -> blur -> canny ==> contours ==> Zones

#include "zone.cpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


cv::Mat src; 
cv::Mat src_gray;
cv::Mat blur;
int thresh = 100;
int max_thresh = 255;
cv::RNG rng(12345);

int blurVal = 25;
int MaxBlur = 100;

int zoneDist = 10;
int maxDist = 50;

/// Function header
void thresh_callback(int, void* );
void blur_callback(int,   void* );
void makeZones_callback(int, void*);

/**
 * @function main
 */
int main( int, char** argv )
{
  /// Load source image and convert it to gray
  src = cv::imread( argv[1], 1 );

  /// Convert image to gray and blur it
  cv::cvtColor( src, src_gray, cv::COLOR_BGR2GRAY );
  cv::blur( src_gray, src_gray, cv::Size(3,3) );

  /// Create Window
  const char* source_window = "Source";
  //namedWindow( source_window, WINDOW_AUTOSIZE );
  cv::namedWindow( source_window, CV_WINDOW_NORMAL );
  cv::imshow( source_window, src );


  cv::createTrackbar( " blur vale   :", "Source", &blurVal,MaxBlur   , blur_callback   );
  blur_callback(0,0);


  cv::createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );

  cv::createTrackbar("zone:","Source", &zoneDist, maxDist, makeZones_callback);
  makeZones_callback(0,0);


  cv::waitKey(0);
  return(0);
}

/**
 * @function thresh_callback
 */
void blur_callback(int, void* )
{
  //std::cout<<std::endl<<blurVal; 
  cv::blur( src_gray ,blur, cv::Size(blurVal/4 + 1,blurVal/4 + 1));  
  thresh_callback(0,0);

  
  cv::namedWindow( "blur", CV_WINDOW_NORMAL );
  cv::imshow( "blur", blur );

}

void thresh_callback(int, void* )
{
  cv::Mat canny_output;
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  /// Detect edges using canny
  cv::Canny( blur, canny_output, thresh, thresh*2, 3 );
  cv::namedWindow("canny",CV_WINDOW_NORMAL);
  cv::imshow("canny",canny_output);
  /// Find contours
  cv::findContours( canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

  /// Draw contours
  cv::Mat drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
  for( size_t i = 0; i< contours.size(); i++ )
  {
       cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       cv::drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, cv::Point() );
  }

  /// Show in a window
  cv::namedWindow( "Contours", CV_WINDOW_NORMAL );
  //namedWindow( "Contours", WINDOW_AUTOSIZE );
  cv::imshow( "Contours", drawing );

  std::cout<<std::endl<<"number of contours -> " <<contours.size();
}


void makeZones_callback(int,void*)
{
  std::cout<<zoneDist<<std::endl;
//  std::vector<zone> myZones;
//  for ( int i = 0; i < contours.size() ; i++)
//  {
//    zone z = new zone;
//    z.addContour(countour[i])
//    int j = i;
//    for(;j < countour.size();j++)
//    {
//       std::vector<cv::Point> points = countour[i];
//      for(int k = 0; k < points.size(); k++)
//	{
//	   if ( zoneDist > z.smallestDistToThePoint(points[k]))
//	   {
//	     z.addContour(contour[j]);
//           j++;  
//	   }
//	}
//
//    }

//  }
}
