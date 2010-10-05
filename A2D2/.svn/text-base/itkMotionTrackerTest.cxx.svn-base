#include "iostream"

#include "itkVideoFileReader.h"
#include "itkVideoFileWriter.h"
#include "itkDifferenceImageFilter.h"
#include "itkReflectImageFilter.h"


int main (int argv, char **argc)
{
  typedef itk::Image< unsigned char, 2>   OutputImageType;  
  itk::VideoFileReader< OutputImageType >::Pointer reader = itk::VideoFileReader< OutputImageType >::New();
  reader->SetFileName("C:/projects/ITK-A2D2/A2D2_build/Debug/inde-circulation.avi");

  itk::ReflectImageFilter<OutputImageType,OutputImageType>::Pointer filter
    = itk::ReflectImageFilter<OutputImageType,OutputImageType>::New();
  filter->SetInput(reader->GetOutput());
  filter->SetDirection(0);

  unsigned long i;

  itk::VideoFileWriter<OutputImageType>::Pointer VideoWriter = itk::VideoFileWriter<OutputImageType>::New();
  VideoWriter->SetInput(filter->GetOutput());
  VideoWriter->SetFileName("C:/projects/ITK-A2D2/A2D2_build/images/Inversed_Indian_Crossroad.avi");

  for (i = 1; i <= 93 ; i ++ )
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
    reader->KeepReading();
    }

  VideoWriter->EndVideo();

  std::cout<<"Done !"<<std::endl;
  std::cin>>i;
  return EXIT_SUCCESS;
}