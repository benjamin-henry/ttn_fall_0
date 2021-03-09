#include "includes.h"

void setup() {
  Serial.begin(115200);
  while (!IMU.begin());

  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  init_model();
  task1 = t1.every(1000 / 20, t1_callback);

  led_green();
  
}

void loop() {
  axl_acq();
  analyze_mouvement();
}
