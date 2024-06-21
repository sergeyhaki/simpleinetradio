//this file: simpleinetradio.ino
#include "Audio.h" //see repository at github "https://github.com/schreibfaul1/ESP32-audioI2S"

//global vars
Audio audio;
String ssid =     "*****";
String password = "*****";

String stations = "https://stream.radiojar.com/hcrb063nn3quv";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid.c_str(), password.c_str());
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(21); // 0...21
  audio.connecttohost(stations);
}

void loop() {
    audio.loop();
    tp.loop();
}


  
