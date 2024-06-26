#include "WiFiMulti.h"
#include "Audio.h"  //see repository at github "https://github.com/schreibfaul1/ESP32-audioI2S"

#define I2S_DOUT 25  // connect to DIN PCM5102
#define I2S_BCLK 27  // connect to BCK PCM5102
#define I2S_LRC 26   // connect to LCK PCM5102
//FLT DMP SCL PCM5102 connect to GND,  XMT connect to 3V3

//global vars
Audio audio;
WiFiMulti wifiMulti;
String ssid = "****";
String password = "****";

String stations = "https://n11.radiojar.com/hcrb063nn3quv?rj-ttl=5&rj-tok=AAABkDzB7xUA_lAB0I6_wEQ4fg";


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(ssid.c_str(), password.c_str());
  wifiMulti.run();
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect(true);
    wifiMulti.run();
  }
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(12);  // 0...21


  audio.connecttohost(stations.c_str());
  //audio.connecttohost("https://n11.radiojar.com/hcrb063nn3quv?rj-ttl=5&rj-tok=AAABkDzB7xUA_lAB0I6_wEQ4fg");
}

void loop() {
  audio.loop();
}
