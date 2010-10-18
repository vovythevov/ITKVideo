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

#include "itkVXLIO.h"
#include "vidl/vidl_convert.h"

namespace itk
{

template< typename TImage >
VXLIO< TImage >::VXLIO()
{
  //this->m_CVImage = 0;
  //this->m_Temp = 0;
  this->m_VIDLImage = 0;
  this->m_VIDLFrame = 0;
  this->m_Reader = 0;
  this->m_ReaderOpen = false;
  this->m_WriterOpen = false;
  this->m_FpS = 25;
  this->m_FrameTotal = 0;
  this->m_Width = 0;
  this->m_Height = 0;
  this->m_Encoder = vidl_ffmpeg_ostream_params::DEFAULT;
  
  this->m_ImportFilter = typename ImportFilterType::New();
}

template< typename TImage >
void VXLIO< TImage >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  /*if (this->m_CVImage != NULL)
    {
    os << indent << "Image dimensions : ["<<this->m_CVImage->width<<","
        <<this->m_CVImage->height<<"]"<<std::endl;
    os << indent << "Origin : "<<this->m_CVImage->origin<<std::endl;
    os << indent << "Image spacing (in bits) : "<<this->m_CVImage->depth<<std::endl;
    os << indent << "Image Size : "<<this->m_CVImage->imageSize<<std::endl;
    os << indent << "Color model : "<<this->m_CVImage->colorModel
        <<" ("<<this->m_CVImage->nChannels<<" channels)"<<std::endl;
    }*/
}
template< typename TImage >
bool VXLIO< TImage >
::SetNextFrameToRead (unsigned long frameNumber)
{
  if ( this->m_Reader->is_open() == true )
    {
    if (this->m_Reader->is_seekable () == true)
      {
      this->m_Reader->seek_frame (frameNumber);
      this->Modified();
      return true;
      }
    }
  return false;
}

template< typename TImage >
void VXLIO< TImage >::UpdateProperties ()
{
  this->m_CurrentFrame = this->m_Reader->frame_number();
}

template< typename TImage >
bool VXLIO< TImage >
::OpenReader(const char* filename)
{
  // load the file 
  this->m_Reader = new vidl_ffmpeg_istream();
  this->m_Reader->open(filename);

  if ( ! this->m_Reader->is_open() )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, the video specified hasn't been " 
      "correctly loaded. It can be because you don't haver ffmpeg "
      "(or vxl didn't find it");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    return false;
    }
  //Get the total number of frame 
  //Also set if the video is seekable or not (which can implies
  // a different way to read it...)
  this->m_FrameTotal = this->m_Reader->num_frames();
  if ( this->m_FrameTotal == 1 )
    {
    this->m_VideoSeekable = false;
    }
  else
    {
    this->m_VideoSeekable = true;
    }

  this->m_FpS = this->m_Reader->frame_rate();
  this->m_Width = this->m_Reader->width();
  this->m_Height = this->m_Reader->height();
  
  /*
  // create the image that will be used as based for creating the itk::Image
  this->m_CVImage = cvCreateImage( cvSize(this->m_Width,this->m_Height), IPL_DEPTH_8U, 1 );
  */

  // asserting that the video has been succesfully loaded
  this->m_ReaderOpen = true;
  return true;
}

template< typename TImage >
bool VXLIO< TImage >
::Close(const char* filname)
{
  if ( this->m_Writer != 0 )
    {
    this->m_Writer->close();
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
VXLIO <TImage> :: Read()
{
  if ( this->m_Reader == NULL )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, when updating, " 
      "make sure you loaded the video first");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    }

  //Setting the filter parameters
  this->m_Start.Fill(0);
  this->m_Size[0] = this->m_Width;
  this->m_Size[1] = this->m_Height;

  this->m_Region.SetIndex( this->m_Start );
  this->m_Region.SetSize( this->m_Size );
  this->m_ImportFilter->SetRegion( this->m_Region );

  double Origin[2] = {0.0,0.0};

  this->m_ImportFilter->SetOrigin( Origin );

  this->m_PixelFormat = this->m_Reader->format();

  this->m_Spacing[0] = vidl_pixel_format_bpp(this->m_PixelFormat)
    * vidl_pixel_format_num_channels(this->m_PixelFormat)/8;
  this->m_Spacing[1] = vidl_pixel_format_bpp(this->m_PixelFormat)
    * vidl_pixel_format_num_channels(this->m_PixelFormat)/8;
  this->m_ImportFilter->SetSpacing( this->m_Spacing );

  //Querying the frame
  this->m_VIDLImage = this->m_Reader->read_frame();

  if ( this->m_VIDLImage == NULL )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error when acquiring, " 
      "make sure you aren't out of boundaries");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    }
  
  this->UpdateProperties();
 
  this->m_ImportFilter->SetImportPointer(reinterpret_cast<typename PixelType*>
    (this->m_VIDLImage->data()),this->m_VIDLImage->size(),false );
  this->m_ImportFilter->Update();

  return this->m_ImportFilter->GetOutput();
}


template< typename TImage >
bool VXLIO< TImage >
::OpenWriter(const char* filename, typename itk::Image<typename TImage::PixelType,2>::Pointer ITKImage)
{
  //Get the image in region so that we can get the size of the image to write
  this->m_Region = ITKImage->GetLargestPossibleRegion();
  this->m_Size = this->m_Region.GetSize();
  
  vidl_ffmpeg_ostream_params parameters ;
  parameters.frame_rate_ = 29.95;//25 by default, can be changed
  //parameters.bit_rate_ = 5000;//5000 by default in VXL
  //Since I'm not sure if changing it is a good idea, I leave the default
  parameters.ni_ = this->m_Size[0];//Set the width
  parameters.nj_ = this->m_Size[1];//Set the Height
  parameters.encoder_ = this->m_Encoder;//DVVIDEO by default can be changed
  //parameter.file_format_ = vidl_ffmpeg_ostream_params::GUESS;//No choice...
  
  this->m_Writer = new vidl_ffmpeg_ostream(filename,parameters);

  return this->m_Writer->open();
}

template< typename TImage >
bool VXLIO < TImage >::Write
(typename itk::Image<typename TImage::PixelType,2>::Pointer ITKImage)
{
  if ( ! this->m_Writer->is_open() )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, the video specified hasn't been " 
      "correctly loaded. It can be because you don't have ffmpeg "
      "(or vxl didn't find it)");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    return false;
    }

  vidl_pixel_format pixelFormat;

  switch (sizeof(TImage::PixelType))
  {
  case 1 :
    pixelFormat = vidl_pixel_format_from_string("VIDL_PIXEL_FORMAT_MONO_1");
    break;
  case 8 :
    pixelFormat = vidl_pixel_format_from_string("VIDL_PIXEL_FORMAT_MONO_8");
    break;
  case 16 :
    pixelFormat = vidl_pixel_format_from_string("VIDL_PIXEL_FORMAT_MONO_16");
    break;
  default :
    itk::ExceptionObject exception;
    exception.SetDescription("Error, VXL Writer doesn't support type that have more than 16 bits");
    exception.SetLocation("VideoFileWriter");
    throw exception;
    return false;
  }
    
  
  vidl_shared_frame  *VIDLFrame = new vidl_shared_frame(
    const_cast<TImage::PixelType*>(ITKImage->GetBufferPointer()),
    static_cast<unsigned int>(this->m_Size[0]),
    static_cast<unsigned int>(this->m_Size[1]),
    pixelFormat);
  
  this->m_Writer->write_frame(VIDLFrame);
  
  /*
   //Retrieve the data so we don't reload the data
  //We instead use the same buffer ( same buffer -> same image )
  cvSetData(this->m_Temp,const_cast<TImage::PixelType*>(ITKImage->GetBufferPointer()),this->m_Temp->widthStep);

  //We need to convert it to a RGB image
  cvCvtColor(this->m_Temp, this->m_CVImage, CV_GRAY2RGB);

  cvWriteFrame(this->m_Writer,this->m_CVImage);
  */

  return true;
}


}; //namespace itk end
