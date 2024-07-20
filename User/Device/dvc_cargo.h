#ifndef _DVC_CARGO_H
#define _DVC_CARGO_H

#include "main.h"
#include <string.h>

struct Struct_Cargo
{
    uint8_t Position_X;
    uint8_t Position_Y;
    uint8_t Phone_Number[11];
    uint8_t Code[4];
    //货物入库时间 单位s
    uint16_t Cargo_Save_Time;
    //货物入库提醒时间 单位s
    uint16_t Cargo_Save_Time_Max;
    //货物入库时间计数  单位ms
    uint16_t Cargo_Save_Time_Cnt;
    //货物取走第一次提醒标志
    bool Cargo_Take_Warning_Flag;
    //货物取走第二次提醒标志/更新取件码
    bool Cargo_Take_Change_Flag;
    Struct_Cargo* Next_Cargo;
} __attribute__((packed));


class Class_Cargo_List
{
public:

    Struct_Cargo* First_Cargo;

    void Init();
    void Add_Cargo(uint8_t Position_X, uint8_t Position_Y, uint8_t* Phone_Number, uint8_t* Code);
    void Delete_Cargo(uint8_t Code[4]);
    Struct_Cargo* Exist_Cargo(uint8_t Code[4]);
    Struct_Cargo* Exist_Cargo_phone(uint8_t Code[4]);
};

#endif
