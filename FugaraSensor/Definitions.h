#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_


// Whether or not we'll print stuff to serial. 
// Not necessary when everything is working properly.
// Modify to `false` when possible - this will increase the sample rate and therefore touch accuracy.
#define SERIAL_IS_ENABLED					true

// Serial baud rate. 38400 is typically a safe value. 
// Use 9600 if problems arise, or 115200 if more prints are required.
#define SERIAL_BAUD							38400

// How much time in milliseconds we wait between sampling all sensors.
#define SAMPLE_DELAY_MS						50



// Number of samples we'll use for calibrating the barometer reference value
#define BAROMETER_CALIBRATION_COUNT			4

// Number of samples stored for each barometer. 
// We might need to increase this if upping the sample rate.
#define BAROMETER_WINDOW_SIZE				4

// Minimum number of values we require to differ from one another in order to assume the sensor is still connected
// (i.e. minimum required number of samples for which abs(s1-s0) > EPSILON).
#define BAROMETER_MIN_NOISE_COUNT			2

// Touch events will not be reported more often than once per (this value) milliseconds
#define BAROMETER_MIN_TOUCH_INTERVAL_MS		200

// If the difference between two barometer pressure samples exceeds this value (in mbars), 
// we'll interpret it as a sensor disconnection. Note: valid pressure readings typically change 
// on the order of single or double-digit (mbars).
#define BAROMETER_MAX_DIFF_ALLOWED			200.0f

// Verbose debug prints of barometer readings
// Turning to `true` will lower the sample rate - use only if necessary.
#define BAROMETER_DEBUG_PRINTS				false



/// Sensor Definitions - TODO: CHANGE THESE

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