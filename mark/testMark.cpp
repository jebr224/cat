
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp"
   
#include <iostream>
#include "mark.cpp"

using namespace std;
using namespace cv;
int main()
{
   vector<Point> play;
   play.push_back(Point(3,4));
   play.push_back(Point(8,8));
   mark testMark(play);



}
