#include <stdint.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160
static void ResetLow(void);
static void ResetHigh(void);
void display_begin(void);
static void CSLow(void);
static void CSHigh(void);
static void DCLow(void);
static void DCHigh(void);
static void initSPI(void);
static uint8_t transferSPI8(uint8_t data);
static uint16_t transferSPI16(uint16_t data);
static void command(uint8_t cmd);
static void data(uint8_t data);
void delay(uint32_t dly);

void openAperture(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void fillRectangle(uint16_t x,uint16_t y,uint16_t width, uint16_t height, uint16_t colour);
void putPixel(uint16_t x, uint16_t y, uint16_t colour);
void putImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *Image, int hOrientation,int vOrientation);
void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t Colour);
static int iabs(int x);
void drawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t Colour);
void drawCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t Colour);
void fillCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t Colour);
void printText(const char *Text,uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour);
void printTextX2(const char *Text, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour);
void printNumber(uint16_t Number, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour);
void printNumberX2(uint16_t Number, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour);
uint16_t RGBToWord(uint16_t R, uint16_t G, uint16_t B);
static void drawLineLowSlope(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1, uint16_t Colour);
static void drawLineHighSlope(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1, uint16_t Colour);
void clear(void);
static uint32_t mystrlen(const char *s);
void invertDisplay(void);
