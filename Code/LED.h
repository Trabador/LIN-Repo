/*============================================================================*/
/*                        SV C BC SOFTWARE GROUP                              */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*
* C Include:        %LED.c%
* Instance:         1
* %version:         1 %
* %created_by:      Alexis Garcia%
* %date_created:    07/08/15 %
*=============================================================================*/
/* DESCRIPTION : Header for the led indicator driver                          */
/*============================================================================*/
/* FUNCTION COMMENT : contains definition and externs variables for the led indicator driver*/
/*                                                                            */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*  REVISION |   DATE      |                               |      AUTHOR      */
/*----------------------------------------------------------------------------*/
/*  1.0      | 07/08/15    |                | Alexis Garcia   				  */
/* Creation of the header                                             		  */
/*============================================================================*/

#ifndef _LED_H
#define _LED_H
/* Includes */
/* -------- */
#include "typedefs.h"
#include "GPIO.h"

/* Exported types and constants */
/* ---------------------------- */

/* Types definition */

#define 	LED			68
#define 	LED_OFF 	(SIU.GPDO[LED].R = 1)
#define 	LED_ON 		(SIU.GPDO[LED].R = 0) 
#define 	LED_TOGGLE 	(SIU.GPDO[LED].R ^= 0x01)
#define 	STARTUP		(SIU.GPDO[LED].R = 0) 
#define 	LED_INIT()	{SIU.PCR[LED].R = 0x0200,SIU.GPDO[LED].R = 1}

/* typedef */

/*==================================================*/ 
/* Declaration of exported constants                */
/*==================================================*/ 
/* BYTE constants */


/* WORD constants */


/* LONG and STRUCTURE constants */
typedef enum
{
	ON = 0,
	OFF,
	TOGGLE
}E_LedStateList;

extern E_LedStateList re_LedSMState;
/*======================================================*/ 
/* Definition of RAM variables                          */
/*======================================================*/ 
/* BYTES */


/* WORDS */


/* LONGS and STRUCTURES */


/*======================================================*/ 
/* close variable declaration sections                  */
/*======================================================*/ 

/* Exported functions prototypes and macros */
/* ---------------------------------------- */

/* Functions prototypes */


/* Functions macros */


/* Exported defines */


#endif