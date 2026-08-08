#ifndef __DHT20_H__
#define __DHT20_H__

#include "i2c.h"
#include "main.h"

// 初始化AHT20
void AHT20_Init();

// 获取温度和湿度
void AHT20_Read(float *Temperature, float *Humidity);

#endif
