//Libraries needed for Sketch
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
int bluetoothRx=10;
int bluetoothTx=9;
//BNO055 Sensor
// Set the delay between fresh samples
#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();
//Neopixel on the Feather M0 Express
#define PIN 8
#define NUMPIXELS 1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);
//Vibration Motor
int Vibration = 9; // Vibration motor on pin 9
SoftwareSerial mySerial(bluetoothRx,bluetoothTx);
void setup(void)
{
  Serial.begin(9600);
  mySerial.begin(38400);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Vibration, OUTPUT);
  /* Initialise the Neopixel */
  pixels.begin();
  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);

  }
  delay(1000);
  bno.setExtCrystalUse(true);
}
void loop(void)
{
 
//    Serial.write('4');
  //Get Acceleration vector data from the Sensor
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(euler.x());
  Serial.print(" Y: ");
  Serial.print(euler.y());
  Serial.print(" Z: ");
  Serial.print(euler.z());
  Serial.print("\n\n");
  //Conditional statements to check with sensor is falling or rising
  //faster than set points (gravity is 9.8ms^2)
  if (euler.z() > 12 || euler.z() < 7)
  {
    pixels.setPixelColor(0, pixels.Color(150, 150, 0));
    pixels.show();
    beep();
  }
  else
  {
    pixels.setPixelColor(0, pixels.Color(0, 0, 150));
    pixels.show();
  }
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
//Beep function to handle short vibration ON and OFF
void beep() {
  digitalWrite(LED_BUILTIN, HIGH);
  mySerial.write('0');
  Serial.println("I SENT 00000000");
  digitalWrite(Vibration, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(Vibration, LOW);
  mySerial.write('2');
  Serial.println("I SENT 22222222");
  delay(1000);
}
