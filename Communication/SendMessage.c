#include "../Communication/SendMessage.h"
#include "../BSP/SERIAL/serial.h"



void Dacai_Send(uint8_t *pData,uint16_t nDataLen)
{
    UART_SendData(pData,nDataLen,&huart2);
}


/**
 * @brief Sends a command to a specified screen and control.
 * 
 * This function constructs a command packet containing a command, screen ID,
 * and control ID. The packet is then sent via UART.
 * 
 * @param command The command to be sent.
 * @param screen_id The ID of the target screen.
 * @param control_id The ID of the target control on the screen.
 */
void Send_Command(uint16_t command,uint16_t screen_id,uint16_t control_id)
{
    uint8_t data[7];
    data[0] = BEGIN_COMD;
    data[1] = (command>>8)&0xff;
    data[2] = command&0xff;
    data[3] = (screen_id>>8)&0xff;
    data[4] = screen_id&0xff;
    data[5] = (control_id>>8)&0xff;
    data[6] = control_id&0xff;
    Dacai_Send(data,7);
}
/**
 * @brief Sends an end command to terminate a communication sequence.
 * 
 * This function constructs an end command packet and sends it via UART to signal
 * the end of a communication sequence.
 */
void Send_EndCommand(void)
{
    uint8_t data[4];
    data[0] = (END_COMD>>24)&0xff;
    data[1] = (END_COMD>>16)&0xff;
    data[2] = (END_COMD>>8)&0xff;
    data[3] = END_COMD&0xff;
    Dacai_Send(data,4);
}
/**
 * @brief Sends an integer value to a specified screen and control ID over UART.
 *
 * This function constructs a message containing an integer value, along with additional
 * parameters such as screen ID, control ID, sign, and zero fill flag, and sends it
 * over UART using the Dacai_Send function.
 *
 * @param screen_id   The ID of the target screen where the message will be sent.
 * @param control_id  The ID of the control on the target screen that will receive the message.
 * @param value       The 32-bit integer value to be sent.
 * @param sign        Flag indicating if the integer should be treated as signed (1) or unsigned (0).
 * @param fill_zero   Flag indicating if the integer should be zero-padded. Only the lower 4 bits are used.
 */
void SendTextInt32(uint16_t screen_id,uint16_t control_id,uint32_t value,uint8_t sign,uint8_t fill_zero)
{
    uint8_t data[11];
    Send_Command(SEND_INTCOMD,screen_id,control_id);
    data[0] = (sign?0x01:0x00);
    data[1] = (fill_zero&0x0f)|0x80;
    data[2] = (value>>24)&0xff;
    data[3] = (value>>16)&0xff;
    data[4] = (value>>8)&0xff;
    data[5] = value&0xff;
    Dacai_Send(data,10);
    Send_EndCommand();
}   

void SendTextFloat(uint16_t screen_id,uint16_t control_id,float value,uint8_t precision,uint8_t show_zero)
{
    uint8_t data[6];
    Send_Command(SEND_FLOATCOMD,screen_id,control_id);
    data[0] = 0x02;
    data[1] = (precision&0x0f)|(show_zero?0x80:0x00);
    for(int i=0;i<4;i++)
    {
            //需要区分大小端
            #if(0)
            TX_8(((uint8 *)&value)[i]);
        #else
            data[2+i] = ((uint8_t *)&value)[3-i];
        #endif
    }
    Dacai_Send(data,6);
    Send_EndCommand();
}

static void String(char *str)
{
    while(*str)
    {
        Dacai_Send(str++,1);
    }
}
/**
 * @brief Sends a string to a specified screen and control ID over UART.
 *
 * This function constructs a 7-byte message containing a command to send a string to a specific
 * screen and control ID. It sends the initial part of the message with the command and IDs, then
 * sends the string itself, followed by an end command to signify the end of the message.
 *
 * @param screen_id  The ID of the screen to which the message is sent.
 * @param control_id The ID of the control within the screen to which the message is sent.
 * @param str        The string to send.
 */
void SendTextString(uint16_t screen_id,uint16_t control_id,char* str)
{

    Send_Command(SEND_STRINGCOMD,screen_id,control_id);
    String(str);
    Send_EndCommand();

}

void SendNdata(uint8_t *pData,uint16_t nDataLen)
{
    uint16_t i ;
    for(i = 0;i<nDataLen;i++)
    {
        Dacai_Send(pData++,1);
    }
}

/**
 * @brief Sends an integer value to a specified screen and control ID over UART.
 * 
 * This function constructs a 17-byte data packet to send an integer value to a specific screen and control ID.
 * It includes a command identifier, screen ID, control ID, integer value, sign, and fill zero flag.
 * 
 * @param screen_id The ID of the screen to which the command is sent.
 * @param control_id The ID of the control on the screen to which the command is sent.
 * @param value The 32-bit integer value to be sent.
 * @param sign Indicates the sign of the integer value (0 for unsigned, 1 for signed).
 * @param fill_zero The number of leading zeros to be filled in the integer value (0-15).
 */
void SendGrapDate(uint16_t screen_id,uint16_t control_id,uint8_t channel,uint8_t *pData,uint16_t nDataLen)
{
    uint8_t data[3];
    Send_Command(SEND_GRAPCOMD,screen_id,control_id);
    data[0] = channel;
    data[1] = (nDataLen>>8)&0xff;
    data[2] = nDataLen&0xff;
    Dacai_Send(data,3);
    SendNdata(pData,nDataLen);
    Send_EndCommand();
}


