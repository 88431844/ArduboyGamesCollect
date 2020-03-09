// hiragana [Game Jam 4]
// taniguchi kotaro
// https://youtu.be/yd6Pf6uxu2E

#include <Arduboy2.h>

Arduboy2 display;

void setup() {
  display.begin();
  display.clear();
}

PROGMEM const unsigned char menu_cursol [] = {
  0b00000011,
  0b00000111,
  0b00000110,
};

struct zahyo {
  char x;
  char y;
};

struct zahyo kana_a[] = {{50, 18}, {77, 14}, {0, 0}, {63, 7}, {59, 35}, {59, 48}, {62, 55}, {0, 0}, {74, 25}, {61, 48}, {50, 56}, {45, 50}, {45, 44}, {56, 33}, {69, 29}, {79, 30}, {88, 38}, {88, 47}, {82, 55}, {71, 59}, {0, 0}, {0, 0}};
struct zahyo kana_i[] = {{44, 15}, {52, 51}, {56, 40}, {0, 0}, {80, 19}, {90, 43}, {0, 0}, {0, 0}};
struct zahyo kana_u[] = {{57, 4}, {69, 8}, {0, 0}, {53, 25}, {70, 19}, {77, 28}, {74, 44}, {62, 58}, {0, 0}, {0, 0}};
struct zahyo kana_e[] = {{56, 3}, {70, 6}, {0, 0}, {52, 26}, {74, 19}, {45, 55}, {64, 39}, {68, 56}, {86, 56}, {0, 0}, {0, 0}};
struct zahyo kana_o[] = {{46, 20}, {67, 14}, {0, 0}, {59, 7}, {57, 57}, {42, 46}, {71, 30}, {84, 33}, {91, 43}, {79, 55}, {0, 0}, {78, 9}, {87, 17}, {0, 0}, {0, 0}};
struct zahyo kana_ka[] = {{47, 22}, {68, 18}, {72, 25}, {62, 54}, {56, 50}, {0, 0}, {62, 7}, {42, 52}, {0, 0}, {81, 16}, {91, 32}, {0, 0}, {0, 0}};
struct zahyo kana_ki[] = {{50, 16}, {74, 9}, {0, 0}, {54, 29}, {81, 22}, {0, 0}, {57, 4}, {81, 40}, {67, 38}, {0, 0}, {50, 49}, {60, 56}, {77, 55}, {0, 0}, {0, 0}};
struct zahyo kana_ku[] = {{70, 6}, {54, 31}, {71, 58}, {0, 0}, {0, 0}};
struct zahyo kana_ke[] = {{47, 9}, {45, 49}, {51, 36}, {0, 0}, {67, 22}, {88, 18}, {0, 0}, {79, 5}, {79, 43}, {70, 59}, {0, 0}, {0, 0}};
struct zahyo kana_ko[] = {{53, 14}, {77, 12}, {65, 20}, {0, 0}, {49, 44}, {58, 52}, {82, 51}, {0, 0}, {0, 0}};
struct zahyo kana_sa[] = {{49, 22}, {80, 12}, {0, 0}, {56, 4}, {81, 39}, {71, 35}, {0, 0}, {52, 49}, {59, 55}, {78, 55}, {0, 0}, {0, 0}};
struct zahyo kana_si[] = {{56, 8}, 53, 46, 61, 56, 71, 56, 87, 50, {0, 0}, {0, 0}};
struct zahyo kana_su[] = {{41, 21}, 91, 15, 0, 0, 69, 5, 69, 40, 64, 44, 59, 39, 60, 33, 65, 30, 71, 37, 70, 47, 60, 61, {0, 0}, {0, 0}};
struct zahyo kana_se[] = {{41, 30}, 92, 23, 0, 0, 78, 7, 75, 42, 69, 39, 0, 0, 54, 13, 55, 46, 61, 52, 83, 52, {0, 0}, {0, 0}};
struct zahyo kana_so[] = {{56, 10}, 77, 7, 44, 33, 88, 26, 67, 34, 60, 42, 60, 49, 66, 55, 81, 57, {0, 0}, {0, 0}};
struct zahyo kana_ta[] = {{45, 19}, {71, 14}, {0, 0}, {60, 6}, {43, 55}, {0, 0}, {66, 31}, {88, 29}, {0, 0}, {67, 53}, {89, 55}, {0, 0}, {0, 0}};
struct zahyo kana_ti[] = {{46, 18}, {73, 12}, {0, 0}, {60, 5}, {51, 41}, {71, 31}, {80, 33}, {85, 37}, {85, 47}, {66, 58}, {0, 0}, {0, 0}};
struct zahyo kana_tu[] = {{40, 26}, {74, 17}, {84, 20}, {90, 28}, {88, 39}, {60, 53}, {0, 0}, {0, 0}};
struct zahyo kana_te[] = {{44, 14}, {88, 6}, {65, 19}, {59, 32}, {60, 42}, {73, 54}, {82, 55}, {0, 0}, {0, 0}};
struct zahyo kana_to[] = {{54, 7}, {59, 30}, {0, 0}, {82, 13}, {50, 41}, {50, 49}, {58, 54}, {82, 53}, {0, 0}, {0, 0}};
struct zahyo kana_na[] = {{45, 15}, 67, 10, 0, 0, 58, 4, 44, 39, 0, 0, 79, 11, 88, 19, 0, 0, 74, 27, 74, 50, 67, 56, 60, 55, 56, 50, 65, 45, 86, 53, {0, 0}, {0, 0}};
struct zahyo kana_ni[] = {{46, 11}, 40, 39, 43, 53, 48, 39, 0, 0, 66, 16, 86, 15, 75, 20, 0, 0, 64, 41, 70, 49, 89, 50, {0, 0}, {0, 0}};
struct zahyo kana_nu[] = {{47, 15}, 59, 50, 0, 0, 69, 8, 49, 52, 41, 41, 44, 32, 67, 19, 80, 19, 90, 29, 90, 41, 77, 54, 70, 53, 67, 48, 76, 44, 92, 53, {0, 0}, {0, 0}};
struct zahyo kana_ne[] = {{52, 6}, 50, 54, 0, 0, 41, 21, 56, 17, 39, 43, 76, 14, 84, 15, 89, 25, 88, 42, 80, 52, 71, 51, 69, 47, 76, 42, 92, 50, {0, 0}, {0, 0}};
struct zahyo kana_no[] = {{66, 15}, 51, 50, 40, 34, 42, 27, 61, 12, 77, 12, 90, 24, 90, 40, 75, 54, {0, 0}, {0, 0}};
struct zahyo kana_ha[] = {{0, 0}, {0, 0}};
struct zahyo kana_hi[] = {{0, 0}, {0, 0}};
struct zahyo kana_hu[] = {{0, 0}, {0, 0}};
struct zahyo kana_he[] = {{0, 0}, {0, 0}};
struct zahyo kana_ho[] = {{0, 0}, {0, 0}};
struct zahyo kana_ma[] = {{0, 0}, {0, 0}};
struct zahyo kana_mi[] = {{0, 0}, {0, 0}};
struct zahyo kana_mu[] = {{0, 0}, {0, 0}};
struct zahyo kana_me[] = {{0, 0}, {0, 0}};
struct zahyo kana_mo[] = {{0, 0}, {0, 0}};
struct zahyo kana_ya[] = {{0, 0}, {0, 0}};
struct zahyo kana_yu[] = {{0, 0}, {0, 0}};
struct zahyo kana_yo[] = {{0, 0}, {0, 0}};
struct zahyo kana_ra[] = {{0, 0}, {0, 0}};
struct zahyo kana_ri[] = {{0, 0}, {0, 0}};
struct zahyo kana_ru[] = {{0, 0}, {0, 0}};
struct zahyo kana_re[] = {{0, 0}, {0, 0}};
struct zahyo kana_ro[] = {{0, 0}, {0, 0}};
struct zahyo kana_wa[] = {{0, 0}, {0, 0}};
struct zahyo kana_wo[] = {{0, 0}, {0, 0}};
struct zahyo kana_n[] = {{0, 0}, {0, 0}};

struct zahyo* hiragana[51] = {
  kana_a,
  kana_i,
  kana_u,
  kana_e,
  kana_o,
  kana_ka,
  kana_ki,
  kana_ku,
  kana_ke,
  kana_ko,
  kana_sa,
  kana_si,
  kana_su,
  kana_se,
  kana_so,
  kana_ta,
  kana_ti,
  kana_tu,
  kana_te,
  kana_to,
  kana_na,
  kana_ni,
  kana_nu,
  kana_ne,
  kana_no,
  kana_ha,
  kana_hi,
  kana_hu,
  kana_he,
  kana_ho,
  kana_ma,
  kana_mi,
  kana_mu,
  kana_me,
  kana_mo,
  kana_ya,
  NULL,
  kana_yu,
  NULL,
  kana_yo,
  kana_ra,
  kana_ri,
  kana_ru,
  kana_re,
  kana_ro,
  kana_wa,
  NULL,
  NULL,
  NULL,
  kana_wo,
  kana_n,
};
const char* moji[51] = {//ヘボン式ローマ字
  "A",
  "I",
  "U",
  "E",
  "O",
  "KA",
  "KI",
  "KU",
  "KE",
  "KO",
  "SA",
  "SHI",
  "SU",
  "SE",
  "SO",
  "TA",
  "CHI",
  "TSU",
  "TE",
  "TO",
  "NA",
  "NI",
  "NU",
  "NE",
  "NO",
  "HA",
  "HI",
  "FU",
  "HE",
  "HO",
  "MA",
  "MI",
  "MU",
  "ME",
  "MO",
  "YA",
  NULL,
  "YU",
  NULL,
  "YO",
  "RA",
  "RI",
  "RU",
  "RE",
  "RO",
  "WA",
  NULL,
  NULL,
  NULL,
  "O",
  "N",
};

char cur_kana = 0;
char cur_cnt = 0;
struct zahyo cur = {0};

unsigned long currTime;
unsigned long prevTime = 0;
#define FPSDELAY      300/30

bool now_push = false;

void loop() {
  while (true) {
    currTime = millis();
    if ( currTime > prevTime + FPSDELAY ) {
      prevTime = currTime;
      byte input = display.buttonsState();
      if (input & (UP_BUTTON | DOWN_BUTTON | RIGHT_BUTTON | LEFT_BUTTON)) {
        if (!now_push) {
          if (input & DOWN_BUTTON) {
            if (
#if 1
              cur_kana < 24
#else
              cur_kana < 50
#endif
            ) {
              cur_kana++;
#if 0
              switch (cur_kana) {
                case 36: cur_kana = 37; break;
                case 38: cur_kana = 39; break;
                case 46: cur_kana = 49; break;
                case 47: cur_kana = 49; break;
                case 48: cur_kana = 49; break;
              }
#endif
            } else {
              cur_kana = 0;
            }
          }
          if (input & UP_BUTTON) {
            if (cur_kana > 0) {
              cur_kana--;
#if 0
              switch (cur_kana) {
                case 36: cur_kana = 35; break;
                case 38: cur_kana = 37; break;
                case 46: cur_kana = 45; break;
                case 47: cur_kana = 45; break;
                case 48: cur_kana = 45; break;
              }
#endif
            } else {
#if 1
              cur_kana = 24;
#else
              cur_kana = 50;
#endif
            }
          }
          if (input & RIGHT_BUTTON) {
            if (cur_kana >= 5) {
              cur_kana -= 5;
            }
            else {
              cur_kana += 20;
            }
#if 0
            switch (cur_kana) {
              case 0: cur_kana = 50; break;
              case 1: cur_kana = 41; break;
              case 2: cur_kana = 42; break;
              case 3: cur_kana = 43; break;
              case 4: cur_kana = 49; break;
              case 36: cur_kana = 31; break;
              case 38: cur_kana = 33; break;
            }
#endif
          }
          if (input & LEFT_BUTTON) {
#if 1
            if (cur_kana < 20) {
              cur_kana += 5;
            } else {
              cur_kana -= 20;
            }
#else
            if (cur_kana < 40) {
              cur_kana += 5;
            }
            switch (cur_kana) {
              case 50: cur_kana = 0; break;
              case 45: cur_kana = 50; break;
              case 40: cur_kana = 45; break;
              case 41: cur_kana =  1; break;
              case 42: cur_kana =  2; break;
              case 43: cur_kana =  3; break;
              case 44: cur_kana = 49; break;
              case 49: cur_kana =  4; break;
              case 36: cur_kana = 41; break;
              case 38: cur_kana = 43; break;
            }
#endif
          }
          cur_cnt = 0;
          now_push = true;
        }
      } else {
        now_push = false;
      }

      if (hiragana[cur_kana] == NULL) {
        //文字なし
      }
      else if (hiragana[cur_kana][cur_cnt].x == 0 && hiragana[cur_kana][cur_cnt].y == 0) { //終了
        //終了
      }
      else if (cur_cnt == 0) { //最初の画スタート
        display.clear();
        display.print(moji[cur_kana]);
        cur.x = hiragana[cur_kana][0].x;
        cur.y = hiragana[cur_kana][0].y;
        cur_cnt++;
        display.drawBitmap(cur.x, cur.y, menu_cursol, 3, 3, 1);
        display.display();
      }
      else if (cur.x == hiragana[cur_kana][cur_cnt].x && cur.y == hiragana[cur_kana][cur_cnt].y) { //　線引き終了
        cur_cnt++;
        if (hiragana[cur_kana][cur_cnt].x == 0 && hiragana[cur_kana][cur_cnt].y == 0) { //次の画スタート
          cur_cnt++;
          if (hiragana[cur_kana][cur_cnt].x != 0 || hiragana[cur_kana][cur_cnt].y != 0) { //次の画スタート
            cur.x = hiragana[cur_kana][cur_cnt].x;
            cur.y = hiragana[cur_kana][cur_cnt].y;
            cur_cnt++;
            display.drawBitmap(cur.x, cur.y, menu_cursol, 3, 3, 1);
            display.display();
          }
        }
      }
      else {  //線を引く処理
        if (abs(hiragana[cur_kana][cur_cnt - 1].x - hiragana[cur_kana][cur_cnt].x) > abs(hiragana[cur_kana][cur_cnt - 1].y - hiragana[cur_kana][cur_cnt].y)) {
          if (cur.x < hiragana[cur_kana][cur_cnt].x)       cur.x++;
          else if (cur.x > hiragana[cur_kana][cur_cnt].x)  cur.x--;
          if (cur.y != hiragana[cur_kana][cur_cnt].y) {
            cur.y = hiragana[cur_kana][cur_cnt - 1].y + (hiragana[cur_kana][cur_cnt].y - hiragana[cur_kana][cur_cnt - 1].y) * abs(cur.x - hiragana[cur_kana][cur_cnt - 1].x) / abs(hiragana[cur_kana][cur_cnt].x - hiragana[cur_kana][cur_cnt - 1].x);
          }
        } else {
          if (cur.y < hiragana[cur_kana][cur_cnt].y)       cur.y++;
          else if (cur.y > hiragana[cur_kana][cur_cnt].y)  cur.y--;
          if (cur.x != hiragana[cur_kana][cur_cnt].x) {
            cur.x = hiragana[cur_kana][cur_cnt - 1].x + (hiragana[cur_kana][cur_cnt].x - hiragana[cur_kana][cur_cnt - 1].x) * abs(cur.y - hiragana[cur_kana][cur_cnt - 1].y) / abs(hiragana[cur_kana][cur_cnt].y - hiragana[cur_kana][cur_cnt - 1].y);
          }
        }
        display.drawBitmap(cur.x, cur.y, menu_cursol, 3, 3, 1);
        display.display();
      }
    }
  }
}
