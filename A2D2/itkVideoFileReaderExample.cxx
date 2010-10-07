
#include "iostream"

#include "itkLightVideoFileReader.h"
#include "itkImageFileWriter.h"


// This is an example that illustate how the Light video file reader works
// It basically saves a image from a video every minute. (every 1500 frame
// if the frame period is 1/25 s)

int main (int argv, char **argc)
{
  typedef itk::Image<unsigned char, 2>   OutputImageType;  
  itk::LightVideoFileReader< OutputImageType >::Pointer reader = itk::LightVideoFileReader< OutputImageType >::New();
  reader->SetFileName("./Testing/Data/25_26_L_Echelle_de_Perceval_La_Chambre_de_la_Reine.avi");

  reader->LoadVideo();
  
  unsigned long FrameTotal = reader->GetFrameTotal();
  unsigned long i;
  char buf[2000];

  itk::ImageFileWriter<OutputImageType>::Pointer writer = itk::ImageFileWriter<OutputImageType>::New();
  writer->SetInput(reader->GetOutput());

  for (i  = 0; i < 9000; i++)
    {
    if ( i % 1500 == 0 )
      {
      //To set a different filename each time
      std::string filename = "./Testing/Results/VideoFileReaderExample_results/Frame_Number_";
      filename += itoa(i,buf,10);
      filename += ".png";
      writer->SetFileName(filename.c_str());
      try
        {
        writer->Update();
        }
      catch (itk::ExceptionObject &e)
        {
        reader->Print(std::cout);
        writer->Print(std::cout);
        std::cerr<<e.GetFile()<<std::endl;
        std::cerr<<e.GetLine()<<std::endl;
        std::cerr<<e.GetLocation()<<std::endl;
        std::cerr<<e.GetNameOfClass()<<std::endl;
        std::cerr<<e.GetDescription()<<std::endl;
        }
      }
    else
      {
      reader->Update();
      }

    reader->KeepReading();
    }

  std::cout<<"Done !"<<std::endl;
  return EXIT_SUCCESS;
}
