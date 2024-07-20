#include "drv_uart.h"
/**
 * @brief 源数据
 *
 */
struct Struct_ER08_UART_Data
{
    uint8_t x;
    uint8_t y;
    uint8_t sign1;
    char tel[11];
    uint8_t sign2;
    char code[4];
} __attribute__((packed));

class Class_ER08
{
public:
    void Init(UART_HandleTypeDef *huart);
    void UART_RxCpltCallback(uint8_t *Rx_Data);
    inline uint8_t Get_x(void);
    inline uint8_t Get_y(void);
    inline char *Get_tel(void);
    inline char *Get_code(void);
    Struct_UART_Manage_Object *UART_Manage_Object;
    int8_t Updata_Flag=0;

protected:
    // 绑定的UART


    Struct_ER08_UART_Data Now_UART_Rx_Data;
    Struct_ER08_UART_Data Pre_UART_Rx_Data;

    uint8_t x;
    uint8_t y;
    
    char tel[11];
    char code[4];
    void Data_Process();
};


uint8_t Class_ER08::Get_x()
{
    return (x);
}


uint8_t Class_ER08::Get_y()
{
    return (y);
}

char *Class_ER08::Get_tel()
{
    return (tel);
}


char *Class_ER08::Get_code()
{
    return (code);
}

