// Emulated Temperature Humidity and Dew Point sensor

#ifndef ONEWIREHUB_TMPSNSR_H
#define ONEWIREHUB_TMPSNSR_H

#include "OneWireItem.h"

class OWI_Temperature_Sensor : public OneWireItem
{
private:
    uint8_t scratchpad[13];
	double _temperature;
	double _humidity;
	double _dew_point;
    void updateCRC(void);

public:
    static constexpr uint8_t family_code{0x80}; 	// SateLight family code for sensors
	static constexpr uint8_t device_type{0x00}; 	// SateLight sensor code for temperature sensor
	
    OWI_Temperature_Sensor(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6,
            uint8_t ID7);

	void duty(OneWireHub *hub) final;
	void setTemperature(double temperature);
	void setHumidity(double humidity);
	void setDewPoint(double dew_point);
	double getTemperature();
	double getHumidity();
	double getDewPoint();
};

#endif