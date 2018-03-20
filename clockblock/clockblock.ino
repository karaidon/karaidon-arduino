#include <MIDI.h>
#include <toneAC.h>
#include <elapsedMillis.h>
MIDI_CREATE_DEFAULT_INSTANCE();

byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xfb;

byte data;
int play_flag = 0;
int pulseCount = 0;

elapsedMillis clickOffTimer;
int clickOffTime = 50;

int ledPin = 19;
int tonePin1 = 9;
int tonePin2 = 10;
int switchPin = 12;

int toneFreq = 1000;

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, OUTPUT);
  MIDI.turnThruOn();
  
}

void loop() {
  if (clickOffTimer >= clickOffTime)
  {
    clickOffTimer = 0;
    digitalWrite(ledPin, LOW);
    digitalWrite(switchPin, LOW);
  }
  if (MIDI.read()) {
    
    switch(MIDI.getType())
    {
      case midi::Start:
      play_flag = 1;
      pulseCount = 0;
      break;

      case midi::Continue:
      play_flag = 1;
      break;

      case midi::Stop:
      play_flag = 0;
      break;

      case midi::Clock:
      sync();
      break;
      
      default:
      break;
    }
  }
}

void sync() {
  pulseCount += 1;
  if (pulseCount > 24)
  {
    toneAC(1250, 10, clickOffTime,true);
    digitalWrite(ledPin, HIGH);
    digitalWrite(switchPin, HIGH);
    pulseCount = 0;
    clickOffTimer = 0;
  }
}
