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
#define eimitaan 0

#define screenWidth 160
#define screenHeight 128
#define tileSize 6

const int anag0 = A0;
const int anog1 = A1;
const int anog2 = A2;
const int anog3 = A3;
int inputA = 0;
int inputB = 0;
int inputC = 0;
int inputD = 0;


int neliox = 70;
int nelioy = 90;
int oldneliox = 0;
int oldnelioy = 0;
int nelionopeus = 1;
int nelionsuunx = 1;
int nelionsuuny = 1;



TFT screen = TFT(LCD_CS, DC, RESET);


void setup() {
  screen.begin();
  Serial.begin(9600);
  screen.background(0, 255, 0);
  screen.stroke(255, 255, 2);
  screen.fill(255,0,0);

}

void loop() {

inputA = map(analogRead(anag0), 0, 1023, 0, 255);
inputB = map(analogRead(anog1), 0, 1023, 0, 255);
inputC = map(analogRead(anog2), 0, 1023, 0, 255);
inputD = map(analogRead(anog3), 0, 1023, 0, 255);
Serial.print(inputA);
Serial.print("       ");
Serial.print(inputB);
Serial.print("       ");
Serial.print(inputC);
Serial.print("       ");
Serial.print(inputD);
Serial.print("\n");

int scwidth = screen.width();
int scheight = screen.height();


platforms(111, 80, 70, 100);

if (millis() % nelionopeus < 2){
  liiku();
}


  
}


void liiku() {
  
  int A = 0;
  int B = 0;
  int C = 0;
  int D = 0;
  A = map(analogRead(anag0), 0, 100, 0, 255);
  B = map(analogRead(anog1), 0, 100, 0, 255);
  C = map(analogRead(anog2), 0, 100, 0, 255);
  D = map(analogRead(anog3), 0, 100, 0, 255);
  if (neliox > screen.width() || neliox < 0){
    nelionsuunx = -nelionsuunx;
  }

  if (nelioy > screen.height() || nelioy < 0){
    nelionsuuny = -nelionsuuny;
  }

  if (A > 2500)
  {
    nelionsuuny = -2;
    nelioy += nelionsuuny;
  }

  if (B > 2500)
  {
    neliox -= nelionsuunx;
  }

  if (C > 2500)
  {
    nelioy -= nelionsuuny;
  }

  if (D > 2500)
  {
    neliox += nelionsuunx;
    //nelioy -= nelionsuuny;
  }

  nelioy += nelionsuuny;
 /* if (nelioy > )
  {
    nelioy += nelionsuuny - gravity;
  }
*/
  /*
  neliox += nelionsuunx;
  nelioy += nelionsuuny;
  */

  screen.fill(255, 255, 255);

  if (oldneliox != neliox || oldnelioy != nelioy) {
    screen.rect(oldneliox, oldnelioy, 5, 5);
    }

  screen.fill(0, 0, 0);
  screen.rect(neliox, nelioy, 5, 5);

  oldneliox = neliox;
  oldnelioy = nelioy;
  
  
}

void platforms(int x, int y, int a, int b)
{
  screen.rect(x, y, 20, 5);
  screen.rect(a, b, 20, 5);
  if (kontakti(neliox, nelioy, x, y - 3, 20, 5))
  {
    nelionsuuny = 0;
  }

  else if (kontakti(neliox, nelioy, a, b - 3, 20, 5))
  {
    nelionsuuny = 0;
  }


  

  else
  {
    nelionsuuny = 1;
  }

  
  
}

boolean kontakti(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight)
{
  boolean result = false;

  if ((x >= rectX && x <= (rectX + rectWidth)) &&
      (y >= rectY && y <= (rectY + rectHeight))) {
        result = true;
      }
      return result;
}
