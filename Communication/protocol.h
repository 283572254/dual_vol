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
    kCtrlButton=0x10,                     //��ť
    kCtrlText,                            //�ı�
    kCtrlProgress,                        //������
    kCtrlSlider,                          //������
    kCtrlMeter,                            //�Ǳ�
    kCtrlDropList,                        //�����б�
    kCtrlAnimation,                       //����
    kCtrlRTC,                             //ʱ����ʾ
    kCtrlGraph,                           //����ͼ�ؼ�
    kCtrlTable,                           //���ؼ�
    kCtrlMenu,                            //�˵��ؼ�
    kCtrlSelector,                        //ѡ��ؼ�
    kCtrlQRCode,                          //��ά��
};


void DaCai_protocol(void);
#endif // PROTOCOL_H



