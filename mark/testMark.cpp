
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp"
   
#include <iostream>
#include "mark.cpp"

using namespace std;
using namespace cv;
int main()
{
   vector<Point> play;
   play.push_back(Point(10,10));
   play.push_back(Point(1,1));
   play.push_back(Point(5,2));
   play.push_back(Point(8,3));
   mark testMark(play);
   std::cout<<"the lowest  x is " << testMark.low_x() << std::endl;
   std::cout<<"the highest x is " <<testMark.high_x()<<std::endl;

   std::cout<<"the lowest  y is " << testMark.low_y() << std::endl;
   std::cout<<"the highest y is " <<testMark.high_y()<<std::endl;

   std::cout<<"The dist to the point (3,3) is  ";
   std::cout<<testMark.quickDist(cv::Point(3,3)) << std::endl;

   std::cout<<"The dist to the point (5,12) is  ";
   std::cout<<testMark.quickDist(cv::Point(5,12)) << std::endl;

   std::cout<<"The dist to the point (5,-12) is  ";
   std::cout<<testMark.quickDist(cv::Point(5,-12)) << std::endl;


   std::cout<<"The dist to the point (12,5) is  ";
   std::cout<<testMark.quickDist(cv::Point(12,5)) << std::endl;


   std::cout<<"The dist to the point (-12,5) is  ";
   std::cout<<testMark.quickDist(cv::Point(-12,5)) << std::endl;

   std::cout<<"The dist to the point (12,12) is  ";
   std::cout<<testMark.quickDist(cv::Point(12,12)) << std::endl;


   std::cout<<"The dist to the point (-1,-1) is  ";
   std::cout<<testMark.quickDist(cv::Point(-1,-1)) << std::endl;

}
