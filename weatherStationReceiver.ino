#include <Streaming.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

//#define DEBUG

#define CE_PIN 9
#define CSN_PIN 10
#define PRINTF_BUF 20

RF24 radio(CE_PIN, CSN_PIN);
const uint64_t pipe = 0xF0F0F0F0E1LL;

char buf[PRINTF_BUF];
#ifdef DEBUG
boolean LOOP = true;
long start = millis();
#endif

void setup() {
#ifdef DEBUG
  long start = millis();
#endif
  Serial.begin(115200);
  setupRF24();
#ifdef DEBUG
  Serial << "setup done: " << millis() - start << " msec" << endl;
#endif
}

void loop() {
#ifdef DEBUG
  if (LOOP) {
    Serial << "loop started" << endl;
    LOOP = false;
    start = millis();
  }
#endif
  while (radio.available()) {
    uint8_t len = radio.getDynamicPayloadSize();
    radio.read(buf, len);
    buf[len] = '\0';
    Serial << buf << endl;
#ifdef DEBUG
    LOOP = true;
    Serial << "loop done: " << millis() - start << " msec" << endl;
#endif
  }
}

void setupRF24() {
  printf_begin();
  radio.begin();
  radio.setAutoAck(1);
  radio.setRetries(15, 15);
  radio.enableDynamicPayloads();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
#ifdef DEBUG
  radio.printDetails();
#endif
}

