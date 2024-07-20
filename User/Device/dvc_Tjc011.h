#include "drv_uart.h"
/**
 * @brief 源数据
 *
 */
struct Struct_Tjc011_UART_Data
{
    uint8_t header;
    uint8_t len;
    char data[18];
} __attribute__((packed));

class Class_Tjc011
{
public:
    void Init(UART_HandleTypeDef *huart);
    void UART_RxCpltCallback(uint8_t *Rx_Data);
    inline char * Get_Input_Number(void);

    uint8_t Input_data_Flag=0;

protected:
    void Data_Process();

    char input_number[4];

    Struct_UART_Manage_Object *UART_Manage_Object;
    Struct_Tjc011_UART_Data Now_UART_Rx_Data;
};

char * Class_Tjc011::Get_Input_Number()
{
    return (input_number);
}
