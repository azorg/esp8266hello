/*
 * file "print.h"
 */

#pragma once
#ifndef PRINT_H
#define PRINT_H
#include <stdint.h>
//-----------------------------------------------------------------------------
#include <stdio.h> // printf(), fprintf()
#define ERR(fmt, arg...) fprintf(stderr, "ERR: " fmt "\r\n", ## arg)
#ifdef DEBUG
#    define DBG(fmt, arg...) printf("DBG: " fmt "\r\n", ## arg)
#else
#  define DBG(fmt, ...) // debug output off
#endif // DEBUG
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
//-----------------------------------------------------------------------------
// print string
int print_str(const char *msg);
//-----------------------------------------------------------------------------
// print end of line
int print_eol();
//-----------------------------------------------------------------------------
// print char
int print_chr(char c);
//-----------------------------------------------------------------------------
// print integer
int print_int(int i);
//-----------------------------------------------------------------------------
// print unsigned integer
int print_uint(unsigned i);
//-----------------------------------------------------------------------------
// print unsigned integer with '0' on the begin
int print_uint_ex(unsigned i, char digits);
//-----------------------------------------------------------------------------
// print binary unsigned integer value
int print_bin(unsigned i, char digits);
//-----------------------------------------------------------------------------
// hex print unsigned integer
int print_hex(unsigned i, char digits);
//-----------------------------------------------------------------------------
// print integer as float in NNN.D format [d = (int) (f * 10.)]
void print_dint(int d);
//-----------------------------------------------------------------------------
// print "ident = value\r\n" (integer value)
void print_ival(const char *ident, int i);
//-----------------------------------------------------------------------------
// print "ident = NNN.D\r\n" (integer value as float)
void print_dval(const char *ident, int d);
//-----------------------------------------------------------------------------
// print "ident = value\r\n" (unsigned integer value)
void print_uval(const char *ident, unsigned i);
//-----------------------------------------------------------------------------
// print "ident = value\r\n" (unsigned bin value)
void print_bval(const char *ident, unsigned i, char digits);
//-----------------------------------------------------------------------------
// print "ident = value\r\n" (unsigned hex value)
void print_hval(const char *ident, unsigned i, char digits);
//-----------------------------------------------------------------------------
// print "ident = value\r\n" (string value)
void print_sval(const char *ident, const char *str);
//-----------------------------------------------------------------------------
// flush USART/LPUART or USB-CDC TX buffers
void print_flush(void);
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cplusplus
//-----------------------------------------------------------------------------
#endif // PRINT_H

/*** end of "print.h" file ***/

