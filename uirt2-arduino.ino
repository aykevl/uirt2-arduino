// UIRT2 protocol implementation for Arduino, using the IRremote library.
//
// If you're running Linux, use LIRC with driver "uirt2" and device
// /dev/ttyACM0 (or similar).
//
// Inspired by:
//   http://trythistv.com/viewtopic.php?t=174
// Here is the protocol specification:
//   https://www.promixis.com/forums/showthread.php?1576-UIRT2-Protocol-for-Jon-Rhees-v-1-6-firmware
//
// This Arduino sketch is released in the public domain. Attribution is
// appreciated (Ayke van Laethem, aykevanlaethem@gmail.com).


#include <IRremote.h>

int RECV_PIN = 11; // set this to the pin connected to the ir receiver

IRrecv irrecv(RECV_PIN);

void setup()
{
  // Baud rate as per the spec
  Serial.begin(115200);

  // Start the receiver
  irrecv.enableIRIn();
}

void loop() {
  decode_results results;
  if (irrecv.decode(&results)) {
    // Send using the original UIR protocol. Each code is sent as six bytes of random data per key.
    uint32_t value = results.value;
    for (uint8_t i=4; i; i--) {
      Serial.write(uint8_t(value));
      value >>= 8;
    }
    // The results only have 4 bytes, but we need to send 6.
    Serial.write(0);
    Serial.write(0);

    // Wait "multiple milliseconds (at least)" as per the spec. This separates
    // transmissions to the host.
    // This delay was randomly chosen.
    delay(5);

    // Continue receiving the next value.
    irrecv.resume();
  }
}
