#include "owi_pressure_sensor.hpp"


OWI_Pressure_Sensor::OWI_Pressure_Sensor(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6,
				 uint8_t ID7)
	: OneWireItem(ID1, ID2, ID3, ID4, ID5, ID6, ID7)
{
	scratchpad[0] = 0xFF; // Pressure Real Byte 0
	scratchpad[1] = 0xFF; // Pressure Real Byte 1
	scratchpad[2] = 0xFF; // Pressure Real Byte 2
	scratchpad[3] = 0xFF; // Pressure Real Byte 3
	scratchpad[4] = 0xFF; // Pressure Relative (Sea Level) Byte 0
	scratchpad[5] = 0xFF; // Pressure Relative (Sea Level) Byte 1
	scratchpad[6] = 0xFF; // Pressure Relative (Sea Level) Byte 2
	scratchpad[7] = 0xFF; // Pressure Relative (Sea Level) Byte 3
	updateCRC();		  // update scratchpad[8]

	_pressure = 0.0;
	_pressure_sea_level = 0.0;
	_altitude = 160;

	// disable bus-features:
	fast_search_rom = false;
}

void OWI_Pressure_Sensor::updateCRC() { scratchpad[8] = crc8(scratchpad, 8); }

void OWI_Pressure_Sensor::duty(OneWireHub *const hub)
{
	uint8_t cmd;
	if (hub->recv(&cmd, 1)) return;
	
	switch (cmd)
	{
		case 0x01: // COMMAND_READ_SCRACHPAD [from OWI_PR_Driver.h]
			hub->send(scratchpad, 9);
			break;
		
		case 0x03: // COMMAND_SET_ALTITUDE
			hub->recv((uint8_t*)&_altitude, 4);
			break;
		default: hub->raiseDeviceError(cmd);
	}
}

void OWI_Pressure_Sensor::setPressure(double pressure){
	float temp_pressure = (float)pressure;
	memcpy(scratchpad, &temp_pressure, 4);
	_pressure = pressure;
	updateCRC();
}
void OWI_Pressure_Sensor::setPressureAtSeaLevel(double pressure_sea_level){
	float temp_pressure = (float)pressure_sea_level;
	memcpy(scratchpad+4, &temp_pressure, 4);
	_pressure_sea_level = pressure_sea_level;
	updateCRC();
}
double OWI_Pressure_Sensor::getPressure(){ return _pressure; }
double OWI_Pressure_Sensor::getPressureAtSeaLevel(){ return _pressure_sea_level; }
double OWI_Pressure_Sensor::getAltitude(){ return _altitude; }