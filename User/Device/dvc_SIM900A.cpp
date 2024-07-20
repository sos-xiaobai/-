#include "dvc_SIM900A.h"
#include "stdarg.h"
#include "stdio.h"


#include "string.h"

void unicodeToUtf8(uint32_t unicode, char *utf8)
{
    if (unicode <= 0x7F)
    {
        utf8[0] = static_cast<char>(unicode);
        utf8[1] = '\0';
    }
    else if (unicode <= 0x7FF)
    {
        utf8[0] = static_cast<char>(0xC0 | (unicode >> 6));
        utf8[1] = static_cast<char>(0x80 | (unicode & 0x3F));
        utf8[2] = '\0';
    }
    else if (unicode <= 0xFFFF)
    {
        utf8[0] = static_cast<char>(0xE0 | (unicode >> 12));
        utf8[1] = static_cast<char>(0x80 | ((unicode >> 6) & 0x3F));
        utf8[2] = static_cast<char>(0x80 | (unicode & 0x3F));
        utf8[3] = '\0';
    }
    else if (unicode <= 0x10FFFF)
    {
        utf8[0] = static_cast<char>(0xF0 | (unicode >> 18));
        utf8[1] = static_cast<char>(0x80 | ((unicode >> 12) & 0x3F));
        utf8[2] = static_cast<char>(0x80 | ((unicode >> 6) & 0x3F));
        utf8[3] = static_cast<char>(0x80 | (unicode & 0x3F));
        utf8[4] = '\0';
    }
}
// 将UTF-8编码的字符转换为Unicode字符
uint32_t utf8ToUnicode(const char *utf8)
{
    uint32_t unicode = 0;
    if ((utf8[0] & 0xF0) == 0xF0)
    {
        unicode = (utf8[0] & 0x07) << 18;
        unicode |= (utf8[1] & 0x3F) << 12;
        unicode |= (utf8[2] & 0x3F) << 6;
        unicode |= (utf8[3] & 0x3F);
    }
    else if ((utf8[0] & 0xE0) == 0xE0)
    {
        unicode = (utf8[0] & 0x0F) << 12;
        unicode |= (utf8[1] & 0x3F) << 6;
        unicode |= (utf8[2] & 0x3F);
    }
    else if ((utf8[0] & 0xC0) == 0xC0)
    {
        unicode = (utf8[0] & 0x1F) << 6;
        unicode |= (utf8[1] & 0x3F);
    }
    else
    {
        unicode = utf8[0];
    }
    return unicode;
}

// 将1个字符转换为16进制数字
// chr:字符,0~9/A~F/a~F
// 返回值:chr对应的16进制数值
uint8_t sim900a_chr2hex(uint8_t chr)
{
    if (chr >= '0' && chr <= '9')
        return chr - '0';
    if (chr >= 'A' && chr <= 'F')
        return (chr - 'A' + 10);
    if (chr >= 'a' && chr <= 'f')
        return (chr - 'a' + 10);
    return 0;
}
// 将1个16进制数字转换为字符
// hex:16进制数字,0~15;
// 返回值:字符
uint8_t sim900a_hex2chr(uint8_t hex)
{
    if (hex <= 9)
        return hex + '0';
    if (hex >= 10 && hex <= 15)
        return (hex - 10 + 'A');
    return '0';
}

/**********************************************************************
描述: ASCII 转 unicode      比如 '1'  转成 "0031"
***********************************************************************/
void ASCII_TO_Unicode(char *ASCII, char *Unicode)
{
    int length=0;
    int i = 0;
    int j = 0;
    memset(Unicode, '\0', sizeof(Unicode));
    length = strlen(ASCII);

    for (i = 0; i < length; i++)
    {
        Unicode[j++] = '0';
        Unicode[j++] = '0';

        Unicode[j++] = (ASCII[i] / 16) + 0x30;
        Unicode[j++] = (ASCII[i] % 16) + 0x30;
    }
}

void Class_SIM900A::Sim900a_Send_Cmd(uint8_t *cmd)
{
    UART_Send_Data(&huart5, cmd, strlen((char *)cmd));
}

/**
 * @brief 发送短信
 * 
 * @param data 取件码
 * @param tel 电话号码
 */
void Class_SIM900A::Sim900a_Send_Data(char *data, char *tel)
{
    char tel_unicode[100] = {0};
    char data_unicode[100] = {0};
    char tel_1[11] = {0};
    char temp[200] = {0};
    memcpy(tel_1, tel, 11);

    ASCII_TO_Unicode(data, (char *)data_unicode);
    ASCII_TO_Unicode(tel_1, (char *)tel_unicode);

    Sim900a_Send_Cmd((uint8_t *)"AT+CMGF=1\r\n");
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"AT+CSCS=\"UCS2\"\r\n");
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"AT+CSCA?\r\n");
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"AT+CSMP=17,167,0,25\r\n");
    HAL_Delay(500);
    sprintf(temp, "AT+CMGS=\"%s\"\r\n", tel_unicode);
    Sim900a_Send_Cmd((uint8_t *)temp);
    memset(temp, 0, sizeof(temp));
    HAL_Delay(500);
    sprintf(temp, "60A876849A8C8BC17801662FFF1A%s", data_unicode);
    Sim900a_Send_Cmd((uint8_t *)temp);
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"\x1A");
}

/**
 * @brief 发送短信
 * 
 * @param tel 电话号码 第一次提醒
 */
void Class_SIM900A::Sim900a_Send_Data_Warning_First(char *data, char *tel)
{
    char tel_unicode[100] = {0};
    char data_unicode[200] = {0};
    char tel_1[11] = {0};
    char temp[200] = {0};
    memcpy(tel_1, tel, 11);

    ASCII_TO_Unicode(data, (char *)data_unicode);
    ASCII_TO_Unicode(tel_1, (char *)tel_unicode);

    Sim900a_Send_Cmd((uint8_t *)"AT+CMGF=1\r\n");
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"AT+CSCS=\"UCS2\"\r\n");
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"AT+CSCA?\r\n");
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"AT+CSMP=17,167,0,25\r\n");
    HAL_Delay(500);
    sprintf(temp, "AT+CMGS=\"%s\"\r\n", tel_unicode);
    Sim900a_Send_Cmd((uint8_t *)temp);
    memset(temp, 0, sizeof(temp));
    HAL_Delay(500);
    //您的包裹已滞留，请尽快取走，取件码为
    sprintf(temp, "60A87684530588F95DF26EDE7559FF0C8BF75C3D5FEB53D68D70FF0C53D64EF678014E3AFF1A%s", data_unicode);
    Sim900a_Send_Cmd((uint8_t *)temp);
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"\x1A");    
}


/**
 * @brief 发送短信
 * 
 * @param tel 电话号码 第二次提醒
 */
void Class_SIM900A::Sim900a_Send_Data_Warning_Second(char *data,char *tel)
{
    char tel_unicode[100] = {0};
    char data_unicode[200] = {0};
    char tel_1[11] = {0};
    char temp[200] = {0};
    memcpy(tel_1, tel, 11);

    ASCII_TO_Unicode(data, (char *)data_unicode);
    ASCII_TO_Unicode(tel_1, (char *)tel_unicode);

    Sim900a_Send_Cmd((uint8_t *)"AT+CMGF=1\r\n");
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"AT+CSCS=\"UCS2\"\r\n");
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"AT+CSCA?\r\n");
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"AT+CSMP=17,167,0,25\r\n");
    HAL_Delay(500);
    sprintf(temp, "AT+CMGS=\"%s\"\r\n", tel_unicode);
    Sim900a_Send_Cmd((uint8_t *)temp);
    memset(temp, 0, sizeof(temp));
    HAL_Delay(500);
    //您的包裹已滞留，已为您产生新的取件码：
    sprintf(temp, "60A87684530588F95DF26EDE7559FF0C5DF24E3A60A84EA7751F65B0768453D64EF67801FF1A%s", data_unicode);
    Sim900a_Send_Cmd((uint8_t *)temp);
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"\x1A");    
}

/**
 * @brief 发送短信
 * 
 * @param data 取件码
 * @param tel 电话号码  更改取件码
 */
void Class_SIM900A::Sim900a_Send_Data_Change(char *data, char *tel)
{
    char tel_unicode[100] = {0};
    char data_unicode[100] = {0};
    char tel_1[11] = {0};
    char temp[200] = {0};
    memcpy(tel_1, tel, 11);

    ASCII_TO_Unicode(data, (char *)data_unicode);
    ASCII_TO_Unicode(tel_1, (char *)tel_unicode);

    Sim900a_Send_Cmd((uint8_t *)"AT+CMGF=1\r\n");
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"AT+CSCS=\"UCS2\"\r\n");
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"AT+CSCA?\r\n");
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"AT+CSMP=17,167,0,25\r\n");
    HAL_Delay(500);
    sprintf(temp, "AT+CMGS=\"%s\"\r\n", tel_unicode);
    Sim900a_Send_Cmd((uint8_t *)temp);
    memset(temp, 0, sizeof(temp));
    HAL_Delay(500);
    //已为您产生新的取件码
    sprintf(temp, "5DF24E3A60A84EA7751F65B0768453D64EF67801FF1A%s", data_unicode);
    Sim900a_Send_Cmd((uint8_t *)temp);
    HAL_Delay(500);
    Sim900a_Send_Cmd((uint8_t *)"\x1A");
}