#include "cv.h"
#include "highgui.h"
#include "itkImageSource.h"
#include "itkExceptionObject.h"
#include "itkDefaultConvertPixelTraits.h"
#include "itkImportImageFilter.h"

#ifndef __itkLightVideoFileReader_h
#define __itkLightVideoFileReader_h

namespace itk
{
/** \class LightVideoFileReader
 * \brief Open an AVI File and stream it using OpenCV libs
 *  Enable you to read video.
 *
 *  When to use : -When you use the frame independently
 *  from the others frame. 
 *
 *  How to use : Mostly like an Image file reader. You create it
 *  with the object factory. Then you set the filename. You can now create 
 *  your pipeline of filter just as like you would with an image reader.
 *  Then two scenario case :
 *  (least likely) You want only one frame. Check \sa VideoFileReader
 *  
 *  (Most Likely) If you want to work a lot of frame 
 *  You first load the video which allows you to know how many frames 
 *  there is in the video.
 *  Now you can loop on that number and call an Update() on your pipeline
 *  last element. At the end of your treatment, just before the end of the loop
 *  you HAVE TO call KeepPlaying() method on your VideoFileReader. Otherwise
 *  your filter will update only once.
 *  
 *
 * \sa VideoFileReader
 *
 * \ingroup OpenCVFilters
 */

template <class TOutputImage,
          class ConvertPixelTraits=DefaultConvertPixelTraits< 
                   ITK_TYPENAME TOutputImage::IOPixelType > >

class ITK_EXPORT LightVideoFileReader : public ImageSource<TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef LightVideoFileReader                            Self;
  typedef ImageSource< TOutputImage >                     Superclass;
  typedef SmartPointer< Self >                            Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;

  typedef itk::ImportImageFilter<OutputImagePixelType,2>   ImportFilterType;

  /** Method for creation through the object factory. **/
  itkNewMacro(Self);

  /** Run-time type information (and related methods). **/
  itkTypeMacro(LightVideoFileReader, ImageSource);

  /** Set/Get method for the Filename (which really is his path) **/
  void SetFileName (const char* filename);
  itkGetStringMacro(FileName);

  /** Get the number of frame **/
  itkGetConstMacro(FrameTotal,unsigned long);

  /** Try to load a video **/
  void LoadVideo();

  /** Method to call so the next the reader isn't up-to-date.**/
  /** This way, on the next update, the reader do read a new frame **/
  /** If this method isn't called, then after the first time the reader has **/
  /** been updated, the filter is up-to-date and doesn't give another frame **/
  void KeepReading();

protected: 
  
  void PrintSelf(std::ostream & os, Indent indent) const;

  void GenerateOutputInformation();

  void GenerateData();  
  


  LightVideoFileReader();
  ~LightVideoFileReader(){};

  std::string                                   m_FileName;
  unsigned long                                 m_FrameTotal;
  CvCapture                                     *m_Capture;
  IplImage                                      *m_CVImage;
  bool                                          m_VideoLoaded;

  typename ImportFilterType::Pointer            m_ImportFilter;

private:
  LightVideoFileReader(const Self &); //purposely not implemented
  void operator=(const Self &);  //purposely not implemented

  void TestFileExistanceAndReadability();

  IplImage                                      *m_Temp;
  typename ImportFilterType::SizeType           m_Size;
  typename ImportFilterType::IndexType          m_Start; 
  typename ImportFilterType::RegionType         m_Region;
  double                                        m_Origin[2];
  double                                        m_Spacing[2];


};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLightVideoFileReader.txx"
#endif

#endif