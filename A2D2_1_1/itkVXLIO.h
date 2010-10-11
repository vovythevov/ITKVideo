#include "itkVideoIOBase.h"
#include "itkExceptionObject.h"
#include "itkImportImageFilter.h"
#include "itkImageToImageFilter.h"
#include "vidl/vidl_ffmpeg_ostream.h"
#include "vidl/vidl_ffmpeg_istream.h"

#ifndef __itkVXLIO_h
#define __itkVXLIO_h

namespace itk
{

/** \class VXLIO
 * \brief 
 *  
 *
 * \sa VideoIOBase
 *
 * \ingroup OpenCVFilters
 */

template <class TImage>
class ITK_EXPORT VXLIO : public VideoIOBase < TImage >
{
public:
  /** Standard class typedefs. **/
  typedef VXLIO                               Self;
  typedef VideoIOBase                        Superclass;
  typedef SmartPointer< Self >                      Pointer;

  /** Run-time type information (and related methods). **/
  itkTypeMacro(VXLIO, Superclass);
  
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

  VXLIO();
  ~VXLIO(){};

protected:

  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  VXLIO(const Self &);    //purposely not implemented
  void operator=(const Self &); //purposely not implemented

  vidl_pixel_format                     m_PixelFormat;
  vidl_frame_sptr                       m_VIDLImage;
  vidl_ffmpeg_istream                   *m_Reader;
  vidl_ffmpeg_ostream                   *m_Writer;
  bool                                  m_ReaderOpen;
  bool                                  m_WriterOpen;
  bool                                  m_VideoSeekable;

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
#include "itkVXLIO.txx"
#endif

#endif // __itkVXLIO_h