#include <Arduboy2.h>
#include <Arduboy2Audio.h>
#include <Arduboy2Core.h>
#include <Sprites.h>
#include <ArduboyTones.h>
#include "glcdfont.c"

#include "Arduboy2.h";

static Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

uint8_t points[3] = {0,0,0};
char text_buffer[16]; 
char initials[3];     //Initials used in high score
uint8_t songNum = 1;
uint8_t boardWidth = 10;
uint8_t boardHeight = 21;
int score = 0;
int gridWidth = 6;
float gravSpeed = 1.0;
float flashSpeed = 40/gravSpeed;
uint8_t piecePivot[2] = {1,4};
uint8_t currentMinoCoords[4][2] = {{},{},{},{}};
char currentMino = 'o';
char nextMino = 'o';
char tempMino = 'o';
char bankMino = 'o';
bool showingNextPiece = false;
bool muted = true;
String gameState = "settingUp";
uint8_t deletingRows[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool boardMap[21][10] = {{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}};
uint16_t threeSec = ((((1000/60)*round(48/gravSpeed))*1.024)*3);
uint16_t twoSec = ((((1000/60)*round(48/gravSpeed))*1.024)*2);
uint16_t oneHalfSec = ((((1000/60)*round(48/gravSpeed))*1.024)*1.5);
uint16_t oneSec = (((1000/60)*round(48/gravSpeed))*1.024);
uint16_t halfSec = ((((1000/60)*round(48/gravSpeed))*1.024)/2);
uint16_t tetrisThemeSong1[] = {
  659.25,oneSec, 493.88,halfSec, 523.25,halfSec, 587.33,oneSec, 523.25, halfSec, 493.88, halfSec, 440.00, oneSec, 440.00, halfSec, 523.25,halfSec, 659.25,oneSec,587.33,halfSec,523.25,halfSec,493.88,oneSec, 493.88,halfSec,523.25,halfSec,587.33,oneSec,659.25,oneSec,523.25,oneSec,440.00,oneSec,440.00,oneSec,0,oneSec,587.33,oneHalfSec,698.46,halfSec,880.00,oneSec,783.99,halfSec,698.46,halfSec,659.25,oneHalfSec,523.25,halfSec,659.25,oneSec,587.33,halfSec,523.25,halfSec,493.88,oneSec,493.88,halfSec,523.25,halfSec,587.33,oneSec,659.25,oneSec,523.25,oneSec,440.00,oneSec,440.00,oneSec,0,oneSec,
  TONES_END };
uint16_t tetrisThemeSong2[] = {
  329.63,twoSec,261.63,twoSec,293.66,twoSec,246.94,twoSec,261.63,twoSec,220.00,twoSec,207.65,twoSec,246.94,twoSec,329.63,twoSec,261.63,twoSec,293.66,twoSec,246.94,twoSec,261.63,oneSec,329.63,oneSec,440.00,twoSec,415.30,threeSec,0,oneSec,
  TONES_END };

void playMusic(){
  if (!muted) {
    threeSec = ((((1000/60)*round(48/gravSpeed))*1.024)*3)/2;
    twoSec = ((((1000/60)*round(48/gravSpeed))*1.024)*2)/2;
    oneHalfSec = ((((1000/60)*round(48/gravSpeed))*1.024)*1.5)/2;
    oneSec = (((1000/60)*round(48/gravSpeed))*1.024)/2;
    halfSec = ((((1000/60)*round(48/gravSpeed))*1.024)/2)/2;
    switch(songNum) {
      case 1:
        sound.tonesInRAM(tetrisThemeSong1);
      break;
      case 2:
        sound.tonesInRAM(tetrisThemeSong1);
      break;
      default:
        sound.tonesInRAM(tetrisThemeSong2);
        songNum = 0;
      break;
    };
  };
  };

void clearBoard() {
  for (uint8_t i=0; i < boardHeight; i++){
      for (uint8_t j=0; j < boardWidth; j++){
            boardMap[i][j] = 0;
      };
    };
  for (uint8_t i=0; i < 21; i++){
      deletingRows[i] = 0;
    };
  for (uint8_t i=0; i < 3; i++){
      points[i] = 0;
    };
};

//i used simple point rotation for each piece and ploted the points as each part of the tetramino
void rotatePiece(char dir) {
  if (currentMino!='o') {
  uint8_t tempMinoCoords[4][2] = {{},{},{},{}};
  piecePivot[0] = currentMinoCoords[0][0];
  piecePivot[1] = currentMinoCoords[0][1];
  for (uint8_t i=0; i < 4; i++){
    tempMinoCoords[i][0] = currentMinoCoords[i][0];
    tempMinoCoords[i][1] = currentMinoCoords[i][1];
  }
  int8_t s = -1;
  uint8_t c = 0;
  for (uint8_t i=0; i < 4; i++){
  tempMinoCoords[i][0] -= piecePivot[0];
  tempMinoCoords[i][1] -= piecePivot[1];

  // rotate point
  uint8_t xnew = tempMinoCoords[i][0] * c - tempMinoCoords[i][1] * s;
  uint8_t ynew = tempMinoCoords[i][0] * s + tempMinoCoords[i][1] * c;

  // translate point back:
  tempMinoCoords[i][0] = xnew + piecePivot[0];
  tempMinoCoords[i][1] = ynew + piecePivot[1];
  }
  if (isValidMove(tempMinoCoords)) {
    for (uint8_t i=0; i < 4; i++){
      currentMinoCoords[i][0] = tempMinoCoords[i][0];
      currentMinoCoords[i][1] = tempMinoCoords[i][1];
    }
   }
  }
};

//draws the game to the screen, pretty simple
void drawBoard() {
   for (uint8_t i=0; i < boardHeight; i++){
      for (uint8_t j=0; j < boardWidth; j++){
        if (boardMap[i][j]) {
            arduboy.drawRect((128-(6.4))-(i*gridWidth),1+(j*gridWidth),gridWidth,gridWidth,WHITE);
          if ((deletingRows[i]>(flashSpeed/4) && deletingRows[i]<(flashSpeed/2))||(deletingRows[i]>((flashSpeed/4)*3) && deletingRows[i]<flashSpeed)) {
            arduboy.fillRect((128-(6.4))-(i*gridWidth),1+(j*gridWidth),gridWidth,gridWidth,WHITE);
          };
        };
      };
    };
    for (uint8_t i=0; i < 4; i++){
      arduboy.drawRect((128-(6.4))-(currentMinoCoords[i][0]*gridWidth),1+(currentMinoCoords[i][1]*gridWidth),gridWidth,gridWidth,WHITE);
    };
    arduboy.drawFastVLine(0,0,61);
    arduboy.drawFastHLine(0,0,128);
    arduboy.drawFastHLine(0,61,128);
    drawChar_Rot90(127,14,scoreChar(2),WHITE,BLACK,1);
    drawChar_Rot90(127,21,scoreChar(1),WHITE,BLACK,1);
    drawChar_Rot90(127,28,scoreChar(0),WHITE,BLACK,1);
    //a girl that i tutor complained that the points looked low to her, so i just added two zeros to the end lol
    drawChar_Rot90(127,35,'0',WHITE,BLACK,1);
    drawChar_Rot90(127,42,'0',WHITE,BLACK,1);
    if (showingNextPiece) showGUI();
    arduboy.display();
  };

// since the screen is sidways, i plan to make a font and so by seperating the score into three seperate digits, putting the score on the screen will be much easier in the future
int addPoint(uint8_t amnt) {
  points[0]+=amnt;
  gravSpeed += 0.1;
  flashSpeed = 48/gravSpeed;
  if (points[0]>=10) {
      points[1]+=(floor(points[0]/boardWidth));
      points[0]=points[0]%boardWidth;
    };
  if (points[1]>=10) {
      points[2]+=(floor(points[1]/boardWidth));
      points[1]=points[1]%boardWidth;
    };
  score = (points[2]*100)+(points[1]*10)+(points[0]);
  };

//just picks a random mino for either the first piece or the next piece
void getNextMino(bool first) {
    switch (round(random(1,8))) {
      case 1:
        nextMino = 'i';
        if (first) currentMino = 'i';
      break;
      case 2:
        nextMino = 'l';
        if (first) currentMino = 'l';
      break;
      case 3:
        nextMino = 'j';
        if (first) currentMino = 'j';
      break;
      case 4:
        nextMino = 't';
        if (first) currentMino = 't';
      break;
      case 5:
        nextMino = 's';
        if (first) currentMino = 's';
      break;
      case 6:
        nextMino = 'z';
        if (first) currentMino = 'z';
      break;
      default:
        nextMino = 'o';
        if (first) currentMino = 'o';
      break;
      };
  }

//get the points for each type of tetramino
void createPiece(char type) {
  currentMino = type;
  tempMino = type;
    switch (type) {
      case 'o':
        currentMinoCoords[0][0] = 1;
        currentMinoCoords[0][1] = 4;
        currentMinoCoords[1][0] = 2;
        currentMinoCoords[1][1] = 4;
        currentMinoCoords[2][0] = 1;
        currentMinoCoords[2][1] = 5;
        currentMinoCoords[3][0] = 2;
        currentMinoCoords[3][1] = 5;
        piecePivot[1] = 1;
        piecePivot[2] = 1;
      break;
      case 'l':
        currentMinoCoords[0][0] = 2;
        currentMinoCoords[0][1] = 4;
        currentMinoCoords[1][0] = 2;
        currentMinoCoords[1][1] = 3;
        currentMinoCoords[2][0] = 2;
        currentMinoCoords[2][1] = 5;
        currentMinoCoords[3][0] = 1;
        currentMinoCoords[3][1] = 5;
        piecePivot[1] = 2;
        piecePivot[2] = 4;
      break;
      case 'j':
        currentMinoCoords[0][0] = 2;
        currentMinoCoords[0][1] = 4;
        currentMinoCoords[1][0] = 2;
        currentMinoCoords[1][1] = 3;
        currentMinoCoords[2][0] = 2;
        currentMinoCoords[2][1] = 5;
        currentMinoCoords[3][0] = 1;
        currentMinoCoords[3][1] = 3;
        piecePivot[1] = 2;
        piecePivot[2] = 4;
      break;
      case 't':
        currentMinoCoords[0][0] = 2;
        currentMinoCoords[0][1] = 4;
        currentMinoCoords[1][0] = 2;
        currentMinoCoords[1][1] = 3;
        currentMinoCoords[2][0] = 2;
        currentMinoCoords[2][1] = 5;
        currentMinoCoords[3][0] = 1;
        currentMinoCoords[3][1] = 4;
        piecePivot[1] = 2;
        piecePivot[2] = 4;
      break;
      case 's':
        currentMinoCoords[0][0] = 2;
        currentMinoCoords[0][1] = 4;
        currentMinoCoords[1][0] = 2;
        currentMinoCoords[1][1] = 3;
        currentMinoCoords[2][0] = 1;
        currentMinoCoords[2][1] = 5;
        currentMinoCoords[3][0] = 1;
        currentMinoCoords[3][1] = 4;
        piecePivot[1] = 2;
        piecePivot[2] = 4;
      break;
      case 'z':
        currentMinoCoords[0][0] = 2;
        currentMinoCoords[0][1] = 4;
        currentMinoCoords[1][0] = 1;
        currentMinoCoords[1][1] = 3;
        currentMinoCoords[2][0] = 2;
        currentMinoCoords[2][1] = 5;
        currentMinoCoords[3][0] = 1;
        currentMinoCoords[3][1] = 4;
        piecePivot[1] = 2;
        piecePivot[2] = 4;
      break;
      case 'i':
        currentMinoCoords[0][0] = 1;
        currentMinoCoords[0][1] = 4;
        currentMinoCoords[1][0] = 1;
        currentMinoCoords[1][1] = 3;
        currentMinoCoords[2][0] = 1;
        currentMinoCoords[2][1] = 5;
        currentMinoCoords[3][0] = 1;
        currentMinoCoords[3][1] = 6;
        piecePivot[1] = 1;
        piecePivot[2] = 5;
      break;
    };
  };

//checks if the move is valid and returns the answer
bool isValidMove(uint8_t coords[4][2]) {
    for (uint8_t i=0; i < 4; i++){
        if (boardMap[coords[i][0]][coords[i][1]] == 1) return false;
        if (coords[i][0] >= boardHeight) return false;
        if (coords[i][1] >= boardWidth) return false;
        if (coords[i][1] < 0) return false;
    };
    return true;
  };

//overlay the piece on the board, i keep the board and the piece seperate until the piece lands
void addPiece() {
    for (uint8_t i=0; i < 4; i++){
      boardMap[currentMinoCoords[i][0]][currentMinoCoords[i][1]] = 1;
    };
  };

//test each row and returns which row is full
void testRows() {
    for (uint8_t i=0; i < boardHeight; i++) {
      if (isFull(i)) {getReadyToRemove(i);};
      }
  };

//actually tests a row to see if it is full
bool isFull(uint8_t row) {
  uint8_t temp = 0;
    for (uint8_t i=0; i < boardWidth; i++){
          if (boardMap[row][i]) temp++;
        };
    if (temp >= boardWidth) return true;
    return false;
  };

//just checks the keys and does what is needed accordingly
void checkKeys() {
  arduboy.pollButtons();
  if (arduboy.pressed(LEFT_BUTTON)) updatePiece('d');
  if (arduboy.pressed(A_BUTTON)) {showingNextPiece = true;} else {showingNextPiece = false;}
  if (arduboy.justPressed(B_BUTTON)) {
      currentMino = bankMino;
      bankMino = tempMino;
      createPiece(currentMino); 
    };
  if (arduboy.justPressed(RIGHT_BUTTON)) rotatePiece('l');
  if (arduboy.pressed(DOWN_BUTTON) && arduboy.pressed(UP_BUTTON)) pauseMenu();
  if (arduboy.justPressed(DOWN_BUTTON)) updatePiece('r');
  if (arduboy.justPressed(UP_BUTTON)) updatePiece('l');
  }

void getReadyToRemove(uint8_t row) {
  if (deletingRows[row]<flashSpeed) deletingRows[row]++;
  if (deletingRows[row]>=flashSpeed) {removeRow(row);deletingRows[row]=0;}
}

int comboAmnt() {
  int combos = 0;
    for (byte i = 0; i < 21; i++) {
        if (deletingRows[i]!=0) combos ++;
      }
      return combos;
  }

//removes the row that is specified
void removeRow(uint8_t row) {
  bool tempBoardMap[10] = {0,0,0,0,0,0,0,0,0,0};
  uint8_t temp = 0;
  bool skipper = 0;
  for (uint8_t i=0; i < boardHeight; i++){
    if (i == row) skipper++;
    for (uint8_t j=0; j < boardWidth; j++){
        temp = boardMap[i+skipper][j];
        boardMap[i][j] = tempBoardMap[j];
        tempBoardMap[j] = temp;
      };
    };
  addPoint(comboAmnt());
  }

//moves a piece in the selected direction if possible
void updatePiece(char dir) {
    uint8_t next_currentMinoCoords[4][2] = {};
    for (uint8_t i=0; i < 4; i++){
      for (uint8_t j=0; j < 2; j++){
        next_currentMinoCoords[i][j]=currentMinoCoords[i][j];
      };
    };
    
    switch (dir) {
      case ('d'):
        for (uint8_t i=0; i < 4; i++){
            boardMap[currentMinoCoords[i][0]][currentMinoCoords[i][1]] = 0;
            next_currentMinoCoords[i][0]=currentMinoCoords[i][0]+1;
        };
        if (isValidMove(next_currentMinoCoords)) {
          for (uint8_t i=0; i < 4; i++){
            for (uint8_t j=0; j < 2; j++){
              currentMinoCoords[i][j]=next_currentMinoCoords[i][j];
            };
          };
        } else {
          addPiece();
          createPiece(nextMino);
          if (!isValidMove(currentMinoCoords)) loseMenu();
          currentMino = nextMino;
          tempMino = currentMino;
          getNextMino(false);
        };
      break;
      case ('l'):
        for (uint8_t i=0; i < 4; i++){
            boardMap[currentMinoCoords[i][0]][currentMinoCoords[i][1]] = 0;
            next_currentMinoCoords[i][1]=currentMinoCoords[i][1]-1;
        };
        if (isValidMove(next_currentMinoCoords)) {
          for (uint8_t i=0; i < 4; i++){
            for (uint8_t j=0; j < 2; j++){
              currentMinoCoords[i][j]=next_currentMinoCoords[i][j];
            };
          };
        };
      break;
      case ('r'):
        for (uint8_t i=0; i < 4; i++){
            boardMap[currentMinoCoords[i][0]][currentMinoCoords[i][1]] = 0;
            next_currentMinoCoords[i][1]=currentMinoCoords[i][1]+1;
        };
        if (isValidMove(next_currentMinoCoords)) {
          for (uint8_t i=0; i < 4; i++){
            for (uint8_t j=0; j < 2; j++){
              currentMinoCoords[i][j]=next_currentMinoCoords[i][j];
            };
          };
        };
      break;
    };
  };

//displays the main menu
void mainMenu() {
  gameState = "mainMenu";
  arduboy.pollButtons();
  songNum = 1;
  gravSpeed = 1.0;
  float flashSpeed = 40/gravSpeed;
  currentMino = 'o';
  nextMino = 'o';
  showingNextPiece = false;
  muted = true;
  arduboy.pollButtons();
  getNextMino(true);
  getNextMino(false);
  createPiece(currentMino);
  clearBoard();
    while(!arduboy.justPressed(B_BUTTON)) {
        arduboy.pollButtons();
        arduboy.clear();
        arduboy.drawRect(0,0,128,64,WHITE);
        drawChar_Rot90(80,12,'T',WHITE,BLACK,1);
        drawChar_Rot90(80,19,'E',WHITE,BLACK,1);
        drawChar_Rot90(80,26,'T',WHITE,BLACK,1);
        drawChar_Rot90(80,33,'R',WHITE,BLACK,1);
        drawChar_Rot90(80,40,'I',WHITE,BLACK,1);
        drawChar_Rot90(80,47,'S',WHITE,BLACK,1);
        arduboy.drawLine(72,12,72,51);
        drawChar_Rot90(24,16,'M',WHITE,BLACK,1);
        drawChar_Rot90(24,23,'U',WHITE,BLACK,1);
        drawChar_Rot90(24,30,'S',WHITE,BLACK,1);
        drawChar_Rot90(24,37,'I',WHITE,BLACK,1);
        drawChar_Rot90(24,44,'C',WHITE,BLACK,1);
        drawChar_Rot90(15,40,'O',WHITE,BLACK,1);
        drawChar_Rot90(15,47,'F',WHITE,BLACK,1);
        drawChar_Rot90(15,54,'F',WHITE,BLACK,1);
        drawChar_Rot90(15,10,'O',WHITE,BLACK,1);
        drawChar_Rot90(15,17,'N',WHITE,BLACK,1);
        if (arduboy.justPressed(DOWN_BUTTON)) {arduboy.audio.off();muted = true;};
        if (arduboy.justPressed(UP_BUTTON)) {arduboy.audio.on();muted = false;}
        if (!muted) arduboy.drawTriangle(10,5,12,8,14,5);
        if (muted) arduboy.drawTriangle(10,35,12,38,14,35);
        arduboy.display();
        arduboy.idle();
      }
      gameState = "playing";
      arduboy.pollButtons();
  }

//i will make the next piece seen when this function is called, not finished quite yet though
void showGUI() {
    arduboy.fillRect(98,32,30,30,BLACK);
    arduboy.drawRect(98,32,30,30,WHITE);
    switch (nextMino) {
      case 'o':
        arduboy.drawRect(98+10,32+10,5,5,WHITE);
        arduboy.drawRect(98+10,32+15,5,5,WHITE);
        arduboy.drawRect(98+15,32+10,5,5,WHITE);
        arduboy.drawRect(98+15,32+15,5,5,WHITE);
      break;
      case 'i':
        arduboy.drawRect(98+10,32+10,5,5,WHITE);
        arduboy.drawRect(98+10,32+15,5,5,WHITE);
        arduboy.drawRect(98+10,32+20,5,5,WHITE);
        arduboy.drawRect(98+10,32+25,5,5,WHITE);
      break;
      case 'l':
        arduboy.drawRect(98+10,32+10,5,5,WHITE);
        arduboy.drawRect(98+10,32+15,5,5,WHITE);
        arduboy.drawRect(98+10,32+20,5,5,WHITE);
        arduboy.drawRect(98+15,32+20,5,5,WHITE);
      break;
      case 'j':
        arduboy.drawRect(98+10,32+10,5,5,WHITE);
        arduboy.drawRect(98+10,32+15,5,5,WHITE);
        arduboy.drawRect(98+10,32+20,5,5,WHITE);
        arduboy.drawRect(98+15,32+10,5,5,WHITE);
      break;
      case 't':
        arduboy.drawRect(98+10,32+10,5,5,WHITE);
        arduboy.drawRect(98+10,32+15,5,5,WHITE);
        arduboy.drawRect(98+10,32+20,5,5,WHITE);
        arduboy.drawRect(98+15,32+15,5,5,WHITE);
      break;
      case 'z':
        arduboy.drawRect(98+10,32+20,5,5,WHITE);
        arduboy.drawRect(98+10,32+15,5,5,WHITE);
        arduboy.drawRect(98+15,32+15,5,5,WHITE);
        arduboy.drawRect(98+15,32+10,5,5,WHITE);
      break;
      case 's':
        arduboy.drawRect(98+10,32+10,5,5,WHITE);
        arduboy.drawRect(98+10,32+15,5,5,WHITE);
        arduboy.drawRect(98+15,32+15,5,5,WHITE);
        arduboy.drawRect(98+15,32+20,5,5,WHITE);
      break;
      };
      arduboy.fillRect(98,0,30,30,BLACK);
      arduboy.drawRect(98,0,30,30,WHITE);
      switch (bankMino) {
        case 'o':
        arduboy.drawRect(98+10,10,5,5,WHITE);
        arduboy.drawRect(98+10,15,5,5,WHITE);
        arduboy.drawRect(98+15,10,5,5,WHITE);
        arduboy.drawRect(98+15,15,5,5,WHITE);
      break;
      case 'i':
        arduboy.drawRect(98+10,10,5,5,WHITE);
        arduboy.drawRect(98+10,15,5,5,WHITE);
        arduboy.drawRect(98+10,20,5,5,WHITE);
        arduboy.drawRect(98+10,25,5,5,WHITE);
      break;
      case 'l':
        arduboy.drawRect(98+10,10,5,5,WHITE);
        arduboy.drawRect(98+10,15,5,5,WHITE);
        arduboy.drawRect(98+10,20,5,5,WHITE);
        arduboy.drawRect(98+15,20,5,5,WHITE);
      break;
      case 'j':
        arduboy.drawRect(98+10,10,5,5,WHITE);
        arduboy.drawRect(98+10,15,5,5,WHITE);
        arduboy.drawRect(98+10,20,5,5,WHITE);
        arduboy.drawRect(98+15,10,5,5,WHITE);
      break;
      case 't':
        arduboy.drawRect(98+10,10,5,5,WHITE);
        arduboy.drawRect(98+10,15,5,5,WHITE);
        arduboy.drawRect(98+10,20,5,5,WHITE);
        arduboy.drawRect(98+15,15,5,5,WHITE);
      break;
      case 'z':
        arduboy.drawRect(98+10,20,5,5,WHITE);
        arduboy.drawRect(98+10,15,5,5,WHITE);
        arduboy.drawRect(98+15,15,5,5,WHITE);
        arduboy.drawRect(98+15,10,5,5,WHITE);
      break;
      case 's':
        arduboy.drawRect(98+10,10,5,5,WHITE);
        arduboy.drawRect(98+10,15,5,5,WHITE);
        arduboy.drawRect(98+15,15,5,5,WHITE);
        arduboy.drawRect(98+15,20,5,5,WHITE);
      break;
      };
    arduboy.display();
  };

//pauses the game and displays the pause menu
void pauseMenu() {
  if (gameState == "playing") {
    gameState = "paused";
  arduboy.pollButtons();
    while(!arduboy.justPressed(B_BUTTON)) {
        arduboy.pollButtons();
        arduboy.fillRect(50,5,20,54,BLACK);
        arduboy.drawRect(50,5,20,54,WHITE);
        drawChar_Rot90(62,12,'P',WHITE,BLACK,1);
        drawChar_Rot90(62,19,'A',WHITE,BLACK,1);
        drawChar_Rot90(62,26,'U',WHITE,BLACK,1);
        drawChar_Rot90(62,33,'S',WHITE,BLACK,1);
        drawChar_Rot90(62,40,'E',WHITE,BLACK,1);
        drawChar_Rot90(62,47,'D',WHITE,BLACK,1);
        arduboy.display();
        arduboy.idle();
      }
      arduboy.pollButtons();
  };
  arduboy.pollButtons();
  gameState = "playing";
}

char scoreChar(uint8_t place) {
    switch (points[place]) {
      case 0:
        return '0';
      break;
      case 1:
        return '1';
      break;
      case 2:
        return '2';
      break;
      case 3:
        return '3';
      break;
      case 4:
        return '4';
      break;
      case 5:
        return '5';
      break;
      case 6:
        return '6';
      break;
      case 7:
        return '7';
      break;
      case 8:
        return '8';
      break;
      case 9:
        return '9';
      break;
     }
  }

//will show the losing screen
void loseMenu() {
  sound.noTone();
  gameState = "gameOver";
  arduboy.pollButtons();
    while(!arduboy.justPressed(B_BUTTON)) {
        arduboy.pollButtons();
        arduboy.clear();
        arduboy.drawRect(0,0,128,64,WHITE);
        drawChar_Rot90(120,8,'Y',WHITE,BLACK,1);
        drawChar_Rot90(120,14,'O',WHITE,BLACK,1);
        drawChar_Rot90(120,20,'U',WHITE,BLACK,1);
        drawChar_Rot90(120,28,'L',WHITE,BLACK,1);
        drawChar_Rot90(120,34,'O',WHITE,BLACK,1);
        drawChar_Rot90(120,40,'S',WHITE,BLACK,1);
        drawChar_Rot90(120,46,'E',WHITE,BLACK,1);
        drawChar_Rot90(120,52,'!',WHITE,BLACK,1);
        drawChar_Rot90(100,14,'S',WHITE,BLACK,1);
        drawChar_Rot90(100,21,'C',WHITE,BLACK,1);
        drawChar_Rot90(100,28,'O',WHITE,BLACK,1);
        drawChar_Rot90(100,35,'R',WHITE,BLACK,1);
        drawChar_Rot90(100,42,'E',WHITE,BLACK,1);
        drawChar_Rot90(100,47,':',WHITE,BLACK,1);
        drawChar_Rot90(90,14,scoreChar(2),WHITE,BLACK,1);
        drawChar_Rot90(90,21,scoreChar(1),WHITE,BLACK,1);
        drawChar_Rot90(90,28,scoreChar(0),WHITE,BLACK,1);
        drawChar_Rot90(90,35,'0',WHITE,BLACK,1);
        drawChar_Rot90(90,42,'0',WHITE,BLACK,1);
        arduboy.display();
        arduboy.idle();
      }
      if (score > 0)
      {
        enterHighScore(3);
      }
      displayHighScores(3);
      mainMenu();
  }

void drawChar_Rot90 (int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size) {
  boolean draw_background = bg != color;
  if ((x > WIDTH) ||              // Clip right   ;this line is modified
    (y > (HEIGHT + 5 * size)) ||  // Clip bottom  ;this line is modified
    ((x - 8 * size - 1) < 0) ||   // Clip left    ;this line is modified
    ((y + 5 * size - 1) < 0)      // Clip top     ;this line is modified
  )
  {
    return;
  }
  for (int8_t i=0; i<6; i++ )
  {
    uint8_t line;
    if (i == 5)
    {
      line = 0x0;
    }
    else
    {
      line = pgm_read_byte(font+(c*5)+i);
    }

    for (int8_t j = 0; j<8; j++)
    {
      uint8_t draw_color = (line & 0x1) ? color : bg;

      if (draw_color || draw_background) {
        for (uint8_t a = 0; a < size; a++ ) {
          for (uint8_t b = 0; b < size; b++ ) {
            arduboy.drawPixel( x - (j * size) - b, y + (i * size) + a, draw_color); //  this line is modified
          }
        }
      }
      line >>= 1;
    }
  }
};

boolean displayHighScores(byte file)
{
  byte y = 0;
  byte x = 108;
  byte hunds, tens, ones;
  // Each block of EEPROM has 10 high scores, and each high score entry
  // is 6 bytes long:  3 bytes for initials and 3 bytes for score.
  int address = file * 10 * 6 + EEPROM_STORAGE_SPACE_START;
  arduboy.clear();
  drawChar_Rot90(127,1,'H',WHITE,BLACK,1);
  drawChar_Rot90(127,7,'I',WHITE,BLACK,1);
  drawChar_Rot90(127,13,'G',WHITE,BLACK,1);
  drawChar_Rot90(127,19,'H',WHITE,BLACK,1);
  drawChar_Rot90(127,28,'S',WHITE,BLACK,1);
  drawChar_Rot90(127,34,'C',WHITE,BLACK,1);
  drawChar_Rot90(127,40,'O',WHITE,BLACK,1);
  drawChar_Rot90(127,46,'R',WHITE,BLACK,1);
  drawChar_Rot90(127,52,'E',WHITE,BLACK,1);
  drawChar_Rot90(127,58,'S',WHITE,BLACK,1);
  arduboy.display();

  for(int i = 0; i < 10; i++)
  {
    if (i<9) {drawChar_Rot90(x-(i*10),y,char(i+49),WHITE,BLACK,1);} else {drawChar_Rot90(x-(i*10),-1,'1',WHITE,BLACK,1);drawChar_Rot90(x-(i*10),4,'0',WHITE,BLACK,1);}
    arduboy.display();
    hunds = EEPROM.read(address + (6*i));
    tens = EEPROM.read(address + (6*i) + 1);
    ones = EEPROM.read(address + (6*i) + 2);

    initials[0] = (char)EEPROM.read(address + (6*i) + 3);
    initials[1] = (char)EEPROM.read(address + (6*i) + 4);
    initials[2] = (char)EEPROM.read(address + (6*i) + 5);

    if (ones > 0 || tens>0 || hunds>0)
    {
      drawChar_Rot90(x-(i*10),y+12,initials[0],WHITE,BLACK,1);
      drawChar_Rot90(x-(i*10),y+18,initials[1],WHITE,BLACK,1);
      drawChar_Rot90(x-(i*10),y+24,initials[2],WHITE,BLACK,1);
      arduboy.display();
      if (hunds < 1) {
      if (tens < 1)
      {
      drawChar_Rot90(x-(i*10),y+36,ones+48,WHITE,BLACK,1);
      drawChar_Rot90(x-(i*10),y+42,'0',WHITE,BLACK,1);
      drawChar_Rot90(x-(i*10),y+48,'0',WHITE,BLACK,1);
      arduboy.display();
      }
      if (tens >= 1)
      {
      drawChar_Rot90(x-(i*10),y+42,ones+48,WHITE,BLACK,1);
      drawChar_Rot90(x-(i*10),y+36,tens+48,WHITE,BLACK,1);
      drawChar_Rot90(x-(i*10),y+48,'0',WHITE,BLACK,1);
      drawChar_Rot90(x-(i*10),y+54,'0',WHITE,BLACK,1);
      arduboy.display();
      }
      } else {
      drawChar_Rot90(x-(i*10),y+42,ones+48,WHITE,BLACK,1);
      drawChar_Rot90(x-(i*10),y+36,tens+48,WHITE,BLACK,1);
      drawChar_Rot90(x-(i*10),y+30,hunds+48,WHITE,BLACK,1);
      drawChar_Rot90(x-(i*10),y+48,'0',WHITE,BLACK,1);
      drawChar_Rot90(x-(i*10),y+54,'0',WHITE,BLACK,1);
      }
    }
  }
  arduboy.display();
  arduboy.pollButtons();
    while(!arduboy.justPressed(B_BUTTON)) {
        arduboy.pollButtons();
        arduboy.idle();
    }
}

void enterInitials()
{
  byte index = 0;

  arduboy.clear();

  initials[0] = ' ';
  initials[1] = ' ';
  initials[2] = ' ';

  while (true)
  {
    arduboy.display();
    arduboy.clear();
    
    drawChar_Rot90(127,3,'H',WHITE,BLACK,1);
    drawChar_Rot90(127,9,'I',WHITE,BLACK,1);
    drawChar_Rot90(127,15,'G',WHITE,BLACK,1);
    drawChar_Rot90(127,21,'H',WHITE,BLACK,1);
    drawChar_Rot90(127,30,'S',WHITE,BLACK,1);
    drawChar_Rot90(127,36,'C',WHITE,BLACK,1);
    drawChar_Rot90(127,42,'O',WHITE,BLACK,1);
    drawChar_Rot90(127,48,'R',WHITE,BLACK,1);
    drawChar_Rot90(127,54,'E',WHITE,BLACK,1);
    drawChar_Rot90(115,21,points[2]+48,WHITE,BLACK,1);
    drawChar_Rot90(115,27,points[1]+48,WHITE,BLACK,1);
    drawChar_Rot90(115,33,points[0]+48,WHITE,BLACK,1);
    drawChar_Rot90(87,21,initials[0],WHITE,BLACK,1);
    drawChar_Rot90(87,28,initials[1],WHITE,BLACK,1);
    drawChar_Rot90(87,36,initials[2],WHITE,BLACK,1);
    arduboy.drawLine(80, 20+(index*7), 80, 27+(index*7), 1);
    delay(100);

    if (arduboy.pressed(UP_BUTTON))
    {
      if (index > 0)
      {
        index--;
      }
    }

    if (arduboy.pressed(DOWN_BUTTON))
    {
      if (index < 2)
      {
        index++;
      }
    }

    if (arduboy.pressed(RIGHT_BUTTON))
    {
      initials[index]++;
      // A-Z 0-9 :-? !-/ ' '
      if (initials[index] == '0')
      {
        initials[index] = ' ';
      }
      if (initials[index] == '!')
      {
        initials[index] = 'A';
      }
      if (initials[index] == '[')
      {
        initials[index] = '0';
      }
      if (initials[index] == '@')
      {
        initials[index] = '!';
      }
    }

    if (arduboy.pressed(LEFT_BUTTON))
    {
      initials[index]--;
      if (initials[index] == ' ') {
        initials[index] = '?';
      }
      if (initials[index] == '/') {
        initials[index] = 'Z';
      }
      if (initials[index] == 31) {
        initials[index] = '/';
      }
      if (initials[index] == '@') {
        initials[index] = ' ';
      }
    }

    if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON))
    {
      if (index < 2)
      {
        index++;
      } else {
        return;
      }
    }
  }

}

void enterHighScore(byte file)
{
  // Each block of EEPROM has 10 high scores, and each high score entry
  // is 6 bytes long:  3 bytes for initials and 3 bytes for score.
  int address = file * 10 * 6 + EEPROM_STORAGE_SPACE_START;
  byte hunds,tens,ones;
  byte tempHunds,tempTens,tempOnes;
  char tempInitials[3];
  unsigned int tmpScore = 0;

  // High score processing
  for(byte i = 0; i < 10; i++)
  {
    hunds = EEPROM.read(address + (6*i));
    tens = EEPROM.read(address + (6*i) + 1);
    ones = EEPROM.read(address + (6*i) + 2);
    if ((tens == 0xFF) && (ones == 0xFF) && (hunds == 0xFF))
    {
      // The values are uninitialized, so treat this entry
      // as a score of 0.
      tmpScore = 0;
    } else
    {
      tmpScore = ones+(tens*10)+(hunds*100);
    }
    if (points[0]+(points[1]*10)+(points[2]*100) > tmpScore)
    {
      enterInitials();
      hunds = points[2];
      tens = points[1];
      ones = points[0];
      for(byte j = i; j < 10; j++)
      {
        tempHunds = (char)EEPROM.read(address + (6*j));
        tempTens = (char)EEPROM.read(address + (6*j) + 1);
        tempOnes = (char)EEPROM.read(address + (6*j) + 2);
        tempInitials[0] = (char)EEPROM.read(address + (6*j) + 3);
        tempInitials[1] = (char)EEPROM.read(address + (6*j) + 4);
        tempInitials[2] = (char)EEPROM.read(address + (6*j) + 5);
        // write score and initials to current slot
        EEPROM.update(address + (6*j), hunds);
        EEPROM.update(address + (6*j) + 1, tens);
        EEPROM.update(address + (6*j) + 2, ones);
        EEPROM.update(address + (6*j) + 3, initials[0]);
        EEPROM.update(address + (6*j) + 4, initials[1]);
        EEPROM.update(address + (6*j) + 5, initials[2]);

        // tmpScore and tmpInitials now hold what we want to
        //write in the next slot.
        tens = tempTens;
        ones = tempOnes;
        hunds = tempHunds;
        initials[0] = tempInitials[0];
        initials[1] = tempInitials[1];
        initials[2] = tempInitials[2];
      }
      ones=0;
      tens=0;
      hunds=0;
      return;
    }
  }
}



void setup() {
  arduboy.begin();
  arduboy.clear();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed();
  arduboy.audio.off();
  sound.volumeMode(VOLUME_ALWAYS_NORMAL);
  mainMenu();
  playMusic();
};

void loop() {
  if (!sound.playing()&&!muted) {songNum++;playMusic();};
  if (!arduboy.nextFrame()) {
    return;
  };
  checkKeys();
  arduboy.clear();
  drawBoard();
  if (arduboy.everyXFrames(round(48/gravSpeed))) updatePiece('d');
  testRows();
}
