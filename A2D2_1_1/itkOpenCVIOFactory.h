/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkOpenCVIOFactory.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOpenCVIOFactory_h
#define __itkOpenCVIOFactory_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkObjectFactoryBase.h"
#include "itkVideoIOBase.h"

namespace itk
{
/** \class VideoeIOFactory
 * \brief Create instances of openCVIO objects using an object factory.
 */
template <class TImage>
class ITK_EXPORT OpenCVIOFactory:public ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef OpenCVIOFactory             Self;
  typedef ObjectFactoryBase           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char * GetITKSourceVersion(void) const;

  virtual const char * GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  static OpenCVIOFactory * FactoryNew() { return new OpenCVIOFactory; }

  /** Run-time type information (and related methods). */
  itkTypeMacro(OpenCVIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    OpenCVIOFactory::Pointer OpenCVFactory = OpenCVIOFactory::New();

    ObjectFactoryBase::RegisterFactory(OpenCVFactory);
  }

protected:
  OpenCVIOFactory();
  ~OpenCVIOFactory();
private:
  OpenCVIOFactory(const Self &); //purposely not implemented
  void operator=(const Self &);     //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkOpenCVIOFactory.txx"
#endif

#endif
