
#include "itkVideoViewerBase.h"
#include "cv.h"
#include "highgui.h"

#ifndef __itkOpenCVViewer_h
#define __itkOpenCVViewer_h

namespace itk
{

template <class TImage>
class ITK_EXPORT OpenCVViewer : public VideoViewerBase<TImage>
{
public:
  /** Standard class typedefs. **/
  typedef OpenCVViewer                               Self;
  typedef LightProcessObject                        Superclass;
  typedef SmartPointer< Self >                      Pointer;

  /** Convinient typedef **/
  typedef TImage                              ImageType;
  typedef typename ImageType::PixelType       PixelType;

  /** Run-time type information (and related methods). **/
  itkTypeMacro(OpenCVViewer, Superclass);

  /** New macro **/
  itkNewMacro(Self);

  /** Try to open a windows **/
  /** Return true if in case of a success, false for a faillure **/
  /** Intended to be overloaded by subclasses **/
  bool Open (const char *WindowName);

  /** Try to close a viewer **/
  /** Return true if in case of a success, false for a faillure **/
  /** Intended to be overloaded by subclasses **/
  bool Close (const char *WindowName);

  /** Return the state of the Viewer (opened or not) **/
  bool IsOpen ();

 /** Display the input image **/
  bool Play (typename itk::Image<PixelType,2>::Pointer ITKImage);

  /** Wait the time specified by the SetWaitTime() **/
  void Wait ();
  /** Wait the time specified **/
  void Wait (int MSec);

  /** Accessors **/
  int GetWaitTime () {return this->m_WaitTime;};
  void SetWaitTime(int MSec);
  
protected:  
  OpenCVViewer();
  ~OpenCVViewer(){};

  void PrintSelf(std::ostream & os, Indent indent) const;
  int            m_WaitTime;
  IplImage          *m_OpenCVImage;

private:
  OpenCVViewer(const Self &);    //purposely not implemented
  void operator=(const Self &); //purposely not implemented

  void InitImage(typename itk::Image<PixelType,2>::Pointer ITKImage);

  std::string       m_WindowName;

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkOpenCVViewer.txx"
#endif

#endif // __itkOpenCVViewer_h
