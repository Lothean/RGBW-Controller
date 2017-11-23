#include <IRremote.h>
#define IR_ON    0xFF02FD         // IR codes of your remote, here, I've used the codes of the standard RGB LED strip controller remote
#define IR_BPlus  0xFF3AC5        // All the codes are in hexadecimal, but it might work as well with any other base
#define IR_BMinus 0xFFBA45
#define IR_PAUSE    0xFF827D
#define IR_ON    0xFF02FD
#define IR_R    0xFF1AE5
#define IR_G    0xFF9A65
#define IR_B      0xFFA25D
#define IR_W    0xFF22DD
#define IR_B1   0xFF2AD5
#define IR_B2   0xFFAA55
#define IR_B3   0xFF926D
#define IR_B4   0xFF12ED
#define IR_B5   0xFF0AF5
#define IR_B6   0xFF8A75
#define IR_B7   0xFFB24D
#define IR_B8   0xFF32CD
#define IR_B9   0xFF38C7
#define IR_B10    0xFFB847
#define IR_B11    0xFF7887
#define IR_B12    0xFFF807
#define IR_B13    0xFF18E7
#define IR_B14    0xFF9867
#define IR_B15    0xFF58A7
#define IR_B16    0xFFD827
#define IR_UPR    0xFF28D7
#define IR_UPG    0xFFA857
#define IR_UPB    0xFF6897
#define IR_QUICK  0xFFE817
#define IR_DOWNR  0xFF08F7
#define IR_DOWNG  0xFF8877
#define IR_DOWNB  0xFF48B7
#define IR_SLOW   0xFFC837
#define IR_DIY1   0xFF30CF
#define IR_DIY2   0xFFB04F
#define IR_DIY3   0xFF708F
#define IR_AUTO   0xFFF00F
#define IR_DIY4   0xFF10EF
#define IR_DIY5   0xFF906F
#define IR_DIY6   0xFF50AF
#define IR_FLASH  0xFFD02F
#define IR_JUMP3  0xFF20DF
#define IR_JUMP7  0xFFA05F
#define IR_FADE3  0xFF609F
#define IR_FADE7  0xFFE01F
bool isOn = true;
int white = 5;
int irSensor = 12; // The input pin for your IR sensor
byte rgbw[] = {5, 9, 6, 10}; //change the pins here according to yours in this order (Red, Green, Blue, White)
byte currentColor[] = {0, 0, 0, 0}; // The current color, stored to make the fade effect (set to zero at first to make the fade effect at first launch)
byte newColor[] = {255, 255, 255, 128}; // The target color, stored to make the fade effect
byte backupColor[] = {255, 255, 255, 128}; // Backup of the old current color, used to make the ON/OFF fade effect
IRrecv irrecv(irSensor);
decode_results results;
void setup() {
  for (int i = 2; i <= 13; i++) { // We define all pins from 2 to 13 as outputs, it's not very clean but hey it works
    pinMode(i, OUTPUT);
  }
  pinMode(irSensor, INPUT_PULLUP); // Since the IR sensor requires a pull-up resistor on its output, we use the internal one (Might not work with your µC, tested with 328P)
  bitSet(TCCR1B, WGM12); //enables fast PWM on ATmega 328P (might be different on others µC)
  irrecv.enableIRIn();
  state();
  byte newColor[] = {255, 255, 255, 128}; // The first color to come up on the Arduino start
  fadeUpdate(); // Apply the color
}

void loop() {
  ir(); // This function is called everytime in the loop to handle IR signals and use them
  delay(15);
}

void refresh () { // Not used at the moment, but might be useful
  for (int i = 0; i <= 3; i++) {
    analogWrite(rgbw[i], currentColor[i]);
  }
}

void fadeUpdate () { // This function takes the color in the global variable newColor and apply it with a crossfade effect based on the currentColor
  while (currentColor[0] != newColor[0] || currentColor[1] != newColor[1] || currentColor[2] != newColor[2] || currentColor[3] != newColor[3]) { // While both colors are not the same, dirty code, looking for improvements
    for (int i = 0; i <= 3; i++) {        // We check through the colors with a for loop, compare them, increase or decrease them and apply the color to make the fade effect
      if (currentColor[i] < newColor[i]) {
        currentColor[i]++;
      }
      if (currentColor[i] > newColor[i]) {
        currentColor[i]--;
      }
      analogWrite(rgbw[i], currentColor[i]);
    }
    delay(5); // Adjust this delay to set your fade speed
  }
}
void ir() { // IR handling

  if (irrecv.decode(&results)) {
    if (results.value == IR_ON) { // When the power switch is pressed on the remote, we commute the global state of the lights, and call the state function
      isOn = !isOn;
      state();
    }
    if (results.value == IR_DIY1 && isOn) { // All of these are colors (RGBW), you can copy this function, change the "IR_DIYX" to the desired code (look in the variables), change the color and voilà
      newColor[0] = 255;
      newColor[1] = 255;
      newColor[2] = 255;
      newColor[3] = 128;
      fadeUpdate();
    }
    if (results.value == IR_DIY2 && isOn) {
      newColor[0] = 128;
      newColor[1] = 128;
      newColor[2] = 128;
      newColor[3] = 64;
      fadeUpdate();
    }
    if (results.value == IR_DIY3 && isOn) {
      newColor[0] = 64;
      newColor[1] = 64;
      newColor[2] = 64;
      newColor[3] = 32;
      fadeUpdate();
    }
    if (results.value == IR_DIY4 && isOn) {
      newColor[0] = 16;
      newColor[1] = 16;
      newColor[2] = 16;
      newColor[3] = 8;
      fadeUpdate();
    }
    if (results.value == IR_DIY5 && isOn) {
      newColor[0] = 1;
      newColor[1] = 1;
      newColor[2] = 1;
      newColor[3] = 1;
      fadeUpdate();
    }
    if (results.value == IR_DIY6 && isOn) {
      newColor[0] = 1;
      newColor[1] = 0;
      newColor[2] = 0;
      newColor[3] = 0;
      fadeUpdate();
    }
    if (results.value == IR_R && isOn) {
      newColor[0] = 255;
      newColor[1] = 0;
      newColor[2] = 0;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_G && isOn) {
      newColor[0] = 0;
      newColor[1] = 255;
      newColor[2] = 0;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B && isOn) {
      newColor[0] = 0;
      newColor[1] = 0;
      newColor[2] = 255;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_W && isOn) {
      newColor[0] = 255;
      newColor[1] = 255;
      newColor[2] = 255;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B1 && isOn) {
      newColor[0] = 255;
      newColor[1] = 165;
      newColor[2] = 0;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B2 && isOn) {
      newColor[0] = 144;
      newColor[1] = 238;
      newColor[2] = 144;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B3 && isOn) {
      newColor[0] = 25;
      newColor[1] = 25;
      newColor[2] = 112;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B4 && isOn) {
      newColor[0] = 220;
      newColor[1] = 217;
      newColor[2] = 205;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B5 && isOn) {
      newColor[0] = 255;
      newColor[1] = 215;
      newColor[2] = 0;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B6 && isOn) {
      newColor[0] = 0;
      newColor[1] = 255;
      newColor[2] = 255;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B7 && isOn) {
      newColor[0] = 72;
      newColor[1] = 61;
      newColor[2] = 139;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B8 && isOn) {
      newColor[0] = 255;
      newColor[1] = 192;
      newColor[2] = 203;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B9 && isOn) {
      newColor[0] = 255;
      newColor[1] = 255;
      newColor[2] = 0;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B10 && isOn) {
      newColor[0] = 173;
      newColor[1] = 216;
      newColor[2] = 230;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B11 && isOn) {
      newColor[0] = 160;
      newColor[1] = 32;
      newColor[2] = 240;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B12 && isOn) {
      newColor[0] = 220;
      newColor[1] = 255;
      newColor[2] = 220;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B13 && isOn) {
      newColor[0] = 255;
      newColor[1] = 255;
      newColor[2] = 224;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B14 && isOn) {
      newColor[0] = 135;
      newColor[1] = 206;
      newColor[2] = 250;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B15 && isOn) {
      newColor[0] = 165;
      newColor[1] = 42;
      newColor[2] = 42;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_B16 && isOn) {
      newColor[0] = 220;
      newColor[1] = 220;
      newColor[2] = 255;
      defineWhite();
      fadeUpdate();
    }
    if (results.value == IR_PAUSE && isOn) {
      bool whiteGoingPositive;
      if (white == 5) {
        whiteGoingPositive = false;
      }
      if (white == 0) {
        whiteGoingPositive = true;
      }
      if (whiteGoingPositive) {
        white++;
      }
      else {
        white--;
      }
      defineWhite();
      fadeUpdate();
    }
    irrecv.resume();
  }
}

void state() { // This function is a bit special, it makes the ON/OFF fade, but also turns HIGH and LOW the pin 13 for PSU's that can be enabled/disabled with a logic signal (for example: Xbox 360 PSU'S)
  if (isOn) { // if you use this pin, make sure that the HIGH and LOW values are in the right place, as it might be inverted for you
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

void defineWhite() {
  switch (white) {
    case 0:
      newColor[3] = 0;
      break;
    case 1:
      newColor[3] = 8;
      break;
    case 2:
      newColor[3] = 16;
      break;
    case 3:
      newColor[3] = 32;
      break;
    case 4:
      newColor[3] = 64;
      break;
    case 5:
      newColor[3] = 128;
      break;
  }
}
