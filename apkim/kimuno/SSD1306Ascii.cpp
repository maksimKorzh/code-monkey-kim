/* Arduino SSD1306Ascii Library
 * Copyright (C) 2015 by William Greiman
 *
 * This file is part of the Arduino SSD1306Ascii Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino SSD1306Ascii Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "SSD1306Ascii.h"

// make RAM accessible (talk about dirty coding...)
extern uint8_t RAM[1024];     // main 1KB RAM    0x000-0x3FF


//------------------------------------------------------------------------------
uint8_t SSD1306Ascii::charWidth(uint8_t c) {
  if (!m_font) {
    return 0;
  }
  uint8_t first = readFontByte(m_font + FONT_FIRST_CHAR);
  uint8_t count = readFontByte(m_font + FONT_CHAR_COUNT);
  if (c < first || c >= (first + count)) {
    return 0;
  }
  if (readFontByte(m_font) || readFontByte(m_font + 1) > 1) {
    // Proportional font.
    return readFontByte(m_font + FONT_WIDTH_TABLE + c - first);
  }
  // Fixed width font.
  return m_magFactor*readFontByte(m_font + FONT_FIXED_WIDTH);
}
//------------------------------------------------------------------------------
void SSD1306Ascii::clear() {
  clear(0, displayWidth() - 1, 0 , displayRows() - 1);
  #if INCLUDE_SCROLLING 
  m_scroll = m_scroll ? 1 : 0;
  ssd1306WriteCmd(SSD1306_SETSTARTLINE | 0); 
  #endif  //INCLUDE_SCROLLING   
}
//------------------------------------------------------------------------------
void SSD1306Ascii::clear(uint8_t c0, uint8_t c1, uint8_t r0, uint8_t r1) {
  if (r1 >= displayRows()) r1 = displayRows() - 1;
  for (uint8_t r = r0; r <= r1; r++) {
    setCursor(c0, r);
    for (uint8_t c = c0; c <= c1; c++) {
      ssd1306WriteRamBuf(0);   
    }
  }
  setCursor(c0, r0);
}
//------------------------------------------------------------------------------
void SSD1306Ascii::clearToEOL() {
  clear (m_col, displayWidth() - 1, m_row, m_row + fontRows() - 1);
}
//------------------------------------------------------------------------------
uint8_t SSD1306Ascii::fontHeight() {
  return m_font ? m_magFactor*readFontByte(m_font + FONT_HEIGHT) : 0;
}
//------------------------------------------------------------------------------
uint8_t SSD1306Ascii::fontWidth() {
  return m_font ? m_magFactor*readFontByte(m_font + FONT_FIXED_WIDTH) : 0;
}
//------------------------------------------------------------------------------
void SSD1306Ascii::init(const DevType* dev) {
  m_col = 0;
  m_row = 0;
  #ifdef __AVR__  
  const uint8_t* table = (const uint8_t*)pgm_read_word(&dev->initcmds);
  #else  // __AVR__
  const uint8_t* table = dev->initcmds;
  #endif  // __AVR
  uint8_t size = readFontByte(&dev->initSize);
  m_displayWidth = readFontByte(&dev->lcdWidth);
  m_displayHeight = readFontByte(&dev->lcdHeight);
  m_colOffset = readFontByte(&dev->colOffset); 
  for (uint8_t i = 0; i < size; i++) {
    ssd1306WriteCmd(readFontByte(table + i));
  }
  #if INCLUDE_SCROLLING 
  m_scroll = m_displayHeight != 64 || INCLUDE_SCROLLING < 2 ? 0 : 1;
  #endif  //INCLUDE_SCROLLING 
  clear();
}
//------------------------------------------------------------------------------
void SSD1306Ascii::reset(uint8_t rst) {
  pinMode(rst, OUTPUT);
  digitalWrite(rst, LOW);
  delay(10);
  digitalWrite(rst, HIGH);
  delay(10);  
}
//------------------------------------------------------------------------------
void SSD1306Ascii::setCol(uint8_t col) {
  if (col >= m_displayWidth) return;
  m_col = col;
  col += m_colOffset;
  ssd1306WriteCmd(SSD1306_SETLOWCOLUMN | (col & 0XF));
  ssd1306WriteCmd(SSD1306_SETHIGHCOLUMN | (col >> 4));    
}  
//------------------------------------------------------------------------------
void SSD1306Ascii::setContrast(uint8_t value) {
  ssd1306WriteCmd(SSD1306_SETCONTRAST);
  ssd1306WriteCmd(value);
}
//------------------------------------------------------------------------------
void SSD1306Ascii::setCursor(uint8_t col, uint8_t row) {
  setCol(col);
  setRow(row);
}  
//------------------------------------------------------------------------------
void SSD1306Ascii::setRow(uint8_t row) {
  if (row >= m_displayHeight/8) return;
  m_row = row;
  ssd1306WriteCmd(SSD1306_SETSTARTPAGE | row); 
}
//------------------------------------------------------------------------------
#if INCLUDE_SCROLLING 
void SSD1306Ascii::setScroll(bool enable) {
  if (m_displayHeight != 64) return;
  clear();
  m_scroll = enable ? 1 : 0;
}
#endif   // INCLUDE_SCROLLING 
//-----------------------------------------------------------------------------
void SSD1306Ascii::ssd1306WriteRam(uint8_t c) {
  if (m_col >= m_displayWidth) return;
  writeDisplay(c, SSD1306_MODE_RAM);
  m_col++;
}
//-----------------------------------------------------------------------------
void SSD1306Ascii::ssd1306WriteRamBuf(uint8_t c) {
  if (m_col >= m_displayWidth) return;
  writeDisplay(c, SSD1306_MODE_RAM_BUF);
  m_col++;
}
//------------------------------------------------------------------------------
GLCDFONTDECL(scaledNibble) = {
  0X00, 0X03, 0X0C, 0X0F,
  0X30, 0X33, 0X3C, 0X3F,
  0XC0, 0XC3, 0XCC, 0XCF,
  0XF0, 0XF3, 0XFC, 0XFF
};
//------------------------------------------------------------------------------
size_t SSD1306Ascii::write(uint8_t ch) {
  const uint8_t* base = m_font;
  if (!base) return 0;
  uint16_t size = readFontByte(base++) << 8;
  size |= readFontByte(base++);
  uint8_t w = readFontByte(base++);
  uint8_t h = readFontByte(base++);
  uint8_t nr = (h + 7)/8;
  uint8_t first = readFontByte(base++);
  uint8_t count = readFontByte(base++);
  if (ch < first || ch >= (first + count)) {
    if (ch == '\r') {
      setCol(0);
      return 1;      
    }    
    if (ch == '\n') {
      #if INCLUDE_SCROLLING == 0
      setCursor(0, m_row + m_magFactor*nr);
      #else  // INCLUDE_SCROLLING
      uint8_t tmp = m_row + m_magFactor*nr;
      if (tmp >= m_displayHeight/8  && m_scroll) {
        tmp = 0;
        m_scroll = 2;
      }
      setCursor(0, tmp);
      if (m_scroll > 1) {
        clearToEOL();
        tmp +=  m_magFactor*nr;
        tmp *= 8;
        if (tmp > m_displayHeight) {
          tmp = 0;
        }
        ssd1306WriteCmd(SSD1306_SETSTARTLINE | tmp);        
      }
      #endif  // INCLUDE_SCROLLING      
      return 1;
    }
    return 0;
  }
  ch -= first;
  uint8_t s = m_magFactor;
  uint8_t thieleShift = 0;
  if (size < 2) {
    if (size) s = 0;
    base += nr*w*ch;
  } else {
    if (h & 7) {
      thieleShift = 8 - (h & 7);
    }
    uint16_t index = 0;
    for (uint8_t i = 0; i < ch; i++) {
      index += readFontByte(base + i);
    }
    w = readFontByte(base + ch);
    base += nr*index + count;
  }
  uint8_t scol = m_col;
  uint8_t srow = m_row;
  for (uint8_t r = 0; r < nr; r++) {
    for (uint8_t m = 0; m < m_magFactor; m++) {
      if (r || m) setCursor(scol, m_row + 1);
      for (uint8_t c = 0; c < w; c++) {
        uint8_t b = readFontByte(base + c + r*w);
        if (thieleShift && (r + 1) == nr) {
          b >>= thieleShift;
        }
        if (m_magFactor == 2) {
           b = m ?  b >> 4 : b & 0XF;
           b = readFontByte(scaledNibble + b);
           ssd1306WriteRamBuf(b);
        }
        ssd1306WriteRamBuf(b);
      }
      for (uint8_t i = 0; i < s; i++) {
        ssd1306WriteRamBuf(0);
      }
    }
  }
  setRow(srow);
  return 1;
}
//------------------------------------------------------------------------------
size_t SSD1306Ascii::write(const char* s) {
  size_t n = strlen(s);
  for (size_t i = 0; i < n; i++) {
    write(s[i]);
  }
  return n;
}


// new by oscar for KIM Uno
//------------------------------------------------------------------------------


// test ROM image... 
const uint8_t Atest[256] PROGMEM = {
// program code
0x72, 0x70, 0x22, 0x78, 0x22, 0x52, 0xC4, 0xC4, 
0xC4, 0xF8, 0x00, 0xB0, 0xF8, 0x00, 0xA0, 0x80, 
0xE2, 0xE2, 0x20, 0xA0, 0xE2, 0x20, 0xA0, 0xE2, 
0x20, 0xA0, 0x3C, 0x1E, 0x30, 0x0F, 0xE2, 0x69,
0x3F, 0x2F, 0x6C, 0xA4, 0x37, 0x33, 0x3F, 0x35,
0x6C, 0x54, 0x14, 0x30, 0x33, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// bitmap  
0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00, 
0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00, 
0x7B,   0xDE,   0xDB,   0xDE,   0x00,   0x00,   0x00,   0x00, 
0x4A,   0x50,   0xDA,   0x52,   0x00,   0x00,   0x00,   0x00, 
0x42,   0x5E,   0xAB,   0xD0,   0x00,   0x00,   0x00,   0x00, 
0x4A,   0x42,   0x8A,   0x52,   0x00,   0x00,   0x00,   0x00, 
0x7B,   0xDE,   0x8A,   0x5E,   0x00,   0x00,   0x00,   0x00, 
0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x00, 
0x00,   0x00,   0x00,   0x00,   0x00,   0x00,   0x07,   0xE0, 
0x00,   0x00,   0x00,   0x00,   0xFF,   0xFF,   0xFF,   0xFF, 
0x00,   0x06,   0x00,   0x01,   0x00,   0x00,   0x00,   0x01, 
0x00,   0x7F,   0xE0,   0x01,   0x00,   0x00,   0x00,   0x02, 
0x7F,   0xC0,   0x3F,   0xE0,   0xFC,   0xFF,   0xFF,   0xFE, 
0x40,   0x0F,   0x00,   0x10,   0x04,   0x80,   0x00,   0x00, 
0x7F,   0xC0,   0x3F,   0xE0,   0x04,   0x80,   0x00,   0x00, 
0x00,   0x3F,   0xD0,   0x40,   0x04,   0x80,   0x00,   0x00, 
0x00,   0x0F,   0x08,   0x20,   0x04,   0x80,   0x7A,   0x1E, 
0x00,   0x00,   0x07,   0x90,   0x04,   0x80,   0x42,   0x10, 
0x00,   0x00,   0x18,   0x7F,   0xFC,   0xF0,   0x72,   0x1C, 
0x00,   0x00,   0x30,   0x00,   0x00,   0x10,   0x42,   0x10, 
0x00,   0x00,   0x73,   0xFC,   0x00,   0x10,   0x7B,   0xD0, 
0x00,   0x00,   0x30,   0x00,   0x3F,   0xF0,   0x00,   0x00, 
0x00,   0x00,   0x18,   0x0F,   0xC0,   0x00,   0x00,   0x00, 
0x00,   0x00,   0x07,   0xF0,   0x00,   0x00,   0x00,   0x00, 

};


void SSD1306Ascii::paintscreen(void)
{
  uint8_t line, block, i, Bx;
  uint32_t B[8], A[8];
      int32_t m=1;
      int32_t n=1;
      uint32_t x,y,t;

  ssd1306WriteCmd(SSD1306_MEMORYMODE);
  ssd1306WriteCmd(0x00);  // horizontal mode

  for (line=0;line<8;line++)        
  {
    for (block=0;block<8;block++)
    {
      for (i=0;i<4;i++)           
      { A[i*2+1] = A[i*2] = pgm_read_byte_near (Atest + line*32 + block + (3-i)*(8));
      }
      
       x = (A[0]<<24)   | (A[m]<<16)   | (A[2*m]<<8) | A[3*m];
       y = (A[4*m]<<24) | (A[5*m]<<16) | (A[6*m]<<8) | A[7*m];
    
       t = (x ^ (x >> 7)) & 0x00AA00AA;  x = x ^ t ^ (t << 7);
       t = (y ^ (y >> 7)) & 0x00AA00AA;  y = y ^ t ^ (t << 7);
    
       t = (x ^ (x >>14)) & 0x0000CCCC;  x = x ^ t ^ (t <<14);
       t = (y ^ (y >>14)) & 0x0000CCCC;  y = y ^ t ^ (t <<14);
    
       t = (x & 0xF0F0F0F0) | ((y >> 4) & 0x0F0F0F0F);
       y = ((x << 4) & 0xF0F0F0F0) | (y & 0x0F0F0F0F);
       x = t;
    
       B[0]=x>>24;    B[n]=x>>16;    B[2*n]=x>>8;  B[3*n]=x;
       B[4*n]=y>>24;  B[5*n]=y>>16;  B[6*n]=y>>8;  B[7*n]=y; 

      for (i=0;i<8;i++)
      { 
        writeDisplay(B[i], SSD1306_MODE_RAM_BUF);
        writeDisplay(B[i], SSD1306_MODE_RAM_BUF);
      }
    }
  }
}

