/*
 * File: "global.h"
 */

#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H
//-----------------------------------------------------------------------------
#include "ALed.h"
#include "ATicker.h"
#include "config.h"
#include "mrl.h"
//-----------------------------------------------------------------------------
extern int Verbose;    // verbose level (0, 1, 2 or 3)
extern ALed Led;       // компонент для моргания светодиодом
extern ATicker Ticker; // тикер с заданным периодом 
extern mrl_t Mrl;      // MicroRL object
//-----------------------------------------------------------------------------
#endif // GLOBAL_H

/*** end of "global.h" file ***/


