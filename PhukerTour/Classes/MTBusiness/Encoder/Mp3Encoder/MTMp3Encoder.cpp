//
//  MTMp3Encoder.cpp
//  PhukerTour
//
//  Created by 王智明 on 2018/3/31.
//  Copyright © 2018年 王智明. All rights reserved.
//

#include "MTMp3Encoder.h"

MTMp3Encoder::MTMp3Encoder() {
    
    printf("-------解码器初始化---------\n");
}

MTMp3Encoder::~MTMp3Encoder(){
    
}

int MTMp3Encoder::Init(const char *pcmFilePath, const char *mp3FilePath, int sampleRate, int channels, int bitRate){
 
    int result = -1;
    pcmFile = fopen(pcmFilePath, "rb");
    if (pcmFile) {
        mp3File = fopen(mp3FilePath, "wb");
        if (mp3File) {
            //初始化编码器
            lameClient = lame_init();
            lame_set_in_samplerate(lameClient, sampleRate);
            lame_set_out_samplerate(lameClient, sampleRate);
            lame_set_num_channels(lameClient, channels);
            lame_set_brate(lameClient, bitRate);
            lame_init_params(lameClient);
            result = 0;
        }
    }

    return result;
}

void MTMp3Encoder::Encode() {
    printf("-------解码器开始解码-------\n");
    
    int bufferSize = 1024 * 256;
    short *buffer = new short[bufferSize/2];
    short *leftBuffer = new short[bufferSize/4];
    short *rightBuffer = new short[bufferSize/4];
    unsigned char *mp3_buffer = new unsigned char[bufferSize];
    size_t readBufferSize = 0;//从pcm文件中读到的buffer大小
    while ((readBufferSize = fread(buffer, 2, bufferSize/2, pcmFile))>0) {
        for (int i = 0; i<readBufferSize; i++) {
            //拆分左右声道
            if (i % 2 == 0) {
                leftBuffer[i/2] = buffer[i];
            }else{
                rightBuffer[i/2] = buffer[i];
            }
        }
        
        size_t wroteSize = lame_encode_buffer(lameClient, (short int *)leftBuffer, (short int *)rightBuffer, (int)(readBufferSize/2), mp3_buffer, bufferSize);
        //写MP3文件
        fwrite(mp3_buffer, 1, wroteSize, mp3File);
    }
    //释放内存
    delete [] buffer;
    delete [] leftBuffer;
    delete [] rightBuffer;
    delete [] mp3_buffer;
}

void MTMp3Encoder::Destory(){
    
    if (pcmFile) {
        fclose(pcmFile);//关闭文件
    }
    
    if (mp3File) {
        fclose(mp3File);
        lame_close(lameClient);//销毁编码器
    }
    
}
