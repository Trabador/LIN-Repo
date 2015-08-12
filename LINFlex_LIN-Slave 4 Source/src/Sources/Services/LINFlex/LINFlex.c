/*==============================================================================*/
/*                        SV C CE SOFTWARE GROUP								*/
/*==============================================================================*/
/*                        OBJECT SPECIFICATION									*/
/*==============================================================================*/
/*	C Source:         %LINFlex.c%												*/
/*	Instance:         LIN_SLAVE													*/
/*	%version:         1 %														*/
/*	%created_by:      uid10734106 %												*/
/*	%date_created:    Wed Aug  12 12:08:00 2015 %								*/
/*==============================================================================*/
/* DESCRIPTION : LINFlex Function for initialization of the module.				*/
/*==============================================================================*/
/* FUNCTION COMMENT :	This file have the initialization for the module		*/
/*						LINFlex by hardware in the MPC5606BK development		*/
/*						Board													*/
/*																				*/
/*==============================================================================*/
/*                               OBJECT HISTORY									*/
/*==============================================================================*/
/* REVISION	|	DATE	|								|      AUTHOR			*/
/*------------------------------------------------------------------------------*/
/*  1.0		|12/08/2015	|								|	Roberto Palos		*/
/*	Creation of the file.														*/
/*	Function from main file are now here										*/
/*==============================================================================*/
#include "MPC5606B.h"


void LIN_LINFlex0Init(void){
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
	/*LED ON, LED OFF, LED TOGGLE, NO Response*/
	LINFLEX_0.IFCR[0].B.DFL = 1;	/* Data Field Length*/
	LINFLEX_0.IFCR[0].B.DIR = 0;	/*Direction: Receives Data and copies in BDRL and BDRM registers*/
	LINFLEX_0.IFCR[0].B.CCS = 1;	/*Classic Checksum On (LIN 1.3 and earlie)*/
	LINFLEX_0.IFCR[0].B.ID = 0x0F;	/*Identifier without identifier parity. 0xCF*/
	/*IDENTIFIER 2*/
	/*LED ON, LED OFF, NO Response*/
	LINFLEX_0.IFCR[1].B.DFL = 0;	/* Data Field Length*/
	LINFLEX_0.IFCR[1].B.DIR = 0;	/*Direction: Receives Data and copies in BDRL and BDRM registers*/
	LINFLEX_0.IFCR[1].B.CCS = 1;	/*Classic Checksum On (LIN 1.3 and earlie)*/
	LINFLEX_0.IFCR[1].B.ID = 0x13;	/*Identifier without identifier parity. 0xD3*/
	/*IDENTIFIER 3*/
	/*LED Status And Slave Status*/
	LINFLEX_0.IFCR[2].B.DFL = 2;	/* Data Field Length*/
	LINFLEX_0.IFCR[2].B.DIR = 1;	/*Direction: Receives Data and copies in BDRL and BDRM registers*/
	LINFLEX_0.IFCR[2].B.CCS = 1;	/*Classic Checksum On (LIN 1.3 and earlie)*/
	LINFLEX_0.IFCR[2].B.ID = 0x23;	/*Identifier without identifier parity. 0xA3*/
	/*IDENTIFIER 4*/
	/*Response Team 8, AGCJRP*/
	LINFLEX_0.IFCR[3].B.DFL = 7;	/* Data Field Length*/
	LINFLEX_0.IFCR[3].B.DIR = 1;	/*Direction: Receives Data and copies in BDRL and BDRM registers*/
	LINFLEX_0.IFCR[3].B.CCS = 1;	/*Classic Checksum On (LIN 1.3 and earlie)*/
	LINFLEX_0.IFCR[3].B.ID = 0x33;	/*Identifier without identifier parity. 0x73*/
	
	
	
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
	SIU.PCR[18].R = 0x0604;         /* MPC56xxB: Configure port B2 as LIN0TX 0x0400*/
	SIU.PCR[19].R = 0x0103;         /* MPC56xxB: Configure port B3 as LIN0RX */
	
}