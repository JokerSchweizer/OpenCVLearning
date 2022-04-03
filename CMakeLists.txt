cmake_minimum_required(VERSION 3.21)
project(OpenCVLearn)
Set(CUR_PROJ OpenCVLearn)
set(CMAKE_CXX_STANDARD 14)

MESSAGE("PROJECT_SOURCE_DIR=" ${PROJECT_SOURCE_DIR})
#############################################################################
#step0 define variables and directories


#############################################################################
#step1 import Link Directories
# find required opencv
find_package(OpenCV REQUIRED)
# directory of opencv headers
include_directories(${OpenCV_INCLUDE_DIRS})


#############################################################################
#step2: include internal header file directory
INCLUDE_DIRECTORIES(
        ${OpenCV_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
)

#step3 Get Source Files
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src SOURCE_FILE)

#step4 Add Subdirectory

#step5 Generate Target and link libs&PCH

add_executable(OpenCVLearn main.cpp ${SOURCE_FILE})



# opencv libraries
target_link_libraries(OpenCVLearn ${OpenCV_LIBS})


#step6 PreBuild Events


#step7 Post Build Events: Copy files to output path