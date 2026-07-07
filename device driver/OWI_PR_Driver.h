/**
 * @file OWI_TH_Driver.h
 * @version 1.0
 *
 * Driver containing the master functions that can be used to control the OWI - Photometer Sensor.
 */

#pragma once

#include <Arduino.h>
#include "OWI.h"

// Configure: Slave device addressing; skip_rom, match_rom, or match_label
// Use skip_rom for single device on bus otherwise match_rom or match_label
// #define MATCH() m_owi.skip_rom()
#define MATCH() m_owi.match_rom(m_rom)
// #define MATCH() m_owi.match_label(m_label)

/**
 * One-Wire Interface (OWI) Remote Arduino Device Driver. Core
 * functions are implemented as one-wire communication. See
 * OWI/examples/Slave, Slave and Master sketches for examples.
 */
class OWI_PR_Sensor : public OWI::Device {
public:
	/** Family code. */
	static const uint8_t FAMILY_CODE = 0x80;

	/**
	 * Construct One-Wire Interface (OWI) Remote Arduino with given bus
	 * and device address.
	 * @param[in] owi bus manager.
	 * @param[in] rom code (default NULL).
	 */
	explicit OWI_PR_Sensor(OWI& owi, uint8_t* rom = NULL) :
	OWI::Device(owi, rom),
	m_label(0)
	{}

	/**
	 * Get device label.
	 * @return short address.
	 */
	uint8_t label()
	{
		return (m_label);
	}

	/**
	 * Set device label.
	 * @param[in] label short address.
	 */
	void label(uint8_t nr)
	{
		m_label = nr;
	}

	/**
	 * Request the sensor to start a reading.
	 * @return zero(0) or negative error code.
	 */
	int sensorTakeMeasurement()
	{
		if (!MATCH()) return (-1);
		m_owi.write(COMMAND_TAKE_MEASUREMENT);
		return (0);
	}

	/**
	 * Get the last measured sensor values.
	 * @return zero(0) or negative error code.
	 */
	int sensorReadScrachpad(double* pressure, double* pressure_sealevel){
		if (!MATCH()) return (-1);
		m_owi.write(COMMAND_READ_SCRACHPAD);	// Send STATUS command
		if(m_owi.read(&m_sensor_scratchpad, sizeof(m_sensor_scratchpad))){	//read response
			// check validity of response variables and return the values to caller
			if(!isnan(m_sensor_scratchpad.pressure) && !isnan(m_sensor_scratchpad.pressure_sealevel)){
				*pressure = m_sensor_scratchpad.pressure;
				*pressure_sealevel = m_sensor_scratchpad.pressure_sealevel;
				return(0);
			}
		}
		return(-1);
	}
	
	/**
	 * Set the altitude of the sensor.
	 * @return zero(0) or negative error code.
	 */
	int sensorSetAltitude(double altitude){
		if (!MATCH()) return (-1);
		m_owi.write(COMMAND_SET_ALTITUDE);
		m_owi.write(altitude);
		return(0);
	}

	/**
	 * Read rom identity code for device. Return zero(0) if successful,
	 * otherwise negative error code.
	 * @return zero(0) or negative error code.
	 */
	int read_rom()
	{
		if (m_owi.read_rom(m_rom)) return (0);
		return (-1);
	}

	/**
	 * Set rom label for current addressed device. Return zero(0) if
	 * successful, otherwise negative error code.
	 * @param[in] nr label number.
	 * @return zero(0) or negative error code.
	 */
	int label_rom(uint8_t nr)
	{
		m_owi.write(OWI::LABEL_ROM);
		m_owi.write(nr);
		m_label = nr;
		return (0);
	}

	/**
	 * Print device rom idenity code to given output stream. Return
	 * zero(0) if successful, otherwise negative error code.
	 * @param[in] out output stream (default Serial).
	 * @return zero(0) or negative error code.
	 */
	int print_rom(Print& out = Serial)
	{
		out.print(F("family="));
		if (m_rom[0] < 0x10) out.print('0');
		out.print(m_rom[0], HEX);
		out.print(F(",rom="));
		for (size_t i = 1; i < OWI::ROM_MAX - 1; i++) {
			if (m_rom[i] < 0x10) out.print('0');
			out.print(m_rom[i], HEX);
		}
		out.print(F(",crc="));
		uint8_t crc = m_rom[sizeof(OWI::ROM_MAX) - 1];
		if (crc < 0x10) out.print('0');
		out.print(crc, HEX);
		return (0);
	}


	/** One-Wire Interface (OWI) Remote Arduino Device function codes. */
enum COMMAND : uint8_t{
	COMMAND_NONE,
	COMMAND_READ_SCRACHPAD,
	COMMAND_TAKE_MEASUREMENT,
	COMMAND_SET_ALTITUDE
};

protected:
	/** Short address. */
	uint8_t m_label;

	/**The values stored into the TH sensors scratchpad */
	struct OWI_scratchpad{
		double pressure = 0;
		double pressure_sealevel = 0;	
		uint8_t crc = 0;
	} __attribute__((packed));
	OWI_scratchpad m_sensor_scratchpad;
};

#undef MATCH