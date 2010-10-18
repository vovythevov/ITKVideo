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
add_test (ITKVideoTestMotion itkMotionTrackerTest ./Testing/Data/inde-circulation.avi ./Testing/Results/Motion_tracked_inde-circulation.avi 1 1 )
 
 #Reader : 
  #with opencv
 add_test(ITKVideoTestReaderOpenCV itkVideoFileReaderExample ./Testing/Data/inde-circulation.avi ./Testing/Results/VideoFileReaderExample_results/OpenCVFrame_Number_ 1)
  #with VXL
 add_test(ITKVideoTestReaderVXL itkVideoFileReaderExample ./Testing/Data/inde-circulation.avi ./Testing/Results/VideoFileReaderExample_results/VXLFrame_Number_ 0)
 
#Pipeline :
  #with reader && writer opencv
add_test(ITKVideoTestPipeline_ReadOpenCV_WriteOpenCV itkVideoFilePipelineTest ./Testing/Data/inde-circulation.avi ./Testing/Results/inde_circulation_ReadOpenCV_WriteOpenCV.avi 1 1)  
  #with opencv reader and VXL writer
add_test(ITKVideoTestPipeline_ReadOpenCV_WriteVXL itkVideoFilePipelineTest ./Testing/Data/inde-circulation.avi ./Testing/Results/inde_circulation_ReadOpenCV_WriteVXL.avi 1 0)
  #with VXL reader and OpenCV writer
add_test(ITKVideoTestPipeline_ReadVXL_WriteVXL itkVideoFilePipelineTest ./Testing/Data/inde-circulation.avi ./Testing/Results/inde_circulation_ReadVXL_WriteVXL.avi 0 0)
  #with VXL reader and writer
add_test(ITKVideoTestPipeline_ReadVXL_WriteOpenCV itkVideoFilePipelineTest ./Testing/Data/inde-circulation.avi ./Testing/Results/inde_circulation_ReadVXL_WriteOpenCV.avi 0 1)  

#Writer :
  #with openCV
add_test(ITKVideoTestWriter_OpenCV itkTransformDICOMToVideo ./Testing/Data/CTHeadAxialDicom/CTHead ./Testing/Results/DicomImageAsAVideoWithOpenCV.avi 1 1 93)
  #with VXL
add_test(ITKVideoTestWriter_VXL itkTransformDICOMToVideo ./Testing/Data/CTHeadAxialDicom/CTHead ./Testing/Results/DicomImageAsAVideoWithVXL.avi 0 1 93)

#Face detection :
add_test(ITKVideoTestFaceDetection itkFaceDetectionFilterTest ./Testing/Data/faces3.png ./Testing/FaceDetectionFilterResult.png ./Testing/Data/haarcascade_frontalface_alt2.xml)
