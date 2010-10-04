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
#ifndef __itkVideoFileWriter_txx
#define __itkVideoFileWriter_txx

#include "itkVideoFileWriter.h"
#include "itkImageFileWriter.h"

#include <itksys/SystemTools.hxx>
#include <fstream>

namespace itk
{

template< typename TInputImage >
VideoFileWriter< TInputImage>
::VideoFileWriter()
{
  //"Security stuff, we don't want to have goog values 
  //if the user hasn't set them
  this->m_FileName = "";
  this->m_VideoCreated = false;
  //Value that the user may not modify
  this->m_FpS = 25;
  //#if ( defined( _WIN32 ) || defined( WIN32 ) )
  this->m_FourCC = CV_FOURCC('P','I','M','1');
//#else
  //this->m_FourCC = CV_FOURCC('P','I','M','1');
//#endif

  //Declaration usefull for debug 
  this->m_Writer = 0;
  this->m_FrameToWrite = 0;
  this->m_Temp = 0;
  this->m_Size.width = 0;
  this->m_Size.height = 0;
  this->m_DoNothingFilter = itk::DoNothingFilter<TInputImage>::New();
}

/*
 * PrintSelf
 */
template< typename TInputImage >
void
VideoFileWriter< TInputImage >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "File name : "<<this->m_FileName<<std::endl; 
  
  if (this->m_VideoCreated == true)
    {
      os << indent << "Image dimensions : ["<<this->m_Size.width<<","
        <<this->m_Size.height<<"]"<<std::endl;
      os << indent << "Encoding : "<<this->m_FourCC<<std::endl;
      os << indent << "Number of frame per second : "<<this->m_FpS<<std::endl;
    }
 
} // end PrintSelf

template< typename TInputImage >
void VideoFileWriter< TInputImage >
::GenerateData()
{
    // Get the input pointer
  TInputImage::Pointer inputPtr = static_cast< TInputImage * >
         ( this->ProcessObject::GetInput(0) );
  
  //To make sure the data is here
  this->m_DoNothingFilter->SetInput(inputPtr);
  this->m_DoNothingFilter->Update();
  //Or maybe I could do that instead of the DoNothingFilter
  /*//For uptdating the whole pipeline
  inputPtr->UpdateOutputInformation();
  inputPtr->UpdateOutputData();*/

  if ( this->m_VideoCreated == false )
    {
    this->CreateVideo();
    } 
  
  if ( this->m_Writer == 0 )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, when using the video");
    exception.SetLocation("LightVideoFileWriter");
    throw exception;
    }
  
  this->TransformITKImageToCVImage();

  cvWriteFrame(this->m_Writer,this->m_FrameToWrite);
}

template< typename TInputImage >
void VideoFileWriter< TInputImage >
::SetFileName(const char* name)
{
  this->m_FileName = name;
  this->m_VideoCreated = false;
  this->Modified();
}

template< typename TInputImage >
void VideoFileWriter< TInputImage >
::SetFourCC(int fourcc)
{
  this->m_FourCC = fourcc;
  this->m_VideoCreated = false;
  this->Modified();
}

template< typename TInputImage >
void VideoFileWriter< TInputImage >
::SetFpS(double framefrequency)
{
  this->m_FpS = framefrequency;
  this->m_VideoCreated = false;
  this->Modified();
}


template< class TInputImage >
void
VideoFileWriter < TInputImage >
::SetInput(const TInputImage *input)
{
  // ProcessObject is not const_correct so this cast is required here.
  this->ProcessObject::SetNthInput( 0,
                                    const_cast< TInputImage * >( input ) );
}

template< class TInputImage >
void
VideoFileWriter < TInputImage >
::Play()
{
  if ( this->m_Writer == 0 || this->m_FrameToWrite == 0 )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, when playing video "
      "Make sure you called an Update on the writer before play()");
    exception.SetLocation("LightVideoFileWriter");
    throw exception;
    }
  
  cvShowImage("FrameToWrite",this->m_FrameToWrite);
  int k = cvWaitKey(1000/this->m_FpS);

}

template< class TInputImage >
void
VideoFileWriter < TInputImage >
::EndVideo()
{  
  this->m_VideoCreated = false; 
  
  if ( this->m_Writer != 0 )
    {
    cvReleaseVideoWriter(&this->m_Writer);
    this->Modified();
    this->m_Writer = 0;
    }
}

template< class TInputImage >
void VideoFileWriter< TInputImage >
::TransformITKImageToCVImage()
{ 
  // Get the input pointer
  TInputImage::Pointer inputPtr = static_cast< TInputImage * >
         ( this->ProcessObject::GetInput(0) );

  //Retrieve the data so we don't reload the data
  //We instead use the same buffer ( same buffer -> same image )
  cvSetData(this->m_Temp,const_cast<TInputImage::PixelType*>(inputPtr->GetBufferPointer()),this->m_Size.width);

  //We need to convert it to a RGB image
  cvCvtColor(this->m_Temp, this->m_FrameToWrite, CV_GRAY2RGB);
}

template< typename TInputImage >
void VideoFileWriter< TInputImage >
::CreateVideo()
{
  // Get the input pointer
  TInputImage::Pointer inputPtr = static_cast< TInputImage * >
         ( this->ProcessObject::GetInput(0) );
  
  //compute the pixel depth
  int depth = sizeof(TInputImage::PixelType);
  
  //Get the image in region
  itk::ImageRegion<2> region = inputPtr->GetLargestPossibleRegion();
  itk::Size<2> itkSize = region.GetSize();
  
  this->m_Size.width = itkSize[0];
  this->m_Size.height = itkSize[1];

  //Create the header
  this->m_Temp = cvCreateImageHeader(this->m_Size,depth,1);
  this->m_FrameToWrite = cvCreateImage(this->m_Size,IPL_DEPTH_8U, 3);

  if ( this->m_Temp == 0 )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, when creating the video");
    exception.SetLocation("LightVideoFileWriter");
    throw exception;
    }

  //Creating the writer 
  this->m_Writer = cvCreateVideoWriter(this->m_FileName.c_str(),
    this->m_FourCC,this->m_FpS,this->m_Size,1); 

  this->m_VideoCreated = true;
}

} // end namespace itk

#endif