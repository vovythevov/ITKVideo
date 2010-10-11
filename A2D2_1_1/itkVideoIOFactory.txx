/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVideoIOFactory.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkVideoIOFactory.h"
#include "itkOpenCVIOFactory.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

namespace itk
{
template <class TImage>
typename VideoIOBase<TImage>::Pointer
VideoIOFactory <TImage>::CreateVideoIO( LIBRARY_USED lib )
{
  RegisterBuiltInFactories();

  std::list< LightObject::Pointer > allobjects =
    ObjectFactoryBase::CreateAllInstance("itkVideoIOBase");
  std::list< LightObject::Pointer >::iterator i;
  int count = 0;

  for ( i = allobjects.begin() ; i != allobjects.end() ; ++i )
    {
    if ( count == lib )
      {
      //VideoIOBase<TImage> *io 
      VideoIOBase<TImage>::Pointer ptr = dynamic_cast< VideoIOBase<TImage>* >(i->GetPointer());
      if ( ptr.IsNotNull() )
        {
        //ptr = io;
        return ptr; 
        }
      }
    ++count;
    }
  
  std::cerr << "Error VideoIO factory did not return an VideoIOBase "<< std::endl;
  return 0;

}

template <class TImage>
void
VideoIOFactory <TImage> ::RegisterBuiltInFactories()
{
  static bool firstTime = true;

  static SimpleMutexLock mutex;

    {
    // This helper class makes sure the Mutex is unlocked
    // in the event an exception is thrown.
    MutexLockHolder< SimpleMutexLock > mutexHolder(mutex);
    if ( firstTime )
      {
      ObjectFactoryBase::RegisterFactory( OpenCVIOFactory<TImage>::New() );
      //ObjectFactoryBase::RegisterFactory( VXLIOFactory<TImage>::New() );
      
      firstTime = false;
      }
    }
}

} // end namespace itk