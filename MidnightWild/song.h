#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM
Song music[] = {                // total song in bytes = 108
  //                            // setup bytes 21
  0x08,                         // Number of tracks
  0x00, 0,                      // Address of track 0
  0x04, 0,                      // Address of track 1
  0x0A, 0,                      // Address of track 2
  0x0F, 0,                      // Address of track 3
  0x16, 0,                      // Address of track 4
  0x2B, 0,                      // Address of track 5
  0x39, 0,                      // Address of track 6
  0x43, 0,                      // Address of track 7
  
  0x02,                         // Channel 0 entry track (PULSE)
  0x01,                         // Channel 1 entry track (SQUARE)
  0x00,                         // Channel 2 entry track (TRIANGLE)
  0x03,                         // Channel 3 entry track (NOISE)

  //"Track 0"                   // ticks = 64 / bytes = 4
  0x40, 0,                      // FX: SET VOLUME: volume = 0
  0x9F + 64,                    // DELAY: 64 ticks
  0x9F,                         // FX: STOP CURRENT CHANNEL

  //"Track 1"                   // ticks = 2048 / bytes = 6
  0x9D, 50,                     // SET song tempo: value = 50
  0xFD, 3, 4,                   // REPEAT: count = 3 + 1 / track = 4   (4 * 512 ticks)
  0x9F,                         // FX: STOP CURRENT CHANNEL

  //"Track 2"                   // ticks = 2048 / bytes = 5
  0xFD, 31, 6,                  // REPEAT: count = 31 + 1 / track = 5  (32 * 64 ticks)
  0x00,                         // NOTE OFF
  0x9F,                         // FX: STOP CURRENT CHANNEL

  //"Track 3"                   // ticks = 2048 / bytes = 7
  0xFD, 7,  0,                  // REPEAT: count = 7 + 1 / track = 8   (8 *64 ticks)
  0xFD, 23, 7,                  // REPEAT: count = 23 + 1 / track = 8  (24 *64 ticks)
  0x9F,                         // FX: STOP CURRENT CHANNEL

  //"Track 4"                   // ticks = 512 / bytes = 21
  0xFD, 1, 5,                   // REPEAT: count = 1 + 1 / track = 5   (2 * 64 ticks)
  0x4B, 3,                      // FX: ADD TRANSPOSITION: notes = 3
  0xFD, 1, 5,                   // REPEAT: count = 1 + 1 / track = 5   (2 * 64 ticks)
  0x4B, -1,                     // FX: ADD TRANSPOSITION: notes = 3
  0xFD, 1, 5,                   // REPEAT: count = 1 + 1 / track = 5   (2 * 64 ticks)
  0x4B, 3,                      // FX: ADD TRANSPOSITION: notes = 3
  0xFD, 1, 5,                   // REPEAT: count = 1 + 1 / track = 5   (2 * 64 ticks)
  0x4B, -5,                     // FX: ADD TRANSPOSITION: notes = 3
  0xFE,                         // RETURN

  //"Track 5"                   // ticks = 64 / bytes = 14
  0x00 + 49,                    // NOTE ON: note = 49
  0x40, 63,                     // FX: SET VOLUME: volume = 63
  0x41, -16,                    // FX: VOLUME SLIDE ON: steps = -8
  0x9F + 16,                    // DELAY: 16 ticks
  0x40, 16,                     // FX: SET VOLUME: volume = 16
  0x41, -4,                     // FX: VOLUME SLIDE ON: steps = -4
  0x9F + 4,                     // DELAY: 4 ticks
  0x43,                         // FX: VOLUME SLIDE OFF
  0x9F + 44,                    // DELAY: 44 ticks
  0xFE,                         // RETURN

  //"track 6"                   // ticks = 64 / bytes = 10
  0x00 + 13,                    // NOTE ON: note = 23
  0x40, 32,                     // FX: SET VOLUME: volume = 32
  0x4E, 1, 0x00 + 0x00 + 30,    // SET TREMOLO OR VIBRATO: depth = 16 / retrig = OFF / TorV = TREMOLO / rate = 3
  0x9F + 62,                    // DELAY: 62 ticks
  0x4F,                         // TREMOLO OR VIBRATO OFF
  0x9F + 2,                     // DELAY: 1 ticks
  0xFE,                         // RETURN

  //"track 7"                   // ticks = 64 / bytes = 20
  0x40, 32,                     // FX: SET VOLUME: volume = 32
  0x9F + 1,                     // DELAY: ticks = 1
  0x40,  0,                     // FX: SET VOLUME: volume = 0
  0x9F + 15,                    // DELAY: ticks = 15

  0x40, 32,                     // FX: SET VOLUME: volume = 32
  0x9F + 1,                     // DELAY: ticks = 1
  0x40,  0,                     // FX: SET VOLUME: volume = 0
  0x9F + 15,                    // DELAY: ticks = 15

  0x40, 32,                     // FX: SET VOLUME: volume = 32
  0x41, -2,                     // FX: VOLUME SLIDE ON: steps = -2
  0x9F + 16,                    // DELAY: ticks = 16
  0x43,                         // FX: VOLUME SLIDE OFF
  0x9F + 16,                    // DELAY: ticks = 16
  0xFE,                         // RETURN
};

Song fightmusic[] = {
0x0a,			// Number of tracks
0x00, 0x00,		// Address of track 0
0x10, 0x00,		// Address of track 1
0x18, 0x00,		// Address of track 2
0x1e, 0x00,		// Address of track 3
0x2b, 0x00,		// Address of track 4
0x32, 0x00,		// Address of track 5
0x39, 0x00,		// Address of track 6
0x43, 0x00,		// Address of track 7
0x62, 0x00,		// Address of track 8
0x6f, 0x00,		// Address of track 9
0x00,			// Channel 0 entry track
0x01,			// Channel 1 entry track
0x02,			// Channel 2 entry track
0x03,			// Channel 3 entry track
//"Track channel 0"
0x40, 12,		// FX: SET VOLUME: volume = 12
0x41, 4,		// FX: SLIDE VOLUME ON: 4
0x42, 2, 0,		// FX: SLIDE VOLUME ADVANCED: 2 0
0x9D, 24,		// FX: SET TEMPO: tempo = 24
0xFD, 1, 7,		// REPEAT: count = 1 + 1 / track = 7
0xFD, 1, 9,		// REPEAT: count = 1 + 1 / track = 9
0x9F,			// FX: STOP CURRENT CHANNEL
//"Track channel 1"
0x40, 3,		// FX: SET VOLUME: volume = 3
0xFD, 1, 9,		// REPEAT: count = 1 + 1 / track = 9
0xFC, 7,		// GOTO track 7
0x9F,			// FX: STOP CURRENT CHANNEL
//"Track channel 2"
0x40, 1,		// FX: SET VOLUME: volume = 1
0x45, 2, 1,		// FX: SLIDE FREQUENCY ADVANCED: 2 1
0x9F,			// FX: STOP CURRENT CHANNEL
//"Track channel 3"
0x40, 12,		// FX: SET VOLUME: volume = 12
0x41, 3,		// FX: SLIDE VOLUME ON: 3
0x47, 0x12, 0x00 + 0x00 + 3,		// FX: SET ARPEGGIO: 0 0 1 2 3
0x52, 3,		// FX: SET GLISSANDO: 3
0xFD, 1, 8,		// REPEAT: count = 1 + 1 / track = 8
0x9F,			// FX: STOP CURRENT CHANNEL
//"Track snare"
0x40, 32,		// FX: SET VOLUME: volume = 32
0x41, -16,		// FX: VOLUME SLIDE ON: steps = -16
0x9F + 2,		// DELAY: ticks = 2
0x43,			// FX: VOLUME SLIDE OFF
0xFE,			// RETURN
//"Track crash"
0x40, 32,		// FX: SET VOLUME: volume = 32
0x41, -2,		// FX: VOLUME SLIDE ON: steps = -2
0x9F + 16,		// DELAY: ticks = 16
0x43,			// FX: VOLUME SLIDE OFF
0xFE,			// RETURN
//"Track shake"
0x49, 4 + 0,		// FX: RETRIG NOISE: point = 1 (*4) / speed = 0 (fastest)
0x40, 32,		// FX: SET VOLUME: volume = 32
0x41, -8,		// FX: VOLUME SLIDE ON: steps = -8
0x9F + 4,		// DELAY: ticks = 4
0x4A,			// FX: RETRIG: off
0x43,			// FX: VOLUME SLIDE OFF
0xFE,			// RETURN
//"Track Track 1"
0x00 + 9,		// NOTE ON: note = 9
0x9F + 1,		// DELAY: ticks = 1
0x00 + 6,		// NOTE ON: note = 6
0x9F + 2,		// DELAY: ticks = 2
0x00 + 34,		// NOTE ON: note = 34
0x9F + 1,		// DELAY: ticks = 1
0x00 + 33,		// NOTE ON: note = 33
0x9F + 1,		// DELAY: ticks = 1
0x00 + 22,		// NOTE ON: note = 22
0x9F + 1,		// DELAY: ticks = 1
0x00 + 21,		// NOTE ON: note = 21
0x9F + 1,		// DELAY: ticks = 1
0x00 + 32,		// NOTE ON: note = 32
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 62,		// NOTE ON: note = 62
0x9F + 1,		// DELAY: ticks = 1
0x00 + 11,		// NOTE ON: note = 11
0x9F + 1,		// DELAY: ticks = 1
0x00 + 53,		// NOTE ON: note = 53
0x9F + 1,		// DELAY: ticks = 1
0x00 + 43,		// NOTE ON: note = 43
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 30,		// NOTE ON: note = 30
0x9F + 1,		// DELAY: ticks = 1
0x00 + 45,		// NOTE ON: note = 45
0x9F + 1,		// DELAY: ticks = 1
0xFE,			// RETURN
//"Track Track 2"
0xFC, 4,		// GOTO track 4
0x9F + 1,		// DELAY: ticks = 1
0xFC, 4,		// GOTO track 4
0x9F + 2,		// DELAY: ticks = 2
0xFC, 5,		// GOTO track 5
0x9F + 2,		// DELAY: ticks = 2
0xFC, 6,		// GOTO track 6
0x9F + 3,		// DELAY: ticks = 3
0xFE,			// RETURN
//"Track Track 3"
0x00 + 15,		// NOTE ON: note = 15
0x9F + 1,		// DELAY: ticks = 1
0x00 + 9,		// NOTE ON: note = 9
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 6,		// NOTE ON: note = 6
0x9F + 1,		// DELAY: ticks = 1
0x00 + 9,		// NOTE ON: note = 9
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 10,		// NOTE ON: note = 10
0x9F + 2,		// DELAY: ticks = 2
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 28,		// NOTE ON: note = 28
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 32,		// NOTE ON: note = 32
0x9F + 1,		// DELAY: ticks = 1
0x00 + 12,		// NOTE ON: note = 12
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 1,		// DELAY: ticks = 1
0x00 + 27,		// NOTE ON: note = 27
0x9F + 1,		// DELAY: ticks = 1
0x00 + 30,		// NOTE ON: note = 30
0x9F + 1,		// DELAY: ticks = 1
0xFE,			// RETURN

};

#endif