cmake_minimum_required(VERSION 2.8)
project(A2D2)

enable_testing ()
include (CTest)

#set(itkVideoCommon_SRCS itkVideoIOFactory.h itkVideoIOFactory.cxx )
   
set(openCVImageFilters_SRCS itkFaceDetectionFilterTest.cxx )
set(itkVideoFilePipelineTest_SRCS itkVideoFilePipelineTest.cxx )
set(itkVMotionTracker_SRCS itkMotionTrackerTest.cxx )
set(itkVTransformDICOMToVideo_SRCS itkTransformDICOMToVideo.cxx )
set(itkVideoFileReaderExample_SRCS itkVideoFileReaderExample.cxx )

find_package ( OpenCV REQUIRED )
include_directories ( ${OpenCV_INCLUDE_DIRS} )

find_package ( VXL REQUIRED )
include( ${VXL_CMAKE_DIR}/UseVXL.cmake )

find_package (ITK REQUIRED)
include( ${ITK_USE_FILE} )


add_executable(itkFaceDetectionFilterTest ${openCVImageFilters_SRCS})
target_link_libraries(itkFaceDetectionFilterTest ${ITK_LIBRARIES} ${OpenCV_LIBS} vidl) 

add_executable(itkVideoFilePipelineTest ${itkVideoFilePipelineTest_SRCS})
target_link_libraries(itkVideoFilePipelineTest ${ITK_LIBRARIES} ${OpenCV_LIBS} vidl)

add_executable(itkMotionTrackerTest ${itkVMotionTracker_SRCS})
target_link_libraries(itkMotionTrackerTest ${ITK_LIBRARIES} ${OpenCV_LIBS} vidl) 

add_executable(itkTransformDICOMToVideo ${itkVTransformDICOMToVideo_SRCS})
target_link_libraries(itkTransformDICOMToVideo ${ITK_LIBRARIES} ${OpenCV_LIBS} vidl)

add_executable(itkVideoFileReaderExample ${itkVideoFileReaderExample_SRCS})
target_link_libraries(itkVideoFileReaderExample ${ITK_LIBRARIES} ${OpenCV_LIBS} vidl)

add_test(test_pipeline itkVideoFilePipelineTest )
add_test(test_face_detection itkFaceDetectionFilterTest)
add_test(test_writer itkTransformDICOMToVideo)
add_test(test_reader itkVideoFileReaderExample)


