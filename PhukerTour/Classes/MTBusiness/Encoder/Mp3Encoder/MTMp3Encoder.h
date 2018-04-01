//
//  MTMp3Encoder.hpp
//  PhukerTour
//
//  Created by 王智明 on 2018/3/31.
//  Copyright © 2018年 王智明. All rights reserved.
//

#ifndef MTMp3Encoder_hpp
#define MTMp3Encoder_hpp

#include <stdio.h>
#include "lame.h"

class MTMp3Encoder {
private:
    FILE* pcmFile;
    FILE* mp3File;
    lame_t lameClient;
    
public:
    MTMp3Encoder();
    
    ~MTMp3Encoder();
    
    /**
     初始化方法

     @param pcmFilePath pcm文件路径
     @param mp3FilePath mp3文件路径
     @param sampleRate 采样率
     @param channels 声道数
     @param bitRate 比特率
     @return 是否调用接口成功
     */
    int Init(const char *pcmFilePath, const char *mp3FilePath, int sampleRate, int channels, int bitRate);
    /**解码*/
    void Encode();
    /**销毁资源*/
    void Destory();
};


#endif /* MTMp3Encoder_hpp */
