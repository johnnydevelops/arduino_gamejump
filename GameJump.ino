/********************************************************************************************************************************************************************************************************
 
 Jumping!
 
 Created by Johnny_11
 Help from @schmitzoide

 Version 1.0

 (Uses LCD1602 screen)

 WARNING: Set partition scheme to Huge APP
 
*********************************************************************************************************************************************************************************************************/

// Include
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Variables
#define SDA 13
#define SCL 14
#define waitfor 100
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

LiquidCrystal_I2C lcd(0x27,16,2);

int xyzPins[] = {32, 33, 12};   //x,y,z pins

int posC = 2;
int posR = 1;
int Score = 0;

int limitC = 15;
int limitR = 1;

String manS = "o";
String obsS = "O";

int obsPos = 15;
int obsPassed = 0;

bool gameOver = false;

// Main code
void setup() {

  // Joystick
  
  Serial.begin(115200);
  pinMode(xyzPins[2], INPUT_PULLUP);  //z axis is a button.
  
  // LCD
  
  // Wire up everything
  
  Wire.begin(SDA, SCL);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  
  // Intro screen
  
  lcd.print("You are a ball.");
  delay(3000);
  lcd.clear();
  lcd.print("Dodge 10");
  lcd.setCursor(0,1);
  lcd.print("boulders to win!");
  delay(3000);
  
  // Game
  
  obsPos = 15;
  lcd.setCursor(3,1);
  lcd.print(manS);
}

void move(int xVal, int yVal) {
  
  // Allows player to move between collumn 0 and 15 and jumping
  
  if (posC + xVal >= 0 && posR + yVal >= 0) {
    posC = MIN(15,posC + xVal);
    posR = MIN(1,yVal+1);
    Serial.printf("C: ");
    Serial.println(posC);
    Serial.printf("R: ");
    Serial.println(posR);
    lcd.clear();
    lcd.setCursor(posC,posR);
    lcd.print(manS);
  }
}

void gameOverScreenInit(String output) {
  
  // Screen, when died or win
  
  gameOver = true;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(output);
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(obsPassed);
}

void checkCollision() {
  
  // Check for collisions
  
  if (posR == 1) {
    if (posC == obsPos) {
      gameOverScreenInit("GAME OVER");
    }
  }
}

void checkWin() {
  
  // Verify, if the player has completed the quest.
  
  if (obsPassed == 10) {
    
    // If he completed it, show the win screen
    
    gameOverScreenInit("WIN");
  }
}

void obstacle() {
  
  // Here's the fun part. The obstacles.
  
  lcd.clear();
  lcd.setCursor(posC,posR);
  lcd.print(manS);
  lcd.setCursor(obsPos,1);
  lcd.print(obsS);
  obsPos = obsPos - 1;
  if (obsPos == -1) {
    
    // Safety code (kinda)
    
    obsPos = -20;
    lcd.clear();
    lcd.setCursor(posC,posR);
    lcd.print(manS);
    lcd.setCursor(obsPos,1);
    lcd.print(obsS);
  }
  if (obsPos == -30) {
    
    // Reset position after passed
    
    obsPassed = obsPassed + 1;
    obsPos = 15;
    lcd.setCursor(posC,posR);
    lcd.print(manS);
    lcd.setCursor(obsPos,1);
    lcd.print(obsS);
  }
  
  // Run the collision function
  
  checkCollision();
}

void restart() {
  
  // Restart the game (kinda the same as setup)
  
  lcd.setCursor(3,1);
  lcd.print(manS);
  obsPos = 15;
  gameOver = false;
  Score = 0;
  obsPassed = 0;
  posC = 2;
  posR = 1;
}

void loop() {
  
  // The whole game handler
  
  // Joystick
  
  int xVal = analogRead(xyzPins[0]) / 4095.0 * 2 - 1;
  int yVal = analogRead(xyzPins[1]) / 4095.0 * 2 - 1;
  int zVal = !digitalRead(xyzPins[2]);
  // Game
  
  if (gameOver == false) {
    
     // If not game over, then repeat this code
     
     move(xVal, yVal);
     obstacle();
     checkWin();

     Score = Score + 1;
     delay(waitfor);
  }
  if (zVal == 1) {
    
    // if joystick button pressed, restart
    
    Serial.println(zVal);
    restart();
  }
}
