#include <SPI.h>
#include <EEPROM.h>

#include <Arduboy.h>
#include "SquarioGame.h"
#include "DefinesImagesAndSounds.h"

Arduboy display;
SquarioGame Game( &display );

char text[16];
unsigned long currTime;
unsigned long prevTime = 0;
bool SoundOn = true;

void setup() {
  display.begin();
}
uint8_t RandomSeedSeed = 1;
void TitleScreen() {
  Game.ActivateButtonCD( );
  while (true) {
    if ( Game.ButtonOffCD() ) {
      byte input = display.buttonsState();
      if ( input & A_BUTTON || input & B_BUTTON ) break;
      if ( ( input & LEFT_BUTTON ) || ( input & RIGHT_BUTTON ) ) SoundOn = !SoundOn;
      if ( input & UP_BUTTON ) {
        if ( RandomSeedSeed == 255 ) RandomSeedSeed = 1;
        else RandomSeedSeed++;
      }
      if ( input & DOWN_BUTTON ) { 
        if ( RandomSeedSeed == 1 ) RandomSeedSeed = 255;
        else RandomSeedSeed--;
      }
      if ( input ) Game.ActivateButtonCD( );
    }
    display.clear();
    display.drawBitmap(14,0,TitleSquarioText,96,32,1);
    display.drawBitmap(66,29,TitleSquarioGuys,53,32,1);
    display.setCursor(0,48); display.print(F("Seed:  ")); display.print(RandomSeedSeed);
    display.setCursor(0,56); display.print(F("Sound: "));
    if ( SoundOn ) display.print(F("On"));
    else display.print(F("Off"));
    display.display();
  }
  randomSeed( RandomSeedSeed );
  for ( uint8_t a = 0; a < GameSeeds; a++ ) Game.Seeds[a] = random( 255 );
}

uint8_t SoundCounter = 0;
const byte *SFX = 0;
const unsigned int *SFXNoteSet;
int SFX_Counter = -1;
unsigned long duration = 0, lastNote = 0;
void SoundEngine() {
  if ( !SoundOn ) return;
  if ( !display.tunes.playing() ) {
    if      ( SoundCounter < 8 ) display.tunes.playScore(Verse);
    else if ( SoundCounter < 12 ) display.tunes.playScore(PreChorus);
    else if ( SoundCounter < 16 ) display.tunes.playScore(Chorus);
    SoundCounter++;
    if ( SoundCounter == 16 ) SoundCounter = 0;
  }
  if (Game.SFX) {
    SFXNoteSet = SFXFrequencies;
    SFX = Game.SFX;
    Game.SFX = NULL;
    SFX_Counter = 2;
    lastNote = 0;
  }
  if (SFX) {
    if ( currTime < lastNote + duration ) return;
    byte Packet = pgm_read_byte(SFX + SFX_Counter++);
    if (Packet == 0xFF) {
      SFX = NULL;
      SFX_Counter = -1;
      return;
    }
    byte note = (Packet >> 4) & 0x0F;
    byte dMultiplier = Packet & 0x0F;
    duration = pgm_read_byte(SFX) + (pgm_read_byte(SFX+1) * dMultiplier);
    unsigned long freq = ( pgm_read_word( SFXNoteSet + note - 1 ) ) / 2;
    display.tunes.tone( freq, duration );
    lastNote = currTime;
  }
}

void loop() {
  SoundCounter = 0;
  TitleScreen();
  Game.NewGame( );
  while ( Game.Event ) { // 0 is Off
    currTime = millis();
    SoundEngine();
    if ( currTime > prevTime + FPSDELAY ) {
      prevTime = currTime;
      display.clear();
      byte input = display.buttonsState();
      if ( input & B_BUTTON )       { Game.ButtonPress( ButtonJump ); }   else { Game.ButtonRelease( ButtonJump ); }
      if ( input & A_BUTTON )       { Game.ButtonPress( ButtonRun ); }    else { Game.ButtonRelease( ButtonRun ); }
      if ( input & UP_BUTTON )      { Game.ButtonPress( ButtonUp ); }     else { Game.ButtonRelease( ButtonUp ); }
      if ( input & DOWN_BUTTON )    { Game.ButtonPress( ButtonDown ); }   else { Game.ButtonRelease( ButtonDown ); }
      if ( input & LEFT_BUTTON )    { Game.ButtonPress( ButtonLeft ); }   else { Game.ButtonRelease( ButtonLeft ); }
      if ( input & RIGHT_BUTTON )   { Game.ButtonPress( ButtonRight ); }  else { Game.ButtonRelease( ButtonRight ); }
      Game.Cycle();
      if ( Game.Event ) Game.Draw();
      display.setCursor(0,0);
      //display.print(F("Score: "));
      //display.print(Game.Score);
      display.display();
    }
  }
  display.tunes.stopScore();
  if (Game.Score) enterHighScore(1);
  displayHighScores(1);
}

//Function by nootropic design to add high scores
void enterHighScore(byte file) {
  // Each block of EEPROM has 10 high scores, and each high score entry
  // is 5 bytes long:  3 bytes for initials and two bytes for score.
  int address = file * 10 * 5;
  byte hi, lo;
  char initials[3];
  char tmpInitials[3];
  unsigned int tmpScore = 0;

  // High score processing
  for (byte i = 0; i < 10; i++) {
    hi = EEPROM.read(address + (5*i));
    lo = EEPROM.read(address + (5*i) + 1);
    if ((hi == 0xFF) && (lo == 0xFF))
    {
      // The values are uninitialized, so treat this entry
      // as a score of 0.
      tmpScore = 0;
    } else
    {
      tmpScore = (hi << 8) | lo;
    }
    if (Game.Score > tmpScore) {
      char index = 0;
      initials[0] = ' ';
      initials[1] = ' ';
      initials[2] = ' ';
      Game.ActivateButtonCD();
      while (true) {  //    enterInitials();
        display.clear();
        display.setCursor(16,0); display.print(F("HIGH SCORE"));
        sprintf(text, "%u", Game.Score);
        display.setCursor(88, 0); display.print(text);
        display.setCursor(56, 20); display.print(initials[0]);
        display.setCursor(64, 20); display.print(initials[1]);
        display.setCursor(72, 20); display.print(initials[2]);
        for (byte i = 0; i < 3; i++) display.drawLine(56 + (i*8), 27, 56 + (i*8) + 6, 27, 1);
        display.drawLine(56, 28, 88, 28, 0);
        display.drawLine(56 + (index*8), 28, 56 + (index*8) + 6, 28, 1);
        display.display(); 
        if ( Game.ButtonOffCD() ) {
          byte input = display.buttonsState();
          if ( input & LEFT_BUTTON || input & B_BUTTON ) {
            index--;
            if (index < 0) index = 0;
            else if ( SoundOn ) display.tunes.tone(1046, 250);
          }
          if ( input & RIGHT_BUTTON ) {
            index++;
            if (index > 2) index = 2;
            else if ( SoundOn ) display.tunes.tone(1046, 250);
          }
          if ( input & DOWN_BUTTON ) {
            initials[index]++;
            if ( SoundOn ) display.tunes.tone(523, 250);
            // A-Z 0-9 :-? !-/ ' '
            if (initials[index] == '0') initials[index] = ' ';
            if (initials[index] == '!') initials[index] = 'A';
            if (initials[index] == '[') initials[index] = '0';
            if (initials[index] == '@') initials[index] = '!';
          }
          if ( input & UP_BUTTON ) {
            initials[index]--;
            if ( SoundOn ) display.tunes.tone(523, 250);
            if (initials[index] == ' ') initials[index] = '?';
            if (initials[index] == '/') initials[index] = 'Z';
            if (initials[index] == 31) initials[index] = '/';
            if (initials[index] == '@') initials[index] = ' ';
          }
          if ( input & A_BUTTON ) {
            if (index < 2) {
              index++;
              if ( SoundOn ) display.tunes.tone(1046, 250);
            } else {
              if ( SoundOn ) display.tunes.tone(1046, 250);
              break;
            }
          }
          if ( input ) Game.ActivateButtonCD();
        }
      }
      
      for(byte j=i;j<10;j++) {
        hi = EEPROM.read(address + (5*j));
        lo = EEPROM.read(address + (5*j) + 1);
        if ((hi == 0xFF) && (lo == 0xFF)) tmpScore = 0;
        else tmpScore = (hi << 8) | lo;
        tmpInitials[0] = (char)EEPROM.read(address + (5*j) + 2);
        tmpInitials[1] = (char)EEPROM.read(address + (5*j) + 3);
        tmpInitials[2] = (char)EEPROM.read(address + (5*j) + 4);

        // write score and initials to current slot
        EEPROM.write(address + (5*j), ((Game.Score >> 8) & 0xFF));
        EEPROM.write(address + (5*j) + 1, (Game.Score & 0xFF));
        EEPROM.write(address + (5*j) + 2, initials[0]);
        EEPROM.write(address + (5*j) + 3, initials[1]);
        EEPROM.write(address + (5*j) + 4, initials[2]);

        // tmpScore and tmpInitials now hold what we want to
        //write in the next slot.
        Game.Score = tmpScore;
        initials[0] = tmpInitials[0];
        initials[1] = tmpInitials[1];
        initials[2] = tmpInitials[2];
      }
      return;
    }
  }
}
void displayHighScores(byte file) {
  unsigned int Score;
  byte y = 10;
  byte x = 24;
  // Each block of EEPROM has 10 high scores, and each high score entry
  // is 5 bytes long:  3 bytes for initials and two bytes for score.
  int address = file*10*5;
  byte hi, lo;
  display.clear();
  display.setCursor(32, 0);
  display.print(F("HIGH SCORES"));
  for(int i = 0; i < 10; i++) {
    sprintf(text, "%2d", i+1);
    display.setCursor(x,y+(i*8));
    display.print(text);
    hi = EEPROM.read(address + (5*i));
    lo = EEPROM.read(address + (5*i) + 1);
    if ((hi == 0xFF) && (lo == 0xFF)) Score = 0;
    else Score = (hi << 8) | lo;
    if (Score > 0) {
      sprintf( text, "%c%c%c %u", 
        (char)EEPROM.read(address + (5*i) + 2),
        (char)EEPROM.read(address + (5*i) + 3),
        (char)EEPROM.read(address + (5*i) + 4),
        Score );
      display.setCursor(x + 24, y + (i*8));
      display.print(text);
    }
  }
  display.display();
  Game.ActivateButtonCD();
  while ( true ) {
    if ( display.buttonsState() && Game.ButtonOffCD() ) return;
  }
}
