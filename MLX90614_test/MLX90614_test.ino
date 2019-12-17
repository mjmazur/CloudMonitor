//*******************************************************************************
// Reads from two MLX90614 thermometer boards and prints the object temperatures
// and the difference between the two.                                          
// 
// Modified from the MLX90614_Serial_Demo found in the library at,
//https://github.com/sparkfun/SparkFun_MLX90614_Arduino_Library
//*******************************************************************************

#include Wire.h; // I2C library, required for MLX90614
#include SparkFunMLX90614.h; // SparkFunMLX90614 Arduino library

const byte IRAddress1 = 0x5A;
const byte IRAddress2 = 0x5B;

// Create 2 IRTherm objects
IRTherm therm1;
IRTherm therm2;

void setup() 
{
  Serial.begin(9600); // Initialize Serial to log output

  // Initialize the sensors and set the units
  therm1.begin(IRAddress1);
  therm1.setUnit(TEMP_C);
  therm2.begin(IRAddress2);
  therm2.setUnit(TEMP_C);
}

void loop() 
{
  // Call thermx.read() to read object and ambient temperatures from the sensor.
  if (therm1.read())
  {
    Serial.print("Object 1: "); Serial.print(therm1.object()); Serial.println("*C");
  }
  
  if (therm2.read())
  {
    Serial.print("Object 2: "); Serial.print(therm2.object()); Serial.println("*C");
    Serial.print("Temperature Difference: "); Serial.print(therm1.object()-therm2.object()); Serial.println("*C");
  }
  
  Serial.println();
  
  delay(2000);
}

//Hardware Hookup:
//MLX90614 ------------- Arduino
//  VDD ------------------ 3.3V
//  VSS ------------------ GND
//  SDA ------------------ SDA (A4)
//  SCL ------------------ SCL (A5)
