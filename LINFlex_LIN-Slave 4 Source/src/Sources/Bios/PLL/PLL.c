/*==============================================================================*/
/*                        SV C CE SOFTWARE GROUP								*/
/*==============================================================================*/
/*                        OBJECT SPECIFICATION									*/
/*==============================================================================*/
/*	C Source:         %PLL.c%													*/
/*	Instance:         LIN_SLAVE													*/
/*	%version:         1 %														*/
/*	%created_by:      uid10734106 %												*/
/*	%date_created:    Wed Aug  12 12:19:00 2015 %								*/
/*==============================================================================*/
/* DESCRIPTION : PLL functions for LINFLEX_0									*/
/*==============================================================================*/
/* FUNCTION COMMENT :	This file include de correct configuration for the		*/
/*						initialization of the LINFLEX_0 module.					*/
/*																				*/
/*==============================================================================*/
/*                               OBJECT HISTORY									*/
/*==============================================================================*/
/* REVISION	|	DATE	|								|      AUTHOR			*/
/*------------------------------------------------------------------------------*/
/*  1.0		|12/08/2015	|								|	Roberto Palos		*/
/*	Creation of the file.														*/
/*	Function from main file are now here.										*/
/*==============================================================================*/
#include "PLL.h"
#include "MPC5606B.h"

void initModesAndClks(void){
  ME.MER.R = 0x0000001D;          /* Enable DRUN, RUN0, SAFE, RESET modes */
                                  /* Initialize PLL before turning it on: */
/* Use 1 of the next 2 lines depending on crystal frequency: */
  CGM.FMPLL_CR.R = 0x02400100;    /* 8 MHz xtal: Set PLL0 to 64 MHz */   
/*CGM.FMPLL_CR.R = 0x12400100;*/  /* 40 MHz xtal: Set PLL0 to 64 MHz */   
  ME.RUN[0].R = 0x001F0074;       /* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL */
  ME.RUNPC[1].R = 0x00000010; 	  /* Peri. Cfg. 1 settings: only run in RUN0 mode */
  ME.PCTL[48].R = 0x01;           /* MPC56xxB/P/S LINFlex 0: select ME.RUNPC[1] */	
  ME.PCTL[68].R = 0x01;           /* MPC56xxB/S SIUL:  select ME.RUNPC[0] */	
                                  /* Mode Transition to enter RUN0 mode: */
  ME.MCTL.R = 0x40005AF0;         /* Enter RUN0 Mode & Key */
  ME.MCTL.R = 0x4000A50F;         /* Enter RUN0 Mode & Inverted Key */  
  while (ME.GS.B.S_MTRANS) {}     /* Wait for mode transition to complete */    
                                  /* Note: could wait here using timer and/or I_TC IRQ */
  while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode */
}

void initPeriClkGen(void){
  CGM.SC_DC[0].R = 0x80;           /* MPC56xxB/S: Enable peri set 1 sysclk divided by 1 */
}

void disableWatchdog(void){
  SWT.SR.R = 0x0000c520;     /* Write keys to clear soft lock bit */
  SWT.SR.R = 0x0000d928; 
  SWT.CR.R = 0x8000010A;     /* Clear watchdog enable (WEN) */
}