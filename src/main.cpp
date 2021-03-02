#include <Arduino.h>
//--------------------------------------------------------------------
// Author:        Aamir Hasan Khan
// Version:       01
// Description:   Environment Monitor
// File Type:     Main Controller ARDUINO MEGA
// Date:          29-02-2020
//--------------------------------------------------------------------
// Start Date:    29-02-2020
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Revised
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Libraries Used
#include <UTFT.h>
#include <URTouch.h>
#include <dht.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// TFT Display Configuration
UTFT myGLCD(CTE32HR, 38, 39, 40, 41);
// UTFT myGLCD(ITDB32S,38,39,40,41);
URTouch myTouch(6, 5, 4, 3, 2);
//extern uint8_t SmallFont[];
extern uint8_t BigFont[];
int Xsize, Ysize;
int X, Y, text_y_center;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Light Intensity Sensor Configuration
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Temperature and Humidity Sensor Configuration
#define DHT11_PIN 8
dht DHT;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Gas Sensor Configuration
#define Gas_Sensor_PIN A0
int sensorValue;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//  Dust Sensor Configuration
#define Dust_Sensor_PIN A5
#define measurePin A5
#define ledPower 12
unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Function Prototypes
void Main_Screen();
void Temp_Humidity();
void Light_Intensity();
void Gas_Reading();
void Dust_Sensor_reading();
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Main Setup Function
void setup()
{

  // Start Debug Serial port
  Serial.begin(9600);
  Serial1.begin(9600);

  // Intilize the TFT Screen
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);
  //myGLCD.setFont(SmallFont); //BigFont
  myGLCD.fillScr(VGA_BLACK);

  Xsize = myGLCD.getDisplayXSize();
  Ysize = myGLCD.getDisplayYSize();

  // Serial.print("Glcd Size");
  // Serial.print("X:  ");
  // Serial.print(Xsize);
  // Serial.print("X:  ");
  // Serial.println(Ysize);

  // Start Light Intensity Sensor
  Serial.println("Light Sensor Test");
  Serial.println("");

  /* Initialise the sensor */
  //if(!tsl.begin())
  //{
  //  Serial.print("No TSL2561 detected");
  //  while(1);
  //}

  //tsl.enableAutoRange(true);
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);

  Serial.println("");

  //Start Dust Sensor
  pinMode(ledPower, OUTPUT);
  Main_Screen();
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Main Loop Function
void loop()
{

  // put your main code here, to run repeatedly:

  myGLCD.print("TEMPERATURE:", 40, 15);
  myGLCD.print("HUMIDITY:", 200, 15);
  myGLCD.print("DUST:", 65, 95);
  myGLCD.print("GAS:", 220, 95);
  myGLCD.print("LIGHT INTENSITY:", 100, 165);

  myGLCD.setColor(VGA_YELLOW);
  myGLCD.print(" C", 100, 70);
  myGLCD.print(" %", 250, 70);
  myGLCD.print(" %", 100, 150);
  myGLCD.print(" LUX", 180, 220);
  myGLCD.print(" PPM", 250, 150);
  myGLCD.setBackColor(VGA_BLACK);

  Light_Intensity();
  Gas_Reading();
  Dust_Sensor_reading();
  Temp_Humidity();
  delay(250);
}
//--------------------------------------------------------------------

void Main_Screen()
{
  //screen

  myGLCD.setColor(VGA_TEAL);
  myGLCD.fillRect(5, 5, 475, 23);

  myGLCD.setBackColor(VGA_TEAL);
  myGLCD.setColor(VGA_LIME);
  myGLCD.print("ENVIRONMENT MONITOR", 86, 6);

  myGLCD.setColor(VGA_LIME);
  myGLCD.drawRoundRect(4, 4, 476, 316);

  myGLCD.drawLine(4, 24, 476, 24);

  myGLCD.drawLine(4, 48, 476, 48);
  myGLCD.drawLine(4, 272, 476, 272);

  //myGLCD.drawLine(4, 148, 476, 148);
  //myGLCD.drawLine(4, 172, 476, 172);

  //myGLCD.drawLine(240, 24, 240, 296);

  myGLCD.drawLine(4, 296, 476, 296);

  myGLCD.setColor(VGA_MAROON);
  myGLCD.fillRect(5, 297, 475, 315);

  myGLCD.setColor(VGA_YELLOW);
  myGLCD.setBackColor(VGA_MAROON);
  myGLCD.print("YANBU UNIVERSITY COLLEGE", 44, 298);

  // myGLCD.setColor(VGA_GREEN);
  myGLCD.setColor(VGA_SILVER);

  myGLCD.drawCircle(120, 98, 45);
  myGLCD.drawCircle(360, 98, 45);

  myGLCD.drawCircle(120, 222, 45);
  myGLCD.drawCircle(360, 222, 45);

  myGLCD.drawCircle(240, 160, 45);
  // myGLCD.drawCircle(156, 205, 40);
}

//--------------------------------------------------------------------
// Temperature and Humidity Reading Function
void Temp_Humidity()
{
  DHT.read11(DHT11_PIN);
  float Temperature_reading = DHT.temperature;
  float Humidity_reading = DHT.humidity;

  Serial1.println(Temperature_reading); // Debugging
  Serial1.println(Humidity_reading);    // Debugging

  //myGLCD.setColor(VGA_WHITE);
  //if (Temperature_reading >= 0 && Temperature_reading>=0){
  //myGLCD.printNumF(Temperature_reading,2, 62, 50);
  //myGLCD.printNumF(Humidity_reading,2, 212, 50);
  //}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Light Intensity Reading Function
void Light_Intensity()
{
  /* Get a new sensor event */
  sensors_event_t event;
  tsl.getEvent(&event);
  myGLCD.print("       ", 136, 200);
  /* Display the results (light is measured in lux) */
  if (event.light != 65536)
  {
    myGLCD.setColor(VGA_WHITE);
    myGLCD.printNumF(event.light, 2, 137, 200);
    //delay(250);
  }
  else
  {
    myGLCD.setColor(VGA_RED);
    myGLCD.print("warning", 137, 200);
    //delay(700);
    myGLCD.print("       ", 136, 200);
  }
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Gas Sensor Reading Function
void Gas_Reading()
{
  sensorValue = analogRead(Gas_Sensor_PIN);
  if (sensorValue != 0 && sensorValue != 1023)
  {
    myGLCD.setColor(VGA_WHITE);
    myGLCD.printNumI(sensorValue, 222, 130);
  }
  else
  {
    myGLCD.setColor(VGA_RED);
    myGLCD.print("warning", 205, 130);
    myGLCD.print("        ", 205, 130);
  }
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Dust Sensor Reading Function
void Dust_Sensor_reading()
{
  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin);
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * (5.0 / 1024);
  dustDensity = 0.17 * calcVoltage - 0.1;
  dustDensity = dustDensity * 100;
  if (dustDensity < 0)
  {
    dustDensity = 0.00;
  }
  myGLCD.print("      ", 65, 130);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.printNumF(dustDensity, 2, 65, 130);
  delay(700);
}
//--------------------------------------------------------------------
