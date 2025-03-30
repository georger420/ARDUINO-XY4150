#define pinPen 22
#define pinHorVer 24
#define pinPlusMinus 26
#define pinMove 28
#define pinReadyIn 30
#define pinReadyOut 32

//-------------------------
// pinPen = LOW ... Pen UP
// pinPen = HIGH ... Pen DOWN
// pinMove = change from LOW to HIGH mean 1 step
// pinHorVer = LOW ... Movement of pen carriage
// pinHorVer = HIGH ... Movement of paper
// pinPlusMinus = LOW ... Carriage to the left or paper forward
// pinPlusMinus = HIGH ... Carriage to the right or paper backwords
// pinReadyIn = LOW ... Ready
// pinReadyIn = HIGH ... Not ready
//---------------------------------


int StepDelay;  // Doba 1
int PenDelay; // Doba 2
float PosX, PosY, uhel;
int i;
String povel;
String txtText, velkeHacky, velkeHackyBez, maleHacky, velkeCarky, velkeCarkyBez, maleCarky;
int pripraven;
int homeX, homeY, vel, m, angle, mezera;
int leftBorder, charSpace, rws, rowSpace, chH, chCX, chCY, chW, lftBorder, topBorder ;
int wdth, hght; //šířka a výška papíru v krocích (krok = 0.1. mm)

void setup() {
  velkeHacky = "ČĎĚŇŘ\u0160ŤŽ";
  velkeHackyBez = "CDENRSTZ";
  velkeCarky = "ÁÉ\u00CDÓÚÝ";
  velkeCarkyBez = "AEIOUY";
  maleHacky = "čěňřšž";
  maleCarky = "áéíóúí";
  charSpace = 2;
  rowSpace = 2; // zakladni vzdalenost mezi spodkem prvniho a horejskem druheho radku
  chW = 4; // zakladni sirka znaku
  chH = 9; // zakladni vyska znaku
  chCY = 1; //koeficient vysky znaku
  chCX = 1; //koeficient sirky znaku
  wdth = 1900;
  hght = 2600;
  
  
  
  // put your setup code here, to run once:
  pinMode(pinPen, OUTPUT);
  pinMode(pinHorVer, OUTPUT);
  pinMode(pinPlusMinus, OUTPUT);
  pinMode(pinMove, OUTPUT);
  pinMode(pinReadyOut, OUTPUT);
  pinMode(pinReadyIn, OUTPUT);
  digitalWrite(pinPen, LOW);
  digitalWrite(pinHorVer, HIGH);
  digitalWrite(pinPlusMinus, HIGH);



  StepDelay = 1;
  PenDelay = 250;
  Serial.begin(19200);
  pripraven = isReady();
  Serial.println("pripraven");
  Serial.print("Je pripraven: ");
  Serial.println(pripraven);
  //PenUp();



  //cs();
  setReady();
  Serial.print("Je pripraven: ");
  Serial.println(pripraven);
  homeX = PosX;
  homeY = PosY;

}

void loop() {
  // put your main code here, to run repeatedly:

  povel = "";

  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:


    povel = Serial.readString();
    if (povel.length() > 0) {
      cs();
      vel = 800;
      lt(180);
      fd(vel / 2);
      homeX = PosX;
      homeY = PosY;
      lt(180);
      pd(); 
      while (true) {
        fd(vel);
        lt(170);
        if (sqrt((PosX - homeX) * (PosX - homeX) + (PosY - homeY) * (PosY - homeY))<1) {
          break;
        }
      }      
    }
    pu();
  }
}

void StepPause() { // Pauza 1
  delay(StepDelay);
}

void PenPause() { // Pauza 2
  delay(PenDelay);
}

void setReady() {
  digitalWrite(pinReadyOut, LOW);
}

void setNotReady () {
  digitalWrite(pinReadyOut, HIGH);
}

int isReady() {
  return digitalRead(pinReadyIn);
}

void Horizontal() {
  digitalWrite(pinHorVer, LOW);
}

void Vertical() {
  digitalWrite(pinHorVer, HIGH);
}

void LeftUp() {
  digitalWrite(pinPlusMinus, LOW);
}

void RightDown() {
  digitalWrite(pinPlusMinus, HIGH);
}

void PenUp() {
  digitalWrite(pinPen, LOW);
  PenPause();
}

void PenDown() {
  digitalWrite(pinPen, HIGH);
  PenPause();
}

void Step() {
  digitalWrite(pinMove, LOW);
  delay(1);
  digitalWrite(pinMove, HIGH);
  StepPause();
  digitalWrite(pinMove, LOW);
}

void Forward() {
  Vertical();
  LeftUp();
  Step();
  PosY = PosY + 1;
}

void Bck() {
  Vertical();
  RightDown();
  Step();
  PosY = PosY - 1;
}

void Left() {
  Horizontal();
  LeftUp();
  Step();
  PosX = PosX - 1;
}

void Right() {
  Horizontal();
  RightDown();
  Step();
  PosX = PosX + 1;
}

void InitPlotter() {
  int i;
  for (i = 0; i < 2000; i++) {
    Left();
  }
  PosX = 0;
  PosY = hght;
}

void HorizontalStep(float dxx) {
  if (dxx != 0) {
    if (dxx > 0) {
      Right();
    } else {
      Left();
    }
  }
}

void VerticalStep(float dyy) {
  if (dyy != 0) {
    if (dyy > 0) {
      Forward();
    } else {
      Bck();
    }
  }
}

void pom1(float adx, float ady, float bDXX, float bDYY) {
  int x;
  float K, YP, Y;
  K = adx / ady;
  YP = 0;
  x = 1;
  Y = 0;
  while (x <= ady) {
    Y = K * x;
    VerticalStep(bDYY);
    x++;
    if ((Y - YP) > 0.5) {
      HorizontalStep(bDXX);
      YP = YP + 1;
    }
  }
}

void pom2(float adx, float ady, float bDXX, float bDYY) {
  int x;
  float K, YP, Y;
  K = ady / adx;
  //println("K = " + K);
  YP = 0;
  x = 1;
  Y = 0;
  while (x <= adx) {
    Y = K * x;
    HorizontalStep(bDXX);
    x = x + 1;
    if ((Y - YP) > 0.5) {
      VerticalStep(bDYY);
      YP = YP + 1;
    }
  }
}

void mr(float dx, float dy) {
  float aDXX = dx;
  float aDYY = dy;
  aDXX = dx;
  aDYY = dy;
  dx = abs(dx);
  dy = abs(dy);
  dx = round(dx);
  dy = round(dy);
  if (dy > dx) {
    pom1(dx, dy, aDXX, aDYY);
  } else {
    pom2(dx, dy, aDXX, aDYY);
  }
}

void ma(float xx, float yy) {
  float cdx, cdy;
  cdx = xx - PosX;
  cdy = yy - PosY;
  mr(cdx, cdy);
}

//*****************
//želví grafika
//*****************

void setpos(float xx, float yy) {
  float dx, dy;
  dx = abs(xx - PosX);
  dy = abs(yy - PosY);
  if (xx < PosX) {
    dx = -dx;
  }
  if (yy < PosY) {
    dy = -dy;
  }
  mr(dx, dy);
}

void hm() { // procedura home
  setpos(0, 0);
}

void cs() {
  InitPlotter();
  mr(900, -1300);
  PosX = 0;
  PosY = 0;
  uhel = 0;
}

void fd(float dalka) {
  float dx, dy;
  dx = dalka * sin(uhel);
  dy = dalka * cos(uhel);
  mr(dx, dy);
}

void bk(float dalka) {
  fd(-dalka);
}

float degrad(float alfa) {
  return ((PI * alfa) / 180);
}

float raddeg(float alfa) {
  return ((alfa * 180) / PI);
}

float heading() {
  return (raddeg(uhel));
}

void seth(float alfa) {
  uhel = degrad(alfa);
}

void lt(float alfa) {
  uhel = uhel - degrad(alfa);
}

void rt(float alfa) {
  uhel = uhel + degrad(alfa);
}

void pu() {
  PenUp();
}

void pd() {
  PenDown();
}

//******************************************
// Kreslení textu
//******************************************

void drawText(String co, float pw, float pxy, float pyx, float ph, float chs) {
  int i, delka, pom;
  String znak;
  
  delka = co.length();
  for (i = 0; i<delka; i++) {
    znak = co.substring(i, i + 1);
    pom = velkeHacky.indexOf(znak, 0);
    if (pom >= 0) {
      drawVelkyHacek(pw, pxy, pyx, ph, chs);
      znak = velkeHackyBez.substring(pom, pom + 1);
    }
    pom = velkeCarky.indexOf(znak, 0);
    if (pom >= 0) {
      drawVelkouCarku(pw, pxy, pyx, ph, chs);
      znak = velkeCarkyBez.substring(pom, pom + 1);
    }    
    drawChar(znak, pw, pxy, pyx, ph, chs);
 
  }
  
}

void drawChar(String znak, float pw, float pxy, float pyx, float ph, float chs) {
    int pomZnak;
    pomZnak = znak.charAt(0);
    switch(pomZnak) {
/*
      case "":
        break;
*/
      case ' ':
        pu(); 
        mr(pw *(4 + pyx), ph * (0 + pxy));
        mr(pw *(chs + pyx), ph * (0 + pxy));
        //pu();
        break;
      case '!':
        pu(); 
        mr(pw *(2 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-4 + pxy)); 
        pu(); 
        mr(pw *(0 + pyx), ph * (-1 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-1 + pxy)); 
        pu(); 
        mr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;
      case '\"':
        pu(); 
        mr(pw *(1 + pyx), ph * (4 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (2 + pxy)); 
        pu(); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-2 + pxy)); 
        pu(); 
        mr(pw *(1 + chs + pyx), ph * (-4 + pxy));
        break;        
      case '#':
        pu();
        mr(pw *(1 + pyx), ph * (0 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (6 + pxy));
        pu();
        mr(pw *(2 + pyx), ph * (0 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-6 + pxy));
        pu();
        mr(pw *(1 + pyx), ph * (2 + pxy));
        pd();
        mr(pw *(-4 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(0 + pyx), ph * (2 + pxy));
        pd();
        mr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case '$':
        pu();
        mr(pw *(0 + pyx), ph * (1 + pxy));
        pd();
        mr(pw *(3 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(-2 + pyx), ph * (1 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-6 + pxy));
        pu();
        mr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;
      case '%':
        pd();
        mr(pw *(4 + pyx), ph * (6 + pxy));
        pu();
        mr(pw *(-3 + pyx), ph * (0 + pxy));
        pd();
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        pu();
        mr(pw *(2 + pyx), ph * (-4 + pxy));
        pd();
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        pu();
        mr(pw *(1 + chs + pyx), ph * (-2 + pxy));
        break;
      
      case '&':
        pu();
        mr(pw *(4 + pyx), ph * (0 + pxy));
        pd();
        mr(pw *(-3 + pyx), ph * (4 + pxy));
        mr(pw *(0 + pyx), ph * (1 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        mr(pw *(-3 + pyx), ph * (-2 + pxy));
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (0 + pxy));
        mr(pw *(2 + pyx), ph * (2 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-2 + pxy));
        break;
      case '\'':
        pu();
        mr(pw *(2 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        pu();
        mr(pw *(2 + chs + pyx), ph * (-4 + pxy));
        break;
      case '(':
        pu(); 
        mr(pw *(2 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(-1 + pyx), ph * (-2 + pxy)); 
        mr(pw *(0 + pyx), ph * (-2 + pxy)); 
        mr(pw *(1 + pyx), ph * (-2 + pxy)); 
        pu(); 
        mr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;
      case ')':
        pu(); 
        mr(pw *(2 + pyx), ph * (6 + pxy)); pd(); 
        mr(pw *(1 + pyx), ph * (-2 + pxy)); 
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        mr(pw *(-1 + pyx), ph * (-2 + pxy)); 
        pu(); 
        mr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;        
      case '*':
        pu();
        mr(pw *(3 + pyx), ph * (5 + pxy));
        pd();
        mr(pw *(-2 + pyx), ph * (-4 + pxy));
        pu();
        mr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (-4 + pxy));
        pu();
        mr(pw *(-3 + pyx), ph * (2 + pxy));
        pd();
        mr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-3 + pxy));
        break;
      case '+':
        pu(); 
        mr(pw *(0 + pyx), ph * (3 + pxy)); 
        pd(); 
        mr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(-2 + pyx), ph * (2 + pxy)); pd();  
        mr(pw *(0 + pyx), ph * (-4 + pxy)); pu(); 
        mr(pw *(2 + chs + pyx), ph * (-1 + pxy));
        break;
      case ',':
        pu(); 
        mr(pw *(2 + pyx), ph * (1 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-1 + pxy)); 
        mr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        pu(); 
        mr(pw *(3 + chs + pyx), ph * (1 + pxy));
        break;
      case '-':
        pu(); 
        mr(pw *(0 + pyx), ph * (3 + pxy)); 
        pd(); 
        mr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (-3 + pxy));
        break;        
      case '.':
        pu();
        mr(pw *(2 + pyx), ph * (1 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;
      case '/':
        pd(); 
        mr(pw *(4 + pyx), ph * (6 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (-6 + pxy));
        break;

      case '0':
        pd(); 
        mr(pw *(4 + pyx), ph * (6 + pxy)); 
        pu(); 
        mr(pw *(-1 + pyx), ph * (0 + pxy)); 
        pd(); 
        mr(pw *(-2 + pyx), ph * (0 + pxy)); 
        mr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (1 + pxy)); 
        mr(pw *(0 + pyx), ph * (4 + pxy)); 
        mr(pw *(-1 + pyx), ph * (1 + pxy)); 
        pu(); 
        mr(pw *(1 + chs + pyx), ph * (-6 + pxy));
        break;
      case '1':
        pu(); 
        mr(pw *(1 + pyx), ph * (4 + pxy)); 
        pd();  
        mr(pw *(2 + pyx), ph * (2 + pxy));  
        mr(pw *(0 + pyx), ph * (-6 + pxy)); 
        pu();  
        mr(pw *(1 + chs + pyx), ph * (0 + pxy));
        break;
      case '2':
        pu();  
        mr(pw *(0 + pyx), ph * (5 + pxy)); 
        pd();  
        mr(pw *(1 + pyx), ph * (1 + pxy));  
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy));  
        mr(pw *(0 + pyx), ph * (-1 + pxy));  
        mr(pw *(-4 + pyx), ph * (-3 + pxy));  
        mr(pw *(0 + pyx), ph * (-1 + pxy)); 
        mr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu();  
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case '3':
        pu();  
        mr(pw *(0 + pyx), ph * (5 + pxy));
        pd();  
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(2 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        pu();  
        mr(pw *(4 + chs + pyx), ph * (-1 + pxy));
        break;
      case '4':
        pu(); 
        mr(pw *(2 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(-2 + pyx), ph * (-5 + pxy));
        mr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        pu();
        mr(pw *(1 + chs + pyx), ph * (0 + pxy));
        break;
      case '5':
        pu();
        mr(pw *(4 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(-4 + pyx), ph * (0 + pxy));
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        mr(pw *(3 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        pu();
        mr(pw *(4 + chs + pyx), ph * (-1 + pxy));
        break;
      case '6':
        pu();
        mr(pw *(3 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-4 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(2 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(0 + pyx), ph * (2 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(4 + chs + pyx), ph * (-3 + pxy));
        break;
      case '7':
        pu();
        mr(pw *(0 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(4 + pyx), ph * (0 + pxy));
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        mr(pw *(-4 + pyx), ph * (-5 + pxy));
        pu();
        mr(pw *(4 + chs + pyx), ph * (0 + pxy));
        break;
      case '8':
        pu(); 
        mr(pw *(3 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(2 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(0 + pyx), ph * (2 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        pu();
        mr(pw *(3 + chs + pyx), ph * (-4 + pxy));
        break;
      case '9':
        pu();
        mr(pw *(1 + pyx), ph * (0 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(0 + pyx), ph * (4 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(2 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-3 + pxy));
        break;
      case ':':
        pu();
        mr(pw *(2 + pyx), ph * (3 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;
      case ';':
        pu();
        mr(pw *(2 + pyx), ph * (3 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(3 + chs + pyx), ph * (1 + pxy));
        break;
      case '<':
        pu();
        mr(pw *(3 + pyx), ph * (5 + pxy));
        pd();
        mr(pw *(-2 + pyx), ph * (-2 + pxy));
        mr(pw *(2 + pyx), ph * (-2 + pxy));
        pu();
        mr(pw *(1 + chs + pyx), ph * (-1 + pxy));
        break;
      case '=':
        pu();
        mr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(-4 + pyx), ph * (-2 + pxy));
        pd();
        mr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-2 + pxy));
        break;
      case '>':
        pu();
        mr(pw *(1 + pyx), ph * (5 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (-2 + pxy));
        mr(pw *(-2 + pyx), ph * (-2 + pxy));
        pu();
        mr(pw *(3 + chs + pyx), ph * (-1 + pxy));
        break;
      case '?':
        pu();
        mr(pw *(0 + pyx), ph * (5 + pxy));
        pd();
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(2 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        mr(pw *(-2 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break; 
      case '@':
        pd();
        mr(pw *(0 + pyx), ph * (5 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(2 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-4 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(-1 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(0 + pyx), ph * (3 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-4 + pxy));
        pu();
        mr(pw *(1 + chs + pyx), ph * (0 + pxy));
        break;       
      case 'A':
        pd(); 
        mr(pw *(0 + pyx), ph * (4 + pxy)); 
        mr(pw *(2 + pyx), ph * (2 + pxy)); 
        mr(pw *(2 + pyx), ph * (-2 + pxy)); 
        mr(pw *(0 + pyx), ph * (-4 + pxy)); 
        pu(); 
        mr(pw *(-4 + pyx), ph * (3 + pxy)); 
        pd();
        mr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (-3 + pxy));
        break;
      case 'B':
        pd(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        mr(pw *(3 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(0 + pyx), ph * (-1 + pxy)); 
        mr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(3 + pyx), ph * (0 + pxy)); 
        pd(); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(4 + chs + pyx), ph * (0 + pxy));
        break;        
      case 'C':
        pu(); 
        mr(pw *(4 + pyx), ph * (5 + pxy)); pd(); 
        mr(pw *(-1 + pyx), ph * (1 + pxy)); 
        mr(pw *(-2 + pyx), ph * (0 + pxy)); 
        mr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (1 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (-1 + pxy));
        break;
      case 'D':
        pd(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        mr(pw *(3 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(4 + chs + pyx), ph * (0 + pxy));
        break;        
      case 'E':
        pu(); 
        mr(pw *(4 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(-4 + pyx), ph * (0 + pxy)); 
        mr(pw *(0 + pyx), ph * (-6 + pxy)); 
        mr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(-4 + pyx), ph * (3 + pxy)); 
        pd(); 
        mr(pw *(3 + pyx), ph * (0 + pxy)); 
        pu(); mr(pw *(1 + chs + pyx), ph * (-3 + pxy));
        break;
      case 'F':
        pu(); 
        mr(pw *(4 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(-4 + pyx), ph * (0 + pxy)); 
        mr(pw *(0 + pyx), ph * (-6 + pxy)); 
        pu(); 
        mr(pw *(3 + pyx), ph * (3 + pxy)); 
        pd(); 
        mr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(4 + chs + pyx), ph * (-3 + pxy));
        break;
      case 'G':
        pu(); 
        mr(pw *(4 + pyx), ph * (5 + pxy)); 
        pd(); 
        mr(pw *(-1 + pyx), ph * (1 + pxy)); 
        mr(pw *(-2 + pyx), ph * (0 + pxy)); 
        mr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (1 + pxy)); 
        mr(pw *(0 + pyx), ph * (1 + pxy)); 
        mr(pw *(-2 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(2 + chs + pyx), ph * (-2 + pxy));
        break;        
      case 'H':
        pu(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-6 + pxy)); pu(); 
        mr(pw *(4 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-6 + pxy)); 
        pu(); 
        mr(pw *(-4 + pyx), ph * (3 + pxy)); 
        pd(); 
        mr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (-3 + pxy));  
        break;
      case 'I':
        pu(); 
        mr(pw *(1 + pyx), ph * (0 + pxy)); 
        pd(); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(-2 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(1 + pyx), ph * (0 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-6 + pxy)); 
        pu(); 
        mr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;        
      case 'J':
        pu(); 
        mr(pw *(0 + pyx), ph * (1 + pxy)); 
        pd(); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (1 + pxy)); 
        mr(pw *(0 + pyx), ph * (5 + pxy)); 
        mr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(3 + chs + pyx), ph * (-6 + pxy));
        break;
      case 'K':
        pd(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pu(); 
        mr(pw *(4 + pyx), ph * (0 + pxy)); 
        pd(); 
        mr(pw *(-4 + pyx), ph * (-4 + pxy)); 
        pu(); 
        mr(pw *(1 + pyx), ph * (1 + pxy)); 
        pd(); 
        mr(pw *(3 + pyx), ph * (-3 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;        
      case 'L':
        pu(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-6 + pxy)); 
        mr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'M':
        pd(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        mr(pw *(2 + pyx), ph * (-2 + pxy)); 
        mr(pw *(2 + pyx), ph * (2 + pxy)); 
        mr(pw *(0 + pyx), ph * (-6 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;        
      case 'N':
        pd(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        mr(pw *(4 + pyx), ph * (-6 + pxy)); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (-6 + pxy));
        break;
      case 'O':
        pu(); 
        mr(pw *(1 + pyx), ph * (0 + pxy)); 
        pd(); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (1 + pxy)); 
        mr(pw *(0 + pyx), ph * (4 + pxy)); 
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy)); 
        mr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        pu(); 
        mr(pw *(3 + chs + pyx), ph * (0 + pxy));
        break;
      case 'P':
        pd(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        mr(pw *(3 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(0 + pyx), ph * (-1 + pxy)); 
        mr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(4 + chs + pyx), ph * (-3 + pxy));
        break;
      case 'Q':
        pu(); 
        mr(pw *(1 + pyx), ph * (0 + pxy)); 
        pd(); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (1 + pxy)); 
        mr(pw *(0 + pyx), ph * (4 + pxy)); 
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy)); 
        mr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        pu(); 
        mr(pw *(1 + pyx), ph * (2 + pxy)); 
        pd(); 
        mr(pw *(2 + pyx), ph * (-2 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (0 + pxy));        
        break;
      case 'R':
        pd(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        mr(pw *(3 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(0 + pyx), ph * (-1 + pxy)); 
        mr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        pd(); 
        mr(pw *(2 + pyx), ph * (-3 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;        
      case 'S':
        pu(); 
        mr(pw *(0 + pyx), ph * (1 + pxy)); 
        pd(); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (1 + pxy)); 
        mr(pw *(0 + pyx), ph * (1 + pxy)); 
        mr(pw *(-1 + pyx), ph * (1 + pxy)); 
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy)); 
        mr(pw *(0 + pyx), ph * (1 + pxy)); 
        mr(pw *(1 + pyx), ph * (1 + pxy)); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (-5 + pxy));
        break;
      case 'T':
        pu(); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pu(); 
        mr(pw *(-2 + pyx), ph * (0 + pxy)); 
        pd(); 
        mr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (-6 + pxy));
        break;
      case 'U':
        pu(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-5 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (1 + pxy)); 
        mr(pw *(0 + pyx), ph * (5 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (-6 + pxy));
        break;         
      case 'V':
        pu(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mr(pw *(2 + pyx), ph * (-2 + pxy)); 
        mr(pw *(2 + pyx), ph * (2 + pxy)); 
        mr(pw *(0 + pyx), ph * (4 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (-6 + pxy));
        break;
      case 'W':
        pu(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-6 + pxy)); 
        mr(pw *(2 + pyx), ph * (2 + pxy)); 
        mr(pw *(2 + pyx), ph * (-2 + pxy)); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (-6 + pxy));
        break;         
      case 'X':
        pd(); 
        mr(pw *(4 + pyx), ph * (6 + pxy)); 
        pu(); 
        mr(pw *(-4 + pyx), ph * (0 + pxy)); 
        pd(); 
        mr(pw *(4 + pyx), ph * (-6 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'Y':
        pu(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-2 + pxy)); 
        mr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mr(pw *(2 + pyx), ph * (0 + pxy)); 
        mr(pw *(1 + pyx), ph * (1 + pxy)); 
        mr(pw *(0 + pyx), ph * (2 + pxy)); 
        pu(); 
        mr(pw *(-2 + pyx), ph * (-3 + pxy)); 
        pd(); 
        mr(pw *(0 + pyx), ph * (-3 + pxy)); 
        pu(); 
        mr(pw *(2 + chs + pyx), ph * (0 + pxy)); 
        break;
      case 'Z':
        pu(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mr(pw *(4 + pyx), ph * (0 + pxy)); 
        mr(pw *(-4 + pyx), ph * (-6 + pxy)); 
        mr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;         
      case '[':
        pu();
        mr(pw *(3 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(0 + pyx), ph * (-6 + pxy));
        mr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(1 + chs + pyx), ph * (0 + pxy));
        break;
      case '\\':
        pu(); 
        mr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); mr(pw *(4 + pyx), ph * (-6 + pxy)); 
        pu(); 
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case ']':
        pu();
        mr(pw *(1 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (0 + pxy));
        mr(pw *(0 + pyx), ph * (-6 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy)); 
        pu();
        mr(pw *(3 + chs + pyx), ph * (0 + pxy)); 
        break;
      case '^':
        pu();
        mr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (2 + pxy));
        mr(pw *(2 + pyx), ph * (-2 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case '_':
        pu();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pd();
        mr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (1 + pxy));
        break;
      case '`':
        pu();
        mr(pw *(1 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (-2 + pxy));
        pu();
        mr(pw *(1 + chs + pyx), ph * (-4 + pxy));
        break;
      case 'a':
        pu();
        mr(pw *(1 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-3 + pxy));
        pu();
        mr(pw *(0 + pyx), ph * (1 + pxy));
        pd();
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(2 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-1 + pxy));
        break;
      case 'b':
        pd();
        mr(pw *(0 + pyx), ph * (6 + pxy));
        pu();
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        pd();
        mr(pw *(3 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(-3 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(4 + chs + pyx), ph * (0 + pxy));
        break;
      case 'c':
        pu();
        mr(pw *(4 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(-3 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'd':
        pu();
        mr(pw *(4 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-6 + pxy));
        mr(pw *(-3 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(0 + pyx), ph * (2 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case 'e':
        pu();
        mr(pw *(0 + pyx), ph * (2 + pxy));
        pd();
        mr(pw *(3 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'f':
        pu();
        mr(pw *(1 + pyx), ph * (0 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (5 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(-4 + pyx), ph * (-3 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(2 + chs + pyx), ph * (-3 + pxy));
        break;
      case 'g':
        pu();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pd();
        mr(pw *(3 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(0 + pyx), ph * (4 + pxy));
        mr(pw *(-3 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-1 + pxy));
        break;
      case 'h':
        pd();
        mr(pw *(0 + pyx), ph * (6 + pxy));
        pu();
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        pd();
        mr(pw *(3 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-3 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'i':
        pu();
        mr(pw *(1 + pyx), ph * (0 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(-2 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(1 + pyx), ph * (0 + pxy));
        mr(pw *(0 + pyx), ph * (-4 + pxy));
        pu();
        mr(pw *(0 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(2 + chs + pyx), ph * (-5 + pxy));
        break;
      case 'j':
        pu();
        mr(pw *(1 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (0 + pxy));
        mr(pw *(0 + pyx), ph * (-4 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(-1 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(1 + pyx), ph * (7 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(2 + chs + pyx), ph * (-5 + pxy));
        break;
      case 'k':
        pd();
        mr(pw *(0 + pyx), ph * (6 + pxy));
        pu();
        mr(pw *(4 + pyx), ph * (-2 + pxy));
        pd();
        mr(pw *(-4 + pyx), ph * (-2 + pxy));
        mr(pw *(4 + pyx), ph * (-2 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'l':
        pu();
        mr(pw *(1 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(1 + pyx), ph * (0 + pxy));
        mr(pw *(0 + pyx), ph * (-6 + pxy));
        pu();
        mr(pw *(-1 + pyx), ph * (0 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(1 + chs + pyx), ph * (0 + pxy));
        break;
      case 'm':
        pd();
        mr(pw *(0 + pyx), ph * (4 + pxy));
        mr(pw *(1 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(0 + pyx), ph * (-3 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (4 + pxy));
        mr(pw *(1 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-3 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'n':
        pd();
        mr(pw *(0 + pyx), ph * (4 + pxy));
        mr(pw *(3 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-3 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'o':
        pu();
        mr(pw *(1 + pyx), ph * (0 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(0 + pyx), ph * (2 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(3 + chs + pyx), ph * (0 + pxy));
        break;
      case 'p':
        pd();
        mr(pw *(3 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(0 + pyx), ph * (2 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(-3 + pyx), ph * (0 + pxy));
        mr(pw *(0 + pyx), ph * (-5 + pxy));
        pu();
        mr(pw *(4 + chs + pyx), ph * (1 + pxy));
        break;
      case 'q':
        pu();
        mr(pw *(4 + pyx), ph * (-1 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (5 + pxy));
        mr(pw *(-3 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'r':
        pd();
        mr(pw *(0 + pyx), ph * (4 + pxy));
        mr(pw *(3 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-3 + pxy));
        break;
      case 's':
        pd();
        mr(pw *(3 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(-2 + pyx), ph * (0 + pxy));
        mr(pw *(-1 + pyx), ph * (1 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case 't':
        pu();
        mr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(-1 + pyx), ph * (2 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-5 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'u':
        pu();
        mr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-3 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(3 + pyx), ph * (0 + pxy));
        mr(pw *(0 + pyx), ph * (4 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case 'v':
        pu();
        mr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(2 + pyx), ph * (-4 + pxy));
        mr(pw *(2 + pyx), ph * (4 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case 'w':
        pu();
        mr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(1 + pyx), ph * (-4 + pxy));
        mr(pw *(1 + pyx), ph * (2 + pxy));
        mr(pw *(1 + pyx), ph * (-2 + pxy));
        mr(pw *(1 + pyx), ph * (4 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case 'x':
        pd();
        mr(pw *(4 + pyx), ph * (4 + pxy));
        pu();
        mr(pw *(-4 + pyx), ph * (0 + pxy));
        pd();
        mr(pw *(4 + pyx), ph * (-4 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'y':
        pu();
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        pd();
        mr(pw *(3 + pyx), ph * (0 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(0 + pyx), ph * (4 + pxy));
        pu();
        mr(pw *(-4 + pyx), ph * (0 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-2 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-1 + pxy));
        break;
      case 'z':
        pu();
        mr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mr(pw *(4 + pyx), ph * (0 + pxy));
        mr(pw *(-4 + pyx), ph * (-4 + pxy));
        mr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case '{':
        pu();
        mr(pw *(3 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(1 + chs + pyx), ph * (0 + pxy));
        break;
      case '|':
        pu();
        mr(pw *(2 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(0 + pyx), ph * (-6 + pxy));
        pu();
        mr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;
      case '}':
        pu();
        mr(pw *(1 + pyx), ph * (6 + pxy));
        pd();
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        mr(pw *(1 + pyx), ph * (-1 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        mr(pw *(0 + pyx), ph * (-1 + pxy));
        mr(pw *(-1 + pyx), ph * (-1 + pxy));
        pu();
        mr(pw *(3 + chs + pyx), ph * (0 + pxy));
        break;
      case '~':
        pu();
        mr(pw *(0 + pyx), ph * (3 + pxy));
        pd();
        mr(pw *(1 + pyx), ph * (1 + pxy));
        mr(pw *(2 + pyx), ph * (-2 + pxy));
        mr(pw *(1 + pyx), ph * (1 + pxy));
        pu();
        mr(pw *(chs + pyx), ph * (-3 + pxy));
        break;
      case '\n':
        LineFeed();
        break;
      case '\r':
        crgRet();
        break;
      default:
        pd(); pu();
        mr(pw *(1 + pyx), ph * (0 + pxy));
        pd(); pu();
        mr(pw *(1 + pyx), ph * (0 + pxy));
        pd(); pu();
        mr(pw *(1 + pyx), ph * (0 + pxy));
        pd(); pu();
        mr(pw *(1 + pyx), ph * (0 + pxy));
        pd(); pu();
        mr(pw *(chs + pyx), ph * (0 + pxy));        
        break;
    }  
}


void drawVelkyHacek(float pw, float pxy, float pyx, float ph, float chs) {
  pu(); mr(pw *(1 + pyx), ph * (8 + pxy)); pd(); mr(pw *(1 + pyx), ph * (-1 + pxy)); mr(pw *(1 + pyx), ph * (1 + pxy)); pu(); mr(pw *(-3 + pyx), ph * (-8 + pxy));
}

void drawVelkouCarku(float pw, float pxy, float pyx, float ph, float chs) {
  pu(); mr(pw *(2 + pyx), ph * (7 + pxy)); pd(); mr(pw *(1 + pyx), ph * (1 + pxy)); pu(); mr(pw *(-3 + pyx), ph * (-8 + pxy));
}

float txtStepsLenght(String txt) {
  float delka;
  delka = txt.length();
  
  return (delka * chCX * (chW + charSpace));
}

void LineFeed() {
  pu();
  mr(0, -chCY * (chH + rowSpace));
}

void crgRet() {
  mr(-1 * wdth, 0);
  PosX = 0;
  mr(lftBorder, 0);
}

void setCharacterSize(int charWidth, int charHeight) {
  chCX = charWidth;
  chCY = charHeight;
}
