#include <TFT.h>
#include <SPI.h.>

#define LCD_CS   10
#define SD_CS 4
#define DC   9
#define RESET  8

#define vasen 7
#define oikea 6
#define ylos 3
#define alas 2

#define screenWidth 160
#define screenHeigth 128
#define tileSize 6

int neliox = 0;
int nelioy = 0;
int oldneliox = 0;
int oldnelioy = 0;



TFT screen = TFT(LCD_CS, DC, RESET);


void setup() {
  screen.begin();
  screen.background(255, 255, 255);

}

void loop() {

  int myWidth = TFTscreen.width();
  int myHeight = TFTscreen.height();

}


void liiku() {
  
}
