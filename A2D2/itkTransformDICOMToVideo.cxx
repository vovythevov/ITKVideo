#include "iostream"
#include "cv.h"
#include "highgui.h"

#include "itkVideoFileWriter.h"
#include "itkImageFileReader.h"

// Example on how to create a video from a dicom image.
// In this case the image has the name CTHead1.dcm to CTHead93.dcm
// and is located in the folder C:/projects/ITK-A2D2/A2D2_build/Debug/CTHeadAxialDicom
// we save the video in the folder C:/projects/ITK-A2D2/A2D2_build/images
// under the name DicomImageAsAVideo.avi

int main (int argv, char **argc)
{
  typedef itk::Image<char, 2>   OutputImageType;  
  itk::ImageFileReader< OutputImageType >::Pointer reader = itk::ImageFileReader< OutputImageType >::New();

  unsigned long i;
  char buf[2000];

  itk::VideoFileWriter<OutputImageType>::Pointer VideoWriter = itk::VideoFileWriter<OutputImageType>::New();
  VideoWriter->SetInput(reader->GetOutput());
  VideoWriter->SetFileName("C:/projects/ITK-A2D2/A2D2_build/images/DicomImageAsAVideo.avi");

  for (i = 1; i <= 93 ; i ++ )
    {
    //To set a different filename each time
    std::string filename = "C:/projects/ITK-A2D2/A2D2_build/Debug/CTHeadAxialDicom/CTHead";
    filename += itoa(i,buf,10);
    filename += ".dcm";
    reader->SetFileName(filename.c_str());
    try 
      {  
      try
        {
        //Appending the frame in the video
        VideoWriter->Update();
        }
      catch (itk::ExceptionObject &e)
        {
        VideoWriter->Print(std::cout);
        std::cerr<<e.GetFile()<<std::endl;
        std::cerr<<e.GetLine()<<std::endl;
        std::cerr<<e.GetLocation()<<std::endl;
        std::cerr<<e.GetNameOfClass()<<std::endl;
        std::cerr<<e.GetDescription()<<std::endl;
        }
      }
    catch (cv::Exception &e)
      {
      std::cerr<<e.err<<std::endl;
      std::cerr<<e.func<<std::endl;
      std::cerr<<e.line<<std::endl;
      std::cerr<<e.msg<<std::endl;
      }
    }
  //Optionnal
  VideoWriter->EndVideo();

  std::cout<<"Done !"<<std::endl;
  return EXIT_SUCCESS;
}