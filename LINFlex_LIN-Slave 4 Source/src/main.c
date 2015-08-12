/*==============================================================================*/
/*                        SV C CE SOFTWARE GROUP								*/
/*==============================================================================*/
/*                        OBJECT SPECIFICATION									*/
/*==============================================================================*/
/*	C Source:         %main.c%													*/
/*	Instance:         LIN_SLAVE													*/
/*	%version:         1 %														*/
/*	%created_by:      uid10734106 %												*/
/*	%date_created:    Tue Aug  11 10:18:00 2015 %								*/
/*==============================================================================*/
/* DESCRIPTION : Main File For LIN Slave										*/
/*==============================================================================*/
/* FUNCTION COMMENT :	This file is a debug purpose to test the functionality	*/
/*						of the LINFLEX hardware for MPC5606BK development		*/
/*						Board													*/
/*																				*/
/*==============================================================================*/
/*                               OBJECT HISTORY									*/
/*==============================================================================*/
/* REVISION	|	DATE	|								|      AUTHOR			*/
/*------------------------------------------------------------------------------*/
/*  1.0		|11/08/2015	|								|	Roberto Palos		*/
/*	Creation of the file.														*/
/*	Added LINFLEX slave module configuration and tested with osciloscope.		*/
/*	LINFLEX configuration by bitfields for a beter understanding of module		*/
/*	Check of revisionframe and LIN Status Register to verify the state of the	*/
/*	module.																      	*/
/*------------------------------------------------------------------------------*/
/*  1.1		|12/08/2015	|								|	Roberto Palos		*/
/*	Modification of receiveLINFrame											  	*/
/*	Changed correct Bit-Field for LINFLEX_0.LINIER.B.DBEIE					  	*/
/*	Added void MasterDataReceive Function									  	*/
/*------------------------------------------------------------------------------*/
/*	1.2		|12/08/2015	|								|	Roberto Palos		*/
/*	Added functionality of the slave module response to message.				*/
/*------------------------------------------------------------------------------*/
/*	1.3		|12/08/2015	|								|	Alexis Garcia		*/
/*															Roberto Palos		*/
/*	Removed unnecesary functions.												*/
/*	Naming convention to functions.												*/
/*	Creation of tree folder structure in project.								*/
/*	Added LED.c and LED.h files.												*/
/*	Added template to explain functions, variables and headers.					*/
/*==============================================================================*/

#include "MPC5606B.h"
#include "LINFlex.h"
#include "GPIO.h"
#include "LED.h"
#include "MemAlloc_Cfg.h"
#include "SchM_Cfg.h"


void main(void) {
	initModesAndClks();	/* Initialize mode entries */
	initPeriClkGen();		/* Initialize peripheral clock generation for LINFlex */
	disableWatchdog();	/* Disable watchdog */
	MemAllocInit(&MemAllocConfig);
	LIN_LINFlex0Init();			/* Initialize LINFlex_0 as slave */
	LED_INIT
	SchM_Init(&SchedulerConfig);
	SchM_Start();
  
  for(;;){
  	/*Infinite loop*/
	while (LINFLEX_0.LINSR.B.LINS == HEADER_RECEPTION){
		switch(LINFLEX_0.BIDR.B.ID){
		/*CMD_SLAVE 4*/
		/*LED ON, LED OFF, LED TOGGLE, NO Response*/
		case 0x13:
			if(LINFLEX_0.BDRL.B.DATA0 == CMD_OFF){
				LED_OFF;
			}
			else if(LINFLEX_0.BDRL.B.DATA0 == CMD_ON){
				LED_ON;
			}
			else if(LINFLEX_0.BDRL.B.DATA0 == CMD_TOGGLE){
				LED_TOGGLE;
			}
			else{
				/* Do Nothing*/
			}
			break;
		/*MASTER_CMD_ALL*/
		case 0xCF:
			LINFLEX_0.BIDR.B.ID = 0xff;
			LINFLEX_0.BDRL.B.DATA0 = 0x55;
			LINFLEX_0.LINCR2.B.DTRQ = 1;    /* Request header transmission */
			break;
		/*SLAVE4_RSP	Active Or Inactive*/
		case 0x23:
			LINFLEX_0.BIDR.B.ID = 0xff;
			LINFLEX_0.BDRL.B.DATA0 = 0x55;
			LINFLEX_0.LINCR2.B.DTRQ = 1;    /* Request header transmission */
			break;
		/**/
		case 0x33:
			LINFLEX_0.BIDR.B.ID = 0xff;
			LINFLEX_0.BDRL.B.DATA0 = 0x55;
			LINFLEX_0.LINCR2.B.DTRQ = 1;    /* Request header transmission */
			break;
		
		default:
			break;
		
		}

	//	if(LINFLEX_0.BIDR.B.ID == 0x13)
	}
  }
}