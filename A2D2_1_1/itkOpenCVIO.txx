/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkImageIOBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined( _MSC_VER )
#pragma warning ( disable : 4786 )
#endif

#include "itkOpenCVIO.h"

namespace itk
{

template< typename TImage >
OpenCVIO< TImage >::OpenCVIO()
{
  this->m_CVImage = 0;
  this->m_Temp = 0;
  this->m_Capture = 0;
  this->m_Writer = 0;
  this->m_ReaderOpen = false;
  this->m_WriterOpen = false;
  this->m_FpS = 25;
  this->m_FrameTotal = 0;
  this->m_CurrentFrame = 0;
  this->m_Width = 0;
  this->m_Height = 0;
  this->m_FourCC = CV_FOURCC('P','I','M','1');
  
  this->m_ImportFilter = typename ImportFilterType::New();
}

template< typename TImage >
void OpenCVIO< TImage >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  if (this->m_CVImage != NULL)
    {
    os << indent << "Image dimensions : ["<<this->m_CVImage->width<<","
        <<this->m_CVImage->height<<"]"<<std::endl;
    os << indent << "Origin : "<<this->m_CVImage->origin<<std::endl;
    os << indent << "Image spacing (in bits) : "<<this->m_CVImage->depth<<std::endl;
    os << indent << "Image Size : "<<this->m_CVImage->imageSize<<std::endl;
    os << indent << "Color model : "<<this->m_CVImage->colorModel
        <<" ("<<this->m_CVImage->nChannels<<" channels)"<<std::endl;
    }
}

template< typename TImage >
bool OpenCVIO< TImage >
::OpenReader(const char* filename)
{
  // load the file 
  this->m_Capture = cvCaptureFromFile( filename );
 
  if ( ! this->m_Capture )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, the video specified hasn't been " 
      "correctly loaded");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    return false;
    }
  
  this->m_FrameTotal = static_cast<unsigned long>
    (cvGetCaptureProperty( this->m_Capture, CV_CAP_PROP_FRAME_COUNT ));
  this->m_FpS = static_cast<double>
    (cvGetCaptureProperty( this->m_Capture, CV_CAP_PROP_FPS ));

  this->m_Width = cvGetCaptureProperty( this->m_Capture, CV_CAP_PROP_FRAME_WIDTH );
  this->m_Height = cvGetCaptureProperty( this->m_Capture, CV_CAP_PROP_FRAME_HEIGHT );

  // create the image that will be used as based for creating the itk::Image
  this->m_CVImage = cvCreateImage( cvSize(this->m_Width,this->m_Height), IPL_DEPTH_8U, 1 );

  // asserting that the video has been succesfully loaded
  this->m_ReaderOpen = true;
  return true;
}

template< typename TImage >
bool OpenCVIO< TImage >
::Close(const char* filname)
{
  if ( this->m_Writer != 0 )
    {
    cvReleaseVideoWriter(&this->m_Writer);
    this->m_Writer = 0;
    return true;
    }
  else
    {
    return false;
    }
}

template< typename TImage >
typename itk::Image<typename TImage::PixelType,2>::Pointer
OpenCVIO <TImage> :: Read()
{
  if ( this->m_Capture == NULL || this->m_CVImage == NULL )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, when updating, " 
      "make sure you loaded the video first");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    }

  //Setting the filter parameters
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

  //Querying the frame
  this->m_Temp = cvQueryFrame(this->m_Capture);

  if ( this->m_Temp == NULL )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error when acquiring, " 
      "make sure you aren't out of boundaries");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    }
 
  this->UpdateProperties();

  //CV_RGB2GRAY: convert RGB image to grayscale 
  cvCvtColor(this->m_Temp,this->m_CVImage, CV_RGB2GRAY );
 
  this->m_ImportFilter->SetImportPointer(reinterpret_cast<typename PixelType*>
    (this->m_CVImage->imageData),this->m_CVImage->imageSize,false );
  this->m_ImportFilter->Update();
  
  return this->m_ImportFilter->GetOutput();
}

template< typename TImage >
bool OpenCVIO< TImage >::SetNextFrameToRead(unsigned long frameNumber)
{
  if (this ->m_Capture != NULL)
    {
    cvSetCaptureProperty(this->m_Capture,CV_CAP_PROP_POS_FRAMES,frameNumber);
    this->UpdateProperties();
    this->Modified();
    return true;
    }
  return false;
}

template< typename TImage >
bool OpenCVIO< TImage >
::OpenWriter(const char* filename, typename itk::Image<typename TImage::PixelType,2>::Pointer ITKImage)
{
  //compute the pixel depth
  int depth = sizeof(TImage::PixelType)*8;
  
  //Get the image in region
  this->m_Region = ITKImage->GetLargestPossibleRegion();
  this->m_Size = this->m_Region.GetSize();
  
  CvSize size;
  size.width = this->m_Size[0];
  size.height = this->m_Size[1];

  //Create the header
  this->m_Temp = cvCreateImageHeader(size,depth,1);
  this->m_CVImage = cvCreateImage(size,depth, 3);

  if ( this->m_Temp == NULL )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, when creating the video");
    exception.SetLocation("LightVideoFileWriter");
    throw exception;
    return false;
    }
  else
    {
    //Creating the writer 
    this->m_Writer = cvCreateVideoWriter(
      filename,this->m_FourCC,
      this->m_FpS,size,1); 
    this->m_WriterOpen = true;
    return true;
    }
}

template< typename TImage >
bool OpenCVIO< TImage >::Write
(typename itk::Image<typename TImage::PixelType,2>::Pointer ITKImage)
{
  if ( this->m_Writer == NULL )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, when using the video");
    exception.SetLocation("LightVideoFileWriter");
    throw exception;
    }
  
   //Retrieve the data so we don't reload the data
  //We instead use the same buffer ( same buffer -> same image )
  cvSetData(this->m_Temp,const_cast<TImage::PixelType*>(ITKImage->GetBufferPointer()),this->m_Temp->widthStep);

  //We need to convert it to a RGB image
  cvCvtColor(this->m_Temp, this->m_CVImage, CV_GRAY2RGB);

  cvWriteFrame(this->m_Writer,this->m_CVImage);
  
  return true;
}

template< typename TImage >
void OpenCVIO< TImage >::UpdateProperties()
{
  this->m_CurrentFrame = 
    cvGetCaptureProperty(this->m_Capture,CV_CAP_PROP_POS_FRAMES);
  this->m_PositionInMSec = 
    cvGetCaptureProperty(this->m_Capture,CV_CAP_PROP_POS_MSEC);
  this->m_Ratio = 
    cvGetCaptureProperty(this->m_Capture,CV_CAP_PROP_POS_AVI_RATIO);
  this->m_FpS = 
    cvGetCaptureProperty(this->m_Capture,CV_CAP_PROP_FPS);
  this->m_FourCC =
    cvGetCaptureProperty(this->m_Capture,CV_CAP_PROP_FOURCC);
}

}; //namespace itk end
