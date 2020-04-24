//Kirjastot

#include <SD.h>
#include <TFT.h>
#include <SPI.h.>

//TFT näytön pinnit
#define LCD_CS   10
#define SD_CS 4
#define DC   9
#define RESET  8

//Ajastimen pituus, 100ms
#define period 100

//Näytön leveys ja pituus
#define screenWidth 160
#define screenHeight 128

//Nappien muuttujat
const int anag0 = A0;
const int anog1 = A1;
const int anog2 = A2;
const int anog3 = A3;
const int buzzer = 7;
const int dig3 = 3;

//Sisääntulo muuttujat
int inputA = 0;
int inputB = 0;
int inputC = 0;
int inputD = 0;

//Pelin eri objektien muuttujat

int neliox = 5;
int nelioy = 110;
int oldneliox = 0;
int oldnelioy = 0;
int nelionopeus = 1;
int nelionsuunx = 1;
int nelionsuuny = 1;

int goombaX = 50;
int goombaY = 50;
int goombasuunX = 1;
int goombasuunY = 1;
int oldgoombaX = 0;
int oldgoombaY = 0;

int goomba1X = 60;
int goomba1Y = 50;
int goombasuun1X = 1;
int goombasuun1Y = -1;
int oldgoomba1X = 0;
int oldgoomba1Y = 0;

int goomba2X = 70;
int goomba2Y = 94;
int goomba2nopeus = 15;
int goombasuun2X = 1;
int goombasuun2Y = 1;
int oldgoomba2X = 0;
int oldgoomba2Y = 0;
int goombaX2ded = 0;

int liikTasoX = 50;
int liikTasoY = 115;
int oldliikTasoX = 0;
int oldliikTasoY = 0;
int liikTasoNop = 5;
int liiktasoSuuntaX = 1;
int liiktasoSuuntaY = 1;
int liikTasoX2 = 140;
int liikTasoY2 = 43;
int oldliikTasoX2 = 0;
int oldliikTasoY2 = 0;
int liikTasoNop2 = 5;
int liiktasoSuuntaX2 = 1;
int liiktasoSuuntaY2 = 1;
int kosketusliik = 0;

int elamat = 3;

int aika = 0;
int tippumassa = 0;


//Ajastimen muuttujat
unsigned long start;
unsigned long current;

bool buttonPressed = false;

int ran1 = random(50, 256);
int ran2 = random(50, 256);
int ran3 = random(50, 256);


TFT screen = TFT(LCD_CS, DC, RESET);


void setup() {
  //Käynnistys muuttujat, Serial debuggausta varten.
  screen.begin();
  Serial.begin(9600);
  //Näytön taustaväri, pensseli ja täyttö valkoiseksi
  screen.background(255, 255, 255);
  screen.stroke(255, 255, 255);
  screen.fill(255,255,255);
  //muuttujat ominaisuuksia varten mitä olisimme vielä lisänneet jos olisi ollut enemmän aikaa.
  pinMode(buzzer, OUTPUT);
  pinMode(dig3, OUTPUT);

  //Ajastimen aloitus
  start = millis();
  
}

void loop() {

//Tasojen funktio, perään on lisätty koordinaatit mihin tasot tulee.
platforms(100, 90, 70, 100, 10, 10, 5, 120, 130, 75, 130, 57, 5, 22);


//Tällä piirretään elämien määrä ja peli päättyy kun elämiä on -1
elama();

//Näillä hahmot liikkuu, eli näyttö päivittyy kun millisekuntien ylijäämä on enemmän kuin 2
if (millis() % nelionopeus < 2)
{
  liiku();
  movingPlatform();
  movingPlatform2();
}

if (millis() % goomba2nopeus < 2)
{
  goomba2();
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
/*
  Serial.print(A);
  Serial.print("  ");
  Serial.print(B);
  Serial.print("  ");
  Serial.print(C);
  Serial.print("  ");
  Serial.println(D);
*/  

//Ajastin, jos nappi A on pohjassa ja hahmo ei ole ilmassa niin hahmo pystyy suorittamaan hypyn,
// eli nappi on pohjassa ja aikaan tulee +1 aina 100ms välein kunnes se on 300ms jonka jälkeen täytyy käydä alustalla että aika muuttuja nollaantuu.
  current = start;
  current = millis();
  if (current - start >= period)
  {
    if(A > 2000 && tippumassa != 0)
    {
      aika++;
    }

    else if(aika > 3)
    {
      aika = 0;            
    }

    else if(A < 2000)
    {
      aika = 0;
    }
    
    start = current;
  }
//Näytön reunat, jos hahmo osuu sivuille niin se ei pysty jatkamaan pidemmälle, eli siis on mahdollisuus kääntyä takaisin vielä ennenkuin tippuu alas.
  if (neliox > screen.width() || neliox < 0)
  {
    nelionsuunx = 0;
  }
//Mitä tapahtuu kun hahmo tippuu alas.
  if (nelioy > screen.height() || nelioy < 0)
  {
    kuolema();
  }



  if(aika > 0 && aika < 3)
  {
    nelionsuuny = -1;
    nelioy += nelionsuuny;
  }
//kun nappi B on pohjassa, niin hahmo liikkuu vasemmalle, kosketusliik ansiosta hahmo liikkuu alustan mukana.
  if (B > 2000 || kosketusliik == -1)
  {
    nelionsuunx = 1;
    neliox -= nelionsuunx;
    buttonPressed = true;
  }

  if (C > 2500)
  {
   digitalWrite(dig3, LOW); 
   buttonPressed = true;
  }
//Kun nappi D on pohjassa niin hahmo liikkuu oikealle, kosketusliik ansiosta hahmo liikkuu alustan mukana.
  if (D > 2000 || kosketusliik == 1)
  {
    nelionsuunx = 1;
    neliox += nelionsuunx;
    buttonPressed = true;
  }
//painovoima
  nelioy += nelionsuuny;

  screen.fill(255, 255, 255);
//Pyyhkii vanhan nelion pois, muuten jää kummituksia hahmon perään.
  if (oldneliox != neliox || oldnelioy != nelioy) {
    screen.rect(oldneliox, oldnelioy, 5, 5);
    }
//Mörköjen ja hahmomme muuttuhat
  screen.fill(0, 0, 0);
  screen.rect(neliox, nelioy, 5, 5);
  screen.rect(goombaX, goombaY, 7, 7);
  screen.rect(goomba1X, goomba1Y, 7, 7);
  
  oldgoombaX = goombaX;
  oldgoombaY = goombaY;
  oldneliox = neliox;
  oldnelioy = nelioy;

  goombaY += goombasuunY;
//Mörkö liikkuu edestakaisin aina kun törmää reunalle.
  if (goombaY > screen.height() || goombaY < 0)
  {
    goombasuunY = -goombasuunY;
  }

  oldgoomba1X = goomba1X;
  oldgoomba1Y = goomba1Y;

  goomba1Y += goombasuun1Y;
//Mörkö liikkuu edestakaisin aina kun törmää reunalle.
  if (goomba1Y > screen.height() || goomba1Y < 0)
  {
    goombasuun1Y = -goombasuun1Y;
  }

//Jos hahmomme koskettaa mörköjä.
  if (kontakti(neliox, nelioy, goombaX, goombaY, 7, 7))
  {
    kuolema();
  }

  else if (kontakti(neliox, nelioy, goomba1X, goomba1Y, 7, 7))
  {
    kuolema();
  }

//Vähän ehkä hassusti olen nimennyt muuttujan mutta kun hahmo on tippumassa niin tippumassa on 0, jolloin ajastimeen ei tule enempää laskee laskemasta, 
//kun taas on hahmo on nousemassa ylös eli -1 tai sitten jos hahmo on tasanteella eli 0 niin hahmomme voi suorittaa hypyn.

  
  if(nelionsuuny == 1)
  {
    tippumassa = 0;
  }

  else if(nelionsuuny == -1 || nelionsuuny == 0)
  {
    tippumassa = 1;
  }

}
//Eri tasot, Täällä on liikkuvat alustat, alustat jotka pysyy vain paikoillaan sekä ympyrä mihin hahmomme pitäisi mennä.
void platforms(int x, int y, int a, int b, int q, int c, int g, int j, int k, int l, int k2, int l2, int m, int n)
{
  //piirretään ruudulle tasanteet
  screen.rect(x, y, 20, 5);
  screen.rect(a, b, 30, 5);
  screen.circle(q, c, 5);
  screen.rect(g, j, 20, 5);
  screen.rect(k, l, 20, 5);
  screen.rect(k2, l2, 20, 5);
  screen.rect(m, n, 20, 5);
  
  if (kontakti(neliox, nelioy, x, y - 3, 20, 1))
  {
    nelionsuuny = 0;
    kosketusliik = 0;
  }

  else if (kontakti(neliox, nelioy, a, b - 3, 30, 1))
  {
    nelionsuuny = 0;
    kosketusliik = 0;
  }
  
  else if (kontakti(neliox, nelioy, q, c, 8, 8))
  {
    voitto();
  }

  else if(kontakti(neliox, nelioy, g - 1, j - 3, 17, 1))
  {
    nelionsuuny = 0;
    kosketusliik = 0;
  }
  else if(kontakti(neliox, nelioy, liikTasoX, liikTasoY - 3, 15,1))
  {
    nelionsuuny = 0;
    if (liiktasoSuuntaX == 1)
    {
      kosketusliik = 1;
    }
    else if (liiktasoSuuntaX == -1)
    {
      kosketusliik = -1;
    }
  }
  else if(kontakti(neliox, nelioy, liikTasoX2, liikTasoY2 - 3, 15,1))
  {
    nelionsuuny = 0;
    if (liiktasoSuuntaX2 == 1)
    {
      kosketusliik = 1;
    }
    else if (liiktasoSuuntaX2 == -1)
    {
      kosketusliik = -1;
    }
  }

  else if(kontakti(neliox, nelioy, k, l - 3, 15, 5))
  {
    nelionsuuny = 0;
    kosketusliik = 0;
  }
  
  else if(kontakti(neliox, nelioy, k2, l2 - 3, 15, 5))
  {
    nelionsuuny = 0;
    kosketusliik = 0;
  }
  
  else if(kontakti(neliox, nelioy, m, n - 3, 15, 5))
  {
    nelionsuuny = 0;
    kosketusliik = 0;
  }

    //Painovoima, jos ei kosketa mihinkään tasanteelle niin neliö jatkaa tippunusta kunnes reuna tulee vastaan ja menetät elämän ja palaat lähtöruudulle.
  else
  {
    nelionsuuny = 1;
  }
  
}
//Kontakti funktio
boolean kontakti(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight)
{
  boolean result = false;

  if ((x >= rectX && x <= (rectX + rectWidth)) &&
      (y >= rectY && y <= (rectY + rectHeight))) {
        result = true;
      }
      return result;
}
//Tämä mörkö on alustan päällä ja pyörii edestakaisin
void goomba2()

{
  screen.rect(goomba2X, goomba2Y, 7, 7);
  oldgoomba2X = goomba2X;
  oldgoomba2Y = goomba2Y;  

  goomba2X += goombasuun2X;

  if (goomba2X > 85 || goomba2X < 70)
  {
    goombasuun2X = -goombasuun2X;
  }

    if (kontakti(neliox, nelioy, goomba2X, goomba2Y, 7, 7))
  {
    kuolema();
  }
}

void peliLoppu() {
  screen.background(0, 0, 0);
  screen.stroke(255,0,255);
  screen.setTextSize(3);
  screen.fill(255,255,255);
  screen.rect(10, 25, 135, 80);
  screen.text("Game\n",30,40);
  screen.text("Over",55,65);
  delay(3000);
  screen.setTextSize(1);
  screen.stroke(255,255,0);
  screen.text("Press a button to retry!",15,110); //poista jos buttonPressed ei toimi
  screen.background(0,0,0);
}
void elama()
{
  if(elamat == 3)
  {
    screen.rect(150, 5, 6, 6);
    screen.rect(140, 5, 6, 6);
    screen.rect(130, 5, 6, 6);
  }

  else if (elamat == 2)
  {
    screen.rect(150, 5, 6, 6);
    screen.rect(140, 5, 6, 6);
    screen.fill(255,255,255);
    screen.rect(130, 5, 6, 6);
  }

  else if (elamat == 1)
  {
    screen.rect(150, 5, 6, 6);
    screen.fill(255,255,255);
    screen.rect(140, 5, 6, 6);
    screen.fill(255,255,255);
    screen.rect(130, 5, 6, 6);
  }
  else if (elamat == 0)
  {
    screen.fill(255,255,255);
    screen.rect(150, 5, 6, 6);
    screen.fill(255,0,0);
    screen.rect(150, 5, 5, 5);
  }
//Tulostaa ruudulle peli on päättynyt
  else if (elamat == -1)
  {

    peliLoppu();
    delay(3000); //saako buttonPressed = true; ??
    elamat = 3;
    setup();
    loop();

  }
}
//Liikkuva alusta
void movingPlatform() 
{
  screen.rect(liikTasoX,liikTasoY,15,5);
  oldliikTasoX = liikTasoX;
  oldliikTasoY = liikTasoY;
  liikTasoX += liiktasoSuuntaX;

  if (liikTasoX > 140 || liikTasoX < 20 ) 
  {
    liiktasoSuuntaX = -liiktasoSuuntaX;
  }
}
//Liikkuva alusta
void movingPlatform2() 
{
  screen.rect(liikTasoX2,liikTasoY2,15,5);
  oldliikTasoX2 = liikTasoX2;
  oldliikTasoY2 = liikTasoY2;
  liikTasoX2 += liiktasoSuuntaX2;

  if (liikTasoX2 > 140 || liikTasoX2 < 7 ) 
  {
    liiktasoSuuntaX2 = -liiktasoSuuntaX2;
  }
}
//elämien vähennys funktio ja palauttaa neliön mistä se aloitti.
void kuolema()
{
  elamat--;
  neliox = 5;
  nelioy = 110;
  kosketusliik = 0;
}

//Voitit pelin funktio
void voitto() {
  screen.background(0, 0, 0);
  screen.stroke(255,0,255);
  screen.setTextSize(3);
  screen.fill(255,255,255);
  screen.rect(10, 25, 135, 80);
  screen.text("Voitit",30,40);
  screen.text("Pelin!",30,65);
  delay(3000);
  screen.setTextSize(1);
  screen.stroke(255,255,0);
  screen.background(0,0,0);
}
