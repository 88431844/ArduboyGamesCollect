#ifndef SQUARIO_h
#define SQUARIO_h
#include <Arduboy.h>
#include "DefinesImagesAndSounds.h"

enum SquarioButtons {
  ButtonLeft = 0,
  ButtonUp,
  ButtonRight,
  ButtonDown,
  ButtonJump,
  ButtonRun,
  NumButtons
};
enum Directions {
  Up,
  Down,
  Left,
  Right
};
enum ObjectTypes {
  STSquario = 0, // Sprites
  STBigSquario,
  STTriangleo,
  STStarmano,
  STSmileo,
  STBolt,
  
  STMushroom,

  STQBlock,  // Interactive Tiles
  STBQBlock,
  STMushBlock,
  STBricks,
  STPipeCapLeft,
  STPipeCapRight,
  STTopPipeCapLeft,
  STTopPipeCapRight,
  STPipeLeft,
  STPipeRight,
  
  NumObjectTypes
};
enum EventTypes {
  ETOff = 0,
  ETPlaying,
  ETDeath,
  ETPipeDrop,
  ETPipeRise,
  ETNum
};

class Room;
class Map;
class Sprite;
class AISprite;
class SquarioGame;

class Sprite {
  public:
    void LoadSprite( const unsigned char * DataPointer, int tX, int tY );
    void ClearSprite( );
    bool IsIn ( int tX, int tY );
    bool IsInTopHalf ( int tX, int tY );
    virtual bool GetPixelAbsolute( int tX, int tY );
    byte Collide( int tX, int tY );
    bool CollisionCheckX( byte direction );
    bool CollisionCheckY( byte direction );
    void HeadCollision( );
    bool Falling( );
    void Gravity( );
    void Move( );
    bool Jump( );
    void Duck( );
    void Draw( );
    
    uint8_t Width( );
    uint8_t Height( );
    uint8_t Masks( );
    uint8_t MaxFrame( );
    uint8_t CyclesPerFrame( );
    byte Flags( );
    const unsigned char * FramePointer( );
    const unsigned char * MaskPointer( );
    int RightX( );
    int BottomY( );

    SquarioGame         * Game;
    const unsigned char * SpriteData;
    int                   x, y, vx, vy;
    int                   currentFrame;
    bool                  Mirrored;

};
class AISprite : public Sprite {
  public:
    void Activate( const unsigned char * DataPointer, int tX, int tY );
    void Deactivate( );
    void Think( );
    void Seek( );
    void DetectJump( );
    void DetectWall( );
    void DetectGap( );
    
    uint8_t Speed( );
    byte Intelligence( );
    bool                  Active;
    byte                  Facing;
};
class InteractiveObject {
  public:
    int x, y;
    byte type;
};
class Room {
  public:
    void ClearRoom( );
    void SetTile( int x, int y );
    bool ReadTile( int x, int y );
    byte         data[ RoomBytes ];
};
class Map {
  public:
    void NewMap( );
    void LoadMap( );
    void GenerateRoom( int RoomNum );
    void AddObject( byte type, int x, int y );
    void HandleObject ( int x, int y );
    void RemoveObject( int x, int y );
    byte CheckObject( int x, int y );
    bool CheckTile( int x, int y );
    void AddPipe( int x, int y );
    void AddTopPipe( int x, int y );

    int MinXPixel();
    int MaxXPixel();
    int MaxYPixel();
    int MinXTile();
    int MaxXTile();
    int MaxYTile();
    
    SquarioGame         * Game;
    Room                  rooms[MapRooms];
    InteractiveObject     objects[MapObjects];
    int                   ObjectIndex;
    int                   RandomChance;
    int                   FirstRoom, LastRoom, MapHeight, LastLoadLocation, SpawnBarrier;
};
class SquarioGame {
  public:
    SquarioGame( Arduboy * display );
    void NewGame( );
    void StartLevel( );
    int  SpawnY( );
    void Cycle( );
    bool TestCollision( Sprite * TestSprite1, AISprite * TestSprite2 );
    bool TestRoughCollision( Sprite * TestSprite1, AISprite * TestSprite2 );
    bool TestPixelCollision( Sprite * TestSprite1, AISprite * TestSprite2 );
    void Draw( );
    void Die( );
    void DiePrint( uint8_t y, unsigned int i );
    void DrawMap( );
    void DrawPlayer( );
    void DrawMobs( );
    void AddMob( const unsigned char * DataPointer, int x, int y );
    void AdjustCamera( );
    void ProcessButtons( );
    void ButtonPress( uint8_t pButton );
    void ButtonRelease( uint8_t pButton );
    bool ButtonState[ NumButtons ];
    void ActivateButtonCD( );
    bool ButtonOffCD( );
    int getTextOffset( int s );
    
    Arduboy             * Display;
    Sprite                Player;
    AISprite              Mobs[ SpriteCap ];
    Map                   Level;

    int                   Health;
    unsigned int          Score;
    unsigned int          DistancePoints;
    int                   Coins, Lives, MapNumber;
    byte                  Inventory;
    int                   CameraX, CameraY;
    byte                  Event;
    int                   EventCounter;
    const byte *          SFX;
    byte                  Seeds[ GameSeeds ];
    unsigned long         lastPress;
};


#endif
