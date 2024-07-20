#include "dvc_cargo.h"
#define CARGO_SAVE_TIME_MAX  60


void Class_Cargo_List::Init()
{
    First_Cargo = NULL;
}


void Class_Cargo_List::Add_Cargo(uint8_t Position_X, uint8_t Position_Y, uint8_t* Phone_Number, uint8_t* Code)
{
    //头插法插入新节点
    Struct_Cargo* New_Cargo = new Struct_Cargo;
    New_Cargo->Position_X = Position_X;
    New_Cargo->Position_Y = Position_Y;
    New_Cargo->Cargo_Save_Time_Max = CARGO_SAVE_TIME_MAX;

    memcpy(New_Cargo->Phone_Number, Phone_Number, 11 );
    memcpy(New_Cargo->Code, Code, 4 * sizeof(uint8_t));

    New_Cargo->Next_Cargo = First_Cargo;
    First_Cargo = New_Cargo;
}

void Class_Cargo_List::Delete_Cargo(uint8_t Code[4])
{
    Struct_Cargo* p = First_Cargo;
    Struct_Cargo* q = First_Cargo;
    //如果删除的就是第一个节点
    if(First_Cargo->Code[0] == Code[0] && First_Cargo->Code[1] == Code[1] && First_Cargo->Code[2] == Code[2] && First_Cargo->Code[3] == Code[3])
    {
        First_Cargo = First_Cargo->Next_Cargo;
        delete p;
        return;
    }
    //如果删除的不是第一个节点
    while (p != NULL)
    {
        if (p->Code[0] == Code[0] && p->Code[1] == Code[1] && p->Code[2] == Code[2] && p->Code[3] == Code[3])
        {
            q->Next_Cargo = p->Next_Cargo;
            delete p;
            break;
        }
        q = p;
        p = p->Next_Cargo;
    }
}

Struct_Cargo* Class_Cargo_List::Exist_Cargo(uint8_t Code[4])
{
    Struct_Cargo* p = First_Cargo;
    while (p != NULL)
    {
        if (p->Code[0] == Code[0] && p->Code[1] == Code[1] && p->Code[2] == Code[2] && p->Code[3] == Code[3])
        {
            return p;
        }
        p = p->Next_Cargo;
    }
    return NULL;
}





