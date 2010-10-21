/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVideoViewer.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVideoViewer_txx
#define __itkVideoViewer_txx

#include "itkVideoViewer.h"
#include "itkVideoViewerFactory.h"
#include "itkVideoFileWriter.h"

#include "time.h"

namespace itk
{ 

template< typename TInputImage >
VideoViewer< TInputImage>::VideoViewer()
{
  this->m_ViewerExists = false;
  this->m_ViewerClosed = false;
  this->m_UseOpenCV = true;
  this->m_VideoViewer = 0;
  this->m_WindowName = "";
}

template< typename TInputImage >
bool VideoViewer< TInputImage>::SetWaitTime(int MSec)
{ 
  if (this->m_ViewerExists == true )
    {  
    this->m_VideoViewer->SetWaitTime(MSec);
    this->Modified();
    return true;
    }
  return false;
}

template< typename TInputImage >
void VideoViewer< TInputImage>::SetInput(const TInputImage *input)
{  
  // ProcessObject is not const_correct so this cast is required here.
  this->ProcessObject::SetNthInput( 0,
                                    const_cast< TInputImage * >( input ) );
}

template< typename TInputImage >
void VideoViewer< TInputImage>::UseOpenCV ( bool useOpenCV )
{
  if (this->m_UseOpenCV != useOpenCV && this->m_ViewerExists)
    {
    this->Close();
    itk::ExceptionObject e;
    e.SetDescription("Error, You cannot change the type of Viewer while having a reader opened");
    e.SetLocation("VideoViewer");
    throw e;
    }
  this->m_UseOpenCV = useOpenCV;
  this->Modified();
}

template< typename TInputImage >
void VideoViewer< TInputImage>::Wait()
{
  this->m_VideoViewer->Wait();
}

template< typename TInputImage >
void VideoViewer< TInputImage>::Close()
{
  if (this->m_ViewerExists == true)
    {
    this->m_VideoViewer->Close(this->m_WindowName.c_str());
    this->m_ViewerExists = false;
    this->m_ViewerClosed = true;
    this->Modified();
    }
}

template< typename TInputImage >
void VideoViewer< TInputImage>::Open()
{
  this->Open(this->GetNameOfClass());
  this->m_WindowName = this->GetNameOfClass();
}

template< typename TInputImage >
void VideoViewer< TInputImage>::Open(const char* WindowName)
{
  if (this->m_ViewerExists == false)
    {
      if (this->m_UseOpenCV == true)
      {
      this->m_VideoViewer = VideoViewerFactory<TInputImage>::CreateVideoViewer
          (VideoViewerFactory<TInputImage>::ITK_USE_OPENCV);
      this->m_VideoViewer->Open(WindowName);
      }
      else
      {
      this->m_VideoViewer = VideoViewerFactory<TInputImage>::CreateVideoViewer
          (VideoViewerFactory<TInputImage>::ITK_USE_VXL);
      this->m_VideoViewer->Open(WindowName);
      }
    this->m_ViewerExists = true;
    this->m_ViewerClosed = false;
    this->Modified();
    }
}

template< typename TInputImage >
void VideoViewer< TInputImage>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os<<indent<<"Viewer open : "<<this->m_ViewerExists<<std::endl;
  os<<indent<<"UseOpenCV : "<<this->m_UseOpenCV<<std::endl;
}
  
template< typename TInputImage >
void VideoViewer< TInputImage>
::GenerateData()
{
  InputImagePointer inputPtr = const_cast<InputImageType *>(this->GetInput());
  //To make sure the data is here :
  inputPtr->Update();

  //if the viewer hasn't been closed manually
  if (this->m_ViewerClosed == false )
  {
    //if a windowName has been set
    if (this->m_WindowName != "")
      {
      this->Open(this->m_WindowName.c_str());
      }
    else
      {
      this->Open();
      }
 
  this->m_VideoViewer->Play( inputPtr );
  this->m_VideoViewer->Wait();
  }
  //finaly we graft the input into the output. The only difference is the delay. 
  this->GraftOutput( inputPtr );
}

}

#endif