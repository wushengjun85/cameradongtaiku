#include "camera_thread.h"
#include <QByteArray>
#include <QBuffer>
#include <QImage>
#include <QDebug>
#include "mx6s_v4l2_capture.h"
video_thread::video_thread():QThread()
{
     HWE_OpenVideo();
    quit_flag = false;
}

video_thread::~video_thread()
{

    HWE_CloseVideo();
    this->quit();
    quit_flag = true;
    this->wait();
}

void video_thread::run()
{

#if 0
    m_video.init_video();//打开视频设备
    while(!quit_flag)
    {
    m_video.get_data();//获取视频数据
    unsigned char *rgb;
    rgb = new unsigned char [m_video.buf.length*2];//根据yuyv像素点和rgb像素所站内存比分配rgb
    m_video.yuvtorgbO(rgb,640,480);//yuyv转换成RGB
    QImage img = QImage(rgb,640,480,QImage::Format_RGB888);
    emit image_data(img);
    msleep(150);
    delete[] rgb;//不能在sleep前释放掉否则显示的图片会出现问题
   }
    m_video.release_video();//在线程释放前释放设备
#else

   #define  IMAGEWIDTH    720
  #define  IMAGEHEIGHT   480

    unsigned char u8VideoFrameBuffer[IMAGEWIDTH*IMAGEHEIGHT*2];

    while(!quit_flag)
    {
    	
		// unsigned char *u8VideoFrameBuffer=new unsigned char [IMAGEWIDTH*IMAGEHEIGHT*2];
	    HWE_GetVideoData(u8VideoFrameBuffer,sizeof(u8VideoFrameBuffer),UYVY_CSC_RGB565);
        QImage img = QImage(u8VideoFrameBuffer,IMAGEWIDTH,IMAGEHEIGHT,QImage::Format_RGB16);
        //QImage img = QImage(u8VideoFrameBuffer,IMAGEWIDTH,IMAGEHEIGHT,QImage::Format_RGB32);

	    emit image_data(img);
	  // for(int nIndex=0;nIndex<5;nIndex++)
	   //   printf("[%c]\t",u8VideoFrameBuffer[nIndex]);
	 //  printf("\n=========================\n");
	   // msleep(150);
	 // delete[] u8VideoFrameBuffer;
	  // printf("22msleep 150\n");
   }
	
  //  HWE_CloseVideo();


#endif 	


}
