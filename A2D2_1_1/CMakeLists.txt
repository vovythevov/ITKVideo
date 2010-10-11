cmake_minimum_required(VERSION 2.8)
project(A2D2)

enable_testing ()
include (CTest)

#set(itkVideoCommon_SRCS itkVideoIOFactory.h itkVideoIOFactory.cxx )
   
set(openCVImageFilters_SRCS itkFaceDetectionFilterTest.cxx )
set(itkVideoFilters_SRCS itkVideoFileExample.cxx )
set(itkVMotionTracker_SRCS itkMotionTrackerTest.cxx )
set(itkVTransformDICOMToVideo_SRCS itkTransformDICOMToVideo.cxx )
set(itkVideoFileReaderExample_SRCS itkVideoFileReaderExample.cxx )

find_package ( OpenCV REQUIRED )
include_directories ( ${OpenCV_INCLUDE_DIRS} )

find_package ( VXL REQUIRED )
include( ${VXL_CMAKE_DIR}/UseVXL.cmake )

find_package (ITK REQUIRED)
include( ${ITK_USE_FILE} )

#add_library( itkVideoCommon ${itkVideoCommon_SRCS} )

add_executable(itkFaceDetectionFilterTest ${openCVImageFilters_SRCS})
target_link_libraries(itkFaceDetectionFilterTest ${ITK_LIBRARIES} ${OpenCV_LIBS} vidl ) #itkVideoCommon)

add_executable(itkVideoFileReaderTest ${itkVideoFilters_SRCS})
target_link_libraries(itkVideoFileReaderTest ${ITK_LIBRARIES} ${OpenCV_LIBS} vidl ) #itkVideoCommon)

add_executable(itkMotionTrackerTest ${itkVMotionTracker_SRCS})
target_link_libraries(itkMotionTrackerTest ${ITK_LIBRARIES} ${OpenCV_LIBS} vidl ) #itkVideoCommon)

add_executable(itkTransformDICOMToVideo ${itkVTransformDICOMToVideo_SRCS})
target_link_libraries(itkTransformDICOMToVideo ${ITK_LIBRARIES} ${OpenCV_LIBS} vidl ) #itkVideoCommon)

add_executable(itkVideoFileReaderExample ${itkVideoFileReaderExample_SRCS})
target_link_libraries(itkVideoFileReaderExample ${ITK_LIBRARIES} ${OpenCV_LIBS} vidl ) #itkVideoCommon)


