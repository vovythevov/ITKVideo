#include "iostream"

#include "itkVideoFileReader.h"
#include "itkVideoFileWriter.h"
#include "itkDifferenceImageFilter.h"


int motion_tracker (char* Input, char* Output,bool readerUseOpenCV, bool writerUseOpenCV)
{
  typedef itk::Image< unsigned char, 2>   OutputImageType;  
  itk::VideoFileReader< OutputImageType >::Pointer TestReader = itk::VideoFileReader< OutputImageType >::New();
  TestReader->SetFileName(Input);
  TestReader->UseOpenCV(readerUseOpenCV);
  itk::VideoFileReader< OutputImageType >::Pointer ValidReader = itk::VideoFileReader< OutputImageType >::New();
  ValidReader->SetFileName(Input);
  ValidReader->UseOpenCV(readerUseOpenCV);

  TestReader->SetNextFrameIsFrameRequested(true);
  TestReader->SetFrameRequested(1);

  itk::DifferenceImageFilter<OutputImageType,OutputImageType>::Pointer filter
    = itk::DifferenceImageFilter<OutputImageType,OutputImageType>::New();
  filter->SetTestInput(TestReader->GetOutput());
  filter->SetValidInput(ValidReader->GetOutput());
  TestReader->LoadVideo();
  ValidReader->LoadVideo();

  unsigned long i;
  unsigned long FrameTotal = TestReader->GetFrameTotal();

  itk::VideoFileWriter<OutputImageType>::Pointer VideoWriter = itk::VideoFileWriter<OutputImageType>::New();
  VideoWriter->SetInput(filter->GetOutput());
  VideoWriter->SetFileName(Output);
  VideoWriter->UseOpenCV(writerUseOpenCV);

  for (i = 0; i < FrameTotal-1 ; i ++ )
    {
    try
      {
      VideoWriter->Update();
      }
    catch (itk::ExceptionObject &e)
      {
      std::cerr<<"Error in the loop, step # "<<i<<std::endl;
      std::cerr<<"Total Frame : "<<FrameTotal<<std::endl;
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

int main (int argv, char **argc)
{
  /*int k = motion_tracker("C:/projects/ITK-Vid-A2D2/Data/inde-circulation.avi", 
    "C:/projects/ITK-Vid-A2D2_build/Testing/Motion_tracked_inde-circulation.avi", 
    atoi("1"),
    atoi("1"));
  std::cin>>k;
  return k;*/

  return motion_tracker(argc[1],argc[2],atoi(argc[3]),atoi(argc[4]));
}