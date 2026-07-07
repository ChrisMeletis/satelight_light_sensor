#include "owi_photo_sensor.hpp"


OWI_Photo_Sensor::OWI_Photo_Sensor(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6,
                 uint8_t ID7)
    : OneWireItem(ID1, ID2, ID3, ID4, ID5, ID6, ID7)
{
	scratchpad[0] = 0x00; // luminosity
	scratchpad[1] = 0x00; // luminosity
    updateCRC();          // update scratchpad[4]

	_lux = 0;
    // disable bus-features:
    fast_search_rom = false;
}

void OWI_Photo_Sensor::updateCRC() { scratchpad[2] = crc8(scratchpad, 2); }

void OWI_Photo_Sensor::duty(OneWireHub *const hub)
{
    uint8_t cmd;
    if (hub->recv(&cmd, 1)) return;

    switch (cmd)
    {
        case 0x01: // COMMAND_READ_SCRACHPAD [from OWI_TH_Driver.h]
            hub->send(scratchpad, 3);
            break;

        default: hub->raiseDeviceError(cmd);
    }
}

void OWI_Photo_Sensor::setLux(uint16_t lux){
	memcpy(scratchpad, &lux, 2);
	_lux = lux;
	updateCRC();
}
uint16_t OWI_Photo_Sensor::getLux(){ return _lux; }
