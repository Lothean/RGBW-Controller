#include <IRremote.h>
#include <IRremoteInt.h>
int irSensor = 12;
byte rgbw[] = {5, 9, 6, 10}; //change the pins here according to yours
byte currentColor[] = {255, 255, 255, 128}; // The current color, stored to make the fade effect
void setup() {
  for (int i = 2; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(irSensor, INPUT_PULLUP);
  bitSet(TCCR1B, WGM12); //enables fast PWM on ATmega 328P (might be different on others µC)
  irrecv.enableIRIn();
}

void loop() {
  fadeUpdate(currentColor);
}

void refresh () {
  for (int i = 0; i <= 3; i++) {
    analogWrite(rgbw[i], currentColor[i]);
  }
}

void fadeUpdate (byte newColor[]) {
  while (currentColor != newColor) {
    for (int i = 0; i <= 3; i++) {
      if (currentColor[i] < newColor[i]) {
        currentColor[i]++;
      }
      if (currentColor[i] > newColor[i]) {
        currentColor[i]--;
      }
      analogWrite(rgbw[i], currentColor[i]);
    }
  }
}

