#include "owi_temperature_sensor.hpp"


OWI_Temperature_Sensor::OWI_Temperature_Sensor(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6,
                 uint8_t ID7)
    : OneWireItem(ID1, ID2, ID3, ID4, ID5, ID6, ID7)
{
    scratchpad[0] 	= 0x00; // Temperature
	scratchpad[1] 	= 0x00; // Temperature
	scratchpad[2] 	= 0x00; // Temperature
	scratchpad[3] 	= 0x00; // Temperature
    scratchpad[4] 	= 0x00; // Humidity
    scratchpad[5] 	= 0x00; // Humidity
    scratchpad[6] 	= 0x00; // Humidity
    scratchpad[7] 	= 0x00; // Humidity
    scratchpad[8] 	= 0x00; // Dew Point
	scratchpad[9] 	= 0x00; // Dew Point
	scratchpad[10] 	= 0x00; // Dew Point
	scratchpad[11] 	= 0x00; // Dew Point
    updateCRC();          	// update scratchpad[12]


    // disable bus-features:
    fast_search_rom = false;
}

void OWI_Temperature_Sensor::updateCRC() { scratchpad[12] = crc8(scratchpad, 12); }

void OWI_Temperature_Sensor::duty(OneWireHub *const hub)
{
    uint8_t cmd;
    if (hub->recv(&cmd, 1)) return;
    switch (cmd)
    {
        case 0x01: // COMMAND_READ_SCRACHPAD [from OWI_TH_Driver.h]
            hub->send(scratchpad, 13);
            break;

        default: hub->raiseDeviceError(cmd);
    }
}

void OWI_Temperature_Sensor::setTemperature(double temperature)	{
	_temperature = temperature;
	float converted_value = (float)temperature;
	memcpy(scratchpad, &converted_value, sizeof(converted_value));
	updateCRC();
}
void OWI_Temperature_Sensor::setHumidity(double humidity)		{
	_humidity = humidity;
	float converted_value = (float)humidity;
	memcpy(scratchpad+4, &converted_value, sizeof(converted_value));
	updateCRC();
}
void OWI_Temperature_Sensor::setDewPoint(double dew_point)		{
	_dew_point = dew_point;
	float converted_value = (float)dew_point;
	memcpy(scratchpad+8, &converted_value, sizeof(converted_value));
	updateCRC();
}
double OWI_Temperature_Sensor::getTemperature(){ return _temperature; }
double OWI_Temperature_Sensor::getHumidity(){ return _humidity; }
double OWI_Temperature_Sensor::getDewPoint(){return _dew_point; }
