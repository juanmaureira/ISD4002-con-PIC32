#include "Definiciones.h"
#include <plib.h>
#include "cp0defs.h"



void delayCoop(int);
void delayTrap(short);
void delay10usTrap(short);

void delayCoop(int VarCoop) {

    switch (TemStat) {
        case(SM_SET):
            delay = 0;
            intVarCoop = VarCoop;
            TemStat = SM_WAIT;
            ClearWDT();
            SelTimers = SelTimers | 0x01; //Activa control temporizador
            break;
        case(SM_WAIT):
            if (delay > intVarCoop) {
                ClearWDT();
                TemStat = SM_DONE;
            }
            break;
        case(SM_DONE):
            delay = 0;
            ClearWDT();
            SelTimers = SelTimers & 0x20; //Reestablece control temporizador
            break;
        case(SM_NOP):
            delay = 0;
            ClearWDT();
            SelTimers = SelTimers & 0x20; //Reestablece control temporizador
            break;

        default:
            Debug_WriteString("-Error en loop Coop-\n\r");
            delay = 0;
            SelTimers = SelTimers & 0x20; //Reestablece control temporizador
            break;
    }
}

void delayTrap(short VarNoCoop) {
    delay2 = 0; //Carga valor de espera
    SelTimers = SelTimers | 0x02; //Activa control temporizador
    while (VarNoCoop > delay2); //Espera por termino de tiempo
    SelTimers = SelTimers & 0x01; //Reestablece control temporizador
    delay2 = 0; //Reinicia valor de espera
}

void delay10usTrap(short usec) {
    SelTimers = SelTimers | 0x08;
    mT1ClearIntFlag(); //Asegura no salta en falso
    mT1IntEnable(1); //Habilita interrupcion para Timer1
    delay10us = 0; //Asegura estado inicial
    while (delay10us < usec); //Genera retardo
    delay10us = 0; //Inicializa las variables
    SelTimers = SelTimers & 0x07; //Restaura condicion general
    mT1IntEnable(0); //Inhabilita interrupcion Timer1
}

void delay_cycles(int retardo) {
    int i;
    for (i = 0; i < retardo; i++)
        asm("nop");
}

void delay_us(int retardo) {
    int start, end;
    start = _CP0_GET_COUNT();
    end = start + FOSC / 1000000 / 2 * retardo;
    if (end > start)
        while (_CP0_GET_COUNT() < end);

    else
        while (_CP0_GET_COUNT() > start || _CP0_GET_COUNT() < end);
}

void delay_ms(int retardo) {
    unsigned int int_status;
    while (retardo--) {
        int_status = INTDisableInterrupts();
        OpenCoreTimer(FOSC / 2000);
        INTRestoreInterrupts(int_status);
        mCTClearIntFlag();
        while (!mCTGetIntFlag());
    }
    mCTClearIntFlag();
}