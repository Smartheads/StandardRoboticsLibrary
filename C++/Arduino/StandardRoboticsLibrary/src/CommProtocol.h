/*
* MIT License
*
* Copyright (c) 2018 Robert Hutter
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef _COMMPROTOCOL_H
#define _COMMPROTOCOL_H

#include "SRL.h"

namespace SRL {
  class CommProtocol
  {
    public:
      virtual uint8_t writeBytes(uint8_t reg, uint8_t bytec, byte* bytev, uint8_t start = 0) = 0;
      uint8_t writeByte(uint8_t reg, byte data);
      uint8_t writeBits(uint8_t reg, uint8_t startBit, uint8_t len, byte data);

      virtual uint8_t readBytes(uint8_t reg, byte* buff, uint8_t len) = 0;
      byte readByte(uint8_t reg);
      int16_t readInt16_t(uint8_t reg);

      uint8_t getAddress(void);
      void setAddress(uint8_t address);

    protected:
      uint8_t address;
  };
}

#endif
