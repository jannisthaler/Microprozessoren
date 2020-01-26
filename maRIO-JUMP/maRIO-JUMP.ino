#include "LiquidCrystal.h"
 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 
class object
{
 
public:
  int obj_x, obj_y;
 
  object()
  {
    byte block[8] = {
      0b11111,
      0b11111,
      0b11111,
      0b11111,
      0b11111,
      0b11111,
      0b11111,
      0b11111
    };
 
    lcd.createChar(0, block);
  }
  ~object() {};
 
 
 
  void lose_y()
  {
    obj_y = random(0, 2);
  }
 
  void move()
  {
    obj_x--;
  }
 
  bool colision(int x, int y)
  {
    if (obj_x == x && obj_y == y)
      return true;
    else
      return false;
  }
 
  void draw()
  {
    lcd.setCursor(obj_x, obj_y);
    lcd.print((char)0);
  }
 
};
class player
{
public:
 
  int ply_x, ply_y, score, speed;
  bool pause;
 
  player()
  {
    ply_x = 0;
    ply_y = 0;
    score = 0;
 
    pause = false;
 
    byte outffit[8] = {
      0b01110,
      0b01110,
      0b00100,
      0b01110,
      0b10101,
      0b00100,
      0b01010,
      0b10001
    };
 
    lcd.createChar(1, outffit);
  };
  ~player() {};
 
  void time()
  {
    switch (score)
    {
    case 0:
      speed = 400;
      break;
    case 10:
      speed = 300;
      break;
    case 25:
      speed = 200;
      break;
    case 50:
      speed = 100;
      break;
    default:
      speed = speed;
      break;
    }
  }
 
 
  int keybords()
  {
    enum keys
    {
      NONE,
      RIGHT,
      UP,
      DOWN,
      LEFT,
      SELECT
    };
 
    int key_adc = 0, key_id = 0;
    key_adc = analogRead(A0);
 
    if (key_adc > 750) key_id = keys::NONE;
    if (key_adc < 750) key_id = keys::SELECT;
    if (key_adc < 500) key_id = keys::LEFT;
    if (key_adc < 350) key_id = keys::DOWN;
    if (key_adc < 150) key_id = keys::UP;
    if (key_adc < 50) key_id = keys::RIGHT;
 
    switch (key_id)
    {
    case 2:
    {
      ply_y = 0;
      break;
    }
    case 3:
    {
      ply_y = 1;
      break;
    }
    }
 
  }
 
 
  void draw()
  {
    lcd.setCursor(ply_x, ply_y);
    lcd.print((char)1);
  }
 
};
 
player gamer;
const int enemy = 6;
object tab[enemy];
 
bool game = true;
int start = millis();
int end = millis();
 
bool pauza(int time)
{
  if (end - start > time)
  {
    start = millis();
    return true;
  }
  else
  {
    end = millis();
    return false;
  }
}
 
#pragma endregion
 
void setup()
{
  lcd.begin(2, 16);
 
  for (int i = 0; i < enemy; i++)
  {
    tab[i].lose_y();
    tab[i].obj_x = 16 + (i * 3);
  }
}
 
void loop()
{
  gamer.keybords();
  gamer.time();
 
  if (game == true && gamer.pause == false)
  {
    lcd.clear();
    gamer.draw();
 
    if (pauza(gamer.speed) == true)
    {
      tab[0].move();
      tab[1].move();
      tab[2].move();
      tab[3].move();
      tab[4].move();
      tab[5].move();
    }
    for (int i = 0; i < enemy; i++)
    {
      if (tab[i].colision(gamer.ply_x, gamer.ply_y) == true)
      {
        game = false;
      }
      if (tab[i].obj_x < 0)
      {
        tab[i].lose_y();
        tab[i].obj_x = 17;
        gamer.score = gamer.score + 1;
      }
      if (tab[i].obj_x < 16)
      {
        tab[i].draw();
      }
    }
  }
 
  if (game == false)
  {
    lcd.clear();
    lcd.print("End    Speed:" + (String)gamer.speed);
    lcd.setCursor(0, 1);
    lcd.print("Game   Score:" + (String)gamer.score);
  }
 
 
  delay(200);
}
