#include "iostream"

#include "itkVideoFileReader.h"
#include "itkVideoFileWriter.h"
#include "itkDifferenceImageFilter.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkDerivativeImageFilter.h"
#include "itkBilateralImageFilter.h"
#include "itkBinaryPruningImageFilter.h"
#include "itkBinaryContourImageFilter.h"


int main (int argv, char **argc)
{
  typedef itk::Image< unsigned char, 2>   OutputImageType;  
  itk::VideoFileReader< OutputImageType >::Pointer FirstReader = itk::VideoFileReader< OutputImageType >::New();
  FirstReader->SetFileName("C:/projects/ITK-A2D2/A2D2_build/Debug/inde-circulation.avi");

  itk::VideoFileReader< OutputImageType >::Pointer SecondReader = itk::VideoFileReader< OutputImageType >::New();
  SecondReader->SetFileName("C:/projects/ITK-A2D2/A2D2_build/Debug/inde-circulation.avi");
  SecondReader->SetNextFrameIsFrameRequested(true);
  SecondReader->SetFrameRequested(1);
  
  unsigned long FrameTotal = FirstReader->GetFrameTotal();
  unsigned long i;

   itk::DifferenceImageFilter< OutputImageType,OutputImageType >::Pointer filter 
     = itk::DifferenceImageFilter< OutputImageType,OutputImageType>::New();
  filter->SetTestInput(SecondReader->GetOutput());
  filter->SetValidInput(FirstReader->GetOutput());
  //filter->SetIgnoreBoundaryPixels(true);
  filter->SetToleranceRadius(2);

  itk::BinaryContourImageFilter< OutputImageType,OutputImageType >::Pointer filter 
     = itk::BinaryContourImageFilter< OutputImageType,OutputImageType>::New();
  contourfilter->SetInput(SecondReader->GetOutput());


  itk::VideoFileWriter<OutputImageType>::Pointer VideoWriter = itk::VideoFileWriter<OutputImageType>::New();
  VideoWriter->SetInput(contourfilter->GetOutput());
  VideoWriter->SetFileName("C:/projects/ITK-A2D2/A2D2_build/images/Prunning_filter_inde_circulation.avi");

  for (i =0; i<1000; i++)
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
      }
    if ( i == 0 )
      { 
      SecondReader->SetNextFrameIsFrameRequested(false);
      }

    FirstReader->KeepReading();
    SecondReader->KeepReading();
    }

  VideoWriter->EndVideo();

  FirstReader->SetFileName("C:/projects/ITK-A2D2/A2D2_build/images/Motion_Tracked_Kaamelot.avi");
   try
    {
    FirstReader->Update();
    }
  catch (itk::ExceptionObject &e)
    {
    FirstReader->Print(std::cout);
    std::cerr<<e.GetFile()<<std::endl;
    std::cerr<<e.GetLine()<<std::endl;
    std::cerr<<e.GetLocation()<<std::endl;
    std::cerr<<e.GetNameOfClass()<<std::endl;
    std::cerr<<e.GetDescription()<<std::endl;
    }
  FirstReader->Print(std::cout);


  std::cout<<"Done !"<<std::endl;
  std::cin>>i;
  return EXIT_SUCCESS;
}