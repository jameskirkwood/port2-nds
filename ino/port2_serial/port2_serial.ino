#import "Port2.h"

void setup() {

  Serial.begin(9600);

  Port2::init();
}

void loop() {

  if (Port2::available()) {

    Serial.write(Port2::receive());
  }

  if (Serial.available()) {

    Port2::send(Serial.read());
  }
}
