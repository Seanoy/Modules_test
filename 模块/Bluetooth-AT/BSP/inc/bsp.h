/****************************************Copyright (c)**************************************************
**                               		raynen
**                                      
**                                 http://www.burnon.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			config.h
** Last modified Date:  2009-12-9
** Last Version:		1.0
** Descriptions:		
**

********************************************************************************************************/
#ifndef  __BSP_H
#define  __BSP_H

#ifndef NULL
	#define NULL    ((void *)0)
#endif

#ifndef FALSE
	#define FALSE   (0)
#endif

#ifndef TRUE
	#define TRUE    (1)
#endif
/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/
typedef signed char BOOL; 				/*BOOL TYPE*/
typedef unsigned long long INT64U; 			/* 64bits*/
typedef signed long long INT64S;				/*signed 64bits*/

	
#include 	<stdint.h>
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>


#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "bsp_delay.h"

#endif

/********************************************************************************************************
**                            End Of File
********************************************************************************************************/
