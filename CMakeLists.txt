cmake_minimum_required(VERSION 2.8)
project(ITKVideo)

enable_testing ()
include (CTest)
   
set(openCVImageFilters_SRCS itkFaceDetectionFilterTest.cxx )
set(itkVideoFilePipelineTest_SRCS itkVideoFilePipelineTest.cxx )
set(itkVMotionTracker_SRCS itkMotionTrackerTest.cxx )
set(itkVTransformDICOMToVideo_SRCS itkTransformDICOMToVideo.cxx )
set(itkVideoFileReaderExample_SRCS itkVideoFileReaderExample.cxx )

#Only for creating video examples and other stuff
set(itkVideoDraft_SRCS  itkVideoDraft.cxx)

#Since the find packages doesn't work (at all)
set(OpenCV_DIR "C:/projects/ITK-A2D2/opencv_build/")
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

#Motion tracker :
add_test (ITKVideoTestMotion itkMotionTrackerTest 
  #Input :
  ${CMAKE_CURRENT_SOURCE_DIR}/Data/inde-circulation.avi
  #Output :
  ${CMAKE_CURRENT_BINARY_DIR}/Testing/Motion_tracked_inde-circulation.avi
  #Reader use OpenCV :
  1
  #Writer use OpenCV :
  1 
  )
 
#Reader : 
 #with opencv
 add_test(ITKVideoTestReaderOpenCV itkVideoFileReaderExample
    #Input :
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/inde-circulation.avi 
    #Output whithout extension :
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/VideoFileReaderExample_results/OpenCVFrame_Number_ 
    #Reader Use openCV :
    1)
  #with VXL
 add_test(ITKVideoTestReaderVXL itkVideoFileReaderExample 
    #Input :
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/inde-circulation.avi 
    #Output whithout extension :
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/VideoFileReaderExample_results/VXLFrame_Number_ 
    #Reader Use OpenCV
    0)
 
#Pipeline :

  #with reader && writer opencv
add_test(ITKVideoTestPipelineReadOpenCVWriteOpenCV itkVideoFilePipelineTest
    #Input :
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/inde-circulation.avi
    #Output :
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/inde_circulation_ReadOpenCV_WriteOpenCV.avi 
    #Reader Use OpenCV
    1
    #Writer Use OpenCV
    1)  
    
  #with opencv reader and VXL writer
add_test(ITKVideoTestPipelineReadOpenCVWriteVXL itkVideoFilePipelineTest 
    #Input :
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/inde-circulation.avi 
    #Output :
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/inde_circulation_ReadOpenCV_WriteVXL.avi 
    #Reader Use OpenCV
    1
    #Writer Use OpenCV
    0)
    
  #with VXL reader and OpenCV writer
add_test(ITKVideoTestPipelineReadVXLWriteVXL itkVideoFilePipelineTest 
    #Input
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/inde-circulation.avi 
    #Output
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/Results/inde_circulation_ReadVXL_WriteVXL.avi 
    #Reader Use OpenCV
    0 
    #Writer Use OpenCV
    0)
    
  #with VXL reader and writer
add_test(ITKVideoTestPipelineReadVXLWriteOpenCV itkVideoFilePipelineTest
    #Input
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/inde-circulation.avi 
    #Output
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/inde_circulation_ReadVXL_WriteOpenCV.avi
    #Reader Use OpenCV
    0
    #Writer Use OpenCV
    1)  

#Writer :
  #with openCV
add_test(ITKVideoTestWriterOpenCV itkTransformDICOMToVideo
    #Input whithout extension
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/CTHeadAxialDicom/CTHead
    #Output
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/DicomImageAsAVideoWithOpenCV.avi
    #Writer Use OpenCV
    1
    #First Image
    1
    #Last Image
    93)
  #with VXL
add_test(ITKVideoTestWriterVXL itkTransformDICOMToVideo
    #Input whithout extension
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/CTHeadAxialDicom/CTHead
    #Output
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/DicomImageAsAVideoWithVXL.avi 
    #Writer Use OpenCV
    0
    #First Image
    1
    #Second Image
    93)

#Face detection :
add_test(ITKVideoTestFaceDetection itkFaceDetectionFilterTest
    #Input
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/faces3.png
    #Main Output
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/FaceDetectionFilterResult.png 
    #Faces whithout extension :
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/Face_ 
    #Trainer
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/haarcascade_frontalface_alt2.xml)
