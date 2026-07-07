// Emulated Temperature Humidity and Dew Point sensor
// native bus-features: alarm search

#ifndef ONEWIREHUB_PRSSNSR_H
#define ONEWIREHUB_PRSSNSR_H

#include "OneWireItem.h"

class OWI_Pressure_Sensor : public OneWireItem
{
private:
    uint8_t scratchpad[9];
	double _pressure;
	double _pressure_sea_level;
	double _altitude;

    void updateCRC(void);

public:
    static constexpr uint8_t family_code{0x80}; 	// SateLight family code for sensors
	static constexpr uint8_t device_type{0x02}; 	// SateLight sensor code for pressure sensor

    OWI_Pressure_Sensor(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6,
            uint8_t ID7);

    void duty(OneWireHub *hub) final;
	void setAltitude(double altitude);
	void setPressure(double pressure);
	void setPressureAtSeaLevel(double pressure_sea_level);
	double getTemperature();
	double getPressure();
	double getPressureAtSeaLevel();
	double getAltitude();
};

#endif