project(ffmpeg_encode)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
MESSAGE(STATUS "#########################")
SET(CMAKE_BUILD_TYPE Release)
ADD_DEFINITIONS(    -g -Wall )

SET(EXTRA_INC_DIR D:/dep)

INCLUDE_DIRECTORIES(
      #  /usr/local/include
        ${EXTRA_INC_DIR}/ffmpeg/include
        
)




#SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/linux )
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/linux)

LINK_DIRECTORIES(
      #  /usr/local/lib
       # /usr/local/lib64
       # /usr/lib64/mysql/
       # /usr/lib/x86_64-linux-gnu
        ${PROJECT_SOURCE_DIR}/linux
        ${EXTRA_INC_DIR}/ffmpeg/lib 
)

file(GLOB ALL_CXX_SRCS
        main.cpp 
        )

add_executable(${PROJECT_NAME} ${ALL_CXX_SRCS})
 

set(ALL_LIB  	 
        avcodec.lib 
        avdevice.lib 
        avfilter.lib 
        avformat.lib 
        avutil.lib 
        swresample.lib 
        swscale.lib 
        )


target_link_libraries(${PROJECT_NAME} ${ALL_LIB})
