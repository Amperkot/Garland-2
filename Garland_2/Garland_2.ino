// Подключаем необходимые библиотеки

#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

// Указываем данные для управления светодиодами
#define LED_PIN 2
#define NUMPIXELS 8

// Инициализируем светодиоды
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800); 

// Инициализируем веб-сервер
AsyncWebServer server(80);

char MODE = 'х';

// Функция для окрашивания всех светодиодов
void color(int R,int G, int B)
{

  pixels.clear();
  for(int i=0;i<NUMPIXELS;i++){

    pixels.setPixelColor(i, pixels.Color(R,G,B));
  }

   pixels.show(); 
} 

// Функция для мигания светодиодами
void color_blink(int delay_time)
{

  int R = 0,G = 0, B = 0;
  
  switch (MODE)
  {
    case 'R':
      R = 255;
      break;
    case 'G':
      G = 255;
      break;
    case 'B':
      B = 255;
      break;
    default:
      break;
  }
  color(R,G,B);
  delay(delay_time);
  color(0,0,0);
  delay(delay_time);
}



void setup() {
  WiFi.softAP("Garland", "1234567890");
  pixels.begin();

    if(!SPIFFS.begin()){
    return;
  }


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);
  });
  
  // Метод обработки запроса на смену цвета
  // Запрос выглядит так: 192.168.4.1/post?color=цвет`, где вместо параметра `цвет` можно указать
  server.on("/post", HTTP_GET, [](AsyncWebServerRequest *request){
        String message;
        message = request->getParam("color")->value(); // Получаем значение цвета
        MODE = message[0]; // Получаем первую букву
        request->send(SPIFFS, "/index.html", String(), false);
    });

   server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
  });
  
  server.begin();
  
}

void loop() {
  color_blink(300);
}
