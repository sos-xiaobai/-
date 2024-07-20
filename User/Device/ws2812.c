#include "WS2812.h"


// Some Static Colors
const RGBColor_TypeDef RED      = {0,255,0};
const RGBColor_TypeDef GREEN    = {255,0,0};
const RGBColor_TypeDef BLUE     = {0,0,255};
const RGBColor_TypeDef SKY      = {0,255,255};
const RGBColor_TypeDef MAGENTA  = {255,0,255};
const RGBColor_TypeDef YELLOW   = {255,255,0};
const RGBColor_TypeDef ORANGE   = {127,106,0};
const RGBColor_TypeDef BLACK    = {0,0,0};
const RGBColor_TypeDef WHITE    = {50,50,50};
const RGBColor_TypeDef PURPLE   = {65,105,225};

 uint8_t pixelBuffer[Pixel_S1_Buffer_NUM]={0};                     //����


void rgb_SetColor(uint16_t LedId, RGBColor_TypeDef Color){
	
 	static uint16_t i,j;
  for(i=0;i<Pixel_S1_NUM;i++)
	{
		if(LedId==i)
			{
				for(j=0;j<8;j++)
				{
					pixelBuffer[LedId*24+j]=((Color.R<<j)&0x80)?CODE1:CODE0;
					pixelBuffer[LedId*24+j+8]=((Color.G<<j)&0x80)?CODE1:CODE0;
					pixelBuffer[LedId*24+j+16]=((Color.B<<j)&0x80)?CODE1:CODE0;
				}
			}
	}
							
}

void rgb_SetAllColor( RGBColor_TypeDef Color){
	
 	static uint16_t i,j;
  for(i=0;i<Pixel_S1_NUM;i++)
	{
		for(j=0;j<8;j++)
		{
			pixelBuffer[i*24+j]=((Color.R<<j)&0x80)?CODE1:CODE0;
			pixelBuffer[i*24+j+8]=((Color.G<<j)&0x80)?CODE1:CODE0;
			pixelBuffer[i*24+j+16]=((Color.B<<j)&0x80)?CODE1:CODE0;
		}
	}
	HAL_SPI_Transmit_DMA(&hspi2,pixelBuffer,Pixel_S1_Buffer_NUM);				
}

void RGB_RED(uint16_t Start_LEN,uint16_t End_LEN){
	
  uint16_t i;
	
  for(i = Start_LEN; i < End_LEN; i++){  
    rgb_SetColor(i,RED);
	}

}


void RGB_PURPLE(uint16_t Start_LEN,uint16_t End_LEN){
	
  uint16_t i;
	
  for(i = Start_LEN; i < End_LEN; i++){  
    rgb_SetColor(i,PURPLE);
	}
  
}

void RGB_SKY(uint16_t Start_LEN,uint16_t End_LEN){
	
  uint16_t i;
	
  for(i = Start_LEN; i < End_LEN; i++){  
    rgb_SetColor(i,SKY);
	}

}


void RGB_MAGENTA(uint16_t Start_LEN,uint16_t End_LEN){
	
  uint16_t i;
	
  for(i = Start_LEN; i < End_LEN; i++){  
    rgb_SetColor(i,MAGENTA);
	}
  

}


void RGB_ORANGE(uint16_t Start_LEN,uint16_t End_LEN){
	
  uint16_t i;
	
  for(i = Start_LEN; i < End_LEN; i++){  
    rgb_SetColor(i,ORANGE);
	} 
	
}


void RGB_GREEN(uint16_t Start_LEN,uint16_t End_LEN){
	
  uint16_t i;
	
  for(i = Start_LEN; i < End_LEN; i++){
    rgb_SetColor(i,GREEN);
	}
  

}


//void RGB_BLUE(uint16_t Start_LEN,uint16_t End_LEN){
//	
//  uint16_t i;
//	RGBColor_TypeDef temp_color;
//	temp_color=BLUE;
//	int t;
//  for(i = Start_LEN; i < End_LEN; i++){
//    temp_color.B=BLUE.B-(7-t)*36;
//		t++;
//		rgb_SetColor(i,temp_color);
//		
//	}
//  

//}

void RGB_BLUE(uint16_t Start_LEN,uint16_t End_LEN){
	
  uint16_t i;
	RGBColor_TypeDef temp_color;
	temp_color=BLUE;
	double temp_cnt;
	double t;
  for(i = Start_LEN; i < End_LEN; i++){
		temp_cnt=(double)BLUE.B-pow((double)3,(double)(10-t));

		if(temp_cnt<=150)
			temp_cnt=150-(10-t)*19;
		if(temp_cnt<=0)
			temp_cnt=5;
		temp_color.B=(uint8_t)((float)temp_cnt*0.5);
		temp_color.R=(uint8_t)((float)temp_cnt*0.5);
		temp_color.G=(uint8_t)((float)temp_cnt);
		t++;
		rgb_SetColor(i,temp_color);
		
	}
}
  




void RGB_YELLOW(uint16_t Start_LEN,uint16_t End_LEN){
	
  uint16_t i;
	
  for(i = Start_LEN; i < End_LEN; i++){
    rgb_SetColor(i,YELLOW);
	}
  

}


void RGB_BLACK(uint16_t Start_LEN,uint16_t End_LEN){
	
  uint16_t i;
	
  for(i = Start_LEN; i < End_LEN; i++){
  
    rgb_SetColor(i,BLACK);
	}
  

}


void RGB_WHITE(uint16_t Start_LEN,uint16_t End_LEN){
	
  uint16_t i;
	
  for(i = Start_LEN; i < End_LEN; i++){
    rgb_SetColor(i,WHITE);
    
	}

}

void RGB_RUNING_SET(uint16_t Start_LEN,uint16_t End_LEN)
{
	if(Start_LEN<=End_LEN)
	{
	RGB_BLACK(0,Start_LEN);
	RGB_BLUE(Start_LEN,End_LEN);
	RGB_BLACK(End_LEN,Pixel_S1_NUM);	
	}
	else
	{
	RGB_BLUE(0,End_LEN);
	RGB_BLACK(End_LEN,Start_LEN);
	RGB_BLUE(Start_LEN,Pixel_S1_NUM);	
	}	
}

void RGB_RUNING()
{
	HAL_SPI_Transmit_DMA(&hspi2,pixelBuffer,Pixel_S1_Buffer_NUM);
}



