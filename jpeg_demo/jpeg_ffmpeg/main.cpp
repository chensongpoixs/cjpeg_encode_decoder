#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include <cstdio>
#include <cstdlib>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/error.h>
#include <libavutil/frame.h>
#include <libavutil/log.h>#include <libavcodec/codec.h>

#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
}
 
 
 int main(int argc, char * argv[])
 {
	 