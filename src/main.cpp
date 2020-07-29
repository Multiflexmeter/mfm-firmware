/*
   Author: Mick van Eerd
   Date: 2020-03-24

   Test sketch for integration of JSN-SR04t-2.0, DS18B20 and RN2483.

    LoRa transmit code is based on rn2xx3 library's ESP8266 example by JP Meijers (2016-10-20)
*/

//#include <Arduino.h>   // required before wiring_private.h
//#include "wiring_private.h" // pinPeripheral() function
#include <OneWire.h>
#include <DallasTemperature.h>
#include <rn2xx3.h>
#include <ArduinoLowPower.h>

#define DEBUG

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 16
#define LORA_RESET 15
#define LORA_ON 13
#define LED_PIN 8
#define JSN_ISO_5V_ON 6
#define JSN_ISO_3V_ON 26


#define receiveTimeout 90 //for JSN serial read
//#define SEND_LORA

//create an instance of the rn2xx3 library
rn2xx3 myLora(SerialLoRa);
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//initialize RN2483 LoRa module
void initialize_radio()
{
  #ifdef DEBUG
      Serial.println("Starting LoRa initialization sequence");
  #endif
  //reset RN2xx3
  digitalWrite(LORA_RESET, LOW);
  delay(200);
  digitalWrite(LORA_RESET, HIGH);

  delay(100); //wait for the RN2xx3's startup message
  SerialLoRa.flush();

  //check communication with radio
  String hweui = myLora.hweui();
  while (hweui.length() != 16)
  {
    SerialUSB.println("Communication with RN2xx3 unsuccessful. Power cycle the board.");
    SerialUSB.println(hweui);
    delay(300);
    hweui = myLora.hweui();
  }

  //print out the HWEUI so that we can register it via ttnctl
  SerialUSB.println("When using OTAA, register this DevEUI: ");
  SerialUSB.println(hweui);
  SerialUSB.println("RN2xx3 firmware version:");
  SerialUSB.println(myLora.sysver());

  //configure your keys and join the network
  SerialUSB.println("Trying to join KPN");
  bool join_result = false;

  myLora.setFrequencyPlan(TTN_EU);

  //ABP: initABP(String addr, String AppSKey, String NwkSKey);
  //join_result = myLora.initABP("<addr>", "<AppKey>", "<NwkSKey>");

  //OTAA: initOTAA(String AppEUI, String AppKey);
  join_result = myLora.initOTAA("<AppEUI>", "<AppKey>");

  while (!join_result)
  {
    SerialUSB.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(5000); //delay a minute before retry
    join_result = myLora.init();
  }
  SerialUSB.println("Successfully joined TTN");
  #ifdef DEBUG
      Serial.println("Ended LoRa initialization sequence");
  #endif
}

void led_on()
{
  #ifdef DEBUG
      Serial.println("LED on");
  #endif
  digitalWrite(LED_PIN, HIGH);
}

void led_off()
{
  #ifdef DEBUG
      Serial.println("LED off");
  #endif
  digitalWrite(LED_PIN, LOW);
}

//call with delay
//read serial mode JSN ultasonic distance sensor
uint16_t readDistanceJSN() {
  byte buforek[4];
  unsigned long start;

  #ifdef DEBUG
      Serial.println("Starting distance measurement");
  #endif

  SerialJSN.write(0x55);                                                //request distance measurement
  delay(5);
  start = millis();
  SerialJSN.flush();
  while (!(SerialJSN.available() > 0)) {                                  //wait for data
    delay(10);
    if ((start + receiveTimeout) < millis()) {                         //but dont wait too long
      return 0;
    }
  }
  for (int i = 0; i < 4; i++) {                                     //read 4 bytes which contain distance data
    buforek[i] = (byte) SerialJSN.read();
    SerialJSN.print("in:"); SerialJSN.println(buforek[i], HEX);
    delay(2);
  }

  if (buforek[0] == 0xff && ((buforek[0] + buforek[1] + buforek[2]) & 0x00ff) == buforek[3]) //SUM check
  {
    return ((uint16_t)buforek[1] << 8) + buforek[2];                 //Two's complement make 16 bit int
  }
  #ifdef DEBUG
      Serial.println("Ended distance measurement");
  #endif
  return 0;
}

//read the current ambient temperature
float ReadTemperature() {
#ifdef DEBUG
      Serial.println("Starting temperature measurement");
#endif
  //  SerialUSB.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  //  SerialUSB.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if (tempC != DEVICE_DISCONNECTED_C)
  {
    //    SerialUSB.print("Temperature for the device 1 (index 0) is: ");
    //    SerialUSB.println(tempC);
    return tempC;
  }
  else
  {
    //    SerialUSB.println("Error: Could not read temperature data");
    return 0;
  }
}

//correct distance measured for temperature
float CorrectDistance(uint16_t dist, float temp) {
  float d2 = temp;
  float e2 = dist;

  return e2 - (e2 * ((340 - (331.3 * sqrt(1 + (d2 / 273.15) ))) / 340));
}

//read distance and correct for ambient temperature
float ReadCorrectedDistance() {
  uint16_t distance_raw = readDistanceJSN();
  float temperature_raw = ReadTemperature();
  float corrected_distance = CorrectDistance(distance_raw, temperature_raw);
  SerialUSB.print("Distance raw: ");
  SerialUSB.print(distance_raw);
  SerialUSB.print(", temperature raw: ");
  SerialUSB.print(temperature_raw);
  SerialUSB.print(", corrected distance: ");
  SerialUSB.println(corrected_distance);
  return corrected_distance;
}

void wakeup() {
  // This function will be called once on device wakeup
  // You can do some little operations here (like changing variables which will be used in the loop)
  // Remember to avoid calling delay() and long running functions since this functions executes in interrupt context

  // Attach USB and serial interface and wait for serial port to open
  USBDevice.attach(); 
  Serial.begin(9600);
  //while(!SerialUSB);
  //while(!Serial);
  delay(100);

  SerialUSB.println("Wake up!");
  #ifdef DEBUG
    Serial.println("Wake up!");
  #endif

  digitalWrite(LORA_ON, LOW);
  digitalWrite(JSN_ISO_5V_ON, LOW);
  digitalWrite(JSN_ISO_3V_ON, LOW);
}

void sleep(){
  digitalWrite(LORA_ON, HIGH);
  digitalWrite(JSN_ISO_5V_ON, HIGH);
  digitalWrite(JSN_ISO_3V_ON, HIGH);

  SerialUSB.println("Entering deep sleep mode.");
  #ifdef DEBUG
    Serial.println("Entering deep sleep mode.");
  #endif
	// Detach serial interface and enter deep sleep mode
  SerialUSB.flush();
  Serial.flush();
  USBDevice.detach(); 
  Serial.end();

  LowPower.sleep(4000);   //will call wakeup() with RTC interrupt
}

// the setup routine runs once when you press reset:
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(LORA_ON, OUTPUT);
  pinMode(LORA_RESET, OUTPUT);
  pinMode(JSN_ISO_5V_ON, OUTPUT);
  pinMode(JSN_ISO_3V_ON, OUTPUT);

  //turn on lora module and JSN
  digitalWrite(LORA_ON, LOW);
  digitalWrite(JSN_ISO_5V_ON, LOW);
  digitalWrite(JSN_ISO_3V_ON, LOW);
  led_on();

  // Open serial communications and wait for port to open:
  SerialUSB.begin(9600);
  SerialLoRa.begin(57600);
  SerialJSN.begin(9600);
  Serial.begin(9600);

  //wait for the serials console to open
  while (!SerialLoRa);
  //while (!SerialUSB);
  while (!SerialJSN);
  while (!Serial);
#ifdef DEBUG
  Serial.println("Startup");
#endif

SerialUSB.println("Startup");

#ifdef SEND_LORA
  initialize_radio();
#endif
  led_off();

  //start DS18B20
  sensors.begin();

  LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, wakeup, CHANGE);
}

// the loop routine runs over and over again forever:
void loop() {
  led_on();
#ifdef SEND_LORA
  float corrected_distance = ReadCorrectedDistance();
  SerialUSB.println("TXing");
  Serial.println("TXing");
  myLora.tx(String(corrected_distance));
#else
  delay(2000);
#endif
  led_off();

  sleep();
}
