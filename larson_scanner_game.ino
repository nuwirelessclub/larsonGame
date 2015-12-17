/* Pin Definitions */
//LED Pins (scanner)
int A = 5;
int B = 6;
int C = 7;
int D = 8;
int E = 9;
int F = 10;
int G = 11;
int H = 12;

//Encoder Pins (speed knob)
int EncA = 3;
int EncB = 4;

int Button = 1;

volatile int encoderPos = 0;

int LedPins[] = {5,6,7,8,9,10,11,12};
int speeds[] = {100, 80, 64, 48, 38, 32, 24, 16};
byte leds = 0;

void(* resetFunc) (void) = 0; //declare reset function @ address 0
 
void setup() 
{
  //digitalWrite(Reset, HIGH);
  //pinMode(Reset, OUTPUT);
  
  //LED Outputs (scanner)
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(H, OUTPUT);

  //Encoder Inputs (speed knob)
  pinMode(EncA, INPUT);
  digitalWrite(EncA, HIGH); //pullup resistor
  pinMode(EncB, INPUT); 
  digitalWrite(EncB, HIGH); //pullup resistor

  pinMode(Button, INPUT);
  
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(2), onButtonPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(3), readEncoder, CHANGE);

  sei();
}
 
void loop() 
{
  for (int i = 0; i < 8; i++)
  {
    leds = 0x01 << i;
    updateLeds(leds);
    Serial.println(encoderPos);
    delay(speeds[encoderPos]);
  }
  for (int j = 6; j > 0; j--)
  {
    leds = 0x01 << j;
    updateLeds(leds);
    Serial.println(encoderPos);
    delay(speeds[encoderPos]);
  }
}

void updateLeds(byte leds)
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(LedPins[i], (leds >> i) & 0x01);
  }
}

void winSequence()
{
  delay(300); //so you can see which one you got
  
  for (int i = 0; i < 5; i++)
    {
      updateLeds(0x00);
      delay(100);
      updateLeds(0xff);
      delay(100);
    }
    resetFunc();
}

void loseSequence()
{
  int i = 0;
  
  delay(300); //so you can see which one you got
  
  while (i < 5)
  {
    updateLeds(0x00);
    delay(100);
    updateLeds(0x81);
    delay(100);
    i++;
  }
  resetFunc();
}

//Button ISR
void onButtonPressed()
{
  Serial.println("BUTTON");
  if (leds & 0x18) //stopped on GREEN
  {
    sei();
    winSequence();
  }

  else //stopped on YELLOW or RED
  {
    sei();
    loseSequence();
  }
}

//Encoder ISR
void readEncoder()
{ 
  Serial.println("ENCODER");
  if (digitalRead(EncA) == digitalRead(EncB)) //clockwise
  {
    if (++encoderPos > 7) encoderPos = 7;
  }
  else //counter-clockwise
  {
    if (--encoderPos < 0) encoderPos = 0;
  }
}

