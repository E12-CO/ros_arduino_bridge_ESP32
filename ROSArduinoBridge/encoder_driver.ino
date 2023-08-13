/* *************************************************************
   Encoder definitions

   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.

   ************************************************************ */

#include <ESP32Encoder.h>
ESP32Encoder enc0;
ESP32Encoder enc1;

void initEncoder(){
  enc0.attachSingleEdge(LEFT_ENC_PIN_A, LEFT_ENC_PIN_B);
  enc1.attachSingleEdge(RIGHT_ENC_PIN_A, RIGHT_ENC_PIN_B);
  resetEncoders();
}

/* Wrap the encoder reading function */
long readEncoderL() {
  return enc0.getCount();
}

long readEncoderR(){
  return enc1.getCount();
}

/* Wrap the encoder reset function */
void resetEncoder(int i) {
  if (i == 0) {
    enc0.setCount(0);
  } else {
    enc1.setCount(0);
  }
}

/* Wrap the encoder reset function */
void resetEncoders() {
  enc0.setCount(0);
  enc1.setCount(0);
}
