#include <Ejercicio1.h>


static AppState_t estadoAPP = ESTADO_INICIAL;
static void inicializarApp(void);
static void enviarMensajeLCD(void);
static void terminarApp(void);


/*******************************************************************************
  Planificador de tareas
*******************************************************************************/
void APL_TaskHandler(void)
{
  switch (estadoAPP)
    {
    case ESTADO_INICIAL:
      inicializarApp();
      break;
    case ESTADO_MENSAJE_LCD:
      enviarMensajeLCD();
      break;
    case ESTADO_FINAL:
      terminarApp();		
    }
}


/*******************************************************************************
  Inicializa el uso del LCD
*******************************************************************************/
void inicializarApp()
{
  BSP_OpenLcd(); //Habilita el LCD
  estadoAPP = ESTADO_MENSAJE_LCD;
  SYS_PostTask(APL_TASK_ID);
}

/*******************************************************************************
  Envia un mensaje al Lcd
*******************************************************************************/
void enviarMensajeLCD()
{
  BSP_SendLcdMsg("Hola AVR-GCC");
  estadoAPP = ESTADO_FINAL;
}


/*******************************************************************************
  Bloquea el programa
*******************************************************************************/
void terminarApp()
{
  for(;;);
}
	
/*******************************************************************************
  Punto de entrada a la aplicacion
*******************************************************************************/
int main(void)
{
  SYS_SysInit();
  
  for(;;)
    {
      SYS_RunTask();
    }
}


