#ifndef LSC_AIRTHMETIC_H
#define LSC_AIRTHMETIC_H
#include "main.h"

#define SAMPLING_NUM        40
#define SAMPLING_CHANNEL    1
#define Ab_SAMPLING_NUM     BUFFER_SIZE - SAMPLING_NUM




#define ZERO_POINT          HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4)

void LSC_Airthmeitc(void);
void Vlotage_Resume(void);
#endif // !LSC_AIRTHMETIC_H

