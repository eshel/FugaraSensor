#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#define SERIAL_IS_ENABLED				true
#define SERIAL_BAUD						115200

#define SAMPLE_DELAY_MS					100

#define BAROMETER_CALIBRATION_COUNT			4
#define BAROMETER_WINDOW_SIZE				4
#define BAROMETER_MIN_NOISE_COUNT			2

#define BAROMETER_DEBUG_PRINTS				false

#define MIN_VALID_PRESSURE_VALUE			0.05f


#define SENSOR_A_NAME			"A"
#define SENSOR_A_PIN_SDA		5
#define SENSOR_A_PIN_SCL		3

#define SENSOR_B_NAME			"B"
#define SENSOR_B_PIN_SDA		10
#define SENSOR_B_PIN_SCL		8

#define SENSOR_C_NAME			"C"
#define SENSOR_C_PIN_SDA		11
#define SENSOR_C_PIN_SCL		11

#define SENSOR_D_NAME			"D"
#define SENSOR_D_PIN_SDA		11
#define SENSOR_D_PIN_SCL		11


#endif // #ifndef _DEFINITIONS_H_