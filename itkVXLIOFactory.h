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
#ifndef __itkVXLIOFactory_h
#define __itkVXLIOFactory_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkObjectFactoryBase.h"
#include "itkVideoIOBase.h"

namespace itk
{
/** \class VXLIOFactory
 * \brief Create instances of VXLIO objects using an object factory.
 */
template <class TImage>
class ITK_EXPORT VXLIOFactory:public ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef VXLIOFactory             Self;
  typedef ObjectFactoryBase           Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char * GetITKSourceVersion(void) const;

  virtual const char * GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  static VXLIOFactory * FactoryNew() { return new VXLIOFactory; }

  /** Runtime type information (and related methods). */
  itkTypeMacro(VXLIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    VXLIOFactory::Pointer VXLFactory = VXLIOFactory::New();

    ObjectFactoryBase::RegisterFactory(VXLFactory);
  }

protected:
  VXLIOFactory();
  ~VXLIOFactory();
private:
  VXLIOFactory(const Self &); //purposely not implemented
  void operator=(const Self &);     //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVXLIOFactory.txx"
#endif

#endif