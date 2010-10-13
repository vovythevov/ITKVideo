#include "cv.h"
#include "highgui.h"
#include "iostream"

#include "itkVideoFileReader.h"
#include "itkVideoFileWriter.h"

int test_pipeline (std::string Input, std::string Output, bool readerUseOpenCV, bool writerUseOpenCV)
{
  typedef itk::Image< unsigned char, 2>   OutputImageType;  
  itk::VideoFileReader< OutputImageType >::Pointer reader = itk::VideoFileReader< OutputImageType >::New();
  reader->SetFileName();
 
  try
    {
    reader->LoadVideo();
    }
  catch (itk::ExceptionObject &e)
    {
    std::cerr<<e.GetFile()<<std::endl;
    std::cerr<<e.GetLine()<<std::endl;
    std::cerr<<e.GetLocation()<<std::endl;
    std::cerr<<e.GetNameOfClass()<<std::endl;
    std::cerr<<e.GetDescription()<<std::endl;
    return EXIT_FAILURE;
    }
  
  unsigned long FrameTotal = reader->GetFrameTotal();
  unsigned long i;

  itk::VideoFileWriter<OutputImageType>::Pointer VideoWriter = itk::VideoFileWriter<OutputImageType>::New();
  VideoWriter->SetInput(reader->GetOutput());
  VideoWriter->SetFileName();

  VideoWriter->EndVideo();

  for (i  = 0; i<FrameTotal; i++)
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
      return EXIT_FAILURE;
      }
    reader->KeepReading();
    }

  std::cout<<"Done !"<<std::endl;
  return EXIT_SUCCESS;
}

int main (int argv, char *argc[] )
{
  test_pipeline(
    "./Testing/Data/25_26_L_Echelle_de_Perceval_La_Chambre_de_la_Reine.avi",
    "./Testing/Results/Gray_Kaamelot.avi",
    true,
    true);

  test_pipeline(
    "./Testing/Data/25_26_L_Echelle_de_Perceval_La_Chambre_de_la_Reine.avi",
    "./Testing/Results/Gray_Kaamelot.avi",
    true,
    false);

  test_pipeline(
    "./Testing/Data/25_26_L_Echelle_de_Perceval_La_Chambre_de_la_Reine.avi",
    "./Testing/Results/Gray_Kaamelot.avi",
    false,
    true);
  
  test_pipeline(
    "./Testing/Data/25_26_L_Echelle_de_Perceval_La_Chambre_de_la_Reine.avi",
    "./Testing/Results/Gray_Kaamelot.avi",
    false,
    false);

  return EXIT_SUCCESS;
}
