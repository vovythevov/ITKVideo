#include "itkExceptionObject.h"
#include "itkProcessObject.h"
#include "itkVideoIOBase.h"

#ifndef __itkVideoFileWriter_h
#define __itkVideoFileWriter_h

namespace itk
{
 /** \class DoNothingFilter
 * \brief Do Nothing
 *  Used to call an update for all the class in the pipeline
 *  before VideoFileWriter. This way we are sure that the data is available.
 *  No other interest.
 *
 * \sa LightVideoFileWriter
 *
 * \ingroup OpenCVFilters
 */

template <class TInputImage >

class ITK_EXPORT DoNothingFilter : public ImageToImageFilter<TInputImage,TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef DoNothingFilter                                 Self;
  typedef ImageToImageFilter<TInputImage,TInputImage>     Superclass;
  typedef SmartPointer< Self >                            Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;

   /** Method for creation through the object factory. **/
  itkNewMacro(Self);

  /** Run-time type information (and related methods). **/
  itkTypeMacro(DoNothingFilter, ImageToImageFilter);

protected :  
  void PrintSelf(std::ostream & os, Indent indent) const {}
  
  void GenerateData()
    {
    this->GraftOutput(const_cast< TInputImage * >(this->GetInput()) );
    }
};
/** \class LightVideoFileWriter
 * \brief Write an Video File and stream it using OpenCV libs
 *  Enable you to write video.
 *  
 *  (Only ?) Supports .avi format. Thus, the file you are trying to write
 *  should have this extension. 
 *  By default, the writer write a video with the codec PIM1 with 25 frame/s.
 *  Since it was made in concurrence with the VideoFileReader, it only 
 *  supports grayscale images as an input.
 *  The output is fo now encoded with pixel having a size of 1 bytes and being 
 *  unsigned.
 *  
 *  How to use : Exactly as the ImageFileWriter. Attention,t when an update is called,
 *    since itk only works with image, the VideoFileWriter only add one frame to 
 *    the video (is current input).
 *
 *  Attention, if you want to read the video, you should either :
 *    . Make sure the object VideoFileWriter is not referenced anymore 
 *      ( see \sa SmartPointer )
 *    . Call the method EndVideo() on your writer.
 *
 * \sa VideoFileReader
 * \sa LightVideoFileReader
 *
 * \ingroup OpenCVFilters
 */

template <class TInputImage >

class ITK_EXPORT VideoFileWriter : public ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef VideoFileWriter                                 Self;
  typedef ProcessObject                                   Superclass;
  typedef SmartPointer< Self >                            Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;

  /** Method for creation through the object factory. **/
  itkNewMacro(Self);

  /** Run-time type information (and related methods). **/
  itkTypeMacro(LightVideoFileWriter, ProcessObject);

  /** Set/Get method for the Filename (which really is his path) **/
  void SetFileName (const char *filename);
  itkGetStringMacro(FileName);

  /** Set/Get method for the codec, by default IYUV**/
  void SetFourCC (int fourcc);
  int GetFourCC() {return this->m_VideoIO->GetFourCC();}

  /** Set/Get the FpS. 25 by default **/
  void SetFpS(double framefrequency);
  double GetFpS() {return this->m_VideoIO->GetFpS();}
  //itkGetMacro(FpS,double);

  /** Set/Get the input **/
  void SetInput(const TInputImage *input); 

  /** Set the use of openCV (or vxl) **/
  /** Attention OpenCV only accepts char (or unsigned char) **/
  /** OpenCv by default **/
  void UseOpenCV ( bool useOpenCV );
  
  virtual void Update()
    {
    this->GenerateData();
    }

  /** Play what is written in the file, very useful for debug **/
  void Play();

  /** Method for releasing the video. Also called by the destructor **/
  void EndVideo();

protected: 
  
  void PrintSelf(std::ostream & os, Indent indent) const;
  
  void GenerateData();

  VideoFileWriter();
  ~VideoFileWriter()
    {
    this->EndVideo();
    };

  bool                                      m_UseOpenCV;
  bool                                      m_WriterCreated;
  std::string                               m_FileName;
  //CvVideoWriter                             *m_Writer;
  //IplImage                                  *m_FrameToWrite;
  //double                                    m_FpS;
  //int                                       m_FourCC;
  typename itk::VideoIOBase<TInputImage>::Pointer m_VideoIO;

private:
  VideoFileWriter(const Self &); //purposely not implemented
  void operator=(const Self &);  //purposely not implemented

  void TransformITKImageToCVImage();
  void CreateVideo();

  //IplImage                                                *m_Temp;
  //CvSize                                                  m_Size;
  //bool                                                    m_VideoCreated;
  typename itk::DoNothingFilter<TInputImage>::Pointer     m_DoNothingFilter;

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVideoFileWriter.txx"
#endif

#endif