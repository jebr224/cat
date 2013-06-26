
//#include "zone/zone.cpp"
#include "mark/mark.cpp"
//#include "region/region.cpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



class artWork
{

   protected:
      cv::Mat m_inputImage, 
              m_blurImage,   m_threshHoldImage,
              m_filledImage, m_outputImage;

      std::vector<mark> m_listOfMarks;
 
     //std::vector<cv::Point>  m_boarder;
      cv::Rect m_rect;
  
      int m_thresh, m_maxThresh;
      int m_blur,   m_maxBlur;
      int m_thick,  m_maxThick;
      int m_length, m_maxLength;



   public:
      artWork();
      artWork(std::string fileName);
      artWork(cv::Mat inputImage);       
      //No dynamic mem, default destructor
      

      cv::Mat applyBlur  (int inBlur);
      cv::Mat applyThresh(int inThresh);
      cv::Mat applyThick (int inThick);
      //cv::Mat applyLength(int inLength);


      cv::Mat applyBlur  ();
      cv::Mat applyThresh();
      cv::Mat applyThick ();
      cv::Mat applyLength();
      cv::Mat drawMarks();

      cv::Rect findSquares(int   length, int width,
                                         float tolerance);

      void setBlur(int inBlur);
      void setThresh(int inThresh);
      void setThick(int inThick);
      void setLength(int inLength);


      double angle( cv::Point pt1, cv::Point pt2, cv::Point pt0 );


      cv::Mat getThreshHoldImage(){return  m_threshHoldImage;}
      cv::Mat getFilledImage() {return m_filledImage;}
      cv::Mat getOutputImage() {return m_outputImage;}


      int getBlur(){return m_blur;}
      int getThresh(){return m_thresh;}
      int getThick(){return m_thick;}
      int getLength(){return m_length;}

};


artWork::artWork(std::string fileName)
{
  m_inputImage = cv::imread(fileName,1);
    
  m_blur = 3; 
  m_maxBlur = 25;

  m_thresh = 50; 
  m_maxThresh = 255;

  m_thick = 12;
  m_maxThick = 20;

  m_length = 5;
  m_maxLength = 100;

 
}


artWork::artWork(cv::Mat inputImage)
{
  std::cerr<<"before";  
  m_inputImage =  cv::Mat(inputImage); 
 // Mat copy constructor

  m_blur = 3; 
  m_maxBlur = 25;

  m_thresh = 50; 
  m_maxThresh = 255;

  m_thick = 12;
  m_maxThick = 20;

  m_length = 5;
  m_maxLength = 100;
}    

artWork::artWork()
{
   return;
}


cv::Mat artWork::applyBlur( )
{
   cv::Mat tempImage;
   cv::cvtColor( m_inputImage, tempImage, 
                 cv::COLOR_BGR2GRAY );

   cv::blur(tempImage,m_blurImage, 
            cv::Size(m_blur+1,m_blur+1)); 
   return m_blurImage;
}


cv::Mat artWork::applyThresh()
{
    cv::threshold( m_blurImage,m_threshHoldImage , m_thresh, 255 ,1 );
    return m_threshHoldImage;
}


cv::Mat artWork::applyThick()
{
   std::vector<std::vector<cv::Point> > tempContoursList;
   std::vector<cv::Vec4i> TempHierarchy;
   m_filledImage = cv::Mat::zeros(m_threshHoldImage.size(),CV_8UC1);
   
   cv::findContours( m_threshHoldImage, tempContoursList, TempHierarchy, 
                    cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, 
                    cv::Point(0, 0) );
   for(unsigned int i =0; i < tempContoursList.size() ; i++)
   {
      cv::drawContours(  m_filledImage, tempContoursList, (int)i, 
                        cv::Scalar(255,255,255), m_thick, 
                        8, TempHierarchy, 0, cv::Point() );  
   }
   
   return m_filledImage;   
 
}

/*
cv::Mat artWork::applyLength(int inLength)
{
   return m_inputImage;
}
*/

cv::Mat artWork::drawMarks()
{
   std::vector<std::vector<cv::Point> > tempContoursList;
   std::vector<cv::Vec4i> TempHierarchy;
   cv::RNG rng(12345);
   

   //  m_outputImage = cv::Mat(m_inputImage);
   m_outputImage = cv::Mat::zeros(m_inputImage.size(),CV_8UC3);
   
   cv::findContours( m_filledImage, tempContoursList, TempHierarchy, 
                    cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, 
                    cv::Point(0, 0) );
   for( size_t i = 0; i<tempContoursList.size() ; i++ )
   {
      cv::Scalar color = cv::Scalar( rng.uniform(0, 255), 
                                     rng.uniform(0, 255), 
                                     rng.uniform(0,255));

      cv::drawContours( m_outputImage, tempContoursList, (int)i, 
                        color , 2, 8, TempHierarchy , 0, cv::Point() );
  }

  return m_outputImage;
}



cv::Rect   artWork::findSquares(int length, int width, float tolerance)
{
     int thresh = 50;
     int N = 11;
     //This code is a adapeted version of the find squares demo code
     //
     std::vector<std::vector<cv::Point> > squares;
     cv::Mat *image;
     image = &m_inputImage;
    
    cv::Mat pyr, timg, gray0(image->size(), CV_8U), gray;

    // down-scale and upscale the image to filter out the noise
    cv::pyrDown(*image, pyr, cv::Size(image->cols/2, image->rows/2));
    cv::pyrUp(pyr, timg, image->size());
    std::vector<std::vector<cv::Point> > contours;

    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        cv::mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. 
                // and set the lower to 0 (which forces edges merging)
                cv::Canny(gray0, gray, 0, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                cv::dilate(gray, gray, cv::Mat(), cv::Point(-1,-1));
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/N;
            }

            // find contours and store them all as a list
            cv::findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

            std::vector<cv::Point> approx;

            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    std::fabs(cv::contourArea(cv::Mat(approx))) > 1000 &&
                    cv::isContourConvex(cv::Mat(approx)) )
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = std::fabs(this->angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                 }
            }
          }
       }
    
    //std::vector<cv::Point>  bestFit;
     int bestFitValue = 2147483647;
    int possibleValue;
   std::vector<cv::Point>  boarder;
   // boarder.clear();
    std::cout<<std::endl<<"the number of sq";
    std::cout<<squares.size() <<std::endl;
    
    if(squares.size() ==0)       
       return m_rect;//bestFit;
    else 
    boarder = squares[0];
    for(unsigned int k = 0; k < squares.size(); k++)
    {
	   int xSmall = std::min(squares[k][0].x, std::min(squares[k][1].x, std::min(squares[k][2].x,squares[k][3].x)));	
	   int xbig = std::max(squares[k][0].x, std::max(squares[k][1].x, std::max(squares[k][2].x,squares[k][3].x)));	
	
	   int ySmall = std::min(squares[k][0].y, std::min(squares[k][1].y, std::min(squares[k][2].y,squares[k][3].y)));	
	   int ybig = std::max(squares[k][0].y, std::max(squares[k][1].y, std::max(squares[k][2].y,squares[k][3].y)));	
	
	   possibleValue = std::pow(width-(xbig-xSmall),2) + std::pow(length-(ybig-ySmall),2);
	  // std::cout<<std::endl<<"->"<<possibleValue<<"   ";
	   //std::cout<<xSmall<<" "<<xbig << " - "
	     //       <<ySmall<<" "<<ybig; 
	   if(possibleValue < bestFitValue)
	   {
		   bestFitValue = possibleValue;
		   boarder = squares[k];
	   }
	}
    m_rect = cv::boundingRect( cv::Mat(boarder));
    
return m_rect;

}

double artWork::angle( cv::Point pt1, cv::Point pt2, cv::Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
cv::Mat artWork::applyBlur(int inThresh)
{
   m_blur = inThresh;
   return this->applyBlur();
}

cv::Mat artWork::applyThresh(int inThresh)
{
   m_thresh = inThresh;
   return this->applyThresh();
}

cv::Mat artWork::applyThick(int inThick)
{
   m_thick = inThick;
   return this->applyThick();
}
/*
cv::Mat artWork::applyLength(int inLength)
{
   m_length = inLength;
   return this->applyLength();
}
*/

void artWork::setBlur(int inBlur)
{
   if(inBlur > 0)
   {
      if(inBlur < m_maxBlur)
         m_blur = inBlur;
      else m_blur = m_maxBlur;
   }
   else m_blur = 0;
}
void artWork::setThresh(int inThresh)
{
   if(inThresh > 0)
   {
      if(inThresh < m_maxThresh)
         m_thresh = inThresh;
      else m_thresh = m_maxThresh;
   }
   else m_thresh = 0;
}
void artWork::setThick(int inThick)
{
   if(inThick > 0)
   {
      if(inThick< m_maxThick)
         m_thick = inThick;
      else m_thick = m_maxThick;
   }
   else m_thick = 0;
}
void artWork::setLength(int inLength)
{
   if(inLength > 0)
   {
      if(inLength < m_maxLength)
         m_length = inLength;
      else m_length = m_maxLength;
   }
   else m_length = 0;
}
 



