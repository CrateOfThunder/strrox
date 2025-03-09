/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org>
 */

/* ----------------------------------------------------------------------- */
/* Bidirectional Bit Shifting Extension for Supporting Strings             */
/*              strrox.c -- string bit rotation implementation             */
/* Ver. 1.00                    02AUG2024                   CrateOfThunder */
/* ----------------------------------------------------------------------- */
#include "strrox.h"

typedef unsigned char uch;

static uch _ror11(uch a, uch b) { return ((a << 7) | (b >> 1)); }
static uch _rol11(uch a, uch b) { return ((a << 1) | (b >> 7)); }
static uch _ror(uch b) { return (b << 7); }
static uch _rol(uch b) { return (b >> 7); }
static void _srl(uch *b) { *b >>= 1; return; }
static void _sll(uch *b) { *b <<= 1; return; }

void
strrox(v, size, nbits, shift)
  void *v;
  size_t size;
  size_t nbits;
  int shift;
{
  uch (*_rox11[2])(uch, uch) = { _rol11, _ror11 },
      (*_carry[2])(uch) = { _rol, _ror },
      (*carry)(uch),
      *p1, *p2, *pp[2], *ptr, carry_bit;
  void (*_srx[2])(uch *) = { _sll, _srl };
  int xcrement;

  if ((size == 0) || (nbits == 0) || ((shift != 1) && (shift != 0))) return;

  xcrement = ~shift + (!shift + 1);
  carry = _carry[shift];
  p1 = (uch *)v;
  p2 = &p1[size - 1];

  do {
    pp[0] = p1;
    pp[1] = p2;
    ptr = pp[shift];
    carry_bit = carry(*ptr);

    while (ptr != pp[!shift]) {
      *ptr = _rox11[shift](ptr[-shift], ptr[!shift]);
      ptr = ptr + xcrement;
    }

    _srx[shift](ptr);
    *ptr |= carry_bit;
  } while (--nbits);

  return;
}
