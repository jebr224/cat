
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdlib.h>  //abs
#include <vector>
#include <algorithm> //min
#include <math.h>    //sqrt

class mark
{
protected:
   std::vector<cv::Point *> m_ListOfPoints;
   int m_highest_x, m_lowest_x;
   int m_highest_y, m_lowest_y;
   bool m_inZone;
public:
   mark(std::vector<cv::Point> in);
   ~mark();

   int quickDist(cv::Point inPoint);

   int high_x(){return m_highest_x;}
   int high_y(){return m_highest_y;}
   int low_x() {return m_lowest_x;}
   int low_y() {return m_lowest_y;}
   void set_inZone(bool in) {m_inZone = in;}
   bool inZone() {return m_inZone;} 

};

mark::mark(std::vector<cv::Point> in)
{
   if(in.size() > 0)
   {
      m_highest_x = m_lowest_x = in[0].x;
      m_highest_y = m_lowest_y = in[0].y;
   }

   for(unsigned int i = 0; i < in.size(); i++)
   {
       
       cv::Point *temp = new cv::Point;
       temp->x = in[i].x; temp->y =in[i].y;
       m_ListOfPoints.push_back(temp);

       if(m_highest_x <in[i].x) m_highest_x = in[i].x;
       if(m_highest_y <in[i].y) m_highest_y = in[i].y;
      
       if(m_lowest_x > in[i].x) m_lowest_x = in[i].x;
       if(m_lowest_y > in[i].y) m_lowest_y = in[i].y;

   }   
   m_inZone = false;
}

mark::~mark()
{
   for(unsigned int i =0;i< m_ListOfPoints.size();i++)
   {
      delete (m_ListOfPoints[i]);
   }
}

int mark::quickDist(cv::Point inPoint)
{
   
   if(inPoint.x < m_highest_x && inPoint.x  > m_lowest_x &&
      inPoint.y < m_highest_y && inPoint.y  > m_lowest_y    )
    {
        return 0;
    }
   

   if(inPoint.x < m_highest_x && inPoint.x  > m_lowest_x)
   {
       int distHigh = std::abs(inPoint.y - m_highest_y); 
       int distLow  = std::abs(inPoint.y - m_lowest_y);
       return distHigh > distLow ? distLow : distHigh; //return lowest
   }
   if(inPoint.y < m_highest_y && inPoint.y >m_lowest_y)
   {
       int distHigh = std::abs(inPoint.x- m_highest_x);
       int distLow =   std::abs(inPoint.x- m_lowest_x);
       return distHigh > distLow ? distLow : distHigh; //return lowest
   }
 
   std::vector<int> possibleDist; 
   int hh = (m_highest_x - inPoint.x) *  (m_highest_x - inPoint.x) +
            (m_highest_y - inPoint.y) *  (m_highest_y - inPoint.x);

   int hl = (m_highest_x - inPoint.x) *  (m_highest_x - inPoint.x) +
            (m_lowest_y  - inPoint.y) *  (m_lowest_y - inPoint.x);

   int ll = (m_lowest_x -  inPoint.x) *  (m_lowest_x - inPoint.x) +
            (m_lowest_y  - inPoint.y) *  (m_lowest_y - inPoint.x);

   int lh= (m_lowest_x  - inPoint.x) *  (m_lowest_x  - inPoint.x) +
           (m_highest_y - inPoint.y) *  (m_highest_y - inPoint.x);


   return std::sqrt(std::min(hh, std::min(hl, std::min(lh,ll))));
}


