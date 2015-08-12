/*============================================================================*/
/*                        SV C CE SOFTWARE GROUP                              */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*
* C Source:         %LED.c%
* Instance:         1
* %version:         1 %
* %created_by:      Alexis Garcia%
* %date_created:    07/08/2015%
*=============================================================================*/
/* DESCRIPTION : C source that contains the driver control for led indicator  */
/*============================================================================*/
/* FUNCTION COMMENT : This file inlcudes the driver for the led indictor in the 
						LIN network 										  */
/*                                                							  */
/*                                                                            */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*  REVISION |   DATE      |                               |      AUTHOR      */
/*----------------------------------------------------------------------------*/
/*  1.0      | 07/08/2015  |                               | Alexis Garcia    */
/* Creation of the module		                                              */
/*============================================================================*/

/* Includes */
/* -------- */
#include "LED.h"
#include "GPIO.h"
/* Functions macros, constants, types and datas         */
/* ---------------------------------------------------- */
/* Functions macros */

/*==================================================*/ 
/* Definition of constants                          */
/*==================================================*/ 
/* BYTE constants */

/* WORD constants */

/* LONG and STRUCTURE constants */

/*======================================================*/ 
/* Definition of RAM variables                          */
/*======================================================*/ 
/* BYTE RAM variables */
E_LedStateList re_LedSMState;		/*contains the actual state for the LED state machine*/
/* WORD RAM variables */

/* LONG and STRUCTURE RAM variables */

/*======================================================*/ 
/* close variable declaration sections                  */
/*======================================================*/ 

/* Private defines */

void LED_LedOn(void)
{
	LED_ON;
}

void LED_LedOff(void)
{
	LED_OFF;
}

void LED_LedToggle(void)
{
	LED_TOGGLE;
}

/* Private functions prototypes */
/* ---------------------------- */

/* Exported functions prototypes */
/* ----------------------------- */

/* Exported functions */
/* ------------------ */

void LED_StateMachine(void)
{
	switch(re_LedSMState)
	{
		case ON:
			LED_LedOn();
		break;
		
		case OFF:
			LED_LedOff();
		break;
		
		case TOGGLE:
			LED_LedToggle();
		break;
		
		default:
			re_LedSMState = ON;
		break;
	}
}