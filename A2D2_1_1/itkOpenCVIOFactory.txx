/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkOpenCVIOFactory.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkOpenCVIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkOpenCVIO.h"
#include "itkVersion.h"

namespace itk
{
template <class TImage>
OpenCVIOFactory <TImage> ::OpenCVIOFactory()
{
  this->RegisterOverride( "itkVideoIOBase",
                          "itkOpenCVIO",
                          "OpenCV Video IO",
                          1,
                          CreateObjectFunction< OpenCVIO<TImage> >::New() );
}

template <class TImage>
OpenCVIOFactory <TImage> ::~OpenCVIOFactory()
{}

template <class TImage>
const char *
OpenCVIOFactory <TImage> ::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TImage>
const char *
OpenCVIOFactory <TImage> ::GetDescription(void) const
{
  return "OpenCV IO Video Factory, allows the loading of video into insight using the openCV library";
}
} // end namespace itk

