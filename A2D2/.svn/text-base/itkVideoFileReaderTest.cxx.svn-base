#include "cv.h"
#include "highgui.h"
#include "iostream"

#include "itkVideoFileReader.h"
#include "itkVideoFileWriter.h"
#include "itkFaceDetectionFilter.h"

/*
// I leave that here, just in case.
// It shouldn't have to be used though.
void DrawRectangles (CvSeq *faces, IplImage* img)
{
   assert(faces);

  // Create two points to represent the face locations
  CvPoint pt1, pt2;
  int i;

  // Loop the number of faces found.
  for( i = 0; i < (faces ? faces->total : 0) ; i++ )
    {
    // Create a new rectangle for drawing the face
    CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
    assert(r);
    // Find the dimensions of the face,and scale it if necessary
    pt1.x = r->x;
    pt2.x = (r->x+r->width);
    pt1.y = r->y;
    pt2.y = (r->y+r->height);
    // Draw the rectangle in the input image
    cvRectangle( img, pt1, pt2,
      CV_RGB(255,0,0),3, 8, 0 );
    }
}

void DetectFaces ( IplImage *img )
{
  assert( img ); 

  CvMemStorage* storage = 0;
  CvHaarClassifierCascade* cascade = 0;
  
  // Load the HaarClassifierCascade
  cascade= (CvHaarClassifierCascade*)cvLoad( "./Debug/haarcascade_frontalface_alt.xml" ); 
  assert( cascade ); 


  // Allocate the memory storage
  storage = cvCreateMemStorage(0);
  assert( storage );

  // Find whether the cascade is loaded, to find the faces. If yes, then:
  if( cascade )
    {
    // There can be more than one face in an image. So create a growable sequence of faces.
    // Detect the objects and store them in the sequence
    CvSeq* faces = cvHaarDetectObjects( img, cascade, storage,
                                        1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                        cvSize(40, 40) );

     DrawRectangles(faces,img);

    }
  else
    {
      std::cerr<<"Too bad, didn't worked"<<std::endl;
    }
}*/


int main (int argv, char **argc)
{
  typedef itk::Image< unsigned char, 2>   OutputImageType;  
  itk::VideoFileReader< OutputImageType >::Pointer reader = itk::VideoFileReader< OutputImageType >::New();
  reader->SetFileName("C:/projects/ITK-A2D2/A2D2_build/Debug/25_26_L_Echelle_de_Perceval_La_Chambre_de_la_Reine.avi");
 
  //reader->LoadVideo();
  
  unsigned long FrameTotal = reader->GetFrameTotal();
  unsigned long i;

   itk::FaceDetectionFilter< OutputImageType >::Pointer filter = itk::FaceDetectionFilter< OutputImageType >::New();
  filter->SetTrainerFileName("./Debug/haarcascade_frontalface_alt.xml" ); 
  filter->SetInput(reader->GetOutput());

  itk::VideoFileWriter<OutputImageType>::Pointer VideoWriter = itk::VideoFileWriter<OutputImageType>::New();
  VideoWriter->SetInput(filter->GetOutput());
  VideoWriter->SetFileName("C:/projects/ITK-A2D2/A2D2_build/images/Gray_Kaamelot.avi");

  VideoWriter->EndVideo();

  for (i  = 0; i<FrameTotal-10; i++)
    {
    try
      {
      VideoWriter->Update();
      }
    catch (itk::ExceptionObject &e)
      {
      reader->Print(std::cout);
      VideoWriter->Print(std::cout);
      std::cerr<<e.GetFile()<<std::endl;
      std::cerr<<e.GetLine()<<std::endl;
      std::cerr<<e.GetLocation()<<std::endl;
      std::cerr<<e.GetNameOfClass()<<std::endl;
      std::cerr<<e.GetDescription()<<std::endl;
      }
    reader->KeepReading();
    }

  std::cout<<"Done !"<<std::endl;
  return EXIT_SUCCESS;
}
