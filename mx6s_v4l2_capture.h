#ifndef __MX6S_V4L2_CAPTURE_H__
#define __MX6S_V4L2_CAPTURE_H__

/*****************************************************************
Function:
Description: 打开视频
Parameter:
Return: 
		0,successful
		-1,failed
Date:
	[2016/6/20]  wangxw add;
	
**********************************************************************/

int HWE_OpenVideo();

/*****************************************************************
Function:
Description: 关闭视频
Parameter:
Return:
		0,successful
		-1,failed
Date:
	[2016/6/20]  wangxw add;
	
**********************************************************************/

int HWE_CloseVideo();


/*****************************************************************
Function:
Description: 获取视频数据
Parameter:
	[in,out] data,用来存储数据的空间
	[in]  size,空间大小
	[in] enRGBType,获取视频的视频数据需要转换成该RGB类型，
		V4L2_PIX_FMT_RGB_CUSTOM 不要转换，客户根据需要自己转换；
Return:
Date:
	[2016/6/20]  wangxw add;
	
**********************************************************************/
 
typedef enum V4L2_PIX_FMT_CSC_RGB_TYPE_EN
{
    YUYV_CSC_RGB,
    YUYV_CSC_RGB888,
    UYVY_CSC_RGB565,
    YUV8888_CSC_RGB565,
    V4L2_PIX_FMT_CSC_RGB_CUSTOM,
    V4L2_PIX_FMT_CSC_BUTT
}V4L2_PIX_FMT_CSC_RGB_TYPE_E;


int HWE_GetVideoData(unsigned char *data,int size,V4L2_PIX_FMT_CSC_RGB_TYPE_E enRGBType);

#endif
