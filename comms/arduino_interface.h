#ifndef ARDUINO_INTERFACE_250205
#define ARDUINO_INTERFACE_250205

enum value{HIGH, LOW, OUTPUT};

void pinMode(int pin, int value);
void digitalWrite(int pin, int value);

void setup();
void loop();

#endif
