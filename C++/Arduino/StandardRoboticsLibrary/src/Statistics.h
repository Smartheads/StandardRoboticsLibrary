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
#ifndef _STATISTICS_H
#define _STATISTICS_H

#include "SRL.h"
#include <math.h>

namespace SRL
{
  template <typename number>
  double getMedian(unsigned int argc, number* argv)
  {
    unsigned int j;

    for (unsigned int i = 1; i < argc; i++)
    {
      for (j = i; j > 0 && argv[j - 1] > argv[j]; j--)
      {
        number t = argv[j];
        argv[j] = argv[j - 1];
        argv[j - 1] = t;
      }
    }

	if (argc % 2 == 1)
	{
		return argv[(unsigned int) floor(argc / 2)];
	}
	else
	{
		unsigned int m = (unsigned int) floor(argc / 2);
		return (argv[m] + argv[m+1]) / 2;
	}
  }

  template <typename number>
  number getAverage(unsigned int argc, number* argv)
  {
    number sum = 0;
    for (unsigned int i = 0; i < argc; i++)
    {
      sum += argv[i];
    }

    return sum / argc;
  }

  template <typename number>
  bool equals(number a, number b, number v = ((number) 0))
  {
    return a <= (b + v) && a >= (b - v);
  }
}

#endif
