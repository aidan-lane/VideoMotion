#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"

int main(int argc, char **argv) {

    if(argc < 2) { 
        printf("Please specifiy input file!\n");
        return -1; 
    }

    av_register_all();

    //load file info
    AVFormatContext *pFormatCtx = NULL;
    if(avformat_open_input(&pFormatCtx, argv[1], NULL, 0) != 0) {
        printf("Could not open specified file!\n");
        return -1;
    }

    //load stream information
    if(avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        printf("Could not load stream information!\n");
        return -1;
    }

    //dump info
    av_dump_format(pFormatCtx, 0, argv[1], 0);

    int i = 0;
    AVCodecContext *pCodecCtxOrig = NULL;
    AVCodecContext *pCodecCtx = NULL;

    int videoStream  = -1;
    for(i = 0; i < pFormatCtx->nb_streams; i++) {
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }
    if(videoStream == -1) {
        printf("Could not find video stream!\n");
        return -1;
    }
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;

    AVCodec *pCodec = NULL;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec == NULL) {
        printf("Unsupported Codec!\n");
        return -1;
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
        printf("Could not copy codex context!\n");
        return -1;
    }

    //open codec
    if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec\n");
    }

    return EXIT_SUCCESS;
}