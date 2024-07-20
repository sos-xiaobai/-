#include "drv_uart.h"
#define swap16(x) (x & 0XFF) << 8 | (x & 0XFF00) >> 8 // 高低字节交换宏定义
typedef unsigned int UINT;

/* These types must be 8-bit integer */
typedef char CHAR;
typedef unsigned char UCHAR;
typedef unsigned char BYTE;

/* These types must be 16-bit integer */
typedef short SHORT;
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned short WCHAR;
class Class_SIM900A
{
public:
    void Init();
    
    void Sim900a_Send_Cmd(uint8_t *cmd);
    void Sim900a_Send_Data(char *data,  char *tel);
    void Sim900a_Send_Data_Warning_First(char *data, char *tel);
    void Sim900a_Send_Data_Warning_Second(char *data, char *tel);
    void Sim900a_Send_Data_Change(char *tel, char *data);
    void UART_RxCpltCallback(uint8_t *Rx_Data);
Struct_UART_Manage_Object *UART_Manage_Object;
protected:
    char * Pickup_Code[4];
};