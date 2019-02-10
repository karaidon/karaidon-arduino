#include <MIDI.h>
#include <Bounce2.h>
MIDI_CREATE_DEFAULT_INSTANCE();

Bounce Button_TL = Bounce();
Bounce Button_TM = Bounce();
Bounce Button_TR = Bounce();
Bounce Button_BL = Bounce();
Bounce Button_BM = Bounce();
Bounce Button_BR = Bounce();

bool isExpConnected = false;
int expConnectedFrames = 0;
int prevExpValue = 0;
int currExpCCValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

  Button_TL.attach(2);
  Button_TL.interval(5);

  Button_TM.attach(3);
  Button_TM.interval(5);

  Button_TR.attach(4);
  Button_TR.interval(5);

  Button_BL.attach(5);
  Button_BL.interval(5);

  Button_BM.attach(6);
  Button_BM.interval(5);

  Button_BR.attach(7);
  Button_BR.interval(5);
  MIDI.begin(0);
}

void loop() {
  digitalWrite(13, HIGH);
  Button_TL.update();
  Button_TM.update();
  Button_TR.update();
  Button_BL.update();
  Button_BM.update();
  Button_BR.update();
  if (Button_TL.fell())
  {
    MIDI.sendNoteOn(24, 127, 1);
    //MIDI.sendNoteOn(64, 127, 1);
  }
  else if (Button_TL.rose())
  {
    MIDI.sendNoteOff(24, 0, 1);
   // MIDI.sendNoteOff(64, 0, 1);
  }
  if (Button_BL.fell())
  {
    MIDI.sendNoteOn(62, 127, 1);
    //MIDI.sendNoteOn(62, 127, 1);
    //MIDI.sendNoteOn(63, 127, 1);
  }
  else if (Button_BL.rose())
  {
    MIDI.sendNoteOff(62, 0, 1);
    //MIDI.sendNoteOff(62, 0, 1);
    //MIDI.sendNoteOff(63, 0, 1);
  }

  if (Button_TM.fell())
  {
   // MIDI.sendNoteOn(64, 127, 2);
    MIDI.sendNoteOn(25, 127, 2);
  }
  else if (Button_TM.rose())
  {
   // MIDI.sendNoteOff(64, 0, 2);
    MIDI.sendNoteOff(25, 0, 2);
  }
  if (Button_BM.fell())
  {
    //MIDI.sendNoteOn(61, 127, 2);
    MIDI.sendNoteOn(62, 127, 2);
    //MIDI.sendNoteOn(63, 127, 2);
  }
  else if (Button_BM.rose())
  {
    //MIDI.sendNoteOff(61, 0, 2);
    MIDI.sendNoteOff(62, 0, 2);
    //MIDI.sendNoteOff(63, 0, 2);
  }
  
  if (Button_TR.fell())
  {
     MIDI.sendNoteOn(26, 127, 1);
  }
  else if (Button_TR.rose())
  {
     MIDI.sendNoteOff(26, 0, 1);
  }
  if (Button_BR.fell())
  {
    MIDI.sendNoteOn(63, 127, 3);
  }
  else if (Button_BR.rose())
  {
    MIDI.sendNoteOff(63, 0, 3);
  }

  int currExpValue = analogRead(16);
  if (isExpConnected)
  {
    if (abs(currExpValue - prevExpValue) >= 50)
    {
      isExpConnected = false;
      expConnectedFrames = 0;
      prevExpValue = currExpValue;
      delay(100);
    }
    else
    {
      int newExpCCValue = (int)round((float)currExpValue / 8.0f);
      if (newExpCCValue >= 128) newExpCCValue = 127;
      else if (newExpCCValue < 0) newExpCCValue = 0;
      prevExpValue = currExpValue;
      if (newExpCCValue != currExpCCValue)
      {
        currExpCCValue = newExpCCValue;
        MIDI.sendControlChange(48, currExpCCValue, 1);
        MIDI.sendControlChange(1, currExpCCValue, 1);
      }
      ++expConnectedFrames;
    }
  }
  else
  {
    if (prevExpValue == currExpValue)
    {
      if (expConnectedFrames >= 10) 
      {
        isExpConnected = true;
        delay(100);
      }
      ++expConnectedFrames;
    }
    else expConnectedFrames = 0;
    prevExpValue = currExpValue;
  }
  
}
