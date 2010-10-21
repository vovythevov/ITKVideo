#include "itkExceptionObject.h"
#include "itkProcessObject.h"
#include "itkVideoViewerBase.h"
#include "itkImageToImageFilter.h"

#ifndef __itkVideoViewer_h
#define __itkVideoViewer_h

namespace itk
{

/** \class VideoViewerBase
 * \brief Create a pop up window that display your video
 * 
 *  The purpose of this class is to enables devellopers to 
 *  display the video they are working on all along the 
 *  algorithm they are testing. 
 *  
 *  Input/Output :
 *  This class is an ImageToImageFilter. Consequently you can 
 *  use it as any other filter.
 *  The output of this filter/class is the same image as in input,
 *  except that it may have been alter by the viewer, depending on which
 *  library you chose to use. (See VideoFileWriter) 
 *  Also the output is automatically delayed. This delay enables you
 *  to see the video (See section Parameters fo more information)
 *
 *  Parameters :
 *  This filters/class can have 2 parameters.
 *  WaitTime sets the time you should wait between 
 *  displaying to frame in milliseconds. 
 *  It is important that this time respects the human eye physical 
 *  abilities, otherwise you want see anything. In order to facilitate 
 *  the implementation of a viewer, the default value is set to 40ms
 *  (25 images/s).
 *  The other parameter is the window name. It is important if you use OpenCv 
 *  (see When using OpenCV section). 
 *
 *  
 * \sa VideoViewerBase
 * \sa VideoFileReader
 * \sa VideoFileWriter
 *
 * \ingroup OpenCVFilters
 */

template <class TInputImage >
class ITK_EXPORT VideoViewer : public ImageToImageFilter<TInputImage,TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef VideoViewer                                 Self;
  typedef ProcessObject                                   Superclass;
  typedef SmartPointer< Self >                            Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;

  /** Method for creation through the object factory. **/
  itkNewMacro(Self);

  /** Convinient typedef **/
  typedef TInputImage InputImageType;

  /** Run-time type information (and related methods). **/
  itkTypeMacro(VideoViewer, ImageToImageFilter);

  /** Set/Get the FpS. 25 by default **/
  /** Return true if succeed, false if not **/
  /** (for example if the Viewer isn' opened) **/
  bool SetWaitTime(int MSec);
  double GetWaitTime() {return this->m_VideoWriter->GetWaitTime();}

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

  /** Method for waiting **/
  void Wait();

  /** Set/Get the window name **/ 
  /** You need to specify a different video name if you **/
  /** want a different window. We use the name of the class **/
  /** by default**/
  itkSetStringMacro(WindowName);
  itkGetStringMacro(WindowName);

  /** Close the viewer **/
  void Close ();
  /** Open the viewer **/
  /** If a Viewer already exists, it does nothing **/
  void Open();
  /** Same but you can give the name **/
  void Open(const char* WindowName);

protected: 
  
  void PrintSelf(std::ostream & os, Indent indent) const;
  
  void GenerateData();

  VideoViewer();
  ~VideoViewer()
    {
    this->Close();
    };

  bool                                      m_UseOpenCV;
  bool                                      m_ViewerExists;
  bool                                      m_ViewerClosed;
  typename itk::VideoViewerBase<TInputImage>::Pointer m_VideoViewer;
  std::string                               m_WindowName;

private:
  VideoViewer(const Self &); //purposely not implemented
  void operator=(const Self &);  //purposely not implemented

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVideoViewer.txx"
#endif

#endif