/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVideoIOFactory.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVideoIOFactory_h
#define __itkVideoIOFactory_h

#include "itkObject.h"
#include "itkVideoIOBase.h"

namespace itk
{
/** \class ImageIOFactory
 * \brief Create instances of ImageIO objects using an object factory.
 */
template <class TImage>
class ITK_EXPORT VideoIOFactory:public Object
{
public:
  /** Standard class typedefs. */
  typedef VideoIOFactory             Self;
  typedef Object                     Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Class Methods used to interface with the registered factories */
  enum LIBRARY_USED { 
          ITK_USE_OPENCV,
          ITK_USE_VXL
          };

  /** Run-time type information (and related methods). */
  itkTypeMacro(VideoIOFactory, Object);

  /** Convenient typedefs. */
  typedef typename itk::VideoIOBase<TImage>::Pointer VideoIOBasePointer;

  /** Create the appropriate ImageIO depending on the particulars of the file.
    */
  static VideoIOBasePointer CreateVideoIO( LIBRARY_USED myChoice );

  /** Register Built-in factories */
  static void RegisterBuiltInFactories();

protected:
  VideoIOFactory();
  ~VideoIOFactory();
private:
  VideoIOFactory(const Self &); //purposely not implemented
  void operator=(const Self &); //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVideoIOFactory.txx"
#endif

#endif
