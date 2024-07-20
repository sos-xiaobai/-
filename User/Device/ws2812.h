#ifndef __WS2812_H
#define __WS2812_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "math.h"

typedef struct
{
  uint8_t R;
  uint8_t G;
  uint8_t B;
} RGBColor_TypeDef;

//extern u8 pixelBuffer[][24];
extern const RGBColor_TypeDef RED ;
extern const RGBColor_TypeDef GREEN;
extern const RGBColor_TypeDef BLUE;
extern const RGBColor_TypeDef SKY;
extern const RGBColor_TypeDef MAGENTA ;
extern const RGBColor_TypeDef YELLOW ;
extern const RGBColor_TypeDef ORANGE;
extern const RGBColor_TypeDef BLACK;
extern const RGBColor_TypeDef WHITE;
extern const RGBColor_TypeDef PURPLE;
/* Exported constants --------------------------------------------------------*/
#define Pixel_S1_NUM 20		//灯珠 RGB数量
#define Pixel_S1_Buffer_NUM Pixel_S1_NUM*24
/**************************************
 说明：

 WS2812B编码协议(单位：ns)：
       min     typ     max
bit 0
 T0H:  220      -      420
 T0L:  750      -      1600
 
bit 1 
 T1H:  750      -      1600
 T1L:  220      -      420
 
 
 RESET: time > 300us


8分频APB1，42MHz/8 = 5.25MHz
时钟周期为：1/5.25/1e6 = 1.90e-7=190ns
**************************************/

#define CODE0 0xC0 // 0码, 发送的时间 1100 0000  根据不同的SCK适当调整
#define CODE1 0xFC // 1码, 发送的时间 1111 1100


/* Basic Color Effects -------------------------------------------------------*/
void RGB_RED(uint16_t Start_LEN,uint16_t End_LEN);
void RGB_GREEN(uint16_t Start_LEN,uint16_t End_LEN);
void RGB_BLUE(uint16_t Start_LEN,uint16_t End_LEN);
void RGB_YELLOW(uint16_t Start_LEN,uint16_t End_LEN);
void RGB_MAGENTA(uint16_t Start_LEN,uint16_t End_LEN);
void RGB_BLACK(uint16_t Start_LEN,uint16_t End_LEN);
void RGB_WHITE(uint16_t Start_LEN,uint16_t End_LEN);
void RGB_SKY(uint16_t Start_LEN,uint16_t End_LEN);
void RGB_ORANGE(uint16_t Start_LEN,uint16_t End_LEN);
void RGB_PURPLE(uint16_t Start_LEN,uint16_t End_LEN);
void RGB_RUNING();
void rgb_SetAllColor( RGBColor_TypeDef Color);
/* Complicated Color Effects -------------------------------------------------*/


#endif /* __WS2812_H */
