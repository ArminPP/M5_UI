#ifndef SIGNALPROCESSING_H_
#define SIGNALPROCESSING_H_

#include <Arduino.h>

struct GraphValues_t
{
    char TimeStamp[6];          // hh:mm
    float BMP280_Temperature; // ENVII Sensor
    float SHT30_Temperature;  //
    float Humidity;           //
    float TVOC;               // TVOC/eCO2 Sensor
    float eCO2;               // for better accuracy hum Rh% and Temp °C can be used!
};

#endif // SIGNALPROCESSING_H_