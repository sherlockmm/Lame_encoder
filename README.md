# 交叉编译原理

* 本机编译

  * 首先要有源码，知道最终运行该二进制程序的机器是哪一个（本机），需要编译器、链接器，对于C或者C++,我们使用是gcc和g++。总的来说使用本机器的编译器，将源码编译链接成为一个可以在本机上运行的程序，为本机编译（Native Compilation）

* 交叉编译

  * 在一个平台上生成另外一个平台（Android ,iOS或者其他嵌入式设备）的可执行代码，这里的编译器为交叉工具编译链。当然我们要编译，这里不能在iOS设备或者Android设备上进行编译，尽管现在这两种移动设备的运算性能已经越来越强大，但是在这种环境下编译是不可能的，主要编译工具和整个过程是异常繁琐，所以直接在ARM平台下进行本机编译是不可能的。

     — 一般交叉编译都会提供一下几个工具（无论是PC ,Android ，iOS上的交叉编译工具）

    * CC:编译器，对C源文件进行编译处理，生成汇编文件。
    * AS:将汇编文件生成目标文件（汇编文件使用的是指令助记符，AS将它翻译成机器码）。
    * AR:打包器，用于库操作，可以通过该工具从一个库删除或者增加目标代码模块。
    * LD:链接器，为前面生成的目标代码分配地址空间，将多个文件链接成一个库或者可执行文件
    * GDB：调试工具，可以对运行过程中的程序进行代码调试工作。
    * STRIP:以最终生成的可执行文件或者库文件作为输入，然后消除掉其中的源码。
    * NM:查看静态库文件中的符号表
    * Objdump:查看静态库或者动态库的方法签名

    ```
                                      编译器对比
     编译过成：
     编译：gcc-c main.cpp ./libmad/mad_decoder.cpp -I./libmad/include
     打包：ar cr ../perbuilt/libmedia.a mad_decoder.o 为编译产生的.o文件
     连接：g++ -o main main.o -L ../perbuilt -l mdedia
     **
      交叉编译工具中会提供这些命令行工具
    ```

    ​

# iOS中进行交叉编译

#### 首先介绍一下在iOS 平台上CPU的指令集

* armv6:iPhone 、iPhone 2、iPhone 3G

* armv7:iPhone4、iPhone 4s

* armv7s:iPhone 5、iPhone 5s

* arm64:iPhone 5s 、iPhone 6（p）、iPhone 6s(p) 、iPhone 7（p）

  ** 这些指令集都是向下兼容支持的

  * iOS中可以查看mach-o文件来看支持的指令集。
  * 在iOS 开发中一般Debug下面会设置 Build Active Achitectures 为YES 在release下设置为NO,提高在Debug下面快速高效调试，release下保证在每个平台下面高效运行。
  * Valid achitectures 对应的指令集为armv7、armv7s、arm64 在release下面会生成对应指令集的代码，导致ipa安装包体积增大。

* 基于上面设备和指令集对比在iOS 平台中，交叉编译过程中只编译armv7 arm64

  ##### LAME的交叉编译

  * LAME简介：LAME是目前非常优秀的一种MP3编码引擎，当达到320Kbit/s（比特率：用于衡量音频数据单位时间内的容量大小 —比特率=采样率* 量化格式* 声道数）以上时，LAME编码出来的音频文件质量几乎可以和CD的音质媲美，而且所占的体积非常小。

* [LAME3.100下载链接](https://sourceforge.net/projects/lame/files/lame/3.100/)

* 脚本编写用于编译armv7 用来支持iPhone 5s一下的设备支持

  ```
  ./configure \
  --disable-shared \
  --disable-frontend \
  --host=arm-apple-darwin \
  --prefix="/Users/wangzhiming/Desktop/thin/armv7" \
  CC="xcrun -sdk iphoneos clang -arch armv7" \
  CFLAGS="-arch armv7 -fembed-bitcode -miphoneos-version-min=8.0" \
  LDFLAGS="-arch armv7 -fembed-bitcode -miphoneos-version-min=8.0"
  make clean
  make -j8
  make install
  ```

  *注意*

  * 这里下载好LAME3.100 cd 到该目录下提示没有权限

    <img src="![uild_armv7权](/Users/wangzhiming/Documents/build_armv7权限.png)">

    * 执行chmod 777 build_armv7.sh

  * 等执行完脚本在上面填写的路径prefix中就会看到相应的文件

    <img src="![ucces](/Users/wangzhiming/Documents/success.png)">

* 接下来编译arm64指令集支持(只需将上面shell脚本中的指令集换成arm64)

  ```
  ./configure \
  --disable-shared \
  --disable-frontend \
  --host=arm-apple-darwin \
  --prefix="/Users/wangzhiming/Desktop/thin/arm64" \
  CC="xcrun -sdk iphoneos clang -arch arm64" \
  CFLAGS="-arch arm64 -fembed-bitcode -miphoneos-version-min=8.0" \
  LDFLAGS="-arch arm64 -fembed-bitcode -miphoneos-version-min=8.0"
  make clean
  make -j8
  make install
  ```

  * 最终编译完成后的目录

    <img src = "![4succes](/Users/wangzhiming/Documents/64success.png)">

    ​

* 将armv7和arm64合并

  * cd 到最终编译完成的目录中

  * 执行

    ```
    lipo -create ./arm64/lib/libmp3lame.a ./armv7/lib/libmp3lame.a -output libmp3lame.a
    ```

  * 最终文件 libmp3lame.a 包含armv7和arm64

    <img src="![ibmp3lam](/Users/wangzhiming/Documents/libmp3lame.png)">

  * 在终端查看libmp3lame.a 信息  — file libmp3lame.a

* 集成到iOS 工程中