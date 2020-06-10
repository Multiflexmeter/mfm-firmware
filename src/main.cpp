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

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 16
#define LORA_RESET 15
#define LORA_ON 13
#define LED_PIN 8
#define JSN_ISO_5V_ON 6
#define JSN_ISO_3V_ON 26


#define receiveTimeout 90 //for JSN serial read
#define SEND_LORA

//create an instance of the rn2xx3 library
rn2xx3 myLora(SerialLoRa);
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

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

  //wait for the serials console to open
  while (!SerialLoRa);
  while (!SerialUSB);
  while (!SerialJSN);

  SerialUSB.println("Startup");

#ifdef SEND_LORA
  initialize_radio();
#endif
  led_off();

  //start DS18B20
  sensors.begin();
}

//initialize RN2483 LoRa module
void initialize_radio()
{
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
  SerialUSB.println("Trying to join TTN");
  bool join_result = false;

  myLora.setFrequencyPlan(TTN_EU);

  //ABP: initABP(String addr, String AppSKey, String NwkSKey);
  //join_result = myLora.initABP("02017201", "8D7FFEF938589D95AAD928C2E2E7E48F", "AE17E567AECC8787F749A62F5541D522");

  //OTAA: initOTAA(String AppEUI, String AppKey);
  join_result = myLora.initOTAA("70B3D57ED002CBB4", "73AF941FD6655EE83F6E701D6CB9006D");

  while (!join_result)
  {
    SerialUSB.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(5000); //delay a minute before retry
    join_result = myLora.init();
  }
  SerialUSB.println("Successfully joined TTN");
}

// the loop routine runs over and over again forever:
void loop() {
  led_on();
  float corrected_distance = ReadCorrectedDistance();

#ifdef SEND_LORA
  SerialUSB.println("TXing");
  myLora.tx(String(corrected_distance));
#endif

  led_off();
  delay(2000);
}

void led_on()
{
  digitalWrite(LED_PIN, HIGH);
}

void led_off()
{
  digitalWrite(LED_PIN, LOW);
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

//call with delay
//read serial mode JSN ultasonic distance sensor
uint16_t readDistanceJSN() {
  byte buforek[4];
  unsigned long start;

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
  return 0;
}

//read the current ambient temperature
float ReadTemperature() {
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