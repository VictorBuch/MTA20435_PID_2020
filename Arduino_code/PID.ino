#include <Adafruit_NeoPixel.h>

#include <Adafruit_NeoPixel.h>
 
#define PIN 6
 
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
int a = 0;
int b;
int c;
int save_btn_state;
int photo;
int led_index;
int state = 1;
int led_color[3] = {255, 0, 0};
int sound_pos_guess;
int timer = 10;
bool pressed = false;
bool saving = false;

void setup() {
  strip.begin();
  strip.setBrightness(30); //adjust brightness here
  strip.show(); // Initialize all pixels to 'off'
  pinMode(6, INPUT);
  pinMode(2, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(4, INPUT);
  Serial.begin(9600);
}

void loop()
{
  a = analogRead(A3);
  b = analogRead(A2);
  c = digitalRead(2);
  save_btn_state = digitalRead(4);
  
  // State change
  if (c == HIGH && !pressed){
    pressed = true;
    if ((state + 1) > 3){
      state = 1;
    } else {
      state ++;
    }
    
    switch (state){
      case 1:
        led_color[0] = 255;
        led_color[1] = 0;
        led_color[2] = 0;
        Serial.println("red");
        break;
      case 2:
        led_color[0] = 255;
        led_color[1] = 255;
        led_color[2] = 0;
        Serial.println("green");
        break;
      case 3:
        led_color[0] = 0;
        led_color[1] = 255;
        led_color[2] = 0;
        Serial.println("blue");
        break;
    }
  }
  else if (c == LOW){
    pressed = false;
  }
  
  // Photo resistor
  photo = map(b, 7, 1023, 100, 5);
  strip.setBrightness(photo);
  
  // Led pot change
  led_index = floor(a/(1023/10));
  strip.setPixelColor(led_index, strip.Color(led_color[0], led_color[1], led_color[2]));
  strip.show();
  for (uint16_t i=0; i<strip.numPixels(); i++){
    if (led_index == i){
      continue;
    } else{
      if(!saving){
        strip.setPixelColor(i, strip.Color(0,0,0));
      }
    }
  }
  
  // save rotational position
  if (save_btn_state == HIGH){
    if (timer - 1 > 0){
      timer --;
    } else {
      timer = 0;
    }
    
    if (timer > 0){
      for (uint16_t i=0; i<strip.numPixels(); i++){
        strip.setPixelColor(i, strip.Color(led_color[0],led_color[1],led_color[2]));
      }
    }
    sound_pos_guess = map(led_index, 10, 0, 1, 11);
    Serial.println(sound_pos_guess);
  } else {
    timer = 10;
  }
  delay(10); // Delay a little bit to improve simulation performance
}
