#ifndef GLOBAL_H
#define GLOBAL_H

#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


#define UM_C 0
#define US_C 4
#define CM_C 1
#define CS_C 5
#define SM_C 2
#define SS_C 3

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
typedef struct  
{
    float Main_Voltage;
    float Sub_Voltage;
    float Main_Signal;
    float Sub_Signal;
    float Main_Current;
    float Sub_Current;
}THRESHOLD;

typedef struct  
{
    float Main_Voltage;
    float Sub_Voltage;
    float Main_Signal;
    float Sub_Signal;
    float Main_Current;
    float Sub_Current;
}OBSERVER;


typedef struct  
{
    float Main_Voltage;
    float Sub_Voltage;
    float Main_Signal;
    float Sub_Signal;
    float Main_Current;
    float Sub_Current;
}CHEACK;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern volatile uint8_t StartFlag;
extern volatile uint8_t LSC_flag; //LSCÆô¶¯ÅÐ¾Ý
extern uint8_t PhaseFlag;   



extern uint8_t u8FFt_Sequence[6];

extern THRESHOLD Threshold;
extern OBSERVER Observer;
extern CHEACK Cheack_Flag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */
void Global_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


#endif // !GLOBAL_H

