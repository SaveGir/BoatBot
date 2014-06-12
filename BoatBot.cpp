#include "BoatBot.h"

#define CLK_DELAY {delayMicroseconds(3);}

//Constructor of the class
BoatBot::BoatBot(int sdaPin, int sclPin)
{
  _sdaPin = sdaPin;
  _sclPin = sclPin;
  pinMode(sclPin, OUTPUT);
  //initialize the accelerometer
  AC_init();
}

/***************************************************
Public functions
***************************************************/

long BoatBot::PR_Pressure()
{
  
}

long BoatBot::PR_Temperature()
{
  
}

//Reads the acceleration value measured on x-axis
int BoatBot::AC_xValue()
{
  int x;
  unsigned char data1, data2;
  unsigned char adc_bit = 10;
  
  data1 = singleRead(ACCE_AD_W, ACCE_AD_R, AC_DATAX0);
  data2 = singleRead(ACCE_AD_W, ACCE_AD_R, AC_DATAX1);
  //VERIFICARE QUESTA PARTE!!!!!!-------------------------------------------------------------<<
  // combine two bytes to get the 10-bit acceleration representation, see AN001  
  x = (data1<<3)|(data2&7);                         
  if (x & (1<<(adc_bit-1))) x -= (1<<adc_bit);  // 2's complement to negative 
    
  return x;
}

//Reads the acceleration value measured on x-axis
int BoatBot::AC_yValue()
{
  int x;
  unsigned char data1, data2;
  unsigned char adc_bit = 10;
  
  data1 = singleRead(ACCE_AD_W, ACCE_AD_R, AC_DATAY0);
  data2 = singleRead(ACCE_AD_W, ACCE_AD_R, AC_DATAY1);
  //VERIFICARE QUESTA PARTE!!!!!!-------------------------------------------------------------<<
  // combine two bytes to get the 10-bit acceleration representation, see AN001  
  x = (data1<<3)|(data2&7);                         
  if (x & (1<<(adc_bit-1))) x -= (1<<adc_bit);  // 2's complement to negative 
    
  return x;
}

//Reads the acceleration value measured on x-axis
int BoatBot::AC_zValue()
{
  int x;
  unsigned char data1, data2;
  unsigned char adc_bit = 10;
  
  data1 = singleRead(ACCE_AD_W, ACCE_AD_R, AC_DATAZ0);
  data2 = singleRead(ACCE_AD_W, ACCE_AD_R, AC_DATAZ1);
  //VERIFICARE QUESTA PARTE!!!!!!-------------------------------------------------------------<<
  // combine two bytes to get the 10-bit acceleration representation, see AN001  
  x = (data1<<3)|(data2&7);                         
  if (x & (1<<(adc_bit-1))) x -= (1<<adc_bit);  // 2's complement to negative 
    
  return x;
}


/***************************************************
Private functions used to initialize and setup sensors
***************************************************/

long long BoatBot::PR_read()
{
  short AC1, AC2, AC3, B1, B2, MB, MC, MD;
  unsigned short AC4, AC5, AC6;
  long UT, UP, X1, X2, B5, T, B6, X3, B3, B4, B7, P;
  char buffer;
  
  buffer = singleRead(0xEE, 0xEF, 0xAA);
}

void BoatBot::AC_init()
{
  /* Set the standby mode to allow configuring the measurement options
   * disables autosleep and sleep mode too*/
  singleWrite(ACCE_AD_W, AC_POWER_CTL, 0x00);
  /*Disable the interrupts functions*/
  singleWrite(ACCE_AD_W, AC_INT_ENABLE, 0x00);
  /*Disable self test, set interrupts active High, 10 bit mode measurement
   MSB mode and +-2g range*/
  singleWrite(ACCE_AD_W, AC_DATA_FORMAT, 0x44);
  /*Bypass FIFO registers functions*/
  singleWrite(ACCE_AD_W, AC_FIFO_CTL, 0x00);
  /*Sets the data rate to 100 HZ (power consumption of 140 uA) and disables
   * low power mode*/
  singleWrite(ACCE_AD_W, AC_BW_RATE, 0x0A);
  /*Exit standby mode and start measuring*/
  singleWrite(ACCE_AD_W, AC_POWER_CTL, 0x08);
}

/***************************************************
Private functions used to manage comunication
***************************************************/

//Sends one byte of data in the specified adress of the specified slave.
//If everithings works the function returns 1 and 0 otherwise.
int BoatBot::singleWrite(unsigned char slaveWrite, unsigned char adress, unsigned char data)
{
  int check1;
  int check2;
  int check3;
  
  start();		//send start condition
  sendByte(slaveWrite);	//Send the slave adress in write mode
  check1 = sm_akn();	//Checks the acknowledge from the slave
  if(check1 == 0)	//If slave do not acknowledge send stop condition and returns 0
  {
    stop();
    return 0;
  }
  else
  {
    sendByte(adress);	//Sends the adress of the register to be written
    check2 = sm_akn();	//Checks the acknowledge from the slave
    if(check2 == 0)	//If slave do not acknowledge send stop condition and returns 0
    {
      stop();
      return 0;
    }
    else
    {
      sendByte(data);	//Sends the data byte to the register
    }
    check3 = sm_akn();	//Checks the acknowledge from the slave
    if(check3 == 0)	//If slave do not acknowledge send stop condition and returns 0
    {
      stop();
      return 0;
    }
    else
    {
      stop();		//End the transmission sending the stop condition and returns 1
      return 1;
    }
  }
  
}

//Reads on byte of data in the specified adress of the specified slave.
char BoatBot::singleRead(unsigned char slaveWrite, unsigned char slaveRead, unsigned char adress)
{
  char data;
  int check1;
  int check2;
  int check3;
  
  start();		//send start condition
  sendByte(slaveWrite);	//Send the slave adress in write mode
  check1 = sm_akn();	//Checks the acknowledge from the slave
  if(check1 == 0)	//If slave do not acknowledge send stop condition and returns
  {
    stop();
    return;
  }
  else
  {
    sendByte(adress);	//Sends the adress of the register to be readed
    check2 = sm_akn();	//Checks the acknowledge from the slave
    if(check2 == 0)	//If slave do not acknowledge send stop condition and returns
    {
      stop();
      return;
    }
    else
    {
      start();			//Repeats start condition
      CLK_DELAY;
      sendByte(slaveRead);	//Sends the adress of the slave in read mode
      check3 = sm_akn(); 	//Checks the acknowledge from the slave
      if(check3 == 0)		//If slave do not acknowledge send stop condition and returns
      {
	stop();
	return;
      }
      else
      {
	CLK_DELAY;
	data = reciveByte();	//Recives the data from the slave and store them in the data variable
	ms_nakn();		//Sends not acknowledge
	stop();			//Send stop condition
	return data;		//And returns data
      }
    }
  }
}

/*int BoatBot::multipleWrite(unsigned char slaveWrite, unsigned char adress, unsigned char data1, unsigned char data2)
{
}*/

//The start condition from the master is given by a HIGH/LOW transistion of data line while clock line is high
void BoatBot::start()
{
  SDA_OUT;	//Arduino sends signals on SDA and SCL
  SCL_OUT;
  SDA_HIGH;
  SCL_HIGH;	//Both lines are pulled High
  CLK_DELAY;	//Wait 3 uS
  SDA_LOW;	//Pull down data line
  CLK_DELAY;	//Wait 3 uS
  SCL_LOW;	//Pull down clock lines
  CLK_DELAY;	//Wait 3 uS
}

//The stop condition from the master is given by a LOW/HIGH transition of the data line while clock line si HIGH
void BoatBot::stop()
{
  SDA_OUT;	//Arduino sends signals on SDA and SCL
  SCL_OUT;
  SDA_LOW;	//Both lines are pulled LOW
  SCL_LOW;
  CLK_DELAY;	//Wait 3 uS
  SCL_HIGH;	//Pull clock line HIGH
  CLK_DELAY;	//Wait 3 uS
  SDA_HIGH;	//Pull data line HIGH
  CLK_DELAY;	//Wait 3 uS
}

//Checks the acknowledgement bit from the slave.
//If the slave gives acknowledge the function returns 1 otherwise returns 0
int BoatBot::sm_akn()
{
  SDA_LOW;	//Pull down data line
  SDA_IN;	//Listen to slave
  SCL_LOW;	//Pull down clock line
  CLK_DELAY;	//Wait 3 uS
  SCL_HIGH;	//Pull HIGH clock line
  CLK_DELAY;	//Wait 3 uS
  if(SDA_STATUS == HIGH) //Check if slave acknowledges. If yes it will turn the line LOW.
  {
    return 0;
  }
  if(SDA_STATUS == LOW)
  {
    return 1;
  }
  SCL_LOW;	//Pull clock LOW
  SDA_OUT;	//Reset the SDA line as output
  CLK_DELAY;	//Wait 3 uS
}

//Send the acknowledge bit to the slave
void BoatBot::ms_akn()
{
  SDA_OUT;	//Take control of data line
  SDA_LOW;	//Pull data line LOW
  SCL_LOW;	//Pull clock line LOW
  CLK_DELAY;	//Generate a clock pulse while keeping the data line low
  SCL_HIGH;	
  CLK_DELAY;
  SCL_LOW;
  CLK_DELAY;
}

//Send the Not acknowledge bit to the slave
void BoatBot::ms_nakn()
{
  SDA_OUT;	//Take control of data line
  SCL_LOW;	//Pull clock line LOW
  SDA_HIGH;	//Pull data line HIGH
  CLK_DELAY;	//Generate a clock pulse while keeping the data line HIGH
  SCL_HIGH;	
  CLK_DELAY;
  SCL_LOW;
  CLK_DELAY;
}

//Sends one byte
void BoatBot::sendByte(unsigned char data)
{
  int i;
  
  SDA_OUT;	//Set the data line as output
  SDA_LOW;
  SCL_LOW;	//Set the clock line low to let the data status change 
  
  for(i = 7; i >= 0; i--)
  {
    if((data & (1 << i)) == 1)	//For each bit of data sets data line HIGH or LOW
    {
      SDA_HIGH;
    }
    else
    {
      SDA_LOW;
    }
    CLK_DELAY;		//Generate the pulse clock to send the data
    SCL_HIGH;
    CLK_DELAY;
    SCL_LOW;
  }
}

//Recive one byte
char BoatBot::reciveByte()
{
  int i;
  unsigned char data;
  
  SDA_IN;
  
  for(i = 7; i >= 0; i--)
  {
    SCL_HIGH;
    CLK_DELAY;
    if(SDA_STATUS == 1){data |= (1 << i);}	//check the status of the data line and perform bitwise OR
    SCL_LOW;
    CLK_DELAY;
  }
  
  return data;
}
