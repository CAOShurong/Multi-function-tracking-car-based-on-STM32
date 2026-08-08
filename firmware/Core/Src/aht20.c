#include "aht20.h"

#define AHT20_ADDRESS 0x70

uint8_t readBuffer[6] = {0};

/**
 * @brief  初始化AHT20
 */
void AHT20_Init()
{
  uint8_t readBuffer;
  HAL_Delay(40);
  HAL_I2C_Master_Receive(&hi2c2, AHT20_ADDRESS, &readBuffer, 1, HAL_MAX_DELAY);
  if ((readBuffer & 0x08) == 0x00)
  {
    uint8_t sendBuffer[3] = {0xBE, 0x08, 0x00};
    HAL_I2C_Master_Transmit(&hi2c2, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
  }
}

/**
 * @brief  获取温度和湿度
 * @param  Temperature: 存储获取到的温度
 * @param  Humidity: 存储获取到的湿度
 */
void AHT20_Read(float *Temperature, float *Humidity)
{
  uint8_t sendBuffer[3] = {0xAC, 0x33, 0x00};
  uint8_t readBuffer[6] = {0};

  HAL_I2C_Master_Transmit(&hi2c2, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
  HAL_Delay(75);
  HAL_I2C_Master_Receive(&hi2c2, AHT20_ADDRESS, readBuffer, 6, HAL_MAX_DELAY);

  if ((readBuffer[0] & 0x80) == 0x00)
  {
    uint32_t data = 0;
    data = ((uint32_t)readBuffer[3] >> 4) + ((uint32_t)readBuffer[2] << 4) + ((uint32_t)readBuffer[1] << 12);
    *Humidity = data * 100.0f / (1 << 20);

    data = (((uint32_t)readBuffer[3] & 0x0F) << 16) + ((uint32_t)readBuffer[4] << 8) + (uint32_t)readBuffer[5];
    *Temperature = data * 200.0f / (1 << 20) - 50;
  }
}
