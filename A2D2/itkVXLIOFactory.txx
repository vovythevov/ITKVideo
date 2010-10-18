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
#include "itkVXLIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace itk
{
template <class TImage>
VXLIOFactory <TImage> ::VXLIOFactory()
{
  this->RegisterOverride( "itkVideoIOBase",
                          "itkVXLIO",
                          "VXL Video IO",
                          1,
                          CreateObjectFunction< VXLIO<TImage> >::New() );
}

template <class TImage>
VXLIOFactory <TImage> ::~VXLIOFactory()
{}

template <class TImage>
const char *
VXLIOFactory <TImage> ::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TImage>
const char *
VXLIOFactory <TImage> ::GetDescription(void) const
{
  return "VXL IO Video Factory, allows the loading of video into insight using the VXL (vidl) library";
}
} // end namespace itk
