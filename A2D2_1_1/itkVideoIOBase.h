#include "itkLightProcessObject.h"
#include "itkExceptionObject.h"
#include "itkImage.h"

#ifndef __itkVideoIOBase_h
#define __itkVideoIOBase_h

namespace itk
{

template <class TOutputImage>
class ITK_EXPORT VideoIOBase : public LightProcessObject
{
public:
  /** Standard class typedefs. **/
  typedef VideoIOBase                               Self;
  typedef LightProcessObject                        Superclass;
  typedef SmartPointer< Self >                      Pointer;

  /** Convinient typedef **/
  typedef TOutputImage                              OutputImageType;
  typedef typename OutputImageType::PixelType       OutputPixelType;

  /** Run-time type information (and related methods). **/
  itkTypeMacro(VideoIOBase, Superclass);

  /** Set/Get the name of the file to be read. **/

  /** Try to open a video **/
  /** Return true if in case of a success, false for a faillure **/
  /** Intended to be overloaded by subclasses **/
  virtual bool Open (const char* filename) = 0;

  /** Try to close a video **/
  /** Return true if in case of a success, false for a faillure **/
  /** Intended to be overloaded by subclasses **/
  virtual bool Close (const char* filename) = 0;

  /** Return the state of the video (opened or not) **/
  virtual bool Is_Open () = 0;

  /** Return the image read form a video file **/
  virtual typename itk::Image<typename OutputPixelType,2>::Pointer Read() = 0;

  /** Write a frame and return true if succeed (false otherwise) **/
  virtual bool Write (typename itk::Image<typename OutputPixelType,2>::Pointer ITKImage) = 0;

  /** A bunch of accessors**/
  virtual int GetWidth() = 0;
  virtual int GetHeight() = 0;
  virtual double GetXOrigin() = 0;
  virtual double GetYOrigin() = 0;
  virtual double GetXSpacing() = 0;
  virtual double GetYSpacing() = 0;
  virtual unsigned long GetFrameTotal () = 0;
  virtual double GetFpS() = 0;

protected:  
  
  VideoIOBase();
  ~VideoIOBase(){};

  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  VideoIOBase(const Self &);    //purposely not implemented
  void operator=(const Self &); //purposely not implemented

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVideoIOBase.txx"
#endif

#endif // __itkVideoIOBase_h