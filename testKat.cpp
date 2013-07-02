
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "kat.cpp"

void blurWrapper(int, void*);
void threshWrapper(int, void*);
void thickWrapper(int, void*);
void lengthWapper(int, void*);
void drawWrapper(int, void*);
void shrinkBoarder(int, void*);

int blur,thresh,thick,length,horz,adjBoarder;
artWork chad; 
   cv::Mat page;
   cv::Mat inImage;
   cv::Rect boarder;
   artWork wholeImage;

int main(int argc, char* argv[])
{
   if(argc <2)
   {
      std::cout<<"no input file" <<std::endl;
      //printHelp()
      return 1;
   }
    
   //use the file name to build an image
   //use the image to construct an object
   //Find a rect that is closest to 700 by 450
   //display the inImage and the rect
   inImage = cv::imread(argv[1],1);
   wholeImage = artWork(inImage);
   boarder = wholeImage.findSquares(1500,1000,0);
   cv::rectangle( inImage,
           boarder,
           cv::Scalar( 0, 255, 255 ),
           10,
           8 );



   inImage(boarder).copyTo(page);
   chad = artWork(page);

   
   blur =   chad.getBlur();
   thresh = chad.getThresh();
   thick =  chad.getThick();
   length = chad.getLength();
   
   
   cv::namedWindow( "Source", CV_WINDOW_NORMAL ) ;
   cv::createTrackbar("boarder","Source",&adjBoarder,500,shrinkBoarder);
   cv::createTrackbar("blur","Source", &blur, 25, blurWrapper);
   cv::createTrackbar("thresh","Source", &thresh, 255, threshWrapper);
   cv::createTrackbar("Thick","Source", &thick, 20, thickWrapper);
   cv::createTrackbar("Horizontal","Source", &horz, 100, lengthWapper);
   
   
   blurWrapper(0,0);
      
     
   cv::waitKey(0);

}

void shrinkBoarder(int, void*)
{
   
   
   cv::Mat tempCopyOfInImage = inImage.clone();
   
   cv::Rect tempBoarder;
   tempBoarder.x = boarder.x + adjBoarder;
   tempBoarder.y = boarder.y + adjBoarder;
   tempBoarder.width = boarder.width - adjBoarder*2;
   tempBoarder.height = boarder.height - adjBoarder*2;
   
   cv::rectangle( tempCopyOfInImage,
           tempBoarder,
           cv::Scalar( 255, 0, 255 ),
           10,
           8 );
   
   
   cv::Mat tempImage = inImage.clone();
   cv::namedWindow( "boarder", CV_WINDOW_NORMAL ) ;
   cv::imshow( "boarder",tempCopyOfInImage );
   
   inImage(tempBoarder).copyTo(page);
   chad = artWork(page);
   blurWrapper(0,0);
	
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
   lengthWapper(0,0);
}
void lengthWapper(int, void*)
{
   cv::Mat lengthImage = chad.applyLength(horz);
   cv::namedWindow( "length", CV_WINDOW_NORMAL );
   cv::imshow( "length", lengthImage );
   drawWrapper(0,0);

}

void drawWrapper(int, void*)
{
   cv::Mat draw = chad.drawMarks();
   cv::namedWindow( "draw", CV_WINDOW_NORMAL );
   cv::imshow( "draw", draw ); 
}

