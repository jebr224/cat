
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

      void setBlur(int inBlur);
      void setThresh(int inThresh);
      void setThick(int inThick);
      void setLength(int inLength);

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
  m_inputImage = cv::Mat(inputImage); 
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
   for(int i =0; i < tempContoursList.size() ; i++)
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
 



