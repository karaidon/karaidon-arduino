
/**
Copypasta
Author: Benedict Lee
Contact:Ben@Karaidon.com
2013
MIT License
URL: https://github.com/karaidon/karaidon-arduino

3 Button Keyboard Shortcuts HID Device
**/

#include <DigiKeyboard.h>

boolean debug = false;

int copyPin = 0;
int cutPin = 5;
int pastePin = 2;

boolean copyPinDown = false;
boolean cutPinDown = false;
boolean pastePinDown = false;

unsigned long waitTime = 500;
unsigned long pasteTimer = 0;
unsigned long copyTimer = 0;
unsigned long cutTimer = 0;

void setup()
{
  pinMode(copyPin, INPUT);
  pinMode(cutPin, INPUT);
  pinMode(pastePin, INPUT);
  digitalWrite(copyPin, HIGH);
  digitalWrite(cutPin, HIGH);
  digitalWrite(pastePin,HIGH);
  DigiKeyboard.update();
}

void loop()
{
  DigiKeyboard.update();
  
  if (digitalRead(copyPin) == LOW)
  {
    if (millis() - copyTimer >= waitTime) 
    {
     if (debug == false)
     {
      DigiKeyboard.sendKeyStroke(KEY_C, MOD_CONTROL_LEFT);
      DigiKeyboard.sendKeyStroke(0,0); //Seems to fix the bug where the PC still thinks the CTRL key is held down
     }
      if (debug == true) DigiKeyboard.println("Copy Press");
    }
    if (copyPinDown == false) copyTimer = millis();
    copyPinDown = true;
  }
  if (digitalRead(copyPin) == HIGH)
  {
    if (copyPinDown == true) copyPinDown = false;
    copyTimer = 0;
    if (debug == true) DigiKeyboard.println("Copy Release");
  }
  if (digitalRead(cutPin) == LOW)
  {
    if (millis() - cutTimer >= waitTime) 
    {
     if (debug == false)
     {
      DigiKeyboard.sendKeyStroke(KEY_X, MOD_CONTROL_LEFT);
      DigiKeyboard.sendKeyStroke(0,0);
     }
     if (debug == true) DigiKeyboard.println("Cut Press");
    }
    if (cutPinDown == false) cutTimer = millis();
    cutPinDown = true;
  }
  if (digitalRead(cutPin) == HIGH)
  {
    if (cutPinDown == true) cutPinDown = false;
    cutTimer = 0;
    if (debug == true) DigiKeyboard.println("Cut Release");
  }
  if (digitalRead(pastePin) == LOW)
  {
    if (millis() - pasteTimer >= waitTime) 
    {
      if (debug == false)
      {
        DigiKeyboard.sendKeyStroke(KEY_V, MOD_CONTROL_LEFT);
        DigiKeyboard.sendKeyStroke(0,0);
      }
      if (debug == true) DigiKeyboard.println("Paste Press");
    }
    if (pastePinDown == false) pasteTimer = millis();
    pastePinDown = true;
  }
  if (digitalRead(pastePin) == HIGH)
  {
    if (pastePinDown == true) pastePinDown = false;
    pasteTimer = 0;
    if (debug == true) DigiKeyboard.println("Paste Release");
  }
}
