#include "mq_sensor.h"
#include "math.h"
#include "adc.h"
#include "usart.h"


#define RL			4.7		// RL阻值
#define R0   		30 // 元件在洁净空气中的阻值  35K


void MQ2_Init(void)
{
		Adc_Init();
	
}

 // MQ2传感器数据处理
float MQ2_GetPPM(void)
{

		u32 adcx;
		float Vrl ;
		float RS;
		float ppm;
		
		adcx=Get_Adc_Average(ADC_Channel_5,20);//获取通道5的转换值，20次取平均
		
		Vrl  = 3.3f * adcx / 4096;
	
		RS  = (3.3f - Vrl) / Vrl * RL; 

    ppm = 613.9f * pow(RS/R0,-2.074f);
//		printf("[debug]:adcx=%d\r\n",adcx);
//		printf("[debug]:ppm=%f\r\n",ppm);
//		printf("[debug]:3.3f * adcx=%f\r\n",3.3f * adcx);
//		printf("[debug]:Vrl=%f\r\n",Vrl);
//		printf("[debug]:RS=%f\r\n",RS);
    return  ppm;
	
	

}
