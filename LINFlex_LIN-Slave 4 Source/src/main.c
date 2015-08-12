/*============================================================================*/
/*                        SV C CE SOFTWARE GROUP                              */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*
* C Source:         %main.c%
* Instance:         LIN_SLAVE
* %version:         1 %
* %created_by:      uid10734106 %
* %date_created:    Tue Aug  11 10:18:00 2015 %
*=============================================================================*/
/* DESCRIPTION : Main File For LIN Slave                                      */
/*============================================================================*/
/* FUNCTION COMMENT :	This file is a debug purpose to test the functionality
						of the LINFLEX hardware for MPC5606BK development
						Board                                                 */
/*                                                                            */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*  REVISION |   DATE      |                               |      AUTHOR      */
/*----------------------------------------------------------------------------*/
/*  1.0      | 11/08/2015  |                               | Roberto Palos    */
/*	Creation of the file.
	Added LINFLEX slave module configuration and tested with osciloscope.
	LINFLEX configuration by bitfields for a beter understanding of module
	Check of revisionframe and LIN Status Register to verify the state of the
	module.																      */
/*----------------------------------------------------------------------------*/
/*  1.1      | 12/08/2015  |                                | Roberto Palos   */
/*	Modification of receiveLINFrame											  */
/*	Changed correct Bit-Field for LINFLEX_0.LINIER.B.DBEIE					  */
/*	Added void MasterDataReceive Function									  */
/*============================================================================*/

#include "MPC5606B.h"


#define LED_ON(channel)		(SIU.GPDO[channel].B.PDO =  0)
#define LED_OFF(channel)	(SIU.GPDO[channel].B.PDO =  1)
#define LED_TOGGLE(channel)	(SIU.GPDO[channel].B.PDO ^= 1) 


#define LED1	68
#define LED2	69
#define LED3	70
#define LED4	71

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

void LINFlex_Init(void){

	/*LINFlex by default is in SLEEP mode*/
	/*LIN Control Register 1 Configuration*/
	LINFLEX_0.LINCR1.B.SLEEP = 0;		/* Disable SLEEP Mode*/
	LINFLEX_0.LINCR1.B.INIT = 1;		/* Put LINFlex hardware in init mode */
	/* Wait for the INIT mode */
	/* Example Code For Wait To INIT mode on
	while (0x1000 != (LINFLEX_0.LINSR.R & 0xF000)) {}*/
	while(LINFLEX_0.LINSR.B.LINS != 1){}/*Wait for INIT Mode whit LINSR Register LINS field*/
	LINFLEX_0.LINCR1.B.CCD = 0;			/* Checksum calculation is done by hardware */
	LINFLEX_0.LINCR1.B.CFD = 0;			/* Checksum field is sent after the bytes */
	LINFLEX_0.LINCR1.B.LASE = 1;		/* LIN-Slave auto resynch */
	LINFLEX_0.LINCR1.B.AWUM = 1;		/* Auto Wake Up When LINRX dominant state detection by hardware*/
	LINFLEX_0.LINCR1.B.MBL = 3;			/* Master Break Length 13-bits*/
	LINFLEX_0.LINCR1.B.BF = 0;			/* No interrupt if ID doesnt match any filter */
	LINFLEX_0.LINCR1.B.SFTM = 0;		/* Disable selft test mode */
	LINFLEX_0.LINCR1.B.LBKM = 0;		/* Loop back mode disable*/
	LINFLEX_0.LINCR1.B.MME = 0;			/* Slave mode enable*/
	LINFLEX_0.LINCR1.B.SBDT = 0;		/* Slave mode break detection 11-bit */
	LINFLEX_0.LINCR1.B.RBLM = 0;		/* Receive Buffer not locked */
	LINFLEX_0.LINCR1.B.SLEEP = 0;		/* Disable SLEEP mode bit */
	
	/*	Fractional Baud Rate Generation
		data taken from table 23-1 of document:
		"MPC5606BK Microcontroller Reference Manual"*/
	/*Baud Rate Set to 9600*/
	LINFLEX_0.LINIBRR.B.DIV_M= 416;		/* Mantissa baud rate divider component */
	LINFLEX_0.LINFBRR.B.DIV_F = 11;		/* Fraction baud rate divider comonent */
	
	/* Identifier Filter Enable Register*/
	LINFLEX_0.IFER.B.FACT = 3;	/* Filters 0 and 1 activated*/
								/* Filters 2 and 3 activated*/
								
	/* Identifier Filter Mode Register*/
	LINFLEX_0.IFMR.B.IFM = 0;	/*Filters 0 and 1 are in identifier list mode.*/
								/*Filters 2 and 3 are in identifier list mode*/
	
	

	/*IDENTIFIER 1*/
	
	LINFLEX_0.IFCR[0].B.DFL = 2;	/* Data Field Length*/
	LINFLEX_0.IFCR[0].B.DIR = 0;	/*Direction: Receives Data and copies in BDRL and BDRM registers*/
	LINFLEX_0.IFCR[0].B.CCS = 1;	/*Classic Checksum On (LIN 1.3 and earlie)*/
	LINFLEX_0.IFCR[0].B.ID = 0x0F;	/*Identifier without identifier parity. 0xCF*/
	
	
	/* LIN control register 2*/
	LINFLEX_0.LINCR2.B.IOBE = 0;	/* Idle on bit error off*/
	LINFLEX_0.LINCR2.B.IOPE = 0;	/* Idle on ID parity error off*/
	
	
	/* LIN Interrupt enable register*/
	LINFLEX_0.LINIER.B.SZIE = 0;		/* No interrupt when Stuck At Zero*/
	LINFLEX_0.LINIER.B.OCIE = 0;		/* No interrupt when Output Compare*/
	LINFLEX_0.LINIER.B.BEIE = 0;		/* No interrupt when Bit Error*/
	LINFLEX_0.LINIER.B.CEIE = 0;		/* No interrupt when Checksum Error*/
	LINFLEX_0.LINIER.B.HEIE = 0;		/* No interrupt when Header Error*/
	LINFLEX_0.LINIER.B.FEIE = 0;		/* No interrupt when Framing Error*/
	LINFLEX_0.LINIER.B.BOIE = 0;		/* No interrupt when Buffer Overrun*/
	LINFLEX_0.LINIER.B.LSIE = 0;		/* No interrupt when LIN State Change*/
	LINFLEX_0.LINIER.B.WUIE = 0;		/* No interrupt when Wake Up*/
	LINFLEX_0.LINIER.B.DBFIE = 0;		/* No interrupt when Data Buffer Full*/
	LINFLEX_0.LINIER.B.DBEIE = 0;		/* No interrupt when Data buffer Empty / Time Out */
	LINFLEX_0.LINIER.B.DRIE = 1;		/* Interrupt when Data Reception Completed*/
	LINFLEX_0.LINIER.B.DTIE = 1;		/* Interrupt when Data Transmitted Completed*/
	LINFLEX_0.LINIER.B.HRIE = 1;		/* Interrupt when LIN Header Received is valid*/
	
	/* Enter in normal mode*/
	LINFLEX_0.LINCR1.B.INIT = 0;

	/*Port Configuration For LINFLEX_0 */
	SIU.PCR[18].R = 0x0400;         /* MPC56xxB: Configure port B2 as LIN0TX */
	SIU.PCR[19].R = 0x0103;         /* MPC56xxB: Configure port B3 as LIN0RX */
	
	
	/* Data Debug Purpouse Led On*/
	
	SIU.GPDO[LED1].R = 0;
	SIU.PCR[LED1].R = 0x0200;
	SIU.GPDO[LED2].R = 0;
	SIU.PCR[LED2].R = 0x0200;
	SIU.GPDO[LED3].R = 0;
	SIU.PCR[LED3].R = 0x0200;
	SIU.GPDO[LED4].R = 0;
	SIU.PCR[LED4].R = 0x0200;
		
  
}

/* This functions is only for master*/
void MasterDataReceive(uint16_t bidr_value){
	static vuint32_t rx_data[8];
	LINFLEX_0.BIDR.R = bidr_value;
	LINFLEX_0.LINCR2.B.HTRQ = 1;
	while(0 == LINFLEX_0.LINSR.B.DRF){
		
	}
	rx_data[0] = LINFLEX_0.BDRL.B.DATA0;
	rx_data[1] = LINFLEX_0.BDRL.B.DATA1;
	rx_data[2] = LINFLEX_0.BDRL.B.DATA2;
	rx_data[3] = LINFLEX_0.BDRL.B.DATA3;
	rx_data[4] = LINFLEX_0.BDRM.B.DATA4;
	rx_data[5] = LINFLEX_0.BDRM.B.DATA5;
	rx_data[6] = LINFLEX_0.BDRM.B.DATA6;
	rx_data[7] = LINFLEX_0.BDRM.B.DATA7;
	
	LINFLEX_0.LINSR.R = 0x207;
}

void receiveLINFrame(void){
	static vuint32_t  rx_datas[8];
	uint8_t cmd_Recive;
	static vuint32_t lin_status = 0;
	lin_status = LINFLEX_0.LINSR.R;
	/* wait for RMB */
	while (1 != LINFLEX_0.LINSR.B.RMB) {}  /* Wait for Release Message Buffer */
	/* get the data */
    rx_datas[0] = LINFLEX_0.BDRL.B.DATA0;	// se recive un comando que se guarda en una variale para despues ser ejecutado------------
    cmd_Recive =(uint8_t)rx_datas[0];
    //rx_datas[1] = LINFLEX_0.BDRL.B.DATA1;
    //rx_datas[2] = LINFLEX_0.BDRL.B.DATA2;
    //rx_datas[3] = LINFLEX_0.BDRL.B.DATA3;
    //rx_datas[4] = LINFLEX_0.BDRM.B.DATA4;
    //rx_datas[5] = LINFLEX_0.BDRM.B.DATA5;
    //rx_datas[6] = LINFLEX_0.BDRM.B.DATA6;
    //rx_datas[7] = LINFLEX_0.BDRM.B.DATA7;
	/* clear the DRF and RMB flags by writing 1 to them */
	LINFLEX_0.LINSR.R = 0x0205;
}


void main(void) {

  uint16_t IdleCtr = 0;
  initModesAndClks();	/* Initialize mode entries */
  initPeriClkGen();		/* Initialize peripheral clock generation for LINFlex */
  disableWatchdog();	/* Disable watchdog */
  LINFlex_Init;			/* Initialize LINFlex_0 as slave */
  while (1) {
  	receiveLINFrame();     /* Transmit one frame from master */
  	if(LINFLEX_0.LINSR.B.LINS == 3){
  		LED_ON(LED1);
  	}
  	if(LINFLEX_0.LINSR.B.LINS == 4){
  		LED_ON(LED2);
  	}
  	if(LINFLEX_0.LINSR.B.LINS == 5){
  		LED_ON(LED3);
  	}
  	if(LINFLEX_0.LINSR.B.LINS == 6){
  		LED_ON(LED4);
  	}
  	
  	if(LINFLEX_0.LINSR.B.LINS == 2){
  		LED_OFF(LED1);
  		LED_OFF(LED2);
  		LED_OFF(LED3);
  		LED_OFF(LED4);
  		
  	}
  }
}

