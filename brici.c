#define BW 16
#define BH 9
#define BAT_WIDTH 10
uint16_t ball_x;
uint16_t ball_y;
uint16_t bat_x;
uint16_t bat_y;
typedef struct  {
	uint16_t colour;
	uint16_t x;
	uint16_t y;	
	uint16_t visible;
} block_t;

#define BLOCKCOUNT 24
block_t Blocks[BLOCKCOUNT] = {
{ YELLBLK, 0, 18, BW, BH }, { YELLBLK, 16, 18, BW, BH }, { YELLBLK, 32, 18, BW, BH }, { YELLBLK, 48, 18, BW, BH }, { YELLBLK, 64, 18, BW, BH }, { YELLBLK, 80, 18, BW, BH }, { YELLBLK, 96, 18, BW, BH }, { YELLBLK, 112, 18, BW, BH }, 
{ GRENBLK, 0, 27, BW, BH }, { GRENBLK, 16, 27, BW, BH }, { GRENBLK, 32, 27, BW, BH }, { GRENBLK, 48, 27, BW, BH }, { GRENBLK, 64, 27, BW, BH }, { GRENBLK, 80, 27, BW, BH }, { GRENBLK, 96, 27, BW, BH }, { GRENBLK, 112, 27, BW, BH },
{ BLUEBLK, 0, 36, BW, BH }, { BLUEBLK, 16, 36, BW, BH }, { BLUEBLK, 32, 36, BW, BH }, { BLUEBLK, 48, 36, BW, BH }, { BLUEBLK, 64, 36, BW, BH}, { BLUEBLK, 80, 36, BW, BH }, { BLUEBLK, 96, 36, BW, BH }, { BLUEBLK, 112, 36, BW, BH }, 
};
  
void hideBlock(uint32_t index)
{
	fillRectangle(Blocks[index].x,Blocks[index].y,BW,BH,0);
	Blocks[index].visible = 0;
}
void showBlock(uint32_t index)
{	
	fillRectangle(Blocks[index].x,Blocks[index].y,BW,BH,Blocks[index].colour);
	Blocks[index].visible = 0;
}
void hideBall(void)
{
	fillRectangle(ball_x,ball_y,2,2,0);
}
void showBall(void)
{
	fillRectangle(ball_x,ball_y,2,2,RGBToWord(255,255,0));
}
void moveBall(uint32_t newX, uint32_t newY)
{
	hideBall();
	ball_x = newX;
	ball_y = newY;
	showBall();
}

void hideBat(void)
{
	fillRectangle(bat_x,bat_y,10,3,0);
}
void showBat(void)
{
	fillRectangle(bat_x,bat_y,10,3,RGBToWord(127,127,255));
}
void moveBat(uint32_t newX, uint32_t newY)
{
	hideBat();
	bat_x = newX;
	bat_y = newY;
	showBat();
}

void playBrici()
{
#define MAX_BRICI_LEVELS 4
  int Level = MAX_BRICI_LEVELS;
  int LevelComplete = 0;
  int16_t BallCount = 5;
  uint16_t Index;
  int16_t BallXVelocity = 1;
  int16_t BallYVelocity = 1;
  // Blank the screen to start with
  
  fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  bat_x = 20;
  bat_y = SCREEN_HEIGHT - 20;
  ball_x = 40;
  ball_y = 70;  	
    
  printText("Brici", 5, 10, 40, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
  delay(500);
  while (Level > 0)
  {
    /*Ball.move(random(10, SCREEN_WIDTH-10), SCREEN_HEIGHT/2);
    if (random(2) > 0)
      BallXVelocity = 1;
    else
      BallXVelocity = -1;
    LevelComplete = 0;
    BallYVelocity = -1;*/
    fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_WIDTH, 0);
    for (Index=0;Index<BLOCKCOUNT;Index++)
	{
	  showBlock(Index);
	}
	showBall();
	showBat();
    printText("Level", 5, 5, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
    printNumber(MAX_BRICI_LEVELS - Level + 1, 60, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
    while (!LevelComplete)
    {
      if (RightPressed())
      {
        // Move right
        if (bat_x < (SCREEN_WIDTH - BAT_WIDTH))
        {
          moveBat(bat_x + 2, bat_y); // Move the bat faster than the ball
        }
      }

      if (LeftPressed())
      {
        // Move left
        if (bat_x > 0)
        {
          moveBat(bat_x - 2, bat_y); // Move the bat faster than the ball
        }
      }
      if ((ball_y == bat_y) && (ball_x >= bat_x) && (ball_x <= bat_x + BAT_WIDTH))
      {
        BallYVelocity = -BallYVelocity;
      }
      showBat(); // redraw bat as it might have lost a pixel due to collisions

	  moveBall(ball_x+BallXVelocity,ball_y+BallYVelocity);
      
      if (ball_x == 2)
        BallXVelocity = -BallXVelocity;
      if (ball_x() == SCREEN_WIDTH - 2)
        BallXVelocity = -BallXVelocity;
      if (ball_y() == 2)
        BallYVelocity = -BallYVelocity;

      if (ball_y >= bat_y+3) 
      {
        BallCount--;
        if (BallCount == 0)
        {
          fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
          printText("GAME OVER", 9, 40, 100, RGBToWord(0xff, 0xff, 0xff), 0);
          printText("Fire to restart", 15, 8, 120, RGBToWord(0xff, 0xff, 0), RGBToWord(0, 0, 0));
          while (!FirePressed());
          return;
        }
        /*if (random(100) & BIT0)
          BallXVelocity = 1;
        else
          BallXVelocity = -1;*/

        BallYVelocity = -1;
        //Ball.move(random(10, SCREEN_WIDTH - 10), random(90, 120));
		Ball.move(20,80);
        fillRectangle(SCREEN_WIDTH-5*15, SCREEN_HEIGHT-10, 120, 10, 0);
        for (Index = BallCount; Index > 0; Index--)
          fillRectangle(SCREEN_WIDTH - Index * 15, SCREEN_HEIGHT-10, 10, 10, RGBToWord(0xff, 0xf, 0xf));
      }
      LevelComplete = 1;
      for (Index = 0; Index < BLOCKCOUNT; Index++)
      {
        if (Blocks[Index].visible) // any blocks left?
          LevelComplete = 0;
        int touch = blockTouching(Index,ball_x,ball_y)
        if (touch)
        {
          hideBlock(Index)
          if ( (touch == 1) || (touch == 3) )
            BallYVelocity = -BallYVelocity;
          if ( (touch == 2) || (touch == 4) )
            BallXVelocity = -BallXVelocity;
        }
      }
      // No Blocks left, Move to next level.
      if ((LevelComplete == 1) && (Level > 0))
      {
        Level--;
        printText("Level", 5, 5, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
        printNumber(MAX_BRICI_LEVELS - Level + 1, 60, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));

      }
      delay(10); // Slow the game to human speed
    }
  }
  fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_WIDTH, RGBToWord(0, 0, 0xff));
  printText("VICTORY!", 8, 40, 100, RGBToWord(0xff, 0xff, 0), RGBToWord(0, 0, 0xff));
  printText("Fire to restart", 15, 8, 120, RGBToWord(0xff, 0xff, 0), RGBToWord(0, 0, 0xff));
  while (!FirePressed());
  return;
}
