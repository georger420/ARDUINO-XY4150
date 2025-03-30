//#include <SD.h>
//#include <Keypad.h>
//#include <LiquidCrystal_I2C.h>

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
float PosX, PosY, uhel, alfar, zkosX, zkosY;


String txtText ;
int pripraven;
int homeX, homeY, vel, m, angle, mezera, a4w, a4h;
int leftBorder, charSpace, rws, rowSpace, chH, chCX, chCY, chW, lftBorder, topBorder ;
int wdth, hght; //paper width and height in steps (1 step = 0.1 mm)

void setup() {
  a4w = 1900;
  a4h = 2600;
  charSpace = 2;
  rowSpace = 2; // Base distance between the bottom of the first row and the top of the second row
  chW = 4; // zakladni sirka znaku
  chH = 9; // zakladni vyska znaku
  chCY = 1; //koeficient vysky znaku
  chCX = 1; //koeficient sirky znaku
  wdth = 1900;
  hght = 2600;
  alfar = 0;
  zkosX = 0;
  zkosY = 0;

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

  setReady();
  Serial.print("Je pripraven: ");
  Serial.println(pripraven);
  homeX = PosX;
  homeY = PosY;

}

void loop() {
  // put your main code here, to run repeatedly:
  int i;
  String povel;
  povel = "";

  if (Serial.available() > 0) {

    povel = Serial.readString();
    if (povel.length() > 0) {
      InitPlotter();
      pu();
      lftBorder = (chW * chCX);
      topBorder = chCY * (chH + rws);      

      charSpace = 2;
      ma(950, 1300);
      delay(1000);
      //mr(0, 100);
      //mrxr(0, -100);
      alfar = 0;
      for (i = 0; i < 90; i+=15) {  
        alfar = i;
        ctverec(500);  
      }      

      alfar = 0;
      ma(0, hght);
      mr(50, -50);
      setCharacterSize(5, 6);
      zkosX = 15;
      drawText("Rotace ctverce pomoci transformace souradnic\r\n\r\n", chCX, chCY, charSpace);
      zkosX = 0;
      mr(50, 0);
      setCharacterSize(4, 5);
      drawText("for (i = 0; i < 90; i+=15) {\r\n", chCX, chCY, charSpace);
      mr(50, 0);
      drawText("  alfa = i;\r\n", chCX, chCY, charSpace);
      mr(50, 0);
      drawText("  ctverec(vel);\r\n", chCX, chCY, charSpace);
      mr(50, 0);
      drawText("}\r\n\r\n", chCX, chCY, charSpace);     mr(50, 0);
      drawText("void ctverec(float sirka) {\r\n", chCX, chCY, charSpace);    mr(50, 0);
      drawText("  mrxr(sirka / 2, - sirka / 2);\r\n", chCX, chCY, charSpace);    mr(50, 0);
      drawText("  pd();\r\n", chCX, chCY, charSpace);    mr(50, 0);
      drawText("  mrxr(- sirka, 0);\r\n", chCX, chCY, charSpace);    mr(50, 0);
      drawText("  mrxr(0, sirka);\r\n", chCX, chCY, charSpace);    mr(50, 0);
      drawText("  mrxr(sirka, 0);\r\n", chCX, chCY, charSpace);    mr(50, 0);
      drawText("  mrxr(0, - sirka);\r\n", chCX, chCY, charSpace);    mr(50, 0);
      drawText("  pu();\r\n", chCX, chCY, charSpace);    mr(50, 0);
      drawText("  mrxr(- sirka / 2, sirka / 2);\r\n", chCX, chCY, charSpace);    mr(50, 0);
      drawText("}\r\n", chCX, chCY, charSpace);    mr(50, 0);        
      pu();
    }
  }
}

//**************************
// Plotter control functions
//**************************
void ctverec(float sirka) {
  mrxr(sirka / 2, - sirka / 2);
  pd();
  mrxr(- sirka, 0);
  mrxr(0, sirka);
  mrxr(sirka, 0);
  mrxr(0, - sirka);
  pu();
  mrxr(- sirka / 2, sirka / 2);
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


//***************************
// move realative function
//***************************
void mr(float dx, float dy) {
  float aDXX = dx;
  float aDYY = dy;
  aDXX = dx;
  aDYY = dy;
  dx = abs(dx);
  dy = abs(dy);
  dx = zaokrouhli(dx);
  dy = zaokrouhli(dy);
  if (dy > dx) {
    pom1(dx, dy, aDXX, aDYY);
  } else {
    pom2(dx, dy, aDXX, aDYY);
  }
}


//***************************
// move absolute function
//***************************
void ma(float xx, float yy) {
  float cdx, cdy;
  cdx = xx - PosX;
  cdy = yy - PosY;
  mr(cdx, cdy);
}

int zaokrouhli(float co) {
  float pom;
  pom = co - int(co);
  //println(int(co), pom);
  if (pom >= 0.5) {
    return int(co) + 1;
  } else {
    return int(co);
  }
}

//*************************************
// move realative function
// respecting coordinate transformation
//*************************************
void mrxr(float sx, float sy) {
  float beta, gama, delta, nx, ny;
  nx = sx; ny = sy;
  if (alfar != 0) {
    beta = degrad(alfar);
    Serial.println();
    nx = sx * cos(beta) + sy * sin(beta);
    ny = -sx * sin(beta) + sy * cos(beta);
  }
  if (zkosX != 0) {
    gama = degrad(zkosX);
    nx = (nx + (ny * tan(gama)));
  }
  if (zkosY != 0) {
    delta = degrad(zkosY);
    ny = ny + nx * tan(delta);
  }
  mr(int(nx), int(ny));
  PosX = zaokrouhli(PosX + nx);
  PosY = zaokrouhli(PosY - ny);
}


//*****************
// Turtle graphics
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
  mr(a4w / 2, - a4h / 2);
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
// Text drawing
//******************************************

void drawText(String co, float pw, float ph, float chs) {
  int i, delka, pom;
  String znak;
  String velkeHacky, velkeHackyBez, maleHacky, velkeCarky, velkeCarkyBez, maleCarky;
  velkeHacky = "ČĎĚŇŘŠŤŽ";
  velkeHackyBez = "CDENRSTZ";
  velkeCarky = "ÁÉÍÓÚÝ";
  velkeCarkyBez = "AEIOUY";
  maleHacky = "čěňřšž";
  maleCarky = "áéíóúý";  
  delka = co.length();
  for (i = 0; i<delka; i++) {
    znak = co.substring(i, i + 1);
    pom = velkeHacky.indexOf(znak, 0);
    if (pom >= 0) {
      drawVelkyHacek(pw, ph, chs);
      znak = velkeHackyBez.substring(pom, pom + 1);
    }
    pom = velkeCarky.indexOf(znak, 0);
    if (pom >= 0) {
      drawVelkouCarku(pw, ph, chs);
      znak = velkeCarkyBez.substring(pom, pom + 1);
    }    
    drawChar(znak, pw, ph, chs);
 
  }
  
}

void drawChar(String znak, float pw, float ph, float chs) {
    int pomZnak;
    int pyx, pxy;
    pyx = 0; pxy = 0;    
    pomZnak = znak.charAt(0);
/*    switch(pomZnak) {
      case ' ':
        pu(); mrxr(pw * 4, 0); mrxr(pw * chs, 0);
        break;
      case '!':
        pu(); 
        mrxr(pw *2, ph * 6); 
        pd(); 
        mrxr(0, ph * -4); 
        pu(); 
        mrxr(0, ph * -1); 
        pd(); 
        mrxr(0, ph * -1); 
        pu(); 
        mrxr(pw *(2 + chs), 0);
        break;
      case '\"':
        pu(); 
        mrxr(pw *1, ph * 4); 
        pd(); 
        mrxr(0, ph * 2); 
        pu(); 
        mrxr(pw *2, 0); 
        pd(); 
        mrxr(0, ph * -2); 
        pu(); 
        mrxr(pw *(1 + chs), ph * -4);
        break;        
      case '#':
        pu();
        mrxr(pw *(1 + pyx), ph * (0 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (6 + pxy));
        pu();
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-6 + pxy));
        pu();
        mrxr(pw *(1 + pyx), ph * (2 + pxy));
        pd();
        mrxr(pw *(-4 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(0 + pyx), ph * (2 + pxy));
        pd();
        mrxr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case '$':
        pu();
        mrxr(pw *(0 + pyx), ph * (1 + pxy));
        pd();
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(-2 + pyx), ph * (1 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-6 + pxy));
        pu();
        mrxr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;
      case '%':
        pd();
        mrxr(pw *(4 + pyx), ph * (6 + pxy));
        pu();
        mrxr(pw *(-3 + pyx), ph * (0 + pxy));
        pd();
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        pu();
        mrxr(pw *(2 + pyx), ph * (-4 + pxy));
        pd();
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        pu();
        mrxr(pw *(1 + chs + pyx), ph * (-2 + pxy));
        break;
      
      case '&':
        pu();
        mrxr(pw *(4 + pyx), ph * (0 + pxy));
        pd();
        mrxr(pw *(-3 + pyx), ph * (4 + pxy));
        mrxr(pw *(0 + pyx), ph * (1 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-3 + pyx), ph * (-2 + pxy));
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (0 + pxy));
        mrxr(pw *(2 + pyx), ph * (2 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-2 + pxy));
        break;
      case '\'':
        pu();
        mrxr(pw *(2 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        pu();
        mrxr(pw *(2 + chs + pyx), ph * (-4 + pxy));
        break;
      case '(':
        pu(); 
        mrxr(pw *(2 + pyx), ph * (6 + pxy)); 
        pd(); 
        mrxr(pw *(-1 + pyx), ph * (-2 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-2 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-2 + pxy)); 
        pu(); 
        mrxr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;
      case ')':
        pu(); 
        mrxr(pw *(2 + pyx), ph * (6 + pxy)); pd(); 
        mrxr(pw *(1 + pyx), ph * (-2 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-2 + pxy)); 
        pu(); 
        mrxr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;        
      case '*':
        pu();
        mrxr(pw *(3 + pyx), ph * (5 + pxy));
        pd();
        mrxr(pw *(-2 + pyx), ph * (-4 + pxy));
        pu();
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (-4 + pxy));
        pu();
        mrxr(pw *(-3 + pyx), ph * (2 + pxy));
        pd();
        mrxr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-3 + pxy));
        break;
      case '+':
        pu(); 
        mrxr(pw *(0 + pyx), ph * (3 + pxy)); 
        pd(); 
        mrxr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(-2 + pyx), ph * (2 + pxy)); pd();  
        mrxr(pw *(0 + pyx), ph * (-4 + pxy)); pu(); 
        mrxr(pw *(2 + chs + pyx), ph * (-1 + pxy));
        break;
      case ',':
        pu(); 
        mrxr(pw *(2 + pyx), ph * (1 + pxy)); 
        pd(); 
        mrxr(pw *(0 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        pu(); 
        mrxr(pw *(3 + chs + pyx), ph * (1 + pxy));
        break;
      case '-':
        pu(); 
        mrxr(pw *(0 + pyx), ph * (3 + pxy)); 
        pd(); 
        mrxr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (-3 + pxy));
        break;        
      case '.':
        pu();
        mrxr(pw *(2 + pyx), ph * (1 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;
      case '/':
        pd(); 
        mrxr(pw *(4 + pyx), ph * (6 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (-6 + pxy));
        break;

      case '0':
        pd(); 
        mrxr(pw *(4 + pyx), ph * (6 + pxy)); 
        pu(); 
        mrxr(pw *(-1 + pyx), ph * (0 + pxy)); 
        pd(); 
        mrxr(pw *(-2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (4 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (1 + pxy)); 
        pu(); 
        mrxr(pw *(1 + chs + pyx), ph * (-6 + pxy));
        break;
      case '1':
        pu(); 
        mrxr(pw *(1 + pyx), ph * (4 + pxy)); 
        pd();  
        mrxr(pw *(2 + pyx), ph * (2 + pxy));  
        mrxr(pw *(0 + pyx), ph * (-6 + pxy)); 
        pu();  
        mrxr(pw *(1 + chs + pyx), ph * (0 + pxy));
        break;
      case '2':
        pu();  
        mrxr(pw *(0 + pyx), ph * (5 + pxy)); 
        pd();  
        mrxr(pw *(1 + pyx), ph * (1 + pxy));  
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));  
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));  
        mrxr(pw *(-4 + pyx), ph * (-3 + pxy));  
        mrxr(pw *(0 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu();  
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case '3':
        pu();  
        mrxr(pw *(0 + pyx), ph * (5 + pxy));
        pd();  
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        pu();  
        mrxr(pw *(4 + chs + pyx), ph * (-1 + pxy));
        break;
      case '4':
        pu(); 
        mrxr(pw *(2 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(-2 + pyx), ph * (-5 + pxy));
        mrxr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        pu();
        mrxr(pw *(1 + chs + pyx), ph * (0 + pxy));
        break;
      case '5':
        pu();
        mrxr(pw *(4 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(-4 + pyx), ph * (0 + pxy));
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        pu();
        mrxr(pw *(4 + chs + pyx), ph * (-1 + pxy));
        break;
      case '6':
        pu();
        mrxr(pw *(3 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-4 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(0 + pyx), ph * (2 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(4 + chs + pyx), ph * (-3 + pxy));
        break;
      case '7':
        pu();
        mrxr(pw *(0 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(4 + pyx), ph * (0 + pxy));
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-4 + pyx), ph * (-5 + pxy));
        pu();
        mrxr(pw *(4 + chs + pyx), ph * (0 + pxy));
        break;
      case '8':
        pu(); 
        mrxr(pw *(3 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(0 + pyx), ph * (2 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        pu();
        mrxr(pw *(3 + chs + pyx), ph * (-4 + pxy));
        break;
      case '9':
        pu();
        mrxr(pw *(1 + pyx), ph * (0 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-3 + pxy));
        break;
      case ':':
        pu();
        mrxr(pw *(2 + pyx), ph * (3 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;
      case ';':
        pu();
        mrxr(pw *(2 + pyx), ph * (3 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(3 + chs + pyx), ph * (1 + pxy));
        break;
      case '<':
        pu();
        mrxr(pw *(3 + pyx), ph * (5 + pxy));
        pd();
        mrxr(pw *(-2 + pyx), ph * (-2 + pxy));
        mrxr(pw *(2 + pyx), ph * (-2 + pxy));
        pu();
        mrxr(pw *(1 + chs + pyx), ph * (-1 + pxy));
        break;
      case '=':
        pu();
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(-4 + pyx), ph * (-2 + pxy));
        pd();
        mrxr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-2 + pxy));
        break;
      case '>':
        pu();
        mrxr(pw *(1 + pyx), ph * (5 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (-2 + pxy));
        mrxr(pw *(-2 + pyx), ph * (-2 + pxy));
        pu();
        mrxr(pw *(3 + chs + pyx), ph * (-1 + pxy));
        break;
      case '?':
        pu();
        mrxr(pw *(0 + pyx), ph * (5 + pxy));
        pd();
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break; 
      case '@':
        pd();
        mrxr(pw *(0 + pyx), ph * (5 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-4 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(0 + pyx), ph * (3 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-4 + pxy));
        pu();
        mrxr(pw *(1 + chs + pyx), ph * (0 + pxy));
        break;       
      case 'A':
        pd(); 
        mrxr(pw *(0 + pyx), ph * (4 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (2 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (-2 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-4 + pxy)); 
        pu(); 
        mrxr(pw *(-4 + pyx), ph * (3 + pxy)); 
        pd();
        mrxr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (-3 + pxy));
        break;
      case 'B':
        pd(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        mrxr(pw *(3 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(3 + pyx), ph * (0 + pxy)); 
        pd(); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(4 + chs + pyx), ph * (0 + pxy));
        break;        
      case 'C':
        pu(); 
        mrxr(pw *(4 + pyx), ph * (5 + pxy)); pd(); 
        mrxr(pw *(-1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(-2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (1 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (-1 + pxy));
        break;
      case 'D':
        pd(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        mrxr(pw *(3 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(4 + chs + pyx), ph * (0 + pxy));
        break;        
      case 'E':
        pu(); 
        mrxr(pw *(4 + pyx), ph * (6 + pxy)); 
        pd(); 
        mrxr(pw *(-4 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-6 + pxy)); 
        mrxr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(-4 + pyx), ph * (3 + pxy)); 
        pd(); 
        mrxr(pw *(3 + pyx), ph * (0 + pxy)); 
        pu(); mrxr(pw *(1 + chs + pyx), ph * (-3 + pxy));
        break;
      case 'F':
        pu(); 
        mrxr(pw *(4 + pyx), ph * (6 + pxy)); 
        pd(); 
        mrxr(pw *(-4 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-6 + pxy)); 
        pu(); 
        mrxr(pw *(3 + pyx), ph * (3 + pxy)); 
        pd(); 
        mrxr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(4 + chs + pyx), ph * (-3 + pxy));
        break;
      case 'G':
        pu(); 
        mrxr(pw *(4 + pyx), ph * (5 + pxy)); 
        pd(); 
        mrxr(pw *(-1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(-2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(-2 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(2 + chs + pyx), ph * (-2 + pxy));
        break;        
      case 'H':
        pu(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mrxr(pw *(0 + pyx), ph * (-6 + pxy)); pu(); 
        mrxr(pw *(4 + pyx), ph * (6 + pxy)); 
        pd(); 
        mrxr(pw *(0 + pyx), ph * (-6 + pxy)); 
        pu(); 
        mrxr(pw *(-4 + pyx), ph * (3 + pxy)); 
        pd(); 
        mrxr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (-3 + pxy));  
        break;
      case 'I':
        pu(); 
        mrxr(pw *(1 + pyx), ph * (0 + pxy)); 
        pd(); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mrxr(pw *(-2 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(1 + pyx), ph * (0 + pxy)); 
        pd(); 
        mrxr(pw *(0 + pyx), ph * (-6 + pxy)); 
        pu(); 
        mrxr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;        
      case 'J':
        pu(); 
        mrxr(pw *(0 + pyx), ph * (1 + pxy)); 
        pd(); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (5 + pxy)); 
        mrxr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(3 + chs + pyx), ph * (-6 + pxy));
        break;
      case 'K':
        pd(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pu(); 
        mrxr(pw *(4 + pyx), ph * (0 + pxy)); 
        pd(); 
        mrxr(pw *(-4 + pyx), ph * (-4 + pxy)); 
        pu(); 
        mrxr(pw *(1 + pyx), ph * (1 + pxy)); 
        pd(); 
        mrxr(pw *(3 + pyx), ph * (-3 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;        
      case 'L':
        pu(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mrxr(pw *(0 + pyx), ph * (-6 + pxy)); 
        mrxr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'M':
        pd(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (-2 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (2 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-6 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;        
      case 'N':
        pd(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        mrxr(pw *(4 + pyx), ph * (-6 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (-6 + pxy));
        break;
      case 'O':
        pu(); 
        mrxr(pw *(1 + pyx), ph * (0 + pxy)); 
        pd(); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (4 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        pu(); 
        mrxr(pw *(3 + chs + pyx), ph * (0 + pxy));
        break;
      case 'P':
        pd(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        mrxr(pw *(3 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(4 + chs + pyx), ph * (-3 + pxy));
        break;
      case 'Q':
        pu(); 
        mrxr(pw *(1 + pyx), ph * (0 + pxy)); 
        pd(); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (4 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        pu(); 
        mrxr(pw *(1 + pyx), ph * (2 + pxy)); 
        pd(); 
        mrxr(pw *(2 + pyx), ph * (-2 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (0 + pxy));        
        break;
      case 'R':
        pd(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        mrxr(pw *(3 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(-3 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        pd(); 
        mrxr(pw *(2 + pyx), ph * (-3 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;        
      case 'S':
        pu(); 
        mrxr(pw *(0 + pyx), ph * (1 + pxy)); 
        pd(); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(-1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (-5 + pxy));
        break;
      case 'T':
        pu(); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        pd(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pu(); 
        mrxr(pw *(-2 + pyx), ph * (0 + pxy)); 
        pd(); 
        mrxr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (-6 + pxy));
        break;
      case 'U':
        pu(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mrxr(pw *(0 + pyx), ph * (-5 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (5 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (-6 + pxy));
        break;         
      case 'V':
        pu(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mrxr(pw *(0 + pyx), ph * (-4 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (-2 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (2 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (4 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (-6 + pxy));
        break;
      case 'W':
        pu(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mrxr(pw *(0 + pyx), ph * (-6 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (2 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (-2 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (-6 + pxy));
        break;         
      case 'X':
        pd(); 
        mrxr(pw *(4 + pyx), ph * (6 + pxy)); 
        pu(); 
        mrxr(pw *(-4 + pyx), ph * (0 + pxy)); 
        pd(); 
        mrxr(pw *(4 + pyx), ph * (-6 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'Y':
        pu(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mrxr(pw *(0 + pyx), ph * (-2 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (-1 + pxy)); 
        mrxr(pw *(2 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(1 + pyx), ph * (1 + pxy)); 
        mrxr(pw *(0 + pyx), ph * (2 + pxy)); 
        pu(); 
        mrxr(pw *(-2 + pyx), ph * (-3 + pxy)); 
        pd(); 
        mrxr(pw *(0 + pyx), ph * (-3 + pxy)); 
        pu(); 
        mrxr(pw *(2 + chs + pyx), ph * (0 + pxy)); 
        break;
      case 'Z':
        pu(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); 
        mrxr(pw *(4 + pyx), ph * (0 + pxy)); 
        mrxr(pw *(-4 + pyx), ph * (-6 + pxy)); 
        mrxr(pw *(4 + pyx), ph * (0 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;         
      case '[':
        pu();
        mrxr(pw *(3 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(0 + pyx), ph * (-6 + pxy));
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(1 + chs + pyx), ph * (0 + pxy));
        break;
      case '\\':
        pu(); 
        mrxr(pw *(0 + pyx), ph * (6 + pxy)); 
        pd(); mrxr(pw *(4 + pyx), ph * (-6 + pxy)); 
        pu(); 
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case ']':
        pu();
        mrxr(pw *(1 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        mrxr(pw *(0 + pyx), ph * (-6 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy)); 
        pu();
        mrxr(pw *(3 + chs + pyx), ph * (0 + pxy)); 
        break;
      case '^':
        pu();
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (2 + pxy));
        mrxr(pw *(2 + pyx), ph * (-2 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case '_':
        pu();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pd();
        mrxr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (1 + pxy));
        break;
      case '`':
        pu();
        mrxr(pw *(1 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (-2 + pxy));
        pu();
        mrxr(pw *(1 + chs + pyx), ph * (-4 + pxy));
        break;
      case 'a':
        pu();
        mrxr(pw *(1 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-3 + pxy));
        pu();
        mrxr(pw *(0 + pyx), ph * (1 + pxy));
        pd();
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-1 + pxy));
        break;
      case 'b':
        pd();
        mrxr(pw *(0 + pyx), ph * (6 + pxy));
        pu();
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        pd();
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-3 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(4 + chs + pyx), ph * (0 + pxy));
        break;
      case 'c':
        pu();
        mrxr(pw *(4 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(-3 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'd':
        pu();
        mrxr(pw *(4 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-6 + pxy));
        mrxr(pw *(-3 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(0 + pyx), ph * (2 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case 'e':
        pu();
        mrxr(pw *(0 + pyx), ph * (2 + pxy));
        pd();
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'f':
        pu();
        mrxr(pw *(1 + pyx), ph * (0 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (5 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(-4 + pyx), ph * (-3 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(2 + chs + pyx), ph * (-3 + pxy));
        break;
      case 'g':
        pu();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pd();
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        mrxr(pw *(-3 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-1 + pxy));
        break;
      case 'h':
        pd();
        mrxr(pw *(0 + pyx), ph * (6 + pxy));
        pu();
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        pd();
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-3 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'i':
        pu();
        mrxr(pw *(1 + pyx), ph * (0 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(-2 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(1 + pyx), ph * (0 + pxy));
        mrxr(pw *(0 + pyx), ph * (-4 + pxy));
        pu();
        mrxr(pw *(0 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(2 + chs + pyx), ph * (-5 + pxy));
        break;
      case 'j':
        pu();
        mrxr(pw *(1 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        mrxr(pw *(0 + pyx), ph * (-4 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(1 + pyx), ph * (7 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(2 + chs + pyx), ph * (-5 + pxy));
        break;
      case 'k':
        pd();
        mrxr(pw *(0 + pyx), ph * (6 + pxy));
        pu();
        mrxr(pw *(4 + pyx), ph * (-2 + pxy));
        pd();
        mrxr(pw *(-4 + pyx), ph * (-2 + pxy));
        mrxr(pw *(4 + pyx), ph * (-2 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'l':
        pu();
        mrxr(pw *(1 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(1 + pyx), ph * (0 + pxy));
        mrxr(pw *(0 + pyx), ph * (-6 + pxy));
        pu();
        mrxr(pw *(-1 + pyx), ph * (0 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(1 + chs + pyx), ph * (0 + pxy));
        break;
      case 'm':
        pd();
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        mrxr(pw *(1 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(0 + pyx), ph * (-3 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        mrxr(pw *(1 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-3 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'n':
        pd();
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-3 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'o':
        pu();
        mrxr(pw *(1 + pyx), ph * (0 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(0 + pyx), ph * (2 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(3 + chs + pyx), ph * (0 + pxy));
        break;
      case 'p':
        pd();
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(0 + pyx), ph * (2 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-3 + pyx), ph * (0 + pxy));
        mrxr(pw *(0 + pyx), ph * (-5 + pxy));
        pu();
        mrxr(pw *(4 + chs + pyx), ph * (1 + pxy));
        break;
      case 'q':
        pu();
        mrxr(pw *(4 + pyx), ph * (-1 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (5 + pxy));
        mrxr(pw *(-3 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'r':
        pd();
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-3 + pxy));
        break;
      case 's':
        pd();
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(-2 + pyx), ph * (0 + pxy));
        mrxr(pw *(-1 + pyx), ph * (1 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case 't':
        pu();
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(-1 + pyx), ph * (2 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-5 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(2 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'u':
        pu();
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-3 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case 'v':
        pu();
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(2 + pyx), ph * (-4 + pxy));
        mrxr(pw *(2 + pyx), ph * (4 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case 'w':
        pu();
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(1 + pyx), ph * (-4 + pxy));
        mrxr(pw *(1 + pyx), ph * (2 + pxy));
        mrxr(pw *(1 + pyx), ph * (-2 + pxy));
        mrxr(pw *(1 + pyx), ph * (4 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-4 + pxy));
        break;
      case 'x':
        pd();
        mrxr(pw *(4 + pyx), ph * (4 + pxy));
        pu();
        mrxr(pw *(-4 + pyx), ph * (0 + pxy));
        pd();
        mrxr(pw *(4 + pyx), ph * (-4 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case 'y':
        pu();
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        pd();
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        pu();
        mrxr(pw *(-4 + pyx), ph * (0 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-2 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(3 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-1 + pxy));
        break;
      case 'z':
        pu();
        mrxr(pw *(0 + pyx), ph * (4 + pxy));
        pd();
        mrxr(pw *(4 + pyx), ph * (0 + pxy));
        mrxr(pw *(-4 + pyx), ph * (-4 + pxy));
        mrxr(pw *(4 + pyx), ph * (0 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (0 + pxy));
        break;
      case '{':
        pu();
        mrxr(pw *(3 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(1 + chs + pyx), ph * (0 + pxy));
        break;
      case '|':
        pu();
        mrxr(pw *(2 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(0 + pyx), ph * (-6 + pxy));
        pu();
        mrxr(pw *(2 + chs + pyx), ph * (0 + pxy));
        break;
      case '}':
        pu();
        mrxr(pw *(1 + pyx), ph * (6 + pxy));
        pd();
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        mrxr(pw *(1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        mrxr(pw *(0 + pyx), ph * (-1 + pxy));
        mrxr(pw *(-1 + pyx), ph * (-1 + pxy));
        pu();
        mrxr(pw *(3 + chs + pyx), ph * (0 + pxy));
        break;
      case '~':
        pu();
        mrxr(pw *(0 + pyx), ph * (3 + pxy));
        pd();
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        mrxr(pw *(2 + pyx), ph * (-2 + pxy));
        mrxr(pw *(1 + pyx), ph * (1 + pxy));
        pu();
        mrxr(pw *(chs + pyx), ph * (-3 + pxy));
        break;
      case '\n':
        LineFeed();
        break;
      case '\r':
        crgRet();
        break;
      default:
        pd(); pu();
        mrxr(pw, 0);
        pd(); pu();
        mrxr(pw, 0);
        pd(); pu();
        mrxr(pw, 0);
        pd(); pu();
        mrxr(pw, 0);
        pd(); pu();
        mrxr(pw *chs, 0);        
        break;
    }  
*/
}


void drawVelkyHacek(float pw, float ph, float chs) {
  pu(); 
  mrxr(pw *(1 + 0), ph * (8 + 0)); 
  pd(); 
  mrxr(pw *(1 + 0), ph * (-1 + 0)); 
  mrxr(pw *(1 + 0), ph * (1 + 0)); 
  pu(); 
  mrxr(pw *(-3 + 0), ph * (-8 + 0));
}

void drawVelkouCarku(float pw, float ph, float chs) {
  pu(); 
  mrxr(pw *(2 + 0), ph * (7 + 0)); 
  pd(); 
  mrxr(pw *(1 + 0), ph * (1 + 0)); 
  pu(); 
  mrxr(pw *(-3 + 0), ph * (-8 + 0));
}

float txtStepsLenght(String txt) {
  float delka;
  delka = txt.length();
  
  return (delka * chCX * (chW + charSpace));
}

void LineFeed() {
  pu();
  mrxr(0, -chCY * (chH + rowSpace));
}

void crgRet() {
  mr(-1 * wdth, 0);
  PosX = 0;
  mrxr(lftBorder, 0);
}

void setCharacterSize(int charWidth, int charHeight) {
  chCX = charWidth;
  chCY = charHeight;
}
