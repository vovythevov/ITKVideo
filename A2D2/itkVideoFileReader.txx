/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFaceDetectionFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVideoFileReader_txx
#define __itkVideoFileReader_txx

#include "itkVideoFileReader.h"
#include "itkExceptionObject.h"

namespace itk
{

template< typename TOutputImage >
VideoFileReader< TOutputImage >
::VideoFileReader()
{
  this->InitializeProperties();
}

template< typename TOutputImage >
void VideoFileReader< TOutputImage >
::InitializeProperties()
{
  this->m_FrameRequested = 0;
  this->m_PositionInMSec = 0;
  this->m_Ratio = 0;
  this->m_FrameWidth = 0;
  this->m_FrameHeight = 0;
  this->m_FpS = 25;
  this->m_NextFrameIsFrameRequested = false;
}

/*
 * PrintSelf
 */
template< typename TOutputImage >
void
VideoFileReader< TOutputImage >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Frame requested : "<<this->m_FrameRequested<< std::endl;
  os << indent << "Position in milliseconds : "<<this->m_PositionInMSec<< std::endl;
  os << indent << "Ratio of video elapsed : "<<this->m_Ratio<< std::endl;
  os << indent << "Frame width : "<<this->m_FrameWidth<< std::endl;
  os << indent << "Frame Height : "<<this->m_FrameHeight<< std::endl;
  os << indent << "Number of frame per second : "<<this->m_FpS<< std::endl;
  
} // end PrintSelf

template< typename TOutputImage >
void VideoFileReader<TOutputImage>
::GenerateData()
{
  if (Superclass::m_VideoLoaded == false )
    {
    this->LoadVideo();
    }
  if ( this->m_NextFrameIsFrameRequested == true )
    {
    if (cvSetCaptureProperty(Superclass::m_Capture,
      CV_CAP_PROP_POS_FRAMES,this->m_FrameRequested) == 0)
      {
      std::cerr<<"Error while setting the frame"<<std::endl;
      }
    } 
  this->UpdateProperties();
  Superclass::GenerateData();
}

template< typename TOutputImage >
void VideoFileReader<TOutputImage>
::SetFileName(const char *filename)
{
  this->InitializeProperties();
  Superclass::SetFileName(filename);
}

template< typename TOutputImage >
void VideoFileReader<TOutputImage>
::LoadVideo()
{
  Superclass::LoadVideo();
  this->UpdateProperties();
}

template< typename TOutputImage >
void VideoFileReader<TOutputImage>
::PlayInput(unsigned long frame)
{
  this->SetFrameRequested(frame);
  this->PlayInput();
}

template< typename TOutputImage >
void VideoFileReader<TOutputImage>
::PlayInput()
{
  int result;
  cvShowImage("Reader Input",cvQueryFrame(Superclass::m_Capture));
  result=cvWaitKey(1000/this->m_FpS);
}

template< typename TOutputImage >
void VideoFileReader<TOutputImage>
::PlayOutput (unsigned long frame)
{
  this->SetFrameRequested(frame);
  this->PlayOutput();
}

template< typename TOutputImage >
void VideoFileReader<TOutputImage>
::PlayOutput ()
{
  int result;
  cvShowImage("Reader output",Superclass::m_CVImage);
  result=cvWaitKey(1000/this->m_FpS);
}

template< typename TOutputImage >
void VideoFileReader<TOutputImage>
::SetFrameRequested(unsigned long frame)
{
  this->m_FrameRequested = frame;
  this->Modified();
}

/*template< typename TOutputImage >
typename itk::Image<typename TOutputImage::PixelType,2>::Pointer
VideoFileReader<TOutputImage>
::StreamVideo()
{
  if ( this->m_NextFrameIsFrameRequested == true )
    {
    int result = cvSetCaptureProperty(Superclass::m_Capture,
                         CV_CAP_PROP_POS_FRAMES,this->m_FrameRequested); 
    if (result == 0)
      {
      std::cerr<<"Error while setting the frame"<<std::endl;
      }
    }
  
  this->UpdateProperties();
  return Superclass::StreamVideo();  
}*/

template< typename TOutputImage >
void VideoFileReader<TOutputImage>
::UpdateProperties()
{
  this->m_FrameHeight = 
    cvGetCaptureProperty(Superclass::m_Capture,CV_CAP_PROP_FRAME_HEIGHT);
  this->m_FrameWidth = 
    cvGetCaptureProperty(Superclass::m_Capture,CV_CAP_PROP_FRAME_WIDTH);
  this->m_FrameRequested = 
    cvGetCaptureProperty(Superclass::m_Capture,CV_CAP_PROP_POS_FRAMES);
  this->m_PositionInMSec = 
    cvGetCaptureProperty(Superclass::m_Capture,CV_CAP_PROP_POS_MSEC);
  this->m_Ratio = 
    cvGetCaptureProperty(Superclass::m_Capture,CV_CAP_PROP_POS_AVI_RATIO);
  this->m_FpS = 
    cvGetCaptureProperty(Superclass::m_Capture,CV_CAP_PROP_FPS);
  this->m_FourCC =
    cvGetCaptureProperty(Superclass::m_Capture,CV_CAP_PROP_FOURCC);
}


} // end namespace itk

#endif