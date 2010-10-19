#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFaceDetectionFilter.h"
#include "itkRegionOfInterestImageFilter.h"

int test_face_detection (char* input, char *output, char *trainerFilename)
{

  typedef itk::Image< unsigned char,  2>   InputImageType;
  typedef itk::Image< unsigned char, 2>   OutputImageType;   
  
  itk::FaceDetectionFilter<InputImageType>::Pointer filter 
    = itk::FaceDetectionFilter<InputImageType>::New();
  
  typedef itk::ImageFileReader< InputImageType >  ReaderType;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(input);

  filter->SetInput(reader->GetOutput());
  filter->SetTrainerFileName(trainerFilename);
  filter->SetColor(1);
  filter->SetDrawRectangles(false);
  filter->SetGenerateROI(true);
  filter->Print(std::cout);


  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(output);
  
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
    return EXIT_FAILURE;
    }
  char table[] = "1234567890qwertyuiopasdfghjklzxcvbnm";
  std::list< itk::ImageRegion<2>* >* list = filter->GetFacesAsROI();

  itk::RegionOfInterestImageFilter<InputImageType ,InputImageType >::Pointer regionfilter =
    itk::RegionOfInterestImageFilter< InputImageType,InputImageType >::New();

  regionfilter->SetInput( filter->GetOutput() );

  int i;
  for ( i = 0; i < filter->GetFacesTotal() ; i++)
    {
    std::string title = "./Testing/Results/FaceDetectionFilter/Face_";
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
      return EXIT_FAILURE;
      }
    list->pop_front();
    }

  return EXIT_SUCCESS;
}

int main (int argv, char* argc[])
{
 return test_face_detection(argc[1],argc[2],argc[3]);
}

