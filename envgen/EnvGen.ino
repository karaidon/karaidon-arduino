//a: 23/a9
//d: 22/a8
//s: 21:a7
//r: 20:a6

//a in: 19/a5
//d in: 18/a4
//s in: 17/a3
//r in: 16 /a2

//trig in/gate out: 15/a1
//env out: a14

const int trigMode = 0; //0: low to high, 1: high to low
const int maxAttackTime = 10000;
const int maxDecayTime = 10000;
const int maxReleaseTime = 10000;

int aPotInPin = A9;
int dPotInPin = A8;
int sPotInPin = A7;
int rPotInPin = A6;

int aCVInPin = A5;
int dCVInPin = A4;
int sCVInPin = A3;
int rCVInPin = A2;

int trigInPin = A1;
int envOutPin = A14;

unsigned long overallTimer = 0;
unsigned long timeThisFrame = 0;
unsigned long envTimer = 0;

unsigned int attackTime = 0;
unsigned int decayTime = 0;
unsigned int sustainLevel = 0;
unsigned int releaseTime = 0;

bool trigState = false;
int envState = 0; //0:idle, 1:attack,2:decay,3:sustain,4:release

int currentCVLevel = 0;

void setup()
{
  analogWriteResolution(12);

  pinMode(aPotInPin, INPUT);
  pinMode(dPotInPin, INPUT);
  pinMode(sPotInPin, INPUT);
  pinMode(rPotInPin, INPUT);

  pinMode(aCVInPin, INPUT_PULLUP);
  pinMode(dCVInPin, INPUT_PULLUP);
  pinMode(sCVInPin, INPUT_PULLUP);
  pinMode(rCVInPin, INPUT_PULLUP);

  pinMode(trigInPin, INPUT);
  pinMode(envOutPin, OUTPUT);

}

void loop()
{
  timeThisFrame = millis() - overallTimer;
  overallTimer = millis();

  if (trigMode == 0) //low to high
  {
    if (analogRead(trigInPin) >= 1000)
    {
      trigState = true;
    }
    else trigState = false;
  }
  else if (trigMode == 1) //high to low
  {
    if (analogRead(trigInPin) >= 1000)
    {
      trigState = false;
    }
    else trigState = true;
  }

  attackTime = long((analogRead(aPotInPin) / 1024.0) * maxAttackTime);
  decayTime = int((analogRead(dPotInPin) / 1024.0) * maxDecayTime);
  sustainLevel = analogRead(sPotInPin) * 4; // convert from 10 bit input value to the 12 bit output value
  releaseTime = int((analogRead(rPotInPin) / 1024.0) * maxReleaseTime);

  if (envState == 0)
  {
    currentCVLevel = 0;

    if (trigState == true)
    {
      envTimer = 0;
      envState += 1; //move to attack stage
    }
  }
  else if (envState == 1) //attack
  {
    envTimer += timeThisFrame;

    if (trigState == false)
    {
      currentCVLevel = sustainLevel;
      envTimer = 0;
      envState = 4; //skip to release stage
    }

    if (envTimer >= attackTime)
    {
      envTimer = 0;
      envState += 1; //move to decay stage
    }

    if (decayTime > 0) currentCVLevel = lerp(currentCVLevel, 4096, float(envTimer) / float(attackTime));
    else currentCVLevel = lerp(currentCVLevel, sustainLevel, float(envTimer) / float(attackTime));
  }
  else if (envState == 2) //decay
  {
    envTimer += timeThisFrame;

    if (trigState == false)
    {
      currentCVLevel = sustainLevel;
      envTimer = 0;
      envState = 4; //skip to release stage
    }

    if (envTimer >= decayTime)
    {
      envTimer = 0;
      envState += 1; //move to decay stage
    }

    currentCVLevel = lerp(4096, sustainLevel, float(envTimer) / float(decayTime));
  }
  else if (envState == 3) //sustain
  {
    if (trigState == false)
    {
      envTimer = 0;
      envState += 1; //move to release stage
    }

    currentCVLevel = sustainLevel;
  }
  else if (envState == 4) //release
  {
    envTimer += timeThisFrame;

    if (trigState == true)
    {
      envTimer = 0;
      envState = 1; //retrigger to attack stage
    }
    if (envTimer >= releaseTime)
    {
      envTimer = 0;
      envState = 0; //reset to idle stage
    }

    currentCVLevel = lerp(currentCVLevel, 0, float(envTimer) / float(releaseTime));
  }

  analogWrite(envOutPin, currentCVLevel);
}

int lerp(int _start, int _end, float _pos)
{
  return int(((_end - _start) * _pos) + _start);
}

