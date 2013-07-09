/**
Knobface
Author: Benedict Lee
Contact:Ben@Karaidon.com
2013
MIT License
URL: https://github.com/karaidon/karaidon-arduino

8 Encoder, 8 Rotary Pots, 1 Slide Pot, 2 LED MIDI Controller with 4 Banks
**/

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

boolean debug = false;
boolean encoderIncrementMode = false; //Uses NRPN to provide increment/decrement endless values, may not work with all DAWs.
/*Increment Mode works w/ FL Studio
Does not work w/ Live
Other DAWs not tested*/

Encoder encoder1(1, 0);
Encoder encoder2(3, 2);
Encoder encoder3(5, 4);
Encoder encoder4(7, 6);
Encoder encoder5(9, 8);
Encoder encoder6(11, 10);
Encoder encoder7(32, 12);
Encoder encoder8(15, 14);

int potPins[8] = {16,17,18,19,20,21,22,23};

int faderPin = 34; //A10
int buttonPin = 24;
int LEDPin1 = 26;
int LEDPin2 = 29;
int bankStart[4] = {10,27,44,61};

int potThreshold = 6;  //Filters away analog noise, lower = more sensitive
int faderThreshold = 10; //same as above
int changeLimit = 5; //Prevents sudden CC changes incase of lots of noise
int bounceDelay = 50; //Prevents switch bounce issues

int encoderValue[32];
int encoderRawValue[32];

int prevPotValue[8];
int prevPotRawValue[8];

int currentBank = 0;

int faderValue = 0;
int prevFaderValue = 0;

boolean buttonSwitch = false;

//#define CORE_ADC10_PIN    34
//#define CORE_ADC11_PIN		35
//#define CORE_ADC12_PIN		36
//#define CORE_ADC13_PIN		37

void setup()
{
  if (debug == false) Serial.begin(31250);
  else Serial.begin(38400);
  
  pinMode(buttonPin, INPUT_PULLUP);    // button as input
  pinMode(LEDPin1, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
  
  
  //Initialize Potentiometer values
  for(int i=0;i<8;i++)
  {
    int rawValue = analogRead(potPins[i]);
    int mapValue = map(rawValue,0,1023,-1,128);
    prevPotValue[i] = mapValue;
    prevPotRawValue[i] = rawValue;
    if (mapValue < 0) mapValue = 0;
    if (mapValue > 127) mapValue = 127;
  }
  
  int rawValue = analogRead(faderPin);
  int mapValue = map(rawValue,0,1023,-1,128);
  faderValue = mapValue;
  prevFaderValue = rawValue;
  if (mapValue < 0) mapValue = 0;
  if (mapValue > 127) mapValue = 127;
}

void loop()
{
  //Bank Change Button Check
  if (digitalRead(buttonPin) == LOW)
  {
    if (buttonSwitch == false)
    {
      incBank();
      buttonSwitch = true;      
    }
    delay(bounceDelay); //Prevents switch bounce from changing bank
  }
  else 
  {
    buttonSwitch = false;
  }
  
  //Read encoders one by one
  readEncoder(encoder1, &encoderValue[0+currentBank*8], bankStart[currentBank]+0,&encoderRawValue[0+currentBank*8]);
  readEncoder(encoder2, &encoderValue[1+currentBank*8], bankStart[currentBank]+1,&encoderRawValue[1+currentBank*8]);
  readEncoder(encoder3, &encoderValue[2+currentBank*8], bankStart[currentBank]+2,&encoderRawValue[2+currentBank*8]);
  readEncoder(encoder4, &encoderValue[3+currentBank*8], bankStart[currentBank]+3,&encoderRawValue[3+currentBank*8]);
  readEncoder(encoder5, &encoderValue[4+currentBank*8], bankStart[currentBank]+4,&encoderRawValue[4+currentBank*8]);
  readEncoder(encoder6, &encoderValue[5+currentBank*8], bankStart[currentBank]+5,&encoderRawValue[5+currentBank*8]);
  readEncoder(encoder7, &encoderValue[6+currentBank*8], bankStart[currentBank]+6,&encoderRawValue[6+currentBank*8]);
  readEncoder(encoder8, &encoderValue[7+currentBank*8], bankStart[currentBank]+7,&encoderRawValue[7+currentBank*8]);
  
  //Read Pots
  for(int i=0;i<8;i++)
  {
    int rawValue = analogRead(potPins[i]);
    int mapValue = map(rawValue,0,1023,-1,128);
    if (prevPotValue[i] != mapValue && abs(rawValue - prevPotRawValue[i]) >= potThreshold && abs(mapValue - prevPotValue[i]) <= changeLimit)
    {
     prevPotValue[i] = mapValue;
     prevPotRawValue[i] = rawValue;
     if (mapValue < 0) mapValue = 0;
     if (mapValue > 127) mapValue = 127;
     if (debug == false) usbMIDI.sendControlChange(bankStart[currentBank]+8+i,mapValue,1);
     if (debug == true)
     {
       Serial.print("Pot Change, CC: ");
       Serial.print(bankStart[currentBank]+8+i);
       Serial.print(", value: ");
       Serial.println(mapValue);
     }
    }
  }
  
  //Read Fader
  int rawValue = analogRead(faderPin);
  int mapValue = map(rawValue,0,1023,-1,128);
  if (faderValue != mapValue && abs(rawValue - prevFaderValue) >= faderThreshold && abs(mapValue - faderValue) <= changeLimit)
  {
   faderValue = mapValue;
   prevFaderValue = rawValue;
   if (mapValue < 0) mapValue = 0;
   if (mapValue > 127) mapValue = 127;
   if (debug == false) usbMIDI.sendControlChange(bankStart[currentBank]+8+8,mapValue,1);
   if (debug == true)
   {
     Serial.print("Fader Change, CC: ");
     Serial.print(bankStart[currentBank]+8+8);
     Serial.print(", value: ");
     Serial.println(mapValue);
   }
  }
  
}

void incBank()
{
  currentBank++;
  if (currentBank > 3)
  {
    currentBank = 0;
  }
  changeLED();
}

void changeLED()
{
  //Bank 1: off off
  //Bank 2: on  off
  //Bank 3: off on
  //Bank 4: on  on
  
  if (currentBank == 1 || currentBank == 3)
  {
    digitalWrite(LEDPin1, HIGH);
  }
  else
  {
    digitalWrite(LEDPin1, LOW);
  }
  if (currentBank == 2 || currentBank == 3)
  {
    digitalWrite(LEDPin2, HIGH);
  }
  else
  {
    digitalWrite(LEDPin2, LOW);
  }
}

void readEncoder(Encoder _enc, int *_value, int _cc, int *_rawValue)
{
  long encoderTempValue = _enc.read() - *_rawValue;
  *_rawValue = _enc.read();
  _enc.write(0);
  if (encoderTempValue != 0)
  {
    if (debug == false) 
    {
      if (encoderIncrementMode == true)
      {
        //Increment Mode tested to work in FL Studio
        //Does not work in Ableton Live (ver 8)
        //Other DAWs not tested
        usbMIDI.sendControlChange(99,0,1); //MSB of the NRPN. Since our desired CC is less than 128, not needed maybe?
        usbMIDI.sendControlChange(98,_cc,1); //LSB of the NRPN.
        if (encoderTempValue > 0)
        { 
          usbMIDI.sendControlChange(96,_cc,1); //CC96 is to increment data value by 1.
        }
        else if (encoderTempValue < 0)
        {
          usbMIDI.sendControlChange(97,_cc,1); //CC97 is to decrement data value by 1.
        }
      }
    }
    *_value += encoderTempValue;
    if (*_value > 127)
    {
      *_value = 127;
    }
    else if (*_value < 0)
    {
      *_value = 0;
    }
    if (debug == false) 
    {
      if (encoderIncrementMode == false)
      {
        usbMIDI.sendControlChange(_cc,*_value,1);
      }
    }
    if (debug == true)
    {
      Serial.print("Encoder Change, CC: ");
      Serial.print(_cc);
      Serial.print(", value: ");
      Serial.println(*_value);
    }
  }
}
