#include "BlackLED_Mk2_Boilerplate\BlackLEDMk2.h"

uint8_t mac[6];
uint8_t ip[4] = {2,0,0,2};
uint16_t udpPort = 8080;

int state = 0;
int good = 0;

#define _master_

void setup()
{
  for (int i = 0; i < 16; i++) {
    pinMode(LED_PIN[i],INPUT);
  }
  for (int i = 0; i < 8; i++) {
    pinMode(GPIO_PIN[i], INPUT);
  }
  #ifdef _master_
  ip[3] = 1;
  Serial.begin(115200);
  delay(1000);
  Serial.println("Begin");
  pinMode(BTN0, INPUT_PULLUP);
  #endif

  uint64_t mac_addr = teensyMAC();
  mac[0] = (mac_addr >> 8*5) & 0xFF;
  mac[1] = (mac_addr >> 8*4) & 0xFF;
  mac[2] = (mac_addr >> 8*3) & 0xFF;
  mac[3] = (mac_addr >> 8*2) & 0xFF;
  mac[4] = (mac_addr >> 8*1) & 0xFF;
  mac[5] = (mac_addr >> 8*0) & 0xFF;

  Ethernet.begin(mac, ip);
  udp.begin(udpPort);

}//setup


#ifdef _master_
void loop()
{
  if (digitalRead(BTN0) == LOW) {
    good = 0;
    Serial.print("\n \n \n\n \n \n\n \n \n\n \n \n\n \n \n\n \n \n\n \n \n");
    Serial.println("Begining test");
    for (int i = 0; i < 16; i++) {
      pinMode(LED_PIN[i], INPUT_PULLUP);
      Serial.printf("testing LED_PIN %d", i);
      for(int j = 0; j < 100; j++) {
        delay(5);
        state = 0;
        Serial.print(".");
        if (digitalRead(LED_PIN[i]) == LOW) {
          state = 1;
          good++;
          Serial.println("OK");
          break;
        }
      }
      if (state == 0) {
        Serial.println("FAIL!!!!!!!");
      }
      pinMode(LED_PIN[i],INPUT);
    }
    for (int i = 0; i < 8; i++) {
      pinMode(GPIO_PIN[i], INPUT_PULLUP);
      Serial.printf("testing GPIO_PIN %d", i);
      for(int j = 0; j < 100; j++) {
        delay(5);
        state = 0;
        Serial.print(".");
        if (digitalRead(GPIO_PIN[i]) == LOW) {
          state = 1;
          good++;
          Serial.println("OK");
          break;
        }
      }
      if (state == 0) {
        Serial.println("FAIL!!!!!!!");
      }
      pinMode(GPIO_PIN[i],INPUT);
    }
    Serial.print("Ethernet test");
    state = 0;
    for(int j = 0; j < 100; j++) {
      udp.beginPacket(IPAddress(2,0,0,2), udpPort);
      udp.write("Marko!");
      udp.endPacket();
      Serial.print(".");
      if(udp.parsePacket()) {
        udp.read(udp_buffer,6);
        if( memcmp(udp_buffer, "Polo!", 6) == 0) {
          state = 1;
          good++;
          Serial.println("OK");
          break;
        }
      }
    }
    if (state == 0) {
      Serial.println("FAIL!!!!!!!");
    }
    if (good == 25) {
      Serial.println("Test OK");
    }else {
      Serial.println("Test FAIL!!!!!!!");
    }
  }
  delay(100);
}//loop

#else
void loop()
{
  if (udp.parsePacket()) {
    udp.read(udp_buffer,7);
    if( memcmp(udp_buffer, "Marko!", 7) == 0) {
      udp.beginPacket(IPAddress(2,0,0,1), udpPort);
      udp.write("Polo!");
      udp.endPacket();
    }
  }
  for (int i = 0; i < 16; i++) {
    if (digitalRead(LED_PIN[i]) == HIGH) {
      pinMode(LED_PIN[i], OUTPUT);
      digitalWrite(LED_PIN[i],LOW);
      delay(2);
      pinMode(LED_PIN[i],INPUT);
    }
  }
  for (int i = 0; i < 8; i++) {
    if (digitalRead(GPIO_PIN[i]) == HIGH) {
      pinMode(GPIO_PIN[i], OUTPUT);
      digitalWrite(GPIO_PIN[i],LOW);
      delay(2);
      pinMode(GPIO_PIN[i], INPUT);
    }
  }
}//loop
#endif
