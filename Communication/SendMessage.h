#ifndef __SENDMESSAGE_H__
#define __SENDMESSAGE_H__

#include "main.h"

#define BEGIN_COMD      0XEE
#define SEND_INTCOMD    0XB107
#define SEND_FLOATCOMD  0XB107
#define SEND_STRINGCOMD 0XB110
#define SEND_GRAPCOMD   0XB132
#define END_COMD        0XFFFCFFFF


void SendTextInt32(uint16_t screen_id,uint16_t control_id,uint32_t value,uint8_t sign,uint8_t fill_zero);
void SendTextString(uint16_t screen_id,uint16_t control_id,char* str);
void SendGrapDate(uint16_t screen_id,uint16_t control_id,uint8_t channel,uint8_t *pData,uint16_t nDataLen);
void SendTextFloat(uint16_t screen_id,uint16_t control_id,float value,uint8_t precision,uint8_t show_zero);
#endif // __SENDMESSAGE_H__



