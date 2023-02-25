/*
 * file "print.cpp"
 */

//-----------------------------------------------------------------------------
#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"
#include "print.h"
//-----------------------------------------------------------------------------
// print string to UART
int print_str(const char *msg)
{
  int len = strlen(msg);

#if defined(PRINT_SERIAL)
  int retv = Serial.print(msg);
#endif

  return retv;
}
//-----------------------------------------------------------------------------
// print end of line
int print_eol()
{
  return print_str("\r\n");
}
//-----------------------------------------------------------------------------
// print char
int print_chr(char c)
{
#if defined(PRINT_SERIAL)
  return Serial.print(c);
#endif
}
//-----------------------------------------------------------------------------
// print integer value
int print_int(int i)
{
  char minus = 0;
  char buf[12];
  char *ptr = buf + sizeof(buf) - 1;

  if (i < 0)
  {
    i = -i;
    minus = 1;
  }

  *ptr = '\0';
  do {
    *--ptr = '0' + (i % 10);
    i /= 10;
  } while (i != 0);

  if (minus)
    *--ptr = '-';

  return print_str(ptr);
}
//-----------------------------------------------------------------------------
// print unsigned integer
int print_uint(unsigned i)
{
  char buf[12];
  char *ptr = buf + sizeof(buf) - 1;

  *ptr = '\0';
  do {
    *--ptr = '0' + (i % 10);
    i /= 10;
  } while (i != 0);

  return print_str(ptr);
}
//-----------------------------------------------------------------------------
// print unsigned integer with '0' on the begin
int print_uint_ex(unsigned i, char digits)
{
  char buf[12];
  char *ptr = buf + sizeof(buf) - 1;

  if (digits > 10) digits = 10; // 2**32 = 4294967296 

  *ptr = '\0';
  do {
    *--ptr = '0' + (i % 10);
    digits--;
    i /= 10;
  } while (i != 0);

  while (digits--)
    *--ptr = '0';

  return print_str(ptr);
}
//-----------------------------------------------------------------------------
// print binary unsigned integer value
int print_bin(unsigned i, char digits)
{
  char buf[33];
  char *ptr = buf + 32;
  if (digits > 32) digits = 32; 

  *ptr = '\0';
  while (digits-- > 0)
  {
    *--ptr = '0' + (i & 1);
    i >>= 1;
  }

  return print_str(ptr);
}
//-----------------------------------------------------------------------------
// hex print unsigned integer value
int print_hex(unsigned i, char digits)
{
  char buf[9];
  char *ptr = buf + 8;
  if (digits > 8) digits = 8; 

  *ptr = '\0';
  while (digits-- > 0)
  {
    int c = i & 0xf;
    if (c < 10)
      c += '0';
    else
      c += 'A' - 10;
    *--ptr = c;
    i >>= 4;
  }

  return print_str(ptr);
}
//-----------------------------------------------------------------------------
// print integer as float in NNN.D format [d = (int) (f * 10.)]
void print_dint(int d)
{
  print_int(d / 10);
  print_chr('.');
  print_int(abs(d % 10));
}
//-----------------------------------------------------------------------------
// print "ident = value\r\n" (integer value)
void print_ival(const char *ident, int i)
{
  print_str(ident);
  print_int(i);
  print_eol();
}
//-----------------------------------------------------------------------------
// print "ident = NNN.D\r\n" (integer value as float)
void print_dval(const char *ident, int d)
{
  print_str(ident);
  print_dint(d);
  print_eol();
}
//-----------------------------------------------------------------------------
// print "ident = value\r\n" (unsigned integer value)
void print_uval(const char *ident, unsigned i)
{
  print_str(ident);
  print_uint(i);
  print_eol();
}
//-----------------------------------------------------------------------------
// print "ident = value\r\n" (unsigned bin value)
void print_bval(const char *ident, unsigned i, char digits)
{
  print_str(ident);
  print_bin(i, digits);
  print_eol();
}
//-----------------------------------------------------------------------------
// print "ident = value\r\n" (unsigned hex value)
void print_hval(const char *ident, unsigned i, char digits)
{
  print_str(ident);
  print_hex(i, digits);
  print_eol();
}
//-----------------------------------------------------------------------------
// print "ident = value\r\n" (string value)
void print_sval(const char *ident, const char *str)
{
  print_str(ident);
  print_str(str);
  print_eol();
}
//-----------------------------------------------------------------------------
// flush USART/LPUART or USB-CDC TX buffers
void print_flush()
{
#if defined(PRINT_SERIAL)
  Serial.flush();
#endif
}
//-----------------------------------------------------------------------------

/*** end of "print.cpp" file ***/

