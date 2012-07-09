/**************************************************************************//**
  \file Ejercicio1.h
  
  \brief Archivo .h de la aplicacion

  \author
    Laboratorio Nacional de Informatica Avanzada

  
  \internal
    History:     
******************************************************************************/

#ifndef _EJERCICIO1_H
#define _EJERCICIO1_H

/******************************************************************************
                    Seccion Includes
******************************************************************************/

#ifdef _SLIDERS_
#include "sliders.h"
#endif //#ifdef _SLIDERS_

#ifdef _BUTTONS_
#include "buttons.h"
#endif //#ifdef _BUTTONS_

#ifdef _LEDS_
#include "leds.h"
#endif //#ifdef _LEDS_

#ifdef _LCD_
#include "lcd.h"
#endif //#ifdef _LCD_


/******************************************************************************
                    Definiciones generales
******************************************************************************/

/* Definicion de estados */
typedef enum
{
	ESTADO_INICIAL,
	ESTADO_MENSAJE_LCD,
	ESTADO_FINAL
}AppState_t;


/*******************************************************************************
  Description: just a stub.
  Parameters: are not used.
  Returns: nothing.
*******************************************************************************/

void ZDO_MgmtNwkUpdateNotf(ZDO_MgmtNwkUpdateNotf_t *nwkParams) 
{
  nwkParams = nwkParams;  // Unused parameter warning prevention
}

/*******************************************************************************
  Description: just a stub.
  Parameters: none.
  Returns: nothing.
*******************************************************************************/

void ZDO_WakeUpInd(void) 
{

}

#ifdef _BINDING_

/***********************************************************************************
  Stub for ZDO Binding Indication
  Parameters:
    bindInd - indication
  Return:
    none
 ***********************************************************************************/

void ZDO_BindIndication(ZDO_BindInd_t *bindInd) 
{
  (void)bindInd;
}

/***********************************************************************************

  Stub for ZDO Unbinding Indication
  Parameters:
    unbindInd - indication
  Return:
    none
 ***********************************************************************************/

void ZDO_UnbindIndication(ZDO_UnbindInd_t *unbindInd)
{
  (void)unbindInd;
}

#endif //_BINDING_


#endif 

