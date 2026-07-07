// Emulated Temperature Humidity and Dew Point sensor
// native bus-features: alarm search

#ifndef ONEWIREHUB_PHTSNSR_H
#define ONEWIREHUB_PHTSNSR_H

#include "OneWireItem.h"

class OWI_Photo_Sensor : public OneWireItem
{
private:
    uint8_t scratchpad[3];
	uint16_t _lux;

    void updateCRC(void);

public:
    static constexpr uint8_t family_code{0x80}; 	// SateLight family code for sensors
	static constexpr uint8_t device_type{0x01}; 	// SateLight sensor code for light sensor

    OWI_Photo_Sensor(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6,
            uint8_t ID7);

    void duty(OneWireHub *hub) final;
	void setLux(uint16_t lux);
	uint16_t getLux();
};

#endif