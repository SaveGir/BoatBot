/*
  BoatBot.h - Library for reading and processing datas from:
    ADXL345 	Accelerometer from Analog Devices
    L3G4200D 	Three axis gyroscope from ST Microelectronics
    BMP085 	Digital pressure sensor from Bosch Sensortec
    HMC5883L 	Three axis digital compass from Honeywell
    NEO6	GPS module from u-blox
    
  Created by: 
    Saverio Girardi (saverio@plife-technology.com)
    Mattia Francesco Moro
    
    Trento, Italy, June 10th, 2014.
  
  Released into the public domain.
*/

#ifndef __BoatBot_h
#define __BoatBot_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h" // for Arduino functions, such as pinMode(), digitalWrite()
#else
  #include "WProgram.h" // for Arduino functions, such as pinMode(), digitalWrite()
#endif

//define the adresses of the sensors for I2C comunication
#define GYRO_AD 0xD3	// Gyroscope adress
#define ACCE_AD_W 0xA6	// Accelerometer write adress (pin 12 ALT ADRESS grounded)
#define ACCE_AD_R 0xA7	// Accelerometer read adress (pin 12 ALT ADRESS grounded)
#define COMP_AD 0x48	// Compass adress
#define PRES_AD_W 0xEE	// Pressure sensor adress when writing
#define PRES_AD_R 0xEF	// Pressure sensor adress when reading

//Adresses of the accelerometer registers
#define AC_TRESH_TAP 0x1D 	// Tap threshold
#define AC_OFSX 0x1E		// X-axis offset
#define AC_OFSX 0x1F		// Y-axis offset
#define AC_OFSX 0x20		// Z-axis offset
#define AC_DUR 0x21		// Tap duration
#define AC_LATENT 0x22		// Tap latency
#define AC_WINDOW 0x23		// Tap window
#define AC_TRESH_ACT 0x24	// Activity threshold
#define AC_TRESH_INACT 0x25	// Inactivity threshold
#define AC_TIME_INACT 0x26	// Inactivity time
#define AC_ACT_INACT_CTL 0x27	// Axis enable control for activity and inactivity direction
#define AC_TRESH_FF 0x28	// Free fall threshold
#define AC_TIME_FF 0x29		// Free fall time
#define AC_TAP_AXES 0x2A	// Axis control for single tap/double tap
#define AC_ACT_TAP_STATUS 0x2B	// Source of single tap / double tap
#define AC_BW_RATE 0x2C		// Data rate and power mode control
#define AC_POWER_CTL 0x2D	// Power saving features control
#define AC_INT_ENABLE 0x2E	// Interrupt enable control
#define AC_INT_MAP 0x2F		// Interrupt mapping control
#define AC_INT_SOURCE 0x30	// Source of interrupts
#define AC_DATA_FORMAT 0x31	// Data format control
#define AC_DATAX0 0x32		// X-Axis Data 0
#define AC_DATAX1 0x33		// X-Axis Data 1
#define AC_DATAY0 0x34		// Y-Axis Data 0
#define AC_DATAY1 0x35		// Y-Axis Data 1
#define AC_DATAZ0 0x36		// Z-Axis Data 0
#define AC_DATAZ1 0x37		// Z-Axis Data 1
#define AC_FIFO_CTL 0x38	// FIFO control
#define AC_FIFO_STATUS 0x39	// FIFO status

//Adresses of the gyro registers
#define GY_CTRL_REG1 0x20	// Output Data rate, bandwidth selection, power down mode and axis enable
#define GY_CTRL_REG2 0x21	// High pass filter mode selection and cutoff frequency
#define GY_CTRL_REG3 0x22	// --> check datasheet
#define GY_CTRL_REG4 0x23	// --> check datasheet
#define GY_CTRL_REG5 0x24	// --> check datasheet
#define GY_REFERENCE 0x25	// Reference value for interrupt generation
#define GY_OUT_TEMP 0x26	// Temperature data
#define GY_STATUS_REG 0x27	// --> check datasheet
#define GY_OUT_X_L 0x28		// X-axis angular rate data
#define GY_OUT_X_H 0x29		// X-axis angular rate data
#define GY_OUT_Y_L 0x2A		// Y-axis angular rate data
#define GY_OUT_Y_H 0x2B		// Y-axis angular rate data
#define GY_OUT_Z_L 0x2C		// Z-axis angular rate data
#define GY_OUT_Z_H 0x2D		// Z-axis angular rate data
#define GY_FIFO_CTRL_REG 0x2E	// FIFO mode selection and threshold
#define GY_FIFO_SRC_REG 0x2F	// --> check datasheet
#define GY_INT1_CFG 0x30	// --> check datasheet
#define GY_INT1_SRC 0x31	// --> check datasheet
#define GY_INT1_TSH_XH 0x32	// Interrupt threshold
#define GY_INT1_TSH_XL 0x33	// Interrupt threshold
#define GY_INT1_TSH_YH 0x34	// Interrupt threshold
#define GY_INT1_TSH_YL 0x35	// Interrupt threshold
#define GY_INT1_TSH_ZH 0x36	// Interrupt threshold
#define GY_INT1_TSH_ZL 0x37	// Interrupt threshold
#define GY_INT1_DURATION 0x38	// WAIT enable and duration value

//Adresses of the pressure sensor registers
#define PR_TEMPERATURE 0x2E	// Temperature Value
#define PR_PRESSURE_0 0x34	// Pressure 4.5 ms conversion time
#define PR_PRESSURE_1 0x74	// Pressure 7.5 ms conversion time
#define PR_PRESSURE_2 0xB4	// Pressure 13.5 ms conversion time
#define PR_PRESSURE_3 0xF4	// Pressure 25.5 ms conversion time

//Adresses of the Compass registers
#define CO_CONF_REG_A 0x00	// Configuration register A
#define CO_CONF_REG_B 0x01	// Configuration register B
#define CO_MODE_REG 0x02	// Mode register
#define CO_DO_XMSB 0x03		// Data Output X MSB
#define CO_DO_XLSB 0x04		// Data Output X LSB
#define CO_DO_ZMSB 0x05		// Data Output Z MSB
#define CO_DO_ZLSB 0x06		// Data Output Z LSB
#define CO_DO_YMSB 0x07		// Data Output Y MSB
#define CO_DO_YLSB 0x08		// Data Output Y LSB
#define CO_STATUS 0x09		// Status register
#define CO_ID_A 0x10		// Identification regiter A
#define CO_ID_B 0x11		// Identification regiter B
#define CO_ID_C 0x12		// Identification regiter C

#define SDA _sdaPin
#define SCL _sclPin
#define SDA_STATUS {digitalRead(SDA);}

#define SDA_OUT {pinMode(SDA, OUTPUT);}
#define SDA_IN {pinMode(SDA, INPUT);}
#define SCL_OUT {pinMode(SCL, OUTPUT);}
#define SCL_IN {pinMode(SCL, INPUT);}

#define SDA_HIGH {digitalWrite(SDA, HIGH);}
#define SDA_LOW {digitalWrite(SDA, LOW);}
#define SCL_HIGH {digitalWrite(SCL, HIGH);}
#define SCL_LOW {digitalWrite(SCL, LOW);}

class BoatBot
{
  public:
    BoatBot(int sdaPin, int sclPin);
    long PR_Pressure();
    long PR_Temperature();
    

  private:
    int _sdaPin;
    int _sclPin;
    
    long long PR_read();
    void start();	//OK
    void stop();	//OK
    void ms_akn();	//OK
    void ms_nakn();	//OK
    int sm_nakn();	//OK
    void sendByte(unsigned char data);	//OK
    char reciveByte();			//OK
    int singleWrite(unsigned char slaveWrite, unsigned char adress, unsigned char data);	//OK
    char singleRead(unsigned char slaveWrite, unsigned char slaveRead, unsigned char adress);	//OK
    //int multipleWrite(unsigned char slaveWrite, unsigned char adress, unsigned char data1, unsigned char data2);

};
  

#endif