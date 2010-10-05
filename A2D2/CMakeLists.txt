cmake_minimum_required(VERSION 2.8)
project(A2D2)

set(openCVImageFilters_SRCS itkFaceDetectionFilterTest.cxx )

set(openCVVideoFilters_SRCS itkVideoFileReaderTest.cxx )

set(openCVMotionTracker_SRCS itkMotionTrackerTest.cxx )

set(openCVTransformDICOMToVideo_SRCS itkTransformDICOMToVideo.cxx )


find_package ( OpenCV REQUIRED )
include_directories ( ${OpenCV_INCLUDE_DIRS} )

find_package (ITK REQUIRED)
include( ${ITK_USE_FILE} )

add_executable(itkFaceDetectionFilterTest ${openCVImageFilters_SRCS})
target_link_libraries(itkFaceDetectionFilterTest ${ITK_LIBRARIES} ${OpenCV_LIBS})

add_executable(itkVideoFileReaderTest ${openCVVideoFilters_SRCS})
target_link_libraries(itkVideoFileReaderTest ${ITK_LIBRARIES} ${OpenCV_LIBS})

add_executable(itkMotionTrackerTest ${openCVMotionTracker_SRCS})
target_link_libraries(itkMotionTrackerTest ${ITK_LIBRARIES} ${OpenCV_LIBS})

add_executable(itkTransformDICOMToVideo ${openCVTransformDICOMToVideo_SRCS})
target_link_libraries(itkTransformDICOMToVideo ${ITK_LIBRARIES} ${OpenCV_LIBS})


