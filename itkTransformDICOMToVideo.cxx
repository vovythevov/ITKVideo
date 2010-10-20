#include "iostream"

#include "itkVideoFileWriter.h"
#include "itkImageFileReader.h"

// Example on how to create a video from a dicom image.
// In this case the image has the name CTHead1.dcm to CTHead93.dcm
// and is located in the folder C:/projects/ITK-A2D2/A2D2_build/Debug/CTHeadAxialDicom
// we save the video in the folder C:/projects/ITK-A2D2/A2D2_build/images
// under the name DicomImageAsAVideo.avi

int test_writer (std::string InputWhitoutExtension, std::string Output, bool writerUseOpenCV, int start, int end)
{
  typedef itk::Image<unsigned char, 2>   OutputImageType;  
  itk::ImageFileReader< OutputImageType >::Pointer reader = itk::ImageFileReader< OutputImageType >::New();

  unsigned long i;
  char buf[2000];

  itk::VideoFileWriter<OutputImageType>::Pointer VideoWriter = itk::VideoFileWriter<OutputImageType>::New();
  VideoWriter->SetInput(reader->GetOutput());
  VideoWriter->SetFileName(Output.c_str());
  VideoWriter->UseOpenCV(writerUseOpenCV);

  for (i = start; i <= end ; i ++ )
    {
    //To set a different filename each time
    std::string filename = InputWhitoutExtension;
    filename += itoa(i,buf,10);
    filename += ".dcm";
    reader->SetFileName(filename.c_str()); 
    try
      {
      //Appending the frame in the video
      VideoWriter->Update();
      }
    catch (itk::ExceptionObject &e)
      {
      std::cerr<<i<<std::endl;
      VideoWriter->Print(std::cout);
      std::cerr<<e.GetFile()<<std::endl;
      std::cerr<<e.GetLine()<<std::endl;
      std::cerr<<e.GetLocation()<<std::endl;
      std::cerr<<e.GetNameOfClass()<<std::endl;
      std::cerr<<e.GetDescription()<<std::endl;
      return EXIT_FAILURE;
      }
    }
  //Optionnal
  VideoWriter->EndVideo();

  std::cout<<"Done !"<<std::endl;
  return EXIT_SUCCESS;
}

int main (int argv, char **argc)
{
  return test_writer(argc[1],argc[2],atoi(argc[3]),atoi(argc[4]),atoi(argc[5]));
}