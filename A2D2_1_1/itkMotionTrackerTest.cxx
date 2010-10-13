#include "iostream"

#include "itkVideoFileReader.h"
#include "itkVideoFileWriter.h"
#include "itkDifferenceImageFilter.h"


int main (int argv, char **argc)
{
  typedef itk::Image< unsigned char, 2>   OutputImageType;  
  itk::VideoFileReader< OutputImageType >::Pointer TestReader = itk::VideoFileReader< OutputImageType >::New();
  TestReader->SetFileName("./Testing/Data/inde-circulation.avi");
  itk::VideoFileReader< OutputImageType >::Pointer ValidReader = itk::VideoFileReader< OutputImageType >::New();
  ValidReader->SetFileName("./Testing/Data/inde-circulation.avi");

  TestReader->SetNextFrameIsFrameRequested(true);
  TestReader->SetFrameRequested(1);

  itk::DifferenceImageFilter<OutputImageType,OutputImageType>::Pointer filter
    = itk::DifferenceImageFilter<OutputImageType,OutputImageType>::New();
  filter->SetTestInput(TestReader->GetOutput());
  filter->SetValidInput(ValidReader->GetOutput());

  unsigned long i;

  itk::VideoFileWriter<OutputImageType>::Pointer VideoWriter = itk::VideoFileWriter<OutputImageType>::New();
  VideoWriter->SetInput(filter->GetOutput());
  VideoWriter->SetFileName("./Testing/Results/Motion_Tracker_Indian_Crossroad.avi");

  for (i = 0; i < 1000; i ++ )
    {
    
    try
      {
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
      return EXIT_FAILURE;
      }

    if ( i == 0 )
      {
      TestReader->SetNextFrameIsFrameRequested(false);
      }

    ValidReader->KeepReading();
    TestReader->KeepReading();
    }

  VideoWriter->EndVideo();

  std::cout<<"Done !"<<std::endl;
  return EXIT_SUCCESS;
}