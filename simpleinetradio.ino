//this file: simpleinetradio.ino
#include "Audio.h" //see repository at github "https://github.com/schreibfaul1/ESP32-audioI2S"

#define I2S_DOUT      25 // connect to PCM5102
#define I2S_BCLK      27 // connect to PCM5102
#define I2S_LRC       26 // connect to PCM5102

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
  audio.connecttohost(stations.c_str());
}

void loop() {
    audio.loop();    
}


  
