#include "itkVideoIOBase.h"
#include "itkExceptionObject.h"
#include "itkImportImageFilter.h"
#include "cv.h"
#include "highgui.h"

#ifndef __itkOpenCVIO_h
#define __itkOpenCVIO_h

namespace itk
{

/** \class OpenCVIO
 * \brief 
 *  
 *
 * \sa VideoIOBase
 *
 * \ingroup OpenCVFilters
 */

template <class TImage>
class ITK_EXPORT OpenCVIO : public VideoIOBase < TImage >
{
public:
  /** Standard class typedefs. **/
  typedef OpenCVIO                               Self;
  typedef VideoIOBase                        Superclass;
  typedef SmartPointer< Self >                      Pointer;

  /** Run-time type information (and related methods). **/
  itkTypeMacro(OpenCVIO, Superclass);

  /** Method for creation through the object factory. **/
  itkNewMacro(Self);
  
  /** Convinient typedef **/ 
  typedef itk::ImportImageFilter<typename TImage::PixelType,2>   ImportFilterType;

  /** Try to open a video **/
  /** Return true if in case of a success, false for a faillure **/
  bool OpenReader (const char* filename);
  bool OpenWriter (const char* filename, typename itk::Image<typename TImage::PixelType,2>::Pointer ITKImage);

  /** Try to close a video **/
  /** Return true if in case of a success, false for a faillure **/
  bool Close (const char* filename);

  /** Return the state of the video (opened or not) **/
  bool IsReaderOpen () {return this->m_ReaderOpen;};
  bool IsWriterOpen () {return this->m_WriterOpen;};

  /** Return the image read form a video file **/
  typename itk::Image<typename PixelType,2>::Pointer Read();

  /** Write a frame and return true if succeed (false otherwise) **/
  bool Write (typename itk::Image<typename PixelType,2>::Pointer ITKImage);

  /** A bunch of accessor **/
  int GetWidth() {return this->m_Width;};
  int GetHeight() {return this->m_Height;};
  double GetXOrigin() {return this->m_Origin[0];};
  double GetYOrigin() {return this->m_Origin[1];};
  double GetXSpacing() {return this->m_Spacing[0];};
  double GetYSpacing() {return this->m_Spacing[1];};
  unsigned long GetFrameTotal () {return this->m_FrameTotal;};
  double GetFpS() {return this->m_FpS;};

protected:  
  OpenCVIO();
  ~OpenCVIO(){};

  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  OpenCVIO(const Self &);    //purposely not implemented
  void operator=(const Self &); //purposely not implemented

  IplImage                              *m_CVImage;
  IplImage                              *m_Temp;
  CvCapture                             *m_Capture;
  CvVideoWriter                         *m_Writer;
  bool                                  m_ReaderOpen;
  bool                                  m_WriterOpen;

  int                                   m_FourCC;
  double                                m_FpS;
  unsigned long                         m_FrameTotal;
  int                                   m_Width;
  int                                   m_Height;
  double                                m_Origin[2];
  double                                m_Spacing[2];
  typename ImportFilterType::SizeType   m_Size;
  typename ImportFilterType::IndexType  m_Start;
  typename ImportFilterType::RegionType m_Region;

  typename ImportFilterType::Pointer    m_ImportFilter;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkOpenCVIO.txx"
#endif

#endif // __itkOpenCVIO_h