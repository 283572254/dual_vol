#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "main.h"
#include "stdbool.h"

#define DACAI_BUFFER_SIZE 16

typedef struct 
{
    
    bool isGetData;
    bool isParseData;

    bool  head;
    uint16_t  send_frame;
    uint16_t  receive_frame;
    uint16_t  screenID;
    uint16_t  widgetID;
    uint8_t  widgetType;
    uint8_t DACAI_Buffer[DACAI_BUFFER_SIZE];
    uint8_t  tail[4];
    uint8_t  dataLen;


}_SaveData;

enum CtrlType
{
    kCtrlUnknown=0x0,
    kCtrlButton=0x10,                     //按钮
    kCtrlText,                            //文本
    kCtrlProgress,                        //进度条
    kCtrlSlider,                          //滑动条
    kCtrlMeter,                            //仪表
    kCtrlDropList,                        //下拉列表
    kCtrlAnimation,                       //动画
    kCtrlRTC,                             //时间显示
    kCtrlGraph,                           //曲线图控件
    kCtrlTable,                           //表格控件
    kCtrlMenu,                            //菜单控件
    kCtrlSelector,                        //选择控件
    kCtrlQRCode,                          //二维码
};


void DaCai_protocol(void);
#endif // PROTOCOL_H



