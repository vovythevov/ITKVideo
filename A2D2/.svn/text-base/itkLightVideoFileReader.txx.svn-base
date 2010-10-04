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
#ifndef __itkLightVideoFileReader_txx
#define __itkLightVideoFileReader_txx

#include "itkLightVideoFileReader.h"
#include "itkImageSource.h"
#include "itkExceptionObject.h"
#include "itkConvertPixelBuffer.h"

#include <itksys/SystemTools.hxx>
#include <fstream>

namespace itk
{

template< typename TOutputImage, class ConvertPixelTraits >
LightVideoFileReader< TOutputImage, ConvertPixelTraits >
::LightVideoFileReader()
{
  //"Security stuff, we don't want to have goog values 
  //if the user hasn't set them
  this->m_FileName = "";
  this->m_FrameTotal = -1;
  this->m_VideoLoaded = false;
  //Declaration usefull for debug 
  this->m_Capture = 0;
  this->m_CVImage = 0;
  this->m_Temp = 0;

  //Declaration here so we only declare one filter
  this->m_ImportFilter = ImportFilterType::New();
  
}

/*
 * PrintSelf
 */
template< typename TOutputImage, class ConvertPixelTraits >
void
LightVideoFileReader< TOutputImage, ConvertPixelTraits >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "File name : "<<this->m_FileName<<std::endl; 
  os << indent << "Frame total in that video : "<<this->m_FrameTotal<<std::endl;
  
  if (this->m_CVImage != 0)
    {
      os << indent << "Image dimensions : ["<<this->m_CVImage->width<<","
        <<this->m_CVImage->height<<"]"<<std::endl;
      os << indent << "Origin : "<<this->m_CVImage->origin<<std::endl;
      os << indent << "Image spacing (in bits) : "<<this->m_CVImage->depth<<std::endl;
      os << indent << "Image Size : "<<this->m_CVImage->imageSize<<std::endl;
      os << indent << "Color model : "<<this->m_CVImage->colorModel
        <<" ("<<this->m_CVImage->nChannels<<" channels)"<<std::endl;
    }
 
} // end PrintSelf


template< typename TOutputImage, class ConvertPixelTraits >
void LightVideoFileReader< TOutputImage, ConvertPixelTraits >
::SetFileName(const char* filename)
{ 
  this->m_FileName = filename;
  this->m_VideoLoaded = false;
  this->Modified();
}

template< typename TOutputImage, class ConvertPixelTraits >
void LightVideoFileReader< TOutputImage, ConvertPixelTraits >
::GenerateData()
{  
  if ( this->m_Capture == NULL )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, when updating, " 
      "make sure you loaded the video first");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    }
  
  this->m_Temp = cvQueryFrame(this->m_Capture);

  if ( this->m_Temp == NULL )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error when acquiring, " 
      "make sure you aren't out of boundaries");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    }
 
  //CV_RGB2GRAY: convert RGB image to grayscale 
  cvCvtColor(this->m_Temp,this->m_CVImage, CV_RGB2GRAY );
 
  this->m_ImportFilter->SetImportPointer(reinterpret_cast<typename OutputImagePixelType*>
    (this->m_CVImage->imageData),this->m_CVImage->imageSize,false );

  this->m_ImportFilter->Update();

  this->GraftOutput(this->m_ImportFilter->GetOutput());
}

template< typename TOutputImage, class ConvertPixelTraits >
void LightVideoFileReader< TOutputImage, ConvertPixelTraits >
::GenerateOutputInformation()
{ 
  //We only need to do that once.
  if ( ! this->m_VideoLoaded )
    {
    this->LoadVideo();
    }
  
  typename TOutputImage::Pointer output = this->GetOutput();

  this->m_Start.Fill(0);
  this->m_Size[0] = this->m_CVImage->width;
  this->m_Size[1] = this->m_CVImage->height;

  this->m_Region.SetIndex( this->m_Start );
  this->m_Region.SetSize( this->m_Size );
  this->m_ImportFilter->SetRegion( this->m_Region );

  if ( this->m_CVImage->origin == 0 )
    {
    this->m_Origin[0] = 0.0; // X coordinate
    this->m_Origin[1] = 0.0; // Y coordinate
    }
  else
    {
    this->m_Origin[0] = 0.0; // X coordinate
    this->m_Origin[1] = static_cast<double>(this->m_CVImage->height) ; // Y coordinate
    }
  this->m_ImportFilter->SetOrigin( this->m_Origin );

  this->m_Spacing[0] = (this->m_CVImage->depth*this->m_CVImage->nChannels)/8;
  this->m_Spacing[1] = (this->m_CVImage->depth*this->m_CVImage->nChannels)/8;
  this->m_ImportFilter->SetSpacing( this->m_Spacing );

  output->SetLargestPossibleRegion(this->m_Region);
}

template< typename TOutputImage, class ConvertPixelTraits >
void LightVideoFileReader< TOutputImage, ConvertPixelTraits >
::KeepReading()
{
  this->Modified();
}

template< typename TOutputImage, class ConvertPixelTraits >
void LightVideoFileReader< TOutputImage, ConvertPixelTraits >
::LoadVideo()
{
  //test existence and permission to read
  this->TestFileExistanceAndReadability();

  // load the file 
  this->m_Capture = cvCaptureFromFile( this->m_FileName.c_str() );
 
  if ( ! this->m_Capture )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, the video specified hasn't been " 
      "correctly loaded");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    }
  
  this->m_FrameTotal = static_cast<unsigned long>
    (cvGetCaptureProperty( this->m_Capture, CV_CAP_PROP_FRAME_COUNT ));
  
  int width = cvGetCaptureProperty( this->m_Capture, CV_CAP_PROP_FRAME_WIDTH );
  int height = cvGetCaptureProperty( this->m_Capture, CV_CAP_PROP_FRAME_HEIGHT );

  // create the image that will be used as based for creating the itk::Image
  this->m_CVImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );

  // asserting that the video has been succesfully loaded
  this->m_VideoLoaded = true;
  this->Modified();
}

/*template< typename TOutputImage, class ConvertPixelTraits >
void LightVideoFileReader< TOutputImage, ConvertPixelTraits >
::Play()
{
  int k;
  cvShowImage("video",cvQueryFrame(this->m_Capture));
  k = cvWaitKey(400);
}*/

/*template< typename TOutputImage, class ConvertPixelTraits >
typename itk::Image<typename TOutputImage::PixelType,2>::Pointer 
LightVideoFileReader< TOutputImage, ConvertPixelTraits >::StreamVideo()
{
  if ( this->m_Capture == NULL )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, when updating, " 
      "make sure you loaded the video first");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    }
  
  this->m_Temp = cvQueryFrame(this->m_Capture);

  if ( this->m_Temp == NULL )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error when acquiring, " 
      "make sure you aren't out of boundaries");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    }
 
  //CV_RGB2GRAY: convert RGB image to grayscale 
  cvCvtColor(this->m_Temp,this->m_CVImage, CV_RGB2GRAY );
 
  this->m_ImportFilter->SetImportPointer(reinterpret_cast<typename OutputImagePixelType*>(this->m_CVImage->imageData),
    this->m_CVImage->imageSize,false );

  this->m_ImportFilter->Update();
     
  return this->m_ImportFilter->GetOutput(); 
}*/

template< class TOutputImage, class ConvertPixelTraits >
void
LightVideoFileReader< TOutputImage, ConvertPixelTraits >
::TestFileExistanceAndReadability()
{
  // Test if the file exists.
  if ( !itksys::SystemTools::FileExists( m_FileName.c_str() ) )
    {
    itk::ExceptionObject e(__FILE__, __LINE__);
    std::ostringstream       msg;
    msg << "The file doesn't exist. "
        << std::endl << "Filename = " << m_FileName
        << std::endl;
    e.SetDescription( msg.str().c_str() );
    throw e;
    return;
    }

  // Test if the file can be open for reading access.
  std::ifstream readTester;
  readTester.open( m_FileName.c_str() );
  if ( readTester.fail() )
    {
    readTester.close();
    std::ostringstream msg;
    msg << "The file couldn't be opened for reading. "
        << std::endl << "Filename: " << m_FileName
        << std::endl;
    itk::ExceptionObject e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    return;
    }
  readTester.close();
}

} // end namespace itk

#endif