#include "LedControl.h" //  need the library
#include <LiquidCrystal.h>
LedControl lc=LedControl(12,11,10,1); // 
int UD = 0;
int LR = 0; //Setting up controller//
int start = A2;
int trageDreapta = A3;
int trageStanga = A4;
int restart = A5;
int SW = 13;
long unsigned int timpMiscare = 0;
long unsigned int ultimulReloadRachete ;
int vitezaReload = 100;
int dificultate1 = 10000;
int dificultate2 = 20000;
int scor = 0;
int vieti = 3;

const int v0 = 9, rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd( rs, en, d4, d5, d6, d7);

int xCurrent = 7, yCurrent = 3; //Pozitia curenta
int xPrev, yPrev; //Pozitia precedenta
boolean GAME_OVER = true;
 
// pin 12 is connected to the MAX7219 pin 1
// pin 11 is connected to the CLK pin 13
// pin 10 is connected to LOAD pin 12
// pin 7 is connected to Start
// pin 5 is connected to Right Pushbutton
// pin 6 is connected to Left Pushbutton
// 1 as we are only using 1 MAX7219

// Messages

int gameOverMessage[10][8]={
  {0x00,0x3c,0x40,0x40,0x48,0x44,0x38,0x00},// G
  {0x00,0x38,0x44,0x7c,0x44,0x44,0x44,0x00},// A
  {0x00,0x44,0x6c,0x54,0x44,0x44,0x44,0x00},// M
  {0x00,0x7c,0x40,0x78,0x40,0x40,0x7c,0x00},// E
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//  (space)
  {0x00,0x38,0x44,0x44,0x44,0x44,0x38,0x00},// O
  {0x00,0x44,0x44,0x44,0x44,0x28,0x10,0x00},// V
  {0x00,0x7c,0x40,0x78,0x40,0x40,0x7c,0x00},// E
  {0x00,0x30,0x48,0x48,0x70,0x48,0x44,0x00},// R
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//  (space)
};

int startNewGameMessage[15][8]={
  {0x00,0x3c,0x40,0x38,0x04,0x44,0x38,0x00},// S
  {0x00,0x7c,0x10,0x10,0x10,0x10,0x10,0x00},// T
  {0x00,0x38,0x44,0x7c,0x44,0x44,0x44,0x00},// A
  {0x00,0x30,0x48,0x48,0x70,0x48,0x44,0x00},// R
  {0x00,0x7c,0x10,0x10,0x10,0x10,0x10,0x00},// T
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//  (space)
  {0x00,0x44,0x64,0x54,0x4c,0x44,0x44,0x00},// N
  {0x00,0x7c,0x40,0x78,0x40,0x40,0x7c,0x00},// E
  {0x00,0x00,0x44,0x54,0x54,0x54,0x38,0x00},// W
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//  (space)
  {0x00,0x3c,0x40,0x40,0x48,0x44,0x38,0x00},// G
  {0x00,0x38,0x44,0x7c,0x44,0x44,0x44,0x00},// A
  {0x00,0x44,0x6c,0x54,0x44,0x44,0x44,0x00},// M
  {0x00,0x7c,0x40,0x78,0x40,0x40,0x7c,0x00},// E
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//  (space)
};

// Grid
byte EMPTY_GRID[8] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

//Rachete

struct Racheta{
  int coordX;
  int coordY;
  long unsigned timpUpdate;

  bool lovesteTinta(int, int);
};

Racheta rachetaActiva[20] = {};
Racheta RACHETA_NULA = {};


bool nuExistaRacheta(Racheta racheta) {
  return (
    racheta.coordX == RACHETA_NULA.coordX &&
    racheta.coordY == RACHETA_NULA.coordY &&
    racheta.timpUpdate == RACHETA_NULA.timpUpdate
  );
}

int gasesteIDRacheta() {
  for (int i = 0; i < 20; i++) {
    if (nuExistaRacheta(rachetaActiva[i])) {
      return i;
    }
  }
}


void stergeRacheta(int IDRacheta) {
  rachetaActiva[IDRacheta] = RACHETA_NULA;
}



void trageRachetaDreapta() {
  int IDRacheta = gasesteIDRacheta();
  if((millis() - ultimulReloadRachete) > vitezaReload)
  {
     if (IDRacheta != -1) {
    rachetaActiva[IDRacheta] = (Racheta){5, yCurrent + 1, millis()};
  }
  ultimulReloadRachete = millis();
  if (millis() == dificultate1)
      {
        vitezaReload = 1000;
      }
  }

  

}

void trageRachetaStanga() {
  int IDRacheta = gasesteIDRacheta();

  if((millis() - ultimulReloadRachete) > vitezaReload)
  {
     if (IDRacheta != -1) {
    rachetaActiva[IDRacheta] = (Racheta){5, yCurrent - 1, millis()};
  }
  ultimulReloadRachete = millis();
  }

}

bool Racheta::lovesteTinta(int tintaCoordX, int tintaCoordY) {
  return (
    coordX == tintaCoordX &&
    coordY == tintaCoordY
  );
}

//Meteori

class Meteor {
  public:
    int coordX = 0; //partea de sus a ecranului
    int coordY;

    int movementSpeed = 1000;

    bool sePoateMisca();
    void miscaMeteor();

  private:
    long unsigned int ultimaMiscare = 0;
};

bool Meteor::sePoateMisca() {
  return (millis() - ultimaMiscare) > movementSpeed;
}

Meteor meteoriActivi[64] = {};
bool meteorOcupat[64] = {};


void Meteor::miscaMeteor() {
  int newCoordX = coordX + 1;  // misca in fata
  int newCoordY = coordY + random(2) - 1;  //misca la stanga, dreapta sau deloc

  bool pozitiaNouaEsteOcupata = false;
  Meteor meteor;

  for (int meteorID = 0; meteorID < 64; meteorID++) {
    if (meteorOcupat[meteorID]) {
      meteor = meteoriActivi[meteorID];

      if (meteor.coordX == newCoordX && meteor.coordY == newCoordY) {
        pozitiaNouaEsteOcupata = true;
        break;
      }
    }
  }

  if (!pozitiaNouaEsteOcupata) {
    lc.setLed(0, meteor.coordX, meteor.coordY, false);
    coordX = newCoordX;
    coordY = newCoordY;
    ultimaMiscare = millis();
  }
}


long ultimulMeteorCreat = 500;
int frecventaMeteori = 2000;

void creeazaMeteor() {
  if ((millis() - ultimulMeteorCreat) < frecventaMeteori) {
    return;
    ultimulMeteorCreat = millis();
  }
      
  Meteor meteor;
  meteor.coordY = random(8);

  if (millis() == dificultate1)
      {
        frecventaMeteori = 1000;
        ultimulMeteorCreat = 250;
        meteor.movementSpeed = 500;
      }
     
   if (millis() == dificultate2)
      {
        frecventaMeteori = 500;
        ultimulMeteorCreat = 150;
        meteor.movementSpeed = 250;
      }
  

  for (int meteorID = 0; meteorID < 64; meteorID++) {
    if (!meteorOcupat[meteorID]) {
      meteorOcupat[meteorID] = true;
      meteoriActivi[meteorID] = meteor;
      break;
    }
  }
}

//Functii de desenare

void deseneazaMeteori() {
  for (int meteorID = 0; meteorID < 64; meteorID++) {
    Meteor meteor = meteoriActivi[meteorID];
    
    if (meteorOcupat[meteorID]) {
      lc.setLed(0, meteor.coordX, meteor.coordY, true);
      lc.setLed(0, meteor.coordX - 1, meteor.coordY, false);
      
      

      if (meteor.sePoateMisca()) {
        meteoriActivi[meteorID].miscaMeteor();

        if (meteor.coordY < 0 || meteor.coordY > 7 || meteor.coordX < 0 ) {
          meteorOcupat[meteorID] = false;
        }
      }
    }
  }
}

void deseneazaRachete() {
  Racheta racheta;
  
  for (int IDRacheta = 0; IDRacheta < 20; IDRacheta++) {
    racheta = rachetaActiva[IDRacheta];

    if (!nuExistaRacheta(racheta)) {
      lc.setLed(0, racheta.coordX, racheta.coordY, true);
      lc.setLed(0, racheta.coordX + 1, racheta.coordY, false);

      if (racheta.coordX > 0) {
        rachetaActiva[IDRacheta].coordX -= 1;
        rachetaActiva[IDRacheta].timpUpdate = millis();
      } if(racheta.coordX == 0)
      {
        stergeRacheta(IDRacheta);
      }
    }
  }
}

//Altele

void resetGame() {
  GAME_OVER = true;
  
  
  lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Scor:");
      lcd.print(scor);
      
  
  drawGrid(EMPTY_GRID);

  for(int meteorID = 0; meteorID <64; meteorID++)
    meteorOcupat[meteorID] = false;

      lc.setLed(0, xCurrent, yCurrent, false);   
      lc.setLed(0, xCurrent, yCurrent + 1, false);  
      lc.setLed(0, xCurrent, yCurrent - 1, false);  
      lc.setLed(0, xCurrent - 1, yCurrent, false); 

      blinkScreen(3);

      for (int i = 0; i < 10; i++) {
        for (int row = 0; row < 8; row++) {
          lc.setColumn(0, row, gameOverMessage[i][7 - row]);
          delay(25);
        }
        delay(100);
      }
      
      
}

void restartGame(){
  
  drawGrid(EMPTY_GRID);
  vieti = 3;
  scor = 0;
  

  for(int meteorID = 0; meteorID <64; meteorID++)
    meteorOcupat[meteorID] = false;

      lc.setLed(0, xCurrent, yCurrent, false);   
      lc.setLed(0, xCurrent, yCurrent + 1, false);  
      lc.setLed(0, xCurrent, yCurrent - 1, false);  
      lc.setLed(0, xCurrent - 1, yCurrent, false); 
      

      blinkScreen(1);

      for (int i = 0; i < 15; i++) {
      for (int row = 0; row < 8; row++) {
        lc.setColumn(0, row, startNewGameMessage[i][7 - row]);
        delay(25);
      }
      
}
}

void blinkScreen(int nTimes) {
  for (int i = 0; i < nTimes; i++) {
    for (int row = 0; row < 8; row++) {
      lc.setColumn(0, row, B11111111);
    }
    delay(50);
    
    for (int row = 0; row < 8; row++) {
      lc.setColumn(0, row, B00000000);
    }
    delay(50);
  }
}
void drawGrid(byte * grid) {
  for (int row = 0; row < 8; row++) {
    lc.setColumn(0, row, grid[row]);
  }
}


//Verificari

void verificaDacaMeteorLovesteNava()
{
  for(int meteorID = 0; meteorID < 64; meteorID++)
  {
    Meteor meteor = meteoriActivi[meteorID];

    if(meteorOcupat[meteorID] && meteor.coordX == xCurrent && meteor.coordY == yCurrent || meteorOcupat[meteorID] && meteor.coordX == xCurrent && meteor.coordY == yCurrent - 1 || meteorOcupat[meteorID] && meteor.coordX == xCurrent && meteor.coordY == yCurrent + 1 || meteorOcupat[meteorID] && meteor.coordX == xCurrent - 1 && meteor.coordY == yCurrent)
    {
      
      if (vieti == 0)
      {
        meteorOcupat[meteorID] = false; 
        resetGame();
        
        lcd.clear();  
        
        
      }
        
      else 
      {
        meteorOcupat[meteorID] = false;
        vieti -= 1; 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Vieti:");
        lcd.print(vieti);
      }
        
    }
    
  }
}

void verificaDacaNavaLovesteMeteor()
{
  Racheta racheta;
  Meteor meteor;

  for(int IDRacheta = 0; IDRacheta < 20; IDRacheta++)
  {
    racheta = rachetaActiva[IDRacheta];

    {
      if(!nuExistaRacheta(racheta))
      {
        for(int meteorID = 0; meteorID < 64; meteorID++)
        {
          meteor = meteoriActivi[meteorID];

          if(meteorOcupat[meteorID] && racheta.lovesteTinta(meteor.coordX, meteor.coordY) || racheta.coordX == meteor.coordX && racheta.coordY == meteor.coordY)
          {
            meteorOcupat[meteorID] = false;
            scor += 1;
          }
        }//meteor
      }
    }
  }//racheta
}

void miscaNava(int xTranslate, int yTranslate)
{
  yPrev = yCurrent;
  if(yTranslate < 0)
   { 
    if(yCurrent - 1 != 0)
     yCurrent -= 1;
   }
   if(yTranslate > 0)
   {
    if(yCurrent + 1 != 7)
      yCurrent += 1;
   }
   if (yCurrent != yPrev)
   {
      if ((yCurrent - yPrev) == 1)
      {
        lc.setLed(0, xCurrent, yCurrent, true);   
        lc.setLed(0, xCurrent, yCurrent + 1, true);  
        lc.setLed(0, xCurrent, yCurrent - 1, true);  
        lc.setLed(0, xCurrent - 1, yCurrent, true);    
        lc.setLed(0, xCurrent, yPrev - 1, false);  
        lc.setLed(0, xCurrent - 1, yPrev, false); 
      }
      else
      {
        lc.setLed(0, xCurrent, yCurrent, true);   
        lc.setLed(0, xCurrent, yCurrent + 1, true);  
        lc.setLed(0, xCurrent, yCurrent - 1, true);  
        lc.setLed(0, xCurrent - 1, yCurrent, true);    
        lc.setLed(0, xCurrent, yPrev + 1, false);  
        lc.setLed(0, xCurrent - 1, yPrev, false);
      }
    }
}




void setup()
{
  
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false);// turn off power saving, enables display
  lc.setIntensity(0, 8);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  pinMode(start, INPUT);
  pinMode(trageDreapta, INPUT);
  pinMode(trageStanga, INPUT);
  pinMode(restart, INPUT);
  Serial.begin(9600);

  analogWrite(v0, 100);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Start joc!");
  delay(1000);
  lcd.clear();
  lcd.print("Vieti:");
  lcd.print(vieti);
  
}


void loop()
{
  UD = analogRead(A0);
  LR = analogRead(A1);
  
  int  st = digitalRead(start);
  int  trageD = digitalRead(trageDreapta);
  int  trageS = digitalRead(trageStanga);
  int re = digitalRead(restart);
  char xTranslate = map( LR, 1021, 0, -3, 4);//mapeaza valorile de la joystick
  char yTranslate = map( UD, 1021, 0, -3, 4);
  unsigned long currentTime=millis();
  
  if(st == HIGH)
  {
    GAME_OVER = false;
    vieti = 3;
    scor = 0;
    blinkScreen(3);
    for (int i = 0; i < 15; i++) {
      for (int row = 0; row < 8; row++) {
        lc.setColumn(0, row, startNewGameMessage[i][7 - row]);
        delay(25);
      }
  }
   
  }
  
    // not in shutdown mode
    if(!GAME_OVER)
    {

      if(re == HIGH)
      {
        restartGame();
      }
     // lcd.setCursor(0, 0);
    //  lcd.print(String("Vieti: ") + String(vieti));
      
        
          //deseneaza nava
      lc.setLed(0, xCurrent, yCurrent, true);   
      lc.setLed(0, xCurrent, yCurrent + 1, true);  
      lc.setLed(0, xCurrent, yCurrent - 1, true);  
      lc.setLed(0, xCurrent - 1, yCurrent, true);

      if (currentTime - timpMiscare >= 30)
      {
        miscaNava(xTranslate, yTranslate);
        timpMiscare = currentTime;
      }
   
      if (random(10) < 3) {
      creeazaMeteor();
    }
   
    deseneazaRachete();
    deseneazaMeteori();
    
    verificaDacaMeteorLovesteNava();
    verificaDacaNavaLovesteMeteor();
    

      if(trageS == HIGH)
      {
        trageRachetaStanga();
      }
      if(trageD == HIGH)
      {
        trageRachetaDreapta();
      }
    
    }
    
    } 

  

      

  


