cmake_minimum_required(VERSION 3.0.0)
project(OPENCV4_DEMOS_CPP VERSION 1.0.0)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
aux_source_directory(src SOURCE_FILES)
list(REMOVE_ITEM SOURCE_FILES src/CameraCalibration.cpp
                              src/compose_images.cpp
                              src/compose_videos.cpp
                              src/watershed.cpp
                              src/face_recognition.cpp)
add_executable(OPENCV4_DEMOS_CPP ${SOURCE_FILES})
# add_executable(CameraCalibration src/CameraCalibration.cpp)
# add_executable(compose_images src/compose_images.cpp)
# add_executable(compose_videos src/compose_videos.cpp)
# add_executable(watershed src/watershed.cpp)
# add_executable(face_recognition src/face_recognition.cpp)
target_link_libraries(OPENCV4_DEMOS_CPP ${OpenCV_LIBS})
# target_link_libraries(CameraCalibration ${OpenCV_LIBS})
# target_link_libraries(compose_images ${OpenCV_LIBS})
# target_link_libraries(compose_videos ${OpenCV_LIBS})
# target_link_libraries(watershed ${OpenCV_LIBS})
# target_link_libraries(face_recognition ${OpenCV_LIBS})
