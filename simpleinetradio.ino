#include "WiFiMulti.h"
#include "Audio.h"  //see repository at github "https://github.com/schreibfaul1/ESP32-audioI2S"

#define I2S_DOUT 25  // connect to DIN PCM5102
#define I2S_BCLK 27  // connect to BCK PCM5102
#define I2S_LRC 26   // connect to LCK PCM5102
//FLT DMP SCL PCM5102 connect to GND,  XMT connect to 3V3

//global vars
Audio audio;
WiFiMulti wifiMulti;
String ssid = "***";
String password = "***";

// Создаем изменяемую копию
char buffer[64];

void setup() {
  Serial.begin(115200);
  strcpy(buffer, "http://162.13.25.174/hcrb063nn3quv"); //стартовый адрес + ID радиостанции
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(ssid.c_str(), password.c_str());
  wifiMulti.run();
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect(true);
    wifiMulti.run();
  }

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(18);  // 0...21
  audio.stopSong();
  myConnection();
  Serial.println();
}

void loop() {
  audio.loop();
}

void myConnection() {
  audio.stopSong();
  delay(500);
  audio.connecttohost(buffer);  
  digitalWrite(LED_BUILTIN, HIGH);
}


void newAddr() {
  Serial.print(">>>>>>>>>>>>");
  Serial.println(buffer);
  char* url_start = strstr(buffer, "http://");
  url_start += strlen("http://");  // Пропускаем "https://"
  Serial.print("<<<<<<<<<<<<<");
  Serial.println(url_start);
  // Копируем строку URL для обработки
  char temp[256];
  strncpy(temp, url_start, sizeof(temp) - 1);
  temp[sizeof(temp) - 1] = '\0';  // Гарантируем, что строка завершена

  // Разделяем строку на две части
  char* domain = strtok(temp, "/");  // Первая часть (доменное имя)
  char* path = strtok(NULL, "");     // Вторая часть (путь и параметры)

  Serial.print("********");
  Serial.println(domain);
  Serial.print("---------");
  Serial.println(path);

  IPAddress ip;
  WiFi.hostByName(domain, ip);
  Serial.print(" : ");
  Serial.println(ip);

  // Заменим доменное имя на IP-адрес
  char ipStr[16];  // Буфер для IP-адреса в строковом виде
  snprintf(ipStr, sizeof(ipStr), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);

  strcpy(buffer, "http://");
  strcat(buffer, ipStr);
  strcat(buffer, "/");
  strcat(buffer, path);

  size_t len = strlen(buffer);
  buffer[len - 1] = '\0';  // Ставим нулевой терминатор на место последнего символа

  Serial.print("++++++++");
  Serial.println(buffer);
}


// optional
void audio_info(const char* info) {
  Serial.print("n info        ");
  Serial.println(info);

  // Проверяем наличие подстроки " failed!
  if (strstr(info, "failed!") != NULL) {
    Serial.print("FAIL! >  ");
    digitalWrite(LED_BUILTIN, LOW);
    myConnection();
  }
  
  // Проверяем наличие подстроки "redirect"
  if (strstr(info, "redirect") != NULL) {
    Serial.println("redirect >  ");
    strcpy(buffer, info);
    newAddr();
  }
}
