#include <p32xxxx.h>
#include <plib.h>
#include "pines.h"
#include "Definiciones.h"

char recibe(int);
char fin_call();
char off_modem();
char on_modem();
char config_modem();
char GM862_checkNetwork();
void GM862_switchModem();
void GM862_sms(char *, char *);

//Rutina recepcion datos desde modem

char recibe(int y) {
    //Cadenas que busca en respuesta de modem
    char rec_1[3] = {"OK"};
    char rec_8[3] = {"NO"};
    char rec_2[5] = {"BUSY"};
    char rec_3[6] = {"ERROR"};
    char rec_5[10] = {"NO ANSWER"};
    char rec_6[11] = {"NO CARRIER"};
    char rec_9[10] = {"CREG: 0,1"};
    char rec_11[5] = {"RING"};
    char rec_12[2] = {">"};

    //Variables utilizadas por la funci�n
    char rcvchar = 0, Modem_RX[100], monitor[50], respuesta = 0;
    short control = 0, puntero = 0, temp = 0;

    mU1RXIntEnable(0); //INT RX Usart 1 deshabilitado
    timeout = 0; //Borra control tiempo fuera
    SelTimers = SelTimers | 0x04; //Activa control temporizador
    PORTSetBits(IOPORT_D, STATUS2);
    while (timeout < y) {
        if (DataRdyUART1()) {
            rcvchar = ReadUART1();
            if (((rcvchar == 0x0A) || (rcvchar == 0x0D)) && (puntero < 100)) {
                Modem_RX[puntero++] = 0x20;
                control++;
            } else {
                if (puntero < 100) {
                    Modem_RX[puntero++] = rcvchar;
                }
            }
            if ((control == 4) && !Bandera)timeout = y; //Finaliza si llegan 4 caracteres de control
            if ((control == 8) && Bandera)timeout = y; //Finaliza si llegan 8 caracteres de control
        }
    }
    SelTimers = SelTimers & 0x03; //Desactiva control temporizador

    respuesta = 3;
    if (strstr(Modem_RX, rec_8)) respuesta = 3; //Error
    if (strstr(Modem_RX, rec_1)) respuesta = 1; //Ok
    if (strstr(Modem_RX, rec_2)) respuesta = 3; //Busy
    if (strstr(Modem_RX, rec_3)) respuesta = 3; //Error
    if (strstr(Modem_RX, rec_5)) respuesta = 3; //No Answer
    if (strstr(Modem_RX, rec_6)) respuesta = 9; //No Carrier
    if (strstr(Modem_RX, rec_9)) respuesta = 4; //Modem registrado
    if (strstr(Modem_RX, rec_11)) respuesta = 7; //Ring
    if (strstr(Modem_RX, rec_12)) respuesta = 8; //Listo para escribir mensaje

    for (temp = 0; temp < 100; temp++)Modem_RX[temp] = 0x00; //Limpia buffer de recepcion

    sprintf(monitor, "Ans=%d\n\r", respuesta); //Envia dato procesado a debug
    Debug_WriteString(monitor);
    void UART1ClearErrors(void); //Borra cualquier error que se haya producido
    mU1RXClearIntFlag(); //Asegura no haya disparo en falso
    mU1RXIntEnable(1); //Habilita interrupcion USART1
    PORTClearBits(IOPORT_D, STATUS2);
    return (respuesta);
}

//Bloque finalizacion llamada

char fin_call() {
    char estado;
    Bandera = 0;
    mU1RXIntEnable(0); //No hay interrupcion por datos en USART
    estado = recibe(100);
    GM862_WriteString("ATH\r");
    estado = recibe(1000);
    GM862_WriteString("ATH\r");
    estado = recibe(1000);
    off_modem();
    void UART1ClearErrors(void); //Borra cualquier error que se haya producido
    mU1RXClearIntFlag(); //Asegura no haya disparo en falso
    mU1RXIntEnable(1); //Habilita interrupcion USART1
    return (estado);
}

//Genera switch para encendido y apagado

void GM862_switchModem() {
    PORTSetBits(IOPORT_E, ON_MODEM);
    delayTrap(2500);
    PORTClearBits(IOPORT_E, ON_MODEM);
    delayTrap(2500);
}

//Enciende modem

char on_modem() {
    char intento = 5;

    PORTClearBits(IOPORT_E, ON_MODEM); //Asegura no condicion de error
    PORTClearBits(IOPORT_E, RESET_MODEM); //Asegura no reset enviado a Modem
    do {
        if (!PORTReadBits(IOPORT_B, PM_MODEM)) {
            GM862_switchModem();
        }
        intento = intento - 1;
    } while ((!PORTReadBits(IOPORT_B, PM_MODEM)) && (intento != 0));
    if (PORTReadBits(IOPORT_B, PM_MODEM)) {
        Debug_WriteString("-Modulo encendido-\n\r");
        intento = 1;
    } else {
        Debug_WriteString("-ERROR: Modulo aun apagado-\n\r");
        intento = 0;
    }
    mU1RXClearIntFlag(); //Asegura no haya disparo en falso
    mU1RXIntEnable(1); //Habilita interrupcion USART1
    return (intento);
}

//Apaga modem

char off_modem() {
    char intento = 5;
    char estado = 1;
    char tempstring[20];

    mU1RXIntEnable(0); //No hay interrupcion por datos en USART
    if (PORTReadBits(IOPORT_B, PM_MODEM)) { //Chequea linea Monitor
        GM862_WriteString("AT#SHDN\r");
        estado = recibe(1000);
        delayTrap(2000);
        //        do {
        //            if (PORTReadBits(IOPORT_B, PM_MODEM)) {
        //                GM862_switchModem();
        //                Debug_WriteString("-Hard Off-\n\r");
        //            }
        //            intento = intento - 1;
        //        } while ((PORTReadBits(IOPORT_B, PM_MODEM)) && (intento != 0));
    }
    if (estado != 1) {
        Debug_WriteString("-ERROR: Modulo aun encendido-\n\r");
        intento = 1;
    } else {
        Debug_WriteString("-Modulo apagado-\n\r");
        intento = 0;
    }
    void UART1ClearErrors(void); //Borra cualquier error que se haya producido
    mU1RXClearIntFlag(); //Asegura no haya disparo en falso
    mU1RXIntEnable(1); //Habilita interrupcion USART1
    return (intento);
}

//Chequea por registro en red

char GM862_checkNetwork() {
    char estado = 0;

    Bandera = 1; //Indica que se recepcionar�n 8 caracteres de control
    Debug_WriteString("-Verificando cobertura-\n\r");
    GM862_WriteString("AT+CREG=0\r");
    estado = recibe(2000);
    if (estado == 1) {
        GM862_WriteString("AT+CREG?\r");
        estado = recibe(2000);
        if (estado == 4) {
            Debug_WriteString("-Estoy registrado-\n\r");
        } else {
            Debug_WriteString("-No tengo se�al-\n\r");
        }
    } else Debug_WriteString("-Error de registro-\n\r");
    Bandera = 0; //Reestablece condicion 4 caracteres de control
    return (estado);
}

//Inicializa el modem para su uso (encendido inicial)

void inicia_modem() {
    short intento = 3, estado = 0;
    char tempstring1[50];

    Bandera = 0;
    mU1RXIntEnable(0); //Deshabilita interrupcion
    intento = on_modem();
    Debug_WriteString("-Configurando modem-\r\n");
    estado = recibe(100);
    sprintf(tempstring1, "ATE0\r"); //Sin eco de entrada
    GM862_WriteString(tempstring1);
    estado = recibe(1000);
    sprintf(tempstring1, "AT&K0\r"); //Sin control de flujo
    GM862_WriteString(tempstring1);
    estado = recibe(1000);
    sprintf(tempstring1, "AT#SELINT=2\r"); //Nivel de instrucciones 2
    GM862_WriteString(tempstring1);
    estado = recibe(1000);
    sprintf(tempstring1, "ATV1\r"); //Respuestas completas
    GM862_WriteString(tempstring1);
    estado = recibe(1000);
    sprintf(tempstring1, "AT#CAP=1\r"); //Selecciona Handsfree
    GM862_WriteString(tempstring1);
    estado = recibe(1000);
    sprintf(tempstring1, "AT#SHFNR=1\r"); //Activa reduccion de ruido
    GM862_WriteString(tempstring1);
    estado = recibe(1000);
    sprintf(tempstring1, "AT+CMGF=1\r"); //Activa modo texto en SMS
    GM862_WriteString(tempstring1);
    estado = recibe(1000);
    void UART1ClearErrors(void); //Borra cualquier error que se haya producido
    mU1RXClearIntFlag(); //Asegura no haya disparo en falso
    mU1RXIntEnable(1); //Habilita interrupcion USART1
}

void GM862_sms(char *numero, char *mensaje) {
    char tempstring1[100];
    short intento = 3, estado = 0;

    Bandera = 0;
    sprintf(tempstring1, "AT+CMGF=1\r"); //Activa modo texto en SMS
    GM862_WriteString(tempstring1);
    estado = recibe(1000);
    sprintf(tempstring1, "AT+CMGS=\"%s\"\r", numero); //Envia SMS sin almacenar en memoria a numero1
    GM862_WriteString(tempstring1); //Espera por ">"
    estado = recibe(1500);
    if (estado == 8) {
        GM862_WriteString(mensaje);
        putcUART1(0x1A); //Ctrl-Z
        Bandera = 1;
        estado=recibe(10000);
        Bandera = 0;
    }
}
