/*
本程序实现了YUV420P像素数据编码为JPEG图片。是最简单的FFmpeg编码方面的教程。
*通过学习本例子可以了解FFmpeg的编码流程。


*/
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
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
}
 
static int MyWriteJPEG(AVFrame* pFrame, int width, int height, int iIndex)
{
	// 输出文件路径  
#define MAX_PATH 32
	char out_file[MAX_PATH] = { 0 };
	sprintf_s(out_file, sizeof(out_file), "%d.jpg", iIndex);
	//av_register_all();
	// 分配AVFormatContext对象  
	AVFormatContext* pFormatCtx = avformat_alloc_context();

	// 设置输出文件格式  
	pFormatCtx->oformat = av_guess_format("mjpeg", NULL, NULL);

	// 创建并初始化一个和该url相关的AVIOContext  
	if (avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0) {
		printf("Couldn't open output file.");
		return -1;
	}

	// 构建一个新stream  
	AVStream* pAVStream = avformat_new_stream(pFormatCtx, 0);
	if (pAVStream == NULL) {
		return -1;
	}

	// 设置该stream的信息  
	//AVCodecContext* pCodecCtx = pAVStream->codec; 
	// Get a pointer to the codec context for the video stream  
	// 得到视频流编码的上下文指针  
	AVCodecContext* pCodecCtx = avcodec_alloc_context3(NULL);
	if (pCodecCtx == NULL)
	{
		printf("Could not allocate AVCodecContext\n");
		return -1;
	}
	avcodec_parameters_to_context(pCodecCtx, pAVStream->codecpar);


	pCodecCtx->codec_id = pFormatCtx->oformat->video_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;
	pCodecCtx->width = width;
	pCodecCtx->height = height;
	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = 25;

	// Begin Output some information  
	av_dump_format(pFormatCtx, 0, out_file, 1);
	// End Output some information  

	// 查找解码器  
	const AVCodec* pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec) {
		printf("Codec not found.");
		return -1;
	}
	// 设置pCodecCtx的解码器为pCodec  
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		printf("Could not open codec.");
		return -1;
	}

	//Write Header  
	avformat_write_header(pFormatCtx, NULL);

	int y_size = pCodecCtx->width * pCodecCtx->height;

	//Encode  
	// 给AVPacket分配足够大的空间  
	AVPacket pkt;
	av_new_packet(&pkt, y_size * 3);

	//   
	int got_picture = 0;
	//int ret = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_picture);  
	//编码该帧  
	//avcodec_send_packet(pCodecCtx, &pkt);
	//int ret = avcodec_receive_frame(pCodecCtx, pFrame); 
	avcodec_send_frame(pCodecCtx, pFrame);
	int ret = avcodec_receive_packet(pCodecCtx, &pkt);

	//if( got_picture == 1 ) {  
	//pkt.stream_index = pAVStream->index;  
	ret = av_write_frame(pFormatCtx, &pkt);
	//}  
	av_packet_unref(&pkt);

	//Write Trailer  
	av_write_trailer(pFormatCtx);

	printf("Encode Successful.\n");

	if (pAVStream) {
		avcodec_close(pCodecCtx);
		av_free(pFrame);
	}
	avio_close(pFormatCtx->pb);
	avformat_free_context(pFormatCtx);

	return 0;
}
int main(int argc, char* argv[])
{
	/*AVFormatContext* pFormatCtx = NULL;
	const char* out_file = "cuc_view_encode.jpg";    //Output file
	if (avio_open(&pFormatCtx->pb,out_file, AVIO_FLAG_READ_WRITE) < 0){
	printf("Couldn't open output file.");
	return -1;
	}*/
	/*FILE *in_file = fopen("cuc_view_480x272.yuv", "rb");	//视频YUV源文件
	//读入YUV
	AVFrame* picture=NULL;
	uint8_t* picture_buf1=new uint8_t[1024];
	int y_size = 480*272;
	if (fread(picture_buf1, 1, y_size*3/2, in_file) < 0)
	{
	printf("文件读取错误");
	return -1;
	}
	convertYUVToJPG(picture_buf1);*/

	AVFormatContext* pFormatCtx = NULL;
	//AVOutputFormat* fmt = NULL;
	AVStream* video_st = NULL;
	AVCodecContext* pCodecCtx = NULL;
//	AVCodec* pCodec = NULL;

	uint8_t* picture_buf = NULL;
	AVFrame* picture = NULL;
	int size;

	FILE *in_file = fopen("../../../Tools/2022_10_07_729_484.yuv", "rb");	//视频YUV源文件 
															//FILE *in_file = fopen("G:\\foreman_cif.h264", "rb");	//视频YUV源文件 
	int in_w = 729, in_h = 484;									//宽高
	const char* out_file = "cuc_view_encode.jpg";					//输出文件路径

	//av_register_all();

	//方法1.组合使用几个函数
	pFormatCtx = avformat_alloc_context();
	//猜格式。用MJPEG编码
	const AVOutputFormat*	fmt = av_guess_format("mjpeg", NULL, NULL);
	pFormatCtx->oformat = fmt;
	//注意：输出路径
	if (avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0)
	{
		printf("输出文件打开失败");
		return -1;
	}

	//方法2.更加自动化一些
	//avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file);
	//fmt = pFormatCtx->oformat;

	video_st = avformat_new_stream(pFormatCtx, 0);
	if (video_st == NULL)
	{
		return -1;
	}
	//pCodecCtx = video_st->codec;
	pCodecCtx = avcodec_alloc_context3(NULL);
	if (pCodecCtx == NULL)
	{
		printf("Could not allocate AVCodecContext\n");
		return -1;
	}
	avcodec_parameters_to_context(pCodecCtx, video_st->codecpar);

	pCodecCtx->codec_id = fmt->video_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;

	pCodecCtx->width = in_w;
	pCodecCtx->height = in_h;

	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = 25;
	//输出格式信息
	av_dump_format(pFormatCtx, 0, out_file, 1);

	const AVCodec*  pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec)
	{
		printf("没有找到合适的编码器！");
		return -1;
	}
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		printf("编码器打开失败！");
		return -1;
	}
	picture = av_frame_alloc();
	size = av_image_get_buffer_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, 1);
	picture_buf = (uint8_t *)av_malloc(size);
	if (!picture_buf)
	{
		return -1;
	}
	//avpicture_fill((AVPicture *)picture, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
	av_image_fill_arrays(picture->data, picture->linesize, picture_buf, AV_PIX_FMT_YUVJ420P, pCodecCtx->width, pCodecCtx->height, 1);
	//写文件头
	avformat_write_header(pFormatCtx, NULL);

	AVPacket pkt;
	int y_size = pCodecCtx->width * pCodecCtx->height;
	av_new_packet(&pkt, y_size * 3);
	//读入YUV
	if (fread(picture_buf, 1, y_size * 3 / 2, in_file) < 0)
	{
		printf("文件读取错误");
		return -1;
	}
	MyWriteJPEG(picture, pCodecCtx->width, pCodecCtx->height, 1);
	system("pause");
	return 0;
}

 