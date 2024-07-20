#include "dvc_Tjc011.h"
#include "string.h"

void Class_Tjc011::Init(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        UART_Manage_Object = &UART1_Manage_Object;
    }
    else if (huart->Instance == USART2)
    {
        UART_Manage_Object = &UART2_Manage_Object;
    }
    else if (huart->Instance == USART3)
    {
        UART_Manage_Object = &UART3_Manage_Object;
    }
    else if (huart->Instance == UART4)
    {
        UART_Manage_Object = &UART4_Manage_Object;
    }
    else if (huart->Instance == UART5)
    {
        UART_Manage_Object = &UART5_Manage_Object;
    }
    else if (huart->Instance == USART6)
    {
        UART_Manage_Object = &UART6_Manage_Object;
    }
    //    else if (huart->Instance == UART7)
    //    {
    //        UART_Manage_Object = &UART7_Manage_Object;
    //    }
    //    else if (huart->Instance == UART8)
    //    {
    //        UART_Manage_Object = &UART8_Manage_Object;
    //    }
	//input_number="0000";
}

uint16_t a;
/**
 * @brief 数据处理过程
 *
 */
void Class_Tjc011::Data_Process()
{
    // 获取当前原始值数据
    memcpy(&Now_UART_Rx_Data, UART_Manage_Object->Rx_Buffer, 20 * sizeof(uint8_t));
    // 数据处理过程
    Struct_Tjc011_UART_Data *tmp_buffer = (Struct_Tjc011_UART_Data *)UART_Manage_Object->Rx_Buffer;
    // 解析数据
    if (tmp_buffer->header == 0xFF)
    {
        uint8_t data_length = tmp_buffer->len;
        memcpy(input_number, tmp_buffer->data, data_length);
        //input_number[data_length] = '\0';
        a = strlen(input_number);
    }
    memcpy(UART_Manage_Object->Rx_Buffer,0,20 * sizeof(uint8_t));

    Input_data_Flag = 1;
}

/**
 * @brief UART通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Tjc011::UART_RxCpltCallback(uint8_t *Rx_Data)
{

    Data_Process();
}