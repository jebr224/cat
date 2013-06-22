/**
 * @function findContours_Demo.cpp
 * @brief Demo code to find contours in an image
 * @author OpenCV team
 */


//
// src -> src_gray -> blur -> canny ==> contours ==> Zones

#include "zone/zone.cpp"
//#include "mark/mark.cpp"
#include "region/region.cpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>



cv::Mat src; 
cv::Mat src_gray;
cv::Mat blur;


cv::Mat draw_again;
cv::Mat colorOutImage;

int thresh = 70;
int max_thresh = 255;
cv::RNG rng(12345);

int blurVal = 12;
int Max_Blur = 100;

int thick = 12;
int max_Thick = 20;

int length = 5;
int max_length = 100;

/// Function header
void thresh_callback(int, void* );
void blur_callback(int,   void* );
void makeZones_callback(int, void*);

void length_callback(int,void*);
std::vector<mark*> listOfMarks;

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


  cv::createTrackbar( "blur vale (px)  :", "Source", &blurVal,Max_Blur   , blur_callback   );


  cv::createTrackbar( "Threshold  :", "Source", &thresh, max_thresh, thresh_callback );

  cv::createTrackbar("Distance to :","Source", &thick, max_Thick, thresh_callback);


  cv::createTrackbar("Horizontal :","Source", &length, max_length, length_callback);


  blur_callback(0,0);
  
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
   cv::threshold( blur, canny_output, thresh, 255 ,1 );

  cv::namedWindow("canny",CV_WINDOW_NORMAL);
  cv::imshow("canny",canny_output);
  /// Find contours
  cv::findContours( canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

  for( int f =0; f <listOfMarks.size();f++)
  {
       delete listOfMarks[f];
  }
  listOfMarks.clear();

  /// Draw contours
  cv::Mat drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
  for( size_t i = 0; i< contours.size(); i++ )
  {
       cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       cv::drawContours( drawing, contours, (int)i, cv::Scalar(255,255,255), thick, 8, hierarchy, 0, cv::Point() );
//       mark *temp = new mark(contours[i]);
//       listOfMarks.push_back(temp);

  } 


  /// Show in a window
  cv::namedWindow( "Contours", CV_WINDOW_NORMAL );
  cv::imshow( "Contours", drawing );
//-------------------------------------------again

    
  std::vector<std::vector<cv::Point> > contours_again;
  std::vector<cv::Vec4i> hierarchy_again;

  draw_again = cv::Mat::zeros(   canny_output.size(), CV_8UC3 );

  colorOutImage = cv::Mat::zeros(canny_output.size(), CV_8UC3);

  cv::Mat temp;
  cv::cvtColor(  drawing,temp, cv::COLOR_BGR2GRAY );
  cv::findContours(  temp , contours_again, hierarchy_again, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
  

  for( size_t i = 0; i< contours_again.size(); i++ )
  {
//       cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       cv::Scalar color = cv::Scalar(255,255,255);
       cv::drawContours( draw_again, contours_again, (int)i, color , 2, 8, hierarchy, 0, cv::Point() );
       mark * temp = new mark(contours_again[i]);
       listOfMarks.push_back(temp);
  }

  /// Show in a window
//  cv::namedWindow( "Contours_again", CV_WINDOW_NORMAL );
//  cv::imshow( "Contours_again", draw_again );



  for(int h = 0; h < listOfMarks.size();h++)
  {
     

     int y  = (listOfMarks[h]->high_y() + listOfMarks[h]->low_y()) / 2;
     int x1 = listOfMarks[h]->high_x() + length;
     int x2 = listOfMarks[h]->low_x() - length;
     cv::Point one = cv::Point(x1,y);
     cv::Point two = cv::Point(x2,y);     
     cv::Scalar white =cv::Scalar(255,255,255); 

     cv::line(draw_again, one, two, white, thick,8,0);

  }

  cv::namedWindow( "Contours_again", CV_WINDOW_NORMAL );
  cv::imshow( "Contours_again", draw_again );

   
  std::vector< std::vector<cv::Point> > contour_3;
  std::vector<cv::Vec4i> hierarchy_3;
  cv::Mat temp_3;
  cv::cvtColor(  draw_again,temp_3, cv::COLOR_BGR2GRAY );
  std::cerr<<"cvtColo" <<std::endl;  
  cv::findContours(  temp_3, contour_3, hierarchy_3, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) ); 
  std::cerr<<"found contours" << std::endl;
  for( size_t i = 0; i< contour_3.size(); i++ )
  {
       cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//       cv::Scalar color = cv::Scalar(255,255,255);
       cv::drawContours( colorOutImage, contour_3, (int)i, color , 2, 8, hierarchy_3, 0, cv::Point() );
  }
  std::cerr<<"drawContours"<<std::endl;  
  
  cv::namedWindow( "Contours_3", CV_WINDOW_NORMAL );
  cv::imshow( "Contours_3", colorOutImage );

  std::cout<<std::endl<<"number of contours -> " <<contours.size();
 
//again--------------------------------------------------------------------------


  


//  makeZones_callback(0,0);
}


void makeZones_callback(int,void*)
{
//  std::cout<<zoneDist<<std::endl;

  std::cout<<std::endl<<"The number of marks is " <<listOfMarks.size();



}


void length_callback( int, void*)
{
  
  std::cout<<std::endl<<"in length  " << length ;
    


}
