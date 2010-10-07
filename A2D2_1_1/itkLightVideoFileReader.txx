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
#include "itkVideoIOBase.h"

#include <itksys/SystemTools.hxx>
#include <fstream>

namespace itk
{

template< typename TOutputImage, class ConvertPixelTraits >
LightVideoFileReader< TOutputImage, ConvertPixelTraits >
::LightVideoFileReader()
{
  //Security stuff, we don't want to have good values 
  //if the user hasn't set them
  this->m_FileName = "";
  this->m_VideoIO = 0;
  
  //Declaration of default behavior
  this->m_VideoLoaded = false;
  this->m_UseOpenCV = true;


  /*//Declaration usefull for debug 
  this->m_Capture = 0;
  this->m_CVImage = 0;
  this->m_Temp = 0;*/

  //Declaration here so we only declare one filter
  //this->m_ImportFilter = ImportFilterType::New();
  
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
  /*
  if (this->m_CVImage != 0)
    {
      os << indent << "Image dimensions : ["<<this->m_CVImage->width<<","
        <<this->m_CVImage->height<<"]"<<std::endl;
      os << indent << "Origin : "<<this->m_CVImage->origin<<std::endl;
      os << indent << "Image spacing (in bits) : "<<this->m_CVImage->depth<<std::endl;
      os << indent << "Image Size : "<<this->m_CVImage->imageSize<<std::endl;
      os << indent << "Color model : "<<this->m_CVImage->colorModel
        <<" ("<<this->m_CVImage->nChannels<<" channels)"<<std::endl;
    }*/
 
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
::UseOpenCV ( bool useOpenCV )
{
  if ( useOpenCV != this->m_UseOpenCV )
    {
    this ->m_UseOpenCV = useOpenCV;
    this->Modified();
    }
}

template< typename TOutputImage, class ConvertPixelTraits >
void LightVideoFileReader< TOutputImage, ConvertPixelTraits >
::GenerateData()
{  
  if ( ! this->m_VideoIO->Is_Open() )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, when updating, " 
      "make sure you loaded the video first");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    }

  this->GraftOutput(this->m_VideoIO->Read());

  /*

  this->GraftOutput(this->m_ImportFilter->GetOutput());*/
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
  
  //Retriveing the data for the region
  this->m_Start.Fill(0);
  this->m_Size[0] = this->m_VideoIO->GetWidth();
  this->m_Size[1] = this->m_VideoIO->GetHeight();  
  
  //Setting the filter's region
  this->m_Region.SetIndex( this->m_Start );
  this->m_Region.SetSize( this->m_Size );

  /*this->m_ImportFilter->SetRegion( this->m_Region );

  //Setting the filter's origin
  this->m_Origin[0] = this->m_VideoIO->GetXOrigin(); // X coordinate
  this->m_Origin[1] = this->m_VideoIO->GetYOrigin(); // Y coordinate
  this->m_ImportFilter->SetOrigin( this->m_Origin );

  //Setting the spacing
  this->m_Spacing[0] = this->m_VideoIO->GetXSpacing();
  this->m_Spacing[1] = this->m_VideoIO->GetYSpacing();
  this->m_ImportFilter->SetSpacing( this->m_Spacing );*/

  //Finally setting the region requested
  typename TOutputImage::Pointer output = this->GetOutput();
  output->SetLargestPossibleRegion(this->m_Region);


  /*//We only need to do that once.
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

  output->SetLargestPossibleRegion(this->m_Region);*/
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

  if ( this->m_UseOpenCV == true )
    {
    //I should create a factory for taht, but we'll see later
    this->m_VideoIO = new itk::OpenCVIO<typename OutputImageType>;
    this->m_VideoIO->Open(this->m_FileName.c_str());
    }
  else
    {
    //this->m_VideoIO = itk::VXLIO<typename OutputImageType>::New();
    this->m_VideoIO->Open(this->m_FileName.c_str());
    }

  if ( ! this->m_VideoIO->Is_Open() )
    {
    itk::ExceptionObject exception;
    exception.SetDescription("Error, the video specified hasn't been " 
      "correctly loaded");
    exception.SetLocation("LightVideoFileReader");
    throw exception;
    }
    
  // asserting that the video has been succesfully loaded
  this->m_VideoLoaded = true;
  this->Modified();
}

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