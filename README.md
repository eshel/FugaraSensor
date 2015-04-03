# FugaraSensor

## General

This repository hosts the FugaraSensor project - which samples the Fluid Self's "Neurons" for touch events.
Touch sensing is implemented by BMP180 based barometers. Upon a sudden change of the read barometer value, 
a touch event should be reflected to the host PC, via Modbus-RTU over RS485.

## FugaraSensor project

This project contains the code for the touch sensing. It has a few responsibilities:
* Periodic sampling of up to 4 barometers for air pressure reading, including calibration, stabilization disconnects, etc.
* Recognition of touch event according to last N barometer samples.
* Answer modbus RTU "read register" requests from host PC (modbus master), with the timestamp of the latest "touch" event. Each barometer corresponds to one register. Timestamps are in milliseconds since arduino's boot (enough for 46 days of consecutive operation).
* Modbus RTU slave address is stored in the lower two bytes of the EEPROM (offset 0 - address, offset 1 - bitwise NOT of the address). This may be modified using the SlaveAddress program (included).

## FugaraSensor tweaking & customization

These files should be modified when getting ready for bringing up the project:

* Definitions.h - pin definitions, timeouts, delays, thresholds, serial config, debug prints etc. See documentation inside that file.
* Bmp180Pressure.cpp (`bool detectTouch()`) - modify the implementation of `detectTouch()` to handle real-world barometer pressure data. `true` should be returned exactly once for each touch event. If no new touch event has been recognized, should return false.

## SlaveAddress project

A small utility program to write the slave address into the device's EEPROM memory.

Instructions:

1. Open the SlaveAddress sketch
2. Modify the SLAVE_ADDR constant to the desired address (between 1-31).
3. Compile & Upload
4. Open Serial Monitor (baud 38400 by default)
5. Verify the address has been written correctly (serial monitor prints).
6. Profit.

