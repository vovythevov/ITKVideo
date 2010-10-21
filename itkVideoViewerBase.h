
#include "itkLightProcessObject.h"
#include "itkExceptionObject.h"
#include "itkImage.h"

#ifndef __itkVideoViewerBase_h
#define __itkVideoViewerBase_h

namespace itk
{

/** \class VideoViewerBase
 * \brief Create a pop up window that display your video
 * 
 *  The purpose of this class is to enables devellopers to 
 *  display the video they are working on all along the 
 *  algorithm they are testing. 
 *  
 */

template <class TImage>
class ITK_EXPORT VideoViewerBase : public LightProcessObject
{
public:
  /** Standard class typedefs. **/
  typedef VideoViewerBase                               Self;
  typedef LightProcessObject                        Superclass;
  typedef SmartPointer< Self >                      Pointer;

  /** Convinient typedef **/
  typedef TImage                              ImageType;
  typedef typename ImageType::PixelType       PixelType;

  /** Run-time type information (and related methods). **/
  itkTypeMacro(VideoViewerBase, Superclass);

/** Set/Get the name of the file to be read. **/

  /** Try to open a windows **/
  /** Return true if in case of a success, false for a faillure **/
  /** Intended to be overloaded by subclasses **/
  virtual bool Open (const char* WindowName) = 0;

  /** Try to close a viewer **/
 /** Return true if in case of a success, false for a faillure **/
  /** Intended to be overloaded by subclasses **/
  virtual bool Close (const char* WindowName) = 0;

 /** Return the state of the Viewer (opened or not) **/
  virtual bool IsOpen () = 0;

 /** Display the input image **/
  virtual bool Play(typename itk::Image<PixelType,2>::Pointer ITKImage) = 0;

  /** Wait the time specified by the SetWaitTime() **/
  virtual void Wait() = 0;
  /** Wait the time specified **/
  virtual void Wait (int MSec) = 0;

  /** Accessors **/
  virtual int GetWaitTime () = 0;
  virtual void SetWaitTime(int MSec) = 0;
  
protected:  
  VideoViewerBase();
  ~VideoViewerBase(){};

  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  VideoViewerBase(const Self &);    //purposely not implemented
  void operator=(const Self &); //purposely not implemented

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVideoViewerBase.txx"
#endif

#endif // __itkVideoViewerBase_h
