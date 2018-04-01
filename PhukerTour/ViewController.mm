//
//  ViewController.m
//  PhukerTour
//
//  Created by 王智明 on 2018/3/31.
//  Copyright © 2018年 王智明. All rights reserved.
//

#import "ViewController.h"
#import "MTMp3Encoder.h"

@interface ViewController ()
@end

@implementation ViewController{
    MTMp3Encoder *_mp3Encoder;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    /*1.初始化解码器**/
    MTMp3Encoder *mp3Encoder = new MTMp3Encoder();
    self->_mp3Encoder = mp3Encoder;
    
}

- (IBAction)startEncode:(id)sender {
    
    /**开始解码*/
    _mp3Encoder -> Encode();
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
