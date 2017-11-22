#include <IRremote.h>
#define IR_ON    0xFF02FD
#define IR_DIY1   0xFF30CF
#define IR_DIY2   0xFFB04F
#define IR_DIY3   0xFF708F
bool isOn = true;
int irSensor = 12;
byte rgbw[] = {5, 9, 6, 10}; //change the pins here according to yours
byte currentColor[] = {0, 0, 0, 0}; // The current color, stored to make the fade effect
byte newColor[] = {255, 255, 255, 128};
byte backupColor[] = {255, 255, 255, 128};
IRrecv irrecv(irSensor);
decode_results results;
void setup() {
  for (int i = 2; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(irSensor, INPUT_PULLUP);
  bitSet(TCCR1B, WGM12); //enables fast PWM on ATmega 328P (might be different on others µC)
  irrecv.enableIRIn();
  state();
  byte newColor[] = {255, 255, 255, 128};
  fadeUpdate();
}

void loop() {
  ir();
  delay(15);
}

void refresh () {
  for (int i = 0; i <= 3; i++) {
    analogWrite(rgbw[i], currentColor[i]);
  }
}

void fadeUpdate () {
  while (currentColor[0] != newColor[0] || currentColor[1] != newColor[1] || currentColor[2] != newColor[2] || currentColor[3] != newColor[3]) {
    for (int i = 0; i <= 3; i++) {
      if (currentColor[i] < newColor[i]) {
        currentColor[i]++;
      }
      if (currentColor[i] > newColor[i]) {
        currentColor[i]--;
      }
      analogWrite(rgbw[i], currentColor[i]);
    }
    delay(5);
  }
}
void ir() {

  if (irrecv.decode(&results)) {
    if (results.value == IR_ON) {
      isOn = !isOn;
      state();
    }
    if (results.value == IR_DIY1 && isOn) {
      newColor[0] = 255;
      newColor[1] = 255;
      newColor[2] = 255;
      newColor[3] = 128;
      fadeUpdate();
    }
    if (results.value == IR_DIY2 && isOn) {
      newColor[0] = 1;
      newColor[1] = 1;
      newColor[2] = 1;
      newColor[3] = 1;
      fadeUpdate();
    }
    if (results.value == IR_DIY3 && isOn) {
      newColor[0] = 1;
      newColor[1] = 0;
      newColor[2] = 0;
      newColor[3] = 0;
      fadeUpdate();
    }
    irrecv.resume();
  }
}

void state() {
  if (isOn) {
    for (int i = 0; i <= 3; i++) {
      newColor[i] = backupColor[i];
    }
    digitalWrite(13, HIGH);
    fadeUpdate();

  }
  else {
    for (int i = 0; i <= 3; i++) {
      backupColor[i] = currentColor[i];
    }
    newColor[0] = 0;
    newColor[1] = 0;
    newColor[2] = 0;
    newColor[3] = 0;
    fadeUpdate();
    digitalWrite(13, LOW);
  }
}
