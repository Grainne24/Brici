#include <stm32f031x6.h>
#include "display.h"
#include "musical_notes.h"

void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
volatile uint32_t milliseconds;
void initTimer(void);
void playNote(uint32_t Freq,uint32_t duration); // initialising function for playing a sound (when a wall is hit etc.)

#define BW 16
#define BH 9
#define BAT_WIDTH 10
#define MAX_BRICI_LEVELS 4

uint32_t ball_x;
uint32_t ball_y;
uint32_t bat_x;
uint32_t bat_y;

typedef struct  {
      uint16_t colour;
      uint16_t x;
      uint16_t y;
      uint16_t visible;
} block_t;

#define YELLBLK 0x1f0e
#define GREENBLK 0x001f
#define BLUEBLK 0x00f8
#define LIGHTBLUEBLK 0x004f // add a new block 
#define BLOCKCOUNT 36
block_t Blocks[BLOCKCOUNT] = {
{ YELLBLK, 0, 18, 0 }, { YELLBLK, 16, 18, 0 }, { YELLBLK, 32, 18, 0 }, { YELLBLK, 48, 18, 0 }, { YELLBLK, 64, 18, 0 }, { YELLBLK, 80, 18, 0}, { YELLBLK, 96, 18, 0 }, { YELLBLK, 112, 18, 0 },
{ GREENBLK, 0, 27, 0 }, { GREENBLK, 16, 27, 0 }, { GREENBLK, 32, 27, 0 }, { GREENBLK, 48, 27, 0 }, { GREENBLK, 64, 27, 0 }, { GREENBLK, 80, 27, 0 }, { GREENBLK, 96, 27, 0 }, { GREENBLK, 112, 27, 0 },
{ BLUEBLK, 0, 36, 0 }, { BLUEBLK, 16, 36, 0 }, { BLUEBLK, 32, 36, 0 }, { BLUEBLK, 48, 36, 0 }, { BLUEBLK, 64, 36, 0}, { BLUEBLK, 80, 36, 0 }, { BLUEBLK, 96, 36, 0 }, { BLUEBLK, 112, 36, 0 },
{ LIGHTBLUEBLK, 0, 45, 0 }, { LIGHTBLUEBLK, 16, 45, 0 }, { LIGHTBLUEBLK, 32, 45, 0 }, { LIGHTBLUEBLK, 48, 45, 0 }, { LIGHTBLUEBLK, 64, 45, 0}, { LIGHTBLUEBLK, 80, 45, 0 }, { LIGHTBLUEBLK, 96, 45, 0 }, { LIGHTBLUEBLK, 112, 45, 0 },
};
 
void hideBlock(uint32_t index);
void showBlock(uint32_t index);
void hideBall(void);
void showBall(void);
void moveBall(uint32_t newX, uint32_t newY);
void hideBat(void);
void showBat(void);
void moveBat(uint32_t newX, uint32_t newY);
int blockTouching(int Index,uint16_t ball_x,uint16_t ball_y);
int UpPressed(void);
int DownPressed(void);
int LeftPressed(void);
int RightPressed(void);
void randomize(void);
uint32_t random(uint32_t lower, uint32_t upper);
void playBrici(void);
void SerialBegin(void);
void eputchar(char c);
void eputs(char *String);
int keyPressed (void);
void redOn(void);
void redOff(void);
void greenOn(void);
void greenOff(void);
void yellowOn(void);
void yellowOff(void);
uint32_t myTune[]={523, 523, 587, 392, 392, 440}; //plays the intro tune 
uint32_t myTime[]={100, 100, 200, 100, 100, 200}; //time for the intro tune
uint32_t myTune2[]={1175, 1175, 1175, 1568, 1175, 1568}; //plays next level tune
uint32_t myTime2[]={150, 100, 100, 200, 100, 200}; //time note is played for next level tune
uint32_t myTune3[]={262, 247, 233}; //plays tune when a ball is lost
uint32_t myTime3[]={100, 100, 100}; //time note is played for when a ball is lost
void playTune(uint32_t tune[],uint32_t time[],uint32_t count); //universal function which allows myTune and myTime functions to work
int main()
{
      initClock();
      RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
      initSysTick();
      initTimer();
      SerialBegin();
      display_begin();
      //invertDisplay();// Uncomment this line if you have a display with a blue PCB
      pinMode(GPIOB,0,1);
      pinMode(GPIOB,4,0);
      pinMode(GPIOB,5,0);
      pinMode(GPIOA,8,0);
      pinMode(GPIOA,1,1);	//red - PA1
			pinMode(GPIOA,0,1); //yellow - PA0
			pinMode(GPIOB,3,1); //green - PB3
      enablePullUp(GPIOB,0);
      enablePullUp(GPIOB,4);
      enablePullUp(GPIOB,5);
      enablePullUp(GPIOA,11);
      enablePullUp(GPIOA,8);
      playTune(myTune,myTime,3); //plays intro tune
      while(1)
      {
				playBrici();
      }
}

void playBrici()
{
	eputs("      _             _    _          _      _ \t\r\n"); //displays this before game has started on puTTY
	eputs("     | |           | |  | |        (_)    (_)\t\r\n");
	eputs("  ___| |_ __ _ _ __| |_ | |__  _ __ _  ___ _ \t\r\n");
	eputs(" / __| __/ _` | '__| __|| '_\\| '__| |/ __| |\t\r\n");
	eputs(" \\|_\\ || (_| | |  | | | |_) | |  | | (__| |\t\r\n");
	eputs(" |___/\\__\\__,_|_| |_| |_.__/|_|  |_|\\|_|_|\t\r\n");
	
	eputs("                                (O)\t\r\n");
	eputs("                             __--|--__\t\r\n");
	eputs("                     .------~---------~-----.\t\r\n");
	eputs("                     | .------------------. |\t\r\n");
	eputs("                     | |                  | |\t\r\n");
	eputs("                     | |   .'''.  .'''.   | |\t\r\n");
	eputs("                     | |   :    ''    :   | |\t\r\n");
	eputs("                     | |   :          :   | |\t\r\n");
	eputs("                     | |    '.      .'    | |\t\r\n");
	eputs("                     | |      '.  .'      | |\t\r\n");
	eputs(".------------.       | |        ''        | |  .------------.\t\r\n");
	eputs("| O          |       | `------------------' |  |            |\t\r\n");
	eputs("| O   .-----.|       `.____________________.'  |.-----.     |\t\r\n");
	eputs("| O .'      ||         `-------.  .-------'    ||      `.   |\t\r\n");
	eputs("|o*.'       ||   .--.     ____.'  `.____       ||       `.  |\t\r\n");
	eputs("|.-'        || .-~--~-----~--------------~----. ||        `-.|\t\r\n");
	eputs("||          || |PLAY .---------.|.--------.|()| ||          ||\t\r\n");
	eputs("||          || |     `---------'|`-o-=----'|  | ||          ||\t\r\n");
	eputs("|`-._   PLAY|| |-*-*------------| *--  (==)|  | ||PLAY   _.-'|\t\r\n");
	eputs("|    ~-.____|| |  PLAY BRICI  |          |  | ||____.-~    |\t\r\n");
	eputs("`------------' `------------------------------' `------------'\t\r\n");

 	eputs("  _____ _   _  _____ _______ _____  _    _  _____ _______ _____ ____  _   _  ______\t\r\n");
	eputs(" |_   _|\\ | |/ ____|__   __|  __\\ | | | |/ ____|__   __|_   _/ __\\| \\|  |/ ____|\t\r\n");
	eputs("   | | | \\| | (___    | |  | |__) || | | | |       | |    | || |  | |  \\| | (__\\t\r\n");  
	eputs("   | | | . ` |\\___\\  | |  |  _  / | | | | |       | |    | || |  | | . `  |\\__ \\t\r\n"); 
	eputs("  _| |_| |\\  |____)|  | |  | | \\\\| |_| | |____   | |   _| || |__| | |\\  |____) |\t\r\n");
	eputs(" |_____|_|\\_|_____/   |_|  |_| \\_\\____/ \\_____| |_|  |_____\\___/|_| \\_|_____/\t\r\n");
	
 	eputs("    __&__       ____________________________\t\r\n");
 	eputs("   /    \\     | Press the Up button to     |\t\r\n");
 	eputs("  |       |    | play restart when you lose |\t\r\n");
 	eputs("  |  (o)(o)    |                            |\t\r\n");
 	eputs("  C   .---_)   | Press the left button to   |\t\r\n");
 	eputs("   | |.___|    | play brici                 |\t\r\n");
 	eputs("   | \\__/    <                             |\t\r\n");
 	eputs("   /____\\     | HAVE FUN!!!!!              |\t\r\n");
 	eputs("  /_____/\\    |____________________________|\t\r\n");
 	eputs(" /        \\                                 \t\r\n");
	


  int Level = MAX_BRICI_LEVELS;
  int LevelComplete = 0;
  unsigned int BallCount = 5;
  unsigned int Index;
  int Paused = 1;
  int32_t BallXVelocity = 1;
  int32_t BallYVelocity = 1;
  // Blank the screen to start with
  fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  bat_x = 20;
  bat_y = SCREEN_HEIGHT - 20;
   
  printText("Welcome to Brici", 10, 40, RGBToWord(0x004f, 0x004f, 0x004f), RGBToWord(0, 0, 0)); //change color of the words
      printText("Press Left", 10, 50, RGBToWord(0x004f, 0x004f, 0x004f), RGBToWord(0, 0, 0)); //change color of the words
      while(!LeftPressed());
      randomize();  
      ball_x = random(0,SCREEN_WIDTH);
  ball_y = random(50,(bat_y - 10));      
      // draw the red squares indicating the number of remaining lives.
                        for (Index = BallCount; Index > 0; Index--)
            fillRectangle(SCREEN_WIDTH - Index * 8, SCREEN_HEIGHT-10, 7, 7, RGBToWord(0xff, 0xf, 0xf));
  while (Level > 0)
  {
    moveBall(random(10, SCREEN_WIDTH-10), SCREEN_HEIGHT/2);
    if (random(0,2) > 0) // "flip a coin" to choose ball x velocity
      BallXVelocity = 1;
    else
      BallXVelocity = -1;
    LevelComplete = 0;
    BallYVelocity = -1;  // initial ball motion is up the screen
    fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_WIDTH, 0); // clear the screen
            // draw the blocks.
    for (Index=0;Index<BLOCKCOUNT;Index++)
            {
                  showBlock(Index);
            }
            showBall();
            showBat();
    printText("Level", 5, SCREEN_HEIGHT-10, RGBToWord(0x1f0e, 0x1f0e, 0x1f0e), RGBToWord(0, 0, 0));
    printNumber(MAX_BRICI_LEVELS - Level + 1, 45, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
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
      if (DownPressed ())   //on down pressed pause the game
      {
         if (Paused == 0) // if not paused
         {
            Paused = 1; // then pause 
         }
         else if (Paused == 1)  // if already paused
         {
             Paused = 0; // unpause the game
         }

          while (DownPressed());  //checking for input (if the button is pressed)
       }
                       
                       
        if (Paused == 0)  // check to see if button is pressed a second time (then will continue)
        {
            continue; //continue game
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
				greenOn(); //green LED turns on
				delay(100);
				greenOff(); //green LED turns off
        BallYVelocity = -BallYVelocity;
      }
      showBat(); // redraw bat as it might have lost a pixel due to collisions

                  moveBall(ball_x+BallXVelocity,ball_y+BallYVelocity);
     
      if (ball_x == 2)
                  {
        BallXVelocity = -BallXVelocity;
                        playNote(750, 50); //plays sound for frequency, duration
                        eputs("You hit the right wall! Wrong way buddy ;)\r\n"); //
                  }
      if (ball_x == SCREEN_WIDTH - 2)
                  {
        BallXVelocity = -BallXVelocity;
                        playNote(750, 50); //plays sound for frequency, duration
                        eputs("You hit the left wall! Wrong way buddy ;)\r\n"); //
                  }
      if (ball_y == 2)
                  {
        BallYVelocity = -BallYVelocity;
                        playNote(750, 50); //plays sound for frequency, duration
                        eputs("You hit the top wall! Wrong way buddy ;)\r\n"); // 
                  }
                       

      if (ball_y >= bat_y+3)  // has the ball pass the bat vertically?
      {
        BallCount--;
				redOn(); //Turns on red LED 
				delay(100); //Wait 100ms
				redOff(); //Turns off red LED
        playTune(myTune3,myTime3,3); //plays sound (notes, duration, number of times repeated)
        if (BallCount == 0)
        {
          fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
          printText("GAME OVER", 40, 100, RGBToWord(0x1f0e, 0x1f0e, 0x1f0e), 0);
          printText("Up to restart", 18, 120, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
					
					eputs("   ________    _____      _____  ___________ ____________   _________________________   \t\r\n"); //
					eputs("  /  _____/   /  _ \\    /    \\\\_   _____/\\_____ \\  \\ /   /\\_  ____/ \\______  \\ \t\r\n");
					eputs(" /  \\  ___  /  /\\ \\  / \\ / \\ |    __)_   /   |  \\   Y   /  |    __)_  |       _/  \t\r\n");
					eputs("\\   \\\\ \\/    |   \\/    Y   \\|       \\ /    |   \\     /   |       \\ |    |   \\ \t\r\n");
					eputs(" \\______  /\\___|__ /\\____|__  /_______  /\\_______ /\\___/   /_______  / |____|_  /  \t\r\n");
					eputs("        \\/        \\/        \\/       \\/         \\/                \\/        \\/   \t\r\n");
					
					
					eputs("                                       _         _                \t\r\n");
					eputs("                           __   ___.--'_`.     .'_`--.___   __    \t\r\n");
					eputs("                          ( _`.'. -   'o` )   ( 'o`   - .`.'_ )   \t\r\n");
					eputs("                          \\.'_'      _.-'     `-._      `_`./_   \t\r\n");
					eputs("                         (\\`. )   //\\`         '/\\    ( .'/ )  \t\r\n");
					eputs("                           \\_`-'`---'\\__,       ,__//`---'`-'_/ \t\r\n");
					eputs("                            \\`       `-\\         /-'        '/  \t\r\n");
					eputs("                              `                               '   \t\r\n");
					
					redOn(); //turns on red LED - show the game is over 
					
					while(!UpPressed());
					redOff(); //turns off red LED 
          return;
        }
        if (random(0,100) & 1)
          BallXVelocity = 1;
        else
          BallXVelocity = -1;

        BallYVelocity = -1;
        moveBall(random(10, SCREEN_WIDTH - 10), random(90, 120));
        fillRectangle(SCREEN_WIDTH-5*15, SCREEN_HEIGHT-10, 120, 10, 0);
        for (Index = BallCount; Index > 0; Index--)
        fillRectangle(SCREEN_WIDTH - Index * 8, SCREEN_HEIGHT-10, 7, 7, RGBToWord(0xff, 0xf, 0xf));
      }
			// check for ball hitting blocks and if this level is done.
      LevelComplete = 1;
      for (Index = 0; Index < BLOCKCOUNT; Index++)
      {
        int touch = blockTouching(Index,ball_x,ball_y);
        if (touch)
        {
					yellowOn(); //turns on yellow LED - when it hits a block
          hideBlock(Index);
          if ( (touch == 1) || (touch == 3) )
            BallYVelocity = -BallYVelocity;
          if ( (touch == 2) || (touch == 4) )
            BallXVelocity = -BallXVelocity;
					yellowOff(); //turns off yellow LED
        }
          if (Blocks[Index].visible) // any blocks left?
          LevelComplete = 0;
      }
      // No Blocks left, Move to next level.
      if ((LevelComplete == 1) && (Level > 0))
      {
				greenOn(); //green on when you move on to the next level
        Level--;
        printText("Level",5, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
        printNumber(MAX_BRICI_LEVELS - Level + 1, 45, SCREEN_HEIGHT-10, RGBToWord(0xff, 0xff, 0xff), RGBToWord(0, 0, 0));
				
				playTune(myTune2,myTime2,6); //plays tune when user levels up
				greenOff(); // green off
      }
      delay(10+Level*5); // Slow the game to human speed and make it level dependant.
    }
  }
  fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_WIDTH, RGBToWord(0, 0, 0xff));
  printText("VICTORY!",40, 100, RGBToWord(0xff, 0xff, 0), RGBToWord(0, 0, 0xff));
  printText("Up to restart", 18, 120, RGBToWord(0xff, 0xff, 0), RGBToWord(0, 0, 0xff));
  while (!UpPressed());
  return;
}

uint32_t sound_duration = 0; //initalises sound_duration

void playNote(uint32_t Freq,uint32_t duration)
{
      TIM14->CR1 = 0; // Set Timer 14 to default values
      TIM14->CCMR1 = (1 << 6) + (1 << 5);
      TIM14->CCER |= (1 << 0);
      TIM14->PSC = 48000000UL/65536UL; // yields maximum frequency of 21kHz when ARR = 2;
      TIM14->ARR = (48000000UL/(uint32_t)(TIM14->PSC))/((uint32_t)Freq);
      TIM14->CCR1 = TIM14->ARR/2;  
      TIM14->CNT = 0;
      TIM14->CR1 |= (1 << 0);
      sound_duration = duration;
      /*uint32_t end_time=milliseconds + duration;
      while (milliseconds < end_time);
      TIM14->CR1 |= (1u << 0); //Turn sound off*/

}

void SysTick_Handler(void) //function to turn off sound
{
      milliseconds++;
      if (sound_duration != 0)
      {
				sound_duration --;
			
				if (sound_duration==0)
				{
					TIM14->CR1 &= ~(1u << 0); //sound off
        }
      }
}

void SerialBegin() //
{
      /* On the nucleo board, TX is on PA2 while RX is on PA15 */
      RCC->AHBENR |= (1 << 17); // enable GPIOA
      RCC->APB2ENR |= (1 << 14); // enable USART1
      pinMode(GPIOA,2,2); // enable alternate function on PA2
      pinMode(GPIOA,15,2); // enable alternate function on PA15
      // AF1 = USART1 TX on PA2
      GPIOA->AFR[0] &= 0xfffff0ff;
      GPIOA->AFR[0] |= (1 << 8);
      // AF1 = USART1 RX on PA2
      GPIOA->AFR[1] &= 0x0fffffff;
      GPIOA->AFR[1] |= (1 << 28);
      // De-assert reset of USART1
      RCC->APB2RSTR &= ~(1u << 14);
	
      USART1->CR1 = 0; // disable before configuration
      USART1->CR3 |= (1 << 12); // disable overrun detection
      USART1->BRR = 48000000/9600; // assuming 48MHz clock and 9600 bps data rate
      USART1->CR1 |= (1 << 2) + (1 << 3); // enable Transmistter and receiver
      USART1->CR1 |= 1; // enable the UART

}


void eputchar(char c)
{
      while( (USART1->ISR & (1 << 6)) == 0); // wait for any ongoing
      USART1->ICR=0xffffffff;
      // transmission to finish
      USART1->TDR = c;
}


void eputs(char *String)
{
      while(*String) // keep printing until a NULL is found
      {
            eputchar(*String);
            String++;
      }
}

void initSysTick(void)
{
      SysTick->LOAD = 48000;
      SysTick->CTRL = 7;
      SysTick->VAL = 10;
      __asm(" cpsie i "); // enable interrupts
}

void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
       
  // Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
        // inserted into Flash memory interface
                       
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) );

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
      uint32_t end_time = dly + milliseconds;
      while(milliseconds != end_time)
            __asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
      Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
      Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
      uint32_t mode_value = Port->MODER;
      Mode = Mode << (2 * BitNumber);
      mode_value = mode_value & ~(3u << (BitNumber * 2));
      mode_value = mode_value | Mode;
      Port->MODER = mode_value;
}
void initTimer()
{
      // Power up the timer module
      RCC->APB1ENR |= (1 << 8);
      pinMode(GPIOB,1,2); // Assign a non-GPIO (alternate) function to GPIOB bit 1
      GPIOB->AFR[0] &= ~(0x0fu << 4); // Assign alternate function 0 to GPIOB 1 (Timer 14 channel 1)
      TIM14->CR1 = 0; // Set Timer 14 to default values
      TIM14->CCMR1 = (1 << 6) + (1 << 5);
      TIM14->CCER |= (1 << 0);
      TIM14->PSC = 48000000UL/65536UL; // yields maximum frequency of 21kHz when ARR = 2;
      TIM14->ARR = (48000000UL/(uint32_t)(TIM14->PSC))/((uint32_t)440);
      TIM14->CCR1 = TIM14->ARR/2;  
      TIM14->CNT = 0;
}

char egetchar()
{
      while( (USART1->ISR & (1<<5)) == 0);
      return (char)USART1->RDR;
}

int keyPressed()
{
      if ( (USART1->ISR & (1<<5)) ==0)
      {
            // no byte
            return 0;
      }
      else
      {
            return 1;
      }
}

int UpPressed(void)
{
			if (keyPressed())
      {
				if ((egetchar() | 32) =='w') //allows the user to input 'w' and 'W'
        {
					return 1;
        }
      }
	 
      if ( (GPIOA->IDR & (1<<8)) == 0) 
            return 1; // allows up button to be used 
      else
            return 0;
			
}
int DownPressed(void)
{
      if ( (GPIOA->IDR & (1<<11)) == 0) 
            return 1;
      else
            return 0;
}

int LeftPressed(void)
{
		if (keyPressed())
	{
		if ((egetchar() | 32) =='a') //allows the user to input 'a' and 'A'
		{
			return 1;
    }
	}
	
  if ( (GPIOB->IDR & (1<<5)) == 0)
            // no byte
		return 1;
  else
  // character is waiting
		return 0;
   
}

int RightPressed(void)
{
	if (keyPressed())
  {
		if ((egetchar() | 32) == 'd') //allows the user to input 'd' and 'D'
		{
			return 1;
    }
  }
     
	if ( (GPIOB->IDR & (1<<4)) == 0) 
		return 1;
  else
	  return 0;
                 
}



void hideBlock(uint32_t index)
{
      fillRectangle(Blocks[index].x,Blocks[index].y,BW,BH,0);
      Blocks[index].visible = 0;
}
void showBlock(uint32_t index)
{    
      fillRectangle(Blocks[index].x,Blocks[index].y,BW,BH,Blocks[index].colour);
      Blocks[index].visible = 1;
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
int blockTouching(int Index,uint16_t x,uint16_t y)
{
     
    // This function returns a non zero value if the object at x,y touches the sprite
    // The sprite is assumed to be rectangular and returns a value as follows:
    // 0 : not hit
    // 1 : touching on top face (lesser Y value)
    // 2 : touching on left face (lesser X value)
    // 3 : touching on bottom face (greater Y value)    
    // 4 : touching on right face (greater X value)
    if (Blocks[Index].visible == 0)
        return 0;
    if ( Blocks[Index].y == ball_y  )
    {  // top face?
      if ( (x>=Blocks[Index].x) && ( x < (Blocks[Index].x+BW) ) )
        return 1;      
    }
    if ( x == Blocks[Index].x )
    {
      // left face
      if ( (y>=Blocks[Index].y) && ( y < (Blocks[Index].y+BH) ) )
        return 2;
    }
    if ( y == (Blocks[Index].y+BH-1)  )
    {  // bottom face?
      if ( (x>=Blocks[Index].x) && ( x < (Blocks[Index].x+BW) ) )
        return 3;      
    }
    if ( x == (Blocks[Index].x + BW-1) )
    {
      // right face
      if ( (y>=Blocks[Index].y) && ( y < (Blocks[Index].y+BH) ) )
        return 4;
    }

    return 0; // not touching
}
static uint32_t prbs_shift_register=0;
void randomize(void)
{
      while(prbs_shift_register ==0) // can't have a value of zero here
            prbs_shift_register=milliseconds;
     
}
uint32_t random(uint32_t lower, uint32_t upper) //
{
      uint32_t new_bit=0;    
      uint32_t return_value;
      new_bit= ((prbs_shift_register & (1<<27))>>27) ^ ((prbs_shift_register & (1<<30))>>30);
      new_bit= ~new_bit;
      new_bit = new_bit & 1;
      prbs_shift_register=prbs_shift_register << 1;
      prbs_shift_register=prbs_shift_register | (new_bit);
      return_value = prbs_shift_register;
      return_value = (return_value)%(upper-lower)+lower;
      return return_value;
}
void playTune(uint32_t tune[],uint32_t time[],uint32_t count) //initialises the play tune function
{
      uint32_t index=0;
      while(index < count)
      {
            playNote(tune[index],time[index]);
            delay(time[index]); // delays the tune
            index++; //allows loop to start and end
      }
}

void redOn(void)
{
	GPIOA->ODR = GPIOA->ODR | (1<<1); // Red is connected to PA1 
}

void redOff(void)
{
	GPIOA->ODR = GPIOA->ODR &~ (1<<1); // Red is connected to PA1 
}

void greenOn(void)
{
	GPIOB->ODR = GPIOB->ODR | (1<<3); // Green is connected to PB3 
}

void greenOff(void)
{
	GPIOB->ODR = GPIOB->ODR &~ (1<<3); // Green is connected to PB3 
}

void yellowOn(void)
{
	GPIOA->ODR = GPIOA->ODR | (1<<0); // Yellow is connected to PA0 
}

void yellowOff(void)
{
	GPIOA->ODR = GPIOA->ODR &~ (1<<0); // Yellow is connected to PA0 
}