/**************************************************************************/
/*!
  @file Adafruit_PCD8544.cpp

  @mainpage Adafruit PCD8544 Nokia 5110 LCD Library

  @section intro Introduction

  This is a library for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

  These displays use SPI to communicate, 4 or 5 pins are required to
  interface

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  @section author Author

  Written by Limor Fried/Ladyada  for Adafruit Industries.

  @section license License

  BSD license, check license.txt for more information
  All text above, and the splash screen below must be included in any
  redistribution

 */
/**************************************************************************/

#ifdef __AVR__
#include <avr/pgmspace.h>
#endif
#include "Adafruit_PCD8544.h"
#include "Arduino.h"
#include <stdlib.h>

/** the memory buffer for the LCD */
uint8_t pcd8544_buffer[LCDWIDTH * LCDHEIGHT / 8] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFF, 0xFC, 0xE0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8,
    0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80, 0xC0,
    0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x87, 0x8F, 0x9F,
    0x9F, 0xFF, 0xFF, 0xFF, 0xC1, 0xC0, 0xE0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE, 0xFC, 0xFC, 0xF8, 0xF8,
    0xF0, 0xE0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0,
    0xF1, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x0F, 0x87,
    0xE7, 0xFF, 0xFF, 0xFF, 0x1F, 0x1F, 0x3F, 0xF9, 0xF8, 0xF8, 0xF8, 0xF8,
    0xF8, 0xF8, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x7F, 0x3F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFE, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x7E, 0x3F, 0x3F, 0x0F,
    0x1F, 0xFF, 0xFF, 0xFF, 0xFC, 0xF0, 0xE0, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFC, 0xF0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x0F, 0x1F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF,
    0x7F, 0x7F, 0x1F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// reduces how much is refreshed, which speeds it up!
// originally derived from Steve Evans/JCW's mod but cleaned up and
// optimized
//#define enablePartialUpdate

/*!
  @brief Update the bounding box for partial updates
  @param xmin left
  @param ymin bottom
  @param xmax right
  @param ymax top
 */
void Adafruit_PCD8544::updateBoundingBox(uint8_t xmin, uint8_t ymin,
                                         uint8_t xmax, uint8_t ymax) {
  xUpdateMin = min(xUpdateMin, xmin);
  xUpdateMax = max(xUpdateMax, xmax);
  yUpdateMin = min(yUpdateMin, ymin);
  yUpdateMax = max(yUpdateMax, ymax);
}

/*!
  @brief Constructor for software SPI with explicit CS pin
  @param sclk_pin SCLK pin
  @param din_pin  DIN pin
  @param dc_pin   DC pin
  @param cs_pin   CS pin
  @param rst_pin  RST pin
 */
Adafruit_PCD8544::Adafruit_PCD8544(int8_t sclk_pin, int8_t din_pin,
                                   int8_t dc_pin, int8_t cs_pin, int8_t rst_pin)
    : Adafruit_GFX(LCDWIDTH, LCDHEIGHT) {
  spi_dev = new Adafruit_SPIDevice(cs_pin, sclk_pin, -1, din_pin,
                                   4000000); // 4mhz max speed

  _dcpin = dc_pin;
  _rstpin = rst_pin;
}

/*!
  @brief Constructor for hardware SPI based on hardware controlled SCK (SCLK)
  and MOSI (DIN) pins. CS is still controlled by any IO pin. NOTE: MISO and SS
  will be set as an input and output respectively, so be careful sharing those
  pins!
  @param dc_pin   DC pin
  @param cs_pin   CS pin
  @param rst_pin  RST pin
  @param theSPI   Pointer to SPIClass device for hardware SPI
 */
Adafruit_PCD8544::Adafruit_PCD8544(int8_t dc_pin, int8_t cs_pin, int8_t rst_pin,
                                   SPIClass *theSPI)
    : Adafruit_GFX(LCDWIDTH, LCDHEIGHT) {
  spi_dev = new Adafruit_SPIDevice(cs_pin, 4000000, SPI_BITORDER_MSBFIRST,
                                   SPI_MODE0, theSPI);

  _dcpin = dc_pin;
  _rstpin = rst_pin;
}

/*!
  @brief The most basic function, set a single pixel
  @param x     x coord
  @param y     y coord
  @param color pixel color (BLACK or WHITE)
 */
void Adafruit_PCD8544::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
    return;

  int16_t t;
  switch (rotation) {
  case 1:
    t = x;
    x = y;
    y = LCDHEIGHT - 1 - t;
    break;
  case 2:
    x = LCDWIDTH - 1 - x;
    y = LCDHEIGHT - 1 - y;
    break;
  case 3:
    t = x;
    x = LCDWIDTH - 1 - y;
    y = t;
    break;
  }

  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color)
    pcd8544_buffer[x + (y / 8) * LCDWIDTH] |= 1 << (y % 8);
  else
    pcd8544_buffer[x + (y / 8) * LCDWIDTH] &= ~(1 << (y % 8));

  updateBoundingBox(x, y, x, y);
}

/*!
  @brief The most basic function, get a single pixel
  @param  x x coord
  @param  y y coord
  @return   color of the pixel at x,y
 */
uint8_t Adafruit_PCD8544::getPixel(int8_t x, int8_t y) {
  if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
    return 0;

  return (pcd8544_buffer[x + (y / 8) * LCDWIDTH] >> (y % 8)) & 0x1;
}

/*!
  @brief Initialize the display. Set bias and contrast, enter normal mode.
 */
void Adafruit_PCD8544::initDisplay() {

  // toggle RST low to reset
  if (_rstpin >= 0) {
    pinMode(_rstpin, OUTPUT);
    digitalWrite(_rstpin, LOW);
    delay(1); // 1 ns minimum
    digitalWrite(_rstpin, HIGH);
  }

  setBias(_bias);
  setContrast(_contrast);

  // normal mode
  command(PCD8544_FUNCTIONSET);

  // Set display to Normal
  command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
}

/*!
  @brief Set up SPI, initialize the display, set the bounding box
  @param contrast Initial contrast value
  @param bias     Initial bias value
 */
bool Adafruit_PCD8544::begin(uint8_t contrast, uint8_t bias) {

  if (!spi_dev->begin()) {
    return false;
  }

  // Set common pin outputs.
  pinMode(_dcpin, OUTPUT);
  if (_rstpin >= 0)
    pinMode(_rstpin, OUTPUT);

  _bias = bias;
  _contrast = contrast;
  _reinit_interval = 0;
  _display_count = 0;
  initDisplay();

  // initial display line
  // set page address
  // set column address
  // write display data

  // set up a bounding box for screen updates

  updateBoundingBox(0, 0, LCDWIDTH - 1, LCDHEIGHT - 1);
  // Push out pcd8544_buffer to the Display (will show the AFI logo)
  display();

  return true;
}

/*!
  @brief  Send a command to the LCD
  @param c Command byte
 */
void Adafruit_PCD8544::command(uint8_t c) {
  digitalWrite(_dcpin, LOW);
  spi_dev->write(&c, 1);
}

/*!
  @brief  Send data to the LCD
  @param c Data byte
 */
void Adafruit_PCD8544::data(uint8_t c) {
  digitalWrite(_dcpin, HIGH);
  spi_dev->write(&c, 1);
}

/*!
  @brief  Set the contrast level
  @param val Contrast value
 */
void Adafruit_PCD8544::setContrast(uint8_t val) {
  if (val > 0x7f) {
    val = 0x7f;
  }
  _contrast = val;
  command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
  command(PCD8544_SETVOP | val);
  command(PCD8544_FUNCTIONSET);
}

/*!
  @brief  Set the bias level
  @param val Bias value
 */
void Adafruit_PCD8544::setBias(uint8_t val) {
  if (val > 0x07) {
    val = 0x07;
  }
  _bias = val;
  command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
  command(PCD8544_SETBIAS | val);
  command(PCD8544_FUNCTIONSET);
}

/*!
  @brief  Get the bias level
  @return Bias value
 */
uint8_t Adafruit_PCD8544::getBias() { return _bias; }

/*!
  @brief  Get the contrast level
  @return Contrast value
 */
uint8_t Adafruit_PCD8544::getContrast() { return _contrast; }

/*!
  @brief  Set the interval for reinitializing the display
  @param val Reinit after this many calls to display()
 */
void Adafruit_PCD8544::setReinitInterval(uint8_t val) {
  _reinit_interval = val;
}

/*!
  @brief  Get the reinit interval
  @return Reinit interval
 */
uint8_t Adafruit_PCD8544::getReinitInterval() { return _reinit_interval; }

/*!
  @brief Update the display
 */
void Adafruit_PCD8544::display(void) {
  uint8_t col, maxcol, p;

  if (_reinit_interval) {
    _display_count++;
    if (_display_count >= _reinit_interval) {
      _display_count = 0;
      initDisplay();
#ifdef enablePartialUpdate
      yUpdateMin = 0;
      yUpdateMax = LCDHEIGHT - 1;
      xUpdateMin = 0;
      xUpdateMax = LCDWIDTH - 1;
#endif
    }
  }

  for (p = 0; p < 6; p++) {
#ifdef enablePartialUpdate
    // check if this page is part of update
    if (yUpdateMin >= ((p + 1) * 8)) {
      continue; // nope, skip it!
    }
    if (yUpdateMax < p * 8) {
      break;
    }
#endif

    command(PCD8544_SETYADDR | p);

#ifdef enablePartialUpdate
    col = xUpdateMin;
    maxcol = xUpdateMax;
#else
    // start at the beginning of the row
    col = 0;
    maxcol = LCDWIDTH - 1;
#endif

    command(PCD8544_SETXADDR | col);

    digitalWrite(_dcpin, HIGH);
    spi_dev->write(pcd8544_buffer + (LCDWIDTH * p), maxcol - col + 1);
    /*
    digitalWrite(_cs, LOW);
    for (; col <= maxcol; col++) {
      spiWrite(pcd8544_buffer[(LCDWIDTH * p) + col]);
    }
    if (_cs > 0)
      digitalWrite(_cs, HIGH);
      */
  }

  command(PCD8544_SETYADDR); // no idea why this is necessary but it is to
                             // finish the last byte?
#ifdef enablePartialUpdate
  xUpdateMin = LCDWIDTH - 1;
  xUpdateMax = 0;
  yUpdateMin = LCDHEIGHT - 1;
  yUpdateMax = 0;
#endif
}

/*!
  @brief Clear the entire display
 */
void Adafruit_PCD8544::clearDisplay(void) {
  memset(pcd8544_buffer, 0, LCDWIDTH * LCDHEIGHT / 8);
  updateBoundingBox(0, 0, LCDWIDTH - 1, LCDHEIGHT - 1);
  cursor_y = cursor_x = 0;
}

/*
// this doesnt touch the buffer, just clears the display RAM - might be handy
void Adafruit_PCD8544::clearDisplay(void) {

  uint8_t p, c;

  for(p = 0; p < 8; p++) {

    st7565_command(CMD_SET_PAGE | p);
    for(c = 0; c < 129; c++) {
      //uart_putw_dec(c);
      //uart_putchar(' ');
      st7565_command(CMD_SET_COLUMN_LOWER | (c & 0xf));
      st7565_command(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
      st7565_data(0x0);
    }
    }

}

*/
