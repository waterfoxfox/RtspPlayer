

---

# 极简低延迟RTSP播放(内置编解码) WIN SDK


本RTSP播放SDK，支持特性：
* 1、	支持RTSP TCP拉流，支持掉线自动重连，支持RTSP状态反馈。
* 2、	内置H264、H265、AAC解码，对外输出YUV420、PCM数据。
* 3、	支持指定输出音频采样率、声道数，内置resample功能。
* 4、	支持内置硬解码渲染一体模式，此时外层仅需传入窗口句柄。
* 5、	支持JitterBuff缓存时间可设置，支持极低延迟模式、支持丢包冻结机制。
* 6、	支持TS文件录制。
* 7、   支持多实例，单个DLL、极简SDK接口。
* 8、	C++开发，性能强劲，无外部库依赖，支持32位、64位系统。



---
# API接口
 所有API接口定义均位于SDRtspPlaySdk.h文件中。

### 1、系统环境初始化，仅需调用一次
```c
void  SDRtspPlay_Enviroment_Init(const char * outputPath,  int outputLevel)
参数：
@param: outputPath：日志文件输出的目录，若目录不存在，SDK将自动创建，支持相对路径或绝对路径。日志对于问题定位非常重要，建议开启。
@param: outputLevel：日志输出的级别，只有等于或者高于该级别的日志会输出到文件，日志级别有： DEBUG、INFO、WARNING、ERROR、ALARM、FATAL、NONE，当指定为NONE时，将不会生成日志文件。具体见TERMINAL_LOG_OUTPUT_LEVEL定义。
```

### 2、系统退出时调用一次反初始化
```c
void  SDRtspPlay_Enviroment_Free ()
```

### 3、创建SDK对象
```c
void*  SDRtspPlay_New(RtspPullRenderParams* ptRenderParams, RtspRemoteVideoYuvDataCallback pfOutputVideo, RtspRemoteAudioPcmDataCallback pfOutputAudio, RtspSystemStatusCallback pfSysStatus, void* pObject, unsigned int unIndex);
参数：
@param ptRenderParams: 渲染相关参数
@param pfOutputVideo: 解码后视频输出回调接口
@param pfOutputAudio: 解码后音频输出回调接口
@param pfSysStatus: 系统状态输出回调接口
@param pObject: 上述输出回调接口的透传指针，将通过回调函数形参方式透传外层(一般用于避免全局变量的使用)
@param unIndex: 上述输出回调接口的透传变量，将通过回调函数形参方式透传外层(一般用于多路标识区分)
返回值：对象指针，返回NULL表示失败。
```

### 4、销毁SDK对象
```c
void  SDRtspPlay_Delete(void** ppTerminal);
参数：
@ ppTerminal，模块指针的指针
说明：使用者应该做好与其他API之间的互斥保护
返回值：无

```

### 5、开始接收音视频
```c
BOOL  SDRtspPlay_Online(void* pTerminal, RtspPullLogonParams* ptLogonParams, RtspPullTransParams* ptTransParams);
参数：
@param pTerminal: 模块指针
@param ptLogonParams: 登录相关参数
@param ptTransParams: 传输相关参数
返回值：
返回TRUE表示成功，返回FALSE则为失败。

```

### 6、停止接收音视频
```c
void  SDRtspPlay_Offline(void* pTerminal);
参数：
@ pTerminal，模块指针
返回值：无

```


### 7、获取当前SDK版本信息
```c
unsigned int SDRtspPlay_GetVersion (void* pTerminal);
参数：
@ pTerminal，模块指针
返回值：获得当前SDK的版本信息

```


---

# 相关资源

SDK 商用及定制化、技术支持服务可联系：[http://www.mediapro.cc/](http://www.mediapro.cc/)

