//***************************************************************************//
//* 版权所有  www.mediapro.cc
//*
//* 内容摘要：windows RTSP PLAY SDK，内置音视频编解码
//*			  
//*
//*	说    明：
//*			  
//*				
//* 当前版本：V1.0		
//* 作    者：www.mediapro.cc
//* 完成日期：2020-8-6
//**************************************************************************//

#ifndef _SD_RTSP_PLAY_SDK_H_
#define _SD_RTSP_PLAY_SDK_H_


#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__
#ifdef DLL_EXPORTS
#ifdef __GNUC__
#define DLLIMPORT_RTSP_PLAY_SDK __attribute__ ((dllexport))
#else
#define DLLIMPORT_RTSP_PLAY_SDK __declspec(dllexport) 
#endif
#else
#ifdef __GNUC__
#define DLLIMPORT_RTSP_PLAY_SDK 
#else
#define DLLIMPORT_RTSP_PLAY_SDK 
#endif
#endif
#else
#if __GNUC__ >= 4
#define DLLIMPORT_RTSP_PLAY_SDK __attribute__ ((visibility ("default")))
#else
#define DLLIMPORT_RTSP_PLAY_SDK
#endif
#endif

#ifdef WIN32
	typedef signed __int64       int64_t;
#endif

#ifdef __APPLE__
#ifndef OBJC_BOOL_DEFINED
	typedef int BOOL;
#endif 
#else
#ifndef BOOL
	typedef int BOOL;
#endif
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


//最大Jitter Buff缓存时长,ms
#define RTSP_TERMINAL_JITTER_BUFF_MAX				600

//音频采样率支持的最小值、最大值
#define	RTSP_TERMINAL_SAMPLE_RATE_MIN				8000
#define	RTSP_TERMINAL_SAMPLE_RATE_MAX				48000

//音频声道数支持的最小值、最大值
#define	RTSP_TERMINAL_AUDIO_CHANNELS_MIN			1
#define	RTSP_TERMINAL_AUDIO_CHANNELS_MAX			2


//日志输出的级别
typedef enum RTSP_PULL_LOG_OUTPUT_LEVEL
{
	RTSP_PULL_LOG_OUTPUT_LEVEL_DEBUG				= 1,
	RTSP_PULL_LOG_OUTPUT_LEVEL_INFO					= 2,
	RTSP_PULL_LOG_OUTPUT_LEVEL_WARNING				= 3,
	RTSP_PULL_LOG_OUTPUT_LEVEL_ERROR				= 4,
	RTSP_PULL_LOG_OUTPUT_LEVEL_ALARM				= 5,
	RTSP_PULL_LOG_OUTPUT_LEVEL_FATAL				= 6,
	RTSP_PULL_LOG_OUTPUT_LEVEL_NONE					= 7
} RTSP_PULL_LOG_OUTPUT_LEVEL;


//RTSP状态
typedef enum RTSP_PULL_SYS_STATUS
{
	RTSP_PULL_STATUS_STOPPED						= 0,
	RTSP_PULL_STATUS_CONNECTING						= 1,
	RTSP_PULL_STATUS_CONNFAIL						= 2,
	RTSP_PULL_STATUS_CONNSUCC						= 3,
	RTSP_PULL_STATUS_UNSUPPORT_SIGNAL				= 4,
	RTSP_PULL_STATUS_RESUME							= 5,
	RTSP_PULL_STATUS_AUTHFAILED						= 6,
	RTSP_PULL_STATUS_NODATA							= 7
} RTSP_PULL_SYS_STATUS;



typedef struct RtspPullLogonParams
{
	char acRtspUrl[1024];
	char acServerIp[128];
	char acUserName[128];
	char acPassword[128];
	unsigned short usServerPort;
}RtspPullLogonParams;


typedef struct RtspPullTransParams
{
	unsigned int unJitterBuffDelay;
	unsigned int bFreezeFrameWhenLost;
}RtspPullTransParams;


typedef struct RtspPullRenderParams
{
	unsigned int unAudioSampleRate;
	unsigned int unAudioChannelNum;
	//内置渲染模式时提供窗口句柄，此时将采用硬解码直接渲染，此时将不会对外输出YUV
	void* pWindowHandle;
	//是否保持宽高比渲染或铺满全屏
	BOOL bKeepRatio;
	//保持宽高比显示时，填充区域RGB颜色
	unsigned char ucBackR;
	unsigned char ucBackG;
	unsigned char ucBackB;
}RtspPullRenderParams;


//输出接收并解码后的YUV数据
typedef void (*RtspRemoteVideoYuvDataCallback)(unsigned char* data, unsigned int unWidth, unsigned int unHeight, unsigned int unPts, void *pObject, unsigned int unIndex);

//输出接收并解码后的PCM数据
typedef void (*RtspRemoteAudioPcmDataCallback)(unsigned char* data, unsigned int unLen, unsigned int unPts, void *pObject, unsigned int unIndex);

//输出系统状态变更通知等，具体见RTSP_PULL_SYS_STATUS定义
typedef void (*RtspSystemStatusCallback)(unsigned int unStatus, void *pObject, unsigned int unIndex);


//////////////////////////////////////////////////////////////////////////
// 客户端类封装接口

/***
* 环境初始化，系统只需调用一次，主要用于日志模块的初始化
* @param: outputPath表示日志存放路径，支持相对路径和绝对路径，若目录不存在将自动创建
* @param: outputLevel表示日志输出的级别，只有等于或者高于该级别的日志输出到文件，取值范围参考TERMINAL_LOG_OUTPUT_LEVEL
* @return: 
*/
DLLIMPORT_RTSP_PLAY_SDK void  SDRtspPlay_Enviroment_Init(const char* outputPath, int outputLevel);

DLLIMPORT_RTSP_PLAY_SDK void  SDRtspPlay_Enviroment_Free();


/***
* 创建SDTerminal
* @param ptRenderParams: 渲染相关参数
* @param pfOutputVideo: 解码后视频输出回调接口
* @param pfOutputAudio: 解码后音频输出回调接口
* @param pfSysStatus: 系统状态输出回调接口
* @param pObject: 上述输出回调接口的透传指针，将通过回调函数形参方式透传外层(一般用于避免全局变量的使用)
* @param unIndex: 上述输出回调接口的透传变量，将通过回调函数形参方式透传外层(一般用于多路标识区分)
* @return: 返回模块指针，为NULL则失败
*/
DLLIMPORT_RTSP_PLAY_SDK void*  SDRtspPlay_New(RtspPullRenderParams* ptRenderParams, RtspRemoteVideoYuvDataCallback pfOutputVideo, 
								RtspRemoteAudioPcmDataCallback pfOutputAudio, RtspSystemStatusCallback pfSysStatus, void* pObject, unsigned int unIndex);


/***
* 从服务器下线并销毁对象，使用者应该做好与其他API之间的互斥保护
* 互斥保护方法：业务保证其他API接口停止调用后，才会调用本接口
* @param ppTerminal: 模块指针指针
* @return:
*/
DLLIMPORT_RTSP_PLAY_SDK void  SDRtspPlay_Delete(void** ppTerminal);


/***
* 开始接收音视频
* @param pTerminal: 模块指针
* @param ptLogonParams: 登录相关参数
* @param ptTransParams: 传输相关参数
* @return: 接口调用是否成功
*/
DLLIMPORT_RTSP_PLAY_SDK BOOL  SDRtspPlay_Online(void* pTerminal, RtspPullLogonParams* ptLogonParams, RtspPullTransParams* ptTransParams);


/***
* 停止接收音视频
* @param pTerminal: 模块指针
* @return:
*/
DLLIMPORT_RTSP_PLAY_SDK void  SDRtspPlay_Offline(void* pTerminal);


/***
* 获取SDK版本信息
* @param pTerminal: 模块指针
* @return: SDK版本号
*/
DLLIMPORT_RTSP_PLAY_SDK unsigned int  SDRtspPlay_GetVersion(void* pTerminal);



/***
* 启动下行码流的TS文件录制,Online之后调用生效
* @param pTerminal: 模块指针
* @param strTsFileUrl: TS文件URL
* @return: 接口调用是否成功
*/
DLLIMPORT_RTSP_PLAY_SDK BOOL  SDRtspPlay_StartCapDownStream(void* pTerminal, const char* strTsFileUrl);


/***
* 停止下行码流的TS文件录制
* @param pTerminal: 模块指针
* @return: 
*/
DLLIMPORT_RTSP_PLAY_SDK void  SDRtspPlay_StopCapDownStream(void* pTerminal);



/***
* 启动下行码流的RAW文件录制,Online之后调用生效
* @param pTerminal: 模块指针
* @param strRawFileUrl: RAW文件URL
* @return: 接口调用是否成功
*/
DLLIMPORT_RTSP_PLAY_SDK BOOL  SDRtspPlay_StartCapDownRawStream(void* pTerminal, const char* strRawFileUrl);


/***
* 停止下行码流的RAW文件录制
* @param pTerminal: 模块指针
* @return: 
*/
DLLIMPORT_RTSP_PLAY_SDK void  SDRtspPlay_StopCapDownRawStream(void* pTerminal);


#ifdef __cplusplus
}
#endif

#endif // _SD_RTSP_PLAY_SDK_H_
