
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFaceDetectionFilter.h"
#include "itkRegionOfInterestImageFilter.h"


int main (int argv, char **argc)
{

  typedef itk::Image< unsigned char,  2>   InputImageType;
  typedef itk::Image< unsigned char, 2>   OutputImageType;   
  
  itk::FaceDetectionFilter<InputImageType>::Pointer filter 
    = itk::FaceDetectionFilter<InputImageType>::New();
  
  typedef itk::ImageFileReader< InputImageType >  ReaderType;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName("C:/faces3.png");

  filter->SetInput(reader->GetOutput());
  filter->SetTrainerFileName("./Testing/Data/haarcascade_frontalface_alt2.xml");
  filter->SetColor(1);
  filter->SetDrawRectangles(false);
  filter->SetGenerateROI(true);
  filter->Print(std::cout);


  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName("C:/result.png");
  
  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject &e)
    {
    std::cerr<<e.GetFile()<<std::endl;
    std::cerr<<e.GetLine()<<std::endl;
    std::cerr<<e.GetLocation()<<std::endl;
    std::cerr<<e.GetNameOfClass()<<std::endl;
    std::cerr<<e.GetDescription()<<std::endl;
    //return EXIT_FAILURE;
    }
  char table[] = "1234567890qwertyuiopasdfghjklzxcvbnm";
  std::list< itk::ImageRegion<2>* >* list = filter->GetFacesAsROI();

  itk::RegionOfInterestImageFilter<InputImageType ,InputImageType >::Pointer regionfilter =
    itk::RegionOfInterestImageFilter< InputImageType,InputImageType >::New();

  regionfilter->SetInput( filter->GetOutput() );

  int i;
  for ( i = 0; i < filter->GetFacesTotal() ; i++)
    {
    std::string title = "title";
    title += table[i];
    title += ".png";
    
    regionfilter->SetRegionOfInterest( *list->front() );

    writer->SetInput( regionfilter->GetOutput() );
    writer->SetFileName(title.c_str());

    try
      {
      writer->Update();
      }
    catch (itk::ExceptionObject &e)
      {
      std::cerr<<e.GetFile()<<std::endl;
      std::cerr<<e.GetLine()<<std::endl;
      std::cerr<<e.GetLocation()<<std::endl;
      std::cerr<<e.GetNameOfClass()<<std::endl;
      std::cerr<<e.GetDescription()<<std::endl;
      //return EXIT_FAILURE;
      }
    list->pop_front();
    }

  return EXIT_SUCCESS;
}

