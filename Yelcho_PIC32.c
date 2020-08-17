#include <plib.h>			// Peripheral Library
#include "pines.h"                      //Definiciones de pines
#include "dee_emulation_pic32.h"        //Emulacion de EEPROM
#include "Definiciones.h"               //Definiciones generales

#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_20, FPLLODIV = DIV_1 //80MHz
#pragma config POSCMOD = XT, FNOSC = FRCPLL, FPBDIV = DIV_8
#pragma config FSOSCEN = OFF //sin oscilador secundario

#define SYS_FREQ         (80000000L)
#define PB_DIV         		8
#define PRESCALE       		128
#define TOGGLES_PER_SEC		1
#define T1_TICK       		(SYS_FREQ/PB_DIV/PRESCALE/TOGGLES_PER_SEC)

#define	GetSystemClock()              (80000000ul)
#define	GetPeripheralClock()          (GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()         (GetSystemClock())

#define CONFIG          (CN_ON | CN_IDLE_CON)
#define PINS            (CN6_ENABLE)
#define PULLUPS         (CN6_PULLUP_ENABLE)
#define INTERRUPT       (CHANGE_INT_ON | CHANGE_INT_PRI_2)

#define FOSC 80E6

//Prototipos de funciones utilizadas
void initvars(void);
void detecta_origenRESET();
void Procesa(short);
int main(void);


//Handlers para interrupcion

void __ISR(_UART1_VECTOR, IPL4) IntUart1Handler(void) //Recibe y procesa datos desde modem
{
    if (mU1RXGetIntFlag()) {
        MaqPrev = MaqStat;
        FlagModem=1;
        mU1RXClearIntFlag();
    }
}

void __ISR(_UART2_VECTOR, IPL4) IntUart2Handler(void) //Procesa requerimiento de servicio
{
    if (mU2RXGetIntFlag()) {
        MaqPrev = MaqStat;
        MaqStat = SM_Service;
        mU2RXClearIntFlag();
    }
}

void __ISR(_TIMER_1_VECTOR, ipl2) Timer1Handler(void) //Timer 1, 10uS temporizador tiempos cortos
{
    if (SelTimers & 0x08)
        delay10us++;
    mT1ClearIntFlag();
}

void __ISR(_TIMER_2_VECTOR, ipl2) Timer2Handler(void) //Timer 2, 1mS temporizador central
{
    general0++;
    if (general0 > 1000) //Cuenta 1 segundo
    {
        PORTToggleBits(STATUS);
        FlagAnalog=1;
        general0 = 0;
        general1++;
    }
    if (general1 > 86400) { //24 Horas 86400
        general1 = 0;
        if (MaqStat == SM_Available) {
            MaqStat = SM_Report;
            FlagReport = 0; //Si cambia estado de maquina no es necesario retener
        } else FlagReport = 1; //Debe esperar porque modem esta ocupado
    }

    if (SelTimers & 0x01) //Delay cooperativo
        delay++;
    if (SelTimers & 0x02) //Delay entrampado
        delay2++;
    if (SelTimers & 0x04) //Tiempo espera para USART
        timeout++;
    mT2ClearIntFlag();
}

void __ISR(_EXTERNAL_3_VECTOR, ipl4) INT3Handler(void) //Usuario solicitando servicio
{
    MaqStat = SM_User;
    CallRetry = 3;
    mINT3ClearIntFlag(); //Asegura bandera limpia y no reingreso
}

void __ISR(_EXTERNAL_4_VECTOR, ipl4) INT4Handler(void) //Alarma desde m�dulo
{
    mINT4ClearIntFlag(); //Asegura bandera limpia y no reingreso
}

void __ISR(_CHANGE_NOTICE_VECTOR, ipl2) ChangeNotice_Handler(void) //Alarma exterior
{
    ControlB = mPORTBRead() & 0x10;
    if (ControlB != 0 && MaqStat == SM_Available) {
        MaqStat = SM_Alarm;
        FlagAlarm = 0; //Si cambia estado de maquina, no es necesario retener
    } else FlagAlarm = 1; //Debe esperar porque modem esta ocupado (prioridad usuario)
    mCNClearIntFlag();
}

//Configuracion y ciclo principal

int main(void) {

    //Configuracion de perifericos
    mOSCSetPBDIV(OSC_PB_DIV_8); //Varia Oscilador de perif�ricos
    SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    //Configuracion Timers 1 y 2
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 11); //Configura Timer1 cada 10us
    OpenTimer2(T2_ON | T2_PS_1_256, 39); //Configura Timer2 cada 1ms
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2); //Configura interrupci�n de Timer2 con prioridad 2
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2); //Configura interrupci�n de Timer1 con prioridad 2
    mT1IntEnable(0); //INT Timer 1 deshabilitado
    mT2IntEnable(1); //INT Timer 2 habilitado

    //Configuracion UART 1 y 2
    OpenUART1(UART_EN, UART_RX_ENABLE | UART_TX_ENABLE | UART_BRGH_SIXTEEN, 64); //Configura UART1 9600
    OpenUART2(UART_EN, UART_RX_ENABLE | UART_TX_ENABLE | UART_BRGH_SIXTEEN, 10); //Configura UART2 57600

    INTEnable(INT_SOURCE_UART_RX(UART2), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART2), INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART2), INT_SUB_PRIORITY_LEVEL_2);

    INTEnable(INT_SOURCE_UART_RX(UART1), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART1), INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART1), INT_SUB_PRIORITY_LEVEL_2);

    //Configuracion interrupciones Externas 3 y 4
    ConfigINT3(FALLING_EDGE_INT | EXT_INT_ENABLE | EXT_INT_PRI_2); //Interrupci�n para usuario con prioridad 6
    ConfigINT4(FALLING_EDGE_INT | EXT_INT_ENABLE | EXT_INT_PRI_1); //Interrupci�n para alarma desde modulo GSM con prioridad 6

    mCNOpen(CONFIG, PINS, PULLUPS);
    ControlB = mPORTBRead();
    mCNClearIntFlag();
    ConfigIntCN(INTERRUPT);

    //Habilitacion de interrupciones multivector
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();

    //Control individual de interrupciones Inicial
    mU1RXIntEnable(0); //INT RX Usart 1 deshabilitado
    mU2RXIntEnable(1); //INT RX Usart 2 habilitado
    mINT3IntEnable(1); //INT EXT3 habilitada
    mINT4IntEnable(0); //INT EXT4 deshabilitada

    if (DataEEInit()) {
        Debug_WriteString("Falla en EEPROM!!\n\r");
    }
    Debug_WriteString("\n\r-Iniciando-\n\r");
    detecta_origenRESET();
    init_ADC();
    init_isd(); //Inicializa el chipcorder
    initvars();
    inicia_modem(); //Configura inicialmente el modem
    fin_call();
    Debug_WriteString("-Secuencia de inicio finalizada-\n\r");
    while (1) {
        if (FlagModem != 0) {
            Procesa(500); //Recibe en 500ms y procesa segun respuesta de modem
        }
        if (MaqStat == SM_CallError || MaqStat == SM_EndOfCall || MaqStat == SM_User) { //Proceso de usuario
            Usuario();
        }
        if (TemStat == SM_DONE) { //Se acabo el tiempo
            if (MaqStat == SM_InitiatingCall) { //De establecer llamada
                MaqStat = SM_User;
            }
            if (MaqStat == SM_WaitingEndOfCall) { //De conversacion con FEI
                MaqStat = SM_EndOfCall;
            }
            TemStat = SM_NOP;
        }
        if (MaqStat == SM_Service) Servicio();
        if ((MaqStat == SM_Available && (FlagAlarm != 0 || FlagReport != 0)) || MaqStat == SM_Alarm || MaqStat == SM_Report) ReporteAlarma();
        if (FlagAnalog !=0) {
            leeAnalogos();
            FlagAnalog=0;
        }
        delayCoop(0); //Actualiza temporizador
    }
}

void Procesa(short y) { //Procesa respuestas desde el modem y establece condicion operativa
    char RespMdm = 0;

    RespMdm = recibe(y); //Recibe comando desde modem
    if (MaqPrev == SM_InitiatingCall || MaqPrev == SM_WaitingEndOfCall) {
        switch (MaqPrev) {
            case (SM_InitiatingCall):
                if (RespMdm == 1) {
                    Debug_WriteString("-Llamada establecida-\n\r");
                    MaqStat = SM_WaitingEndOfCall; //Comuniacion establecida
                    TemStat = SM_SET; //Habilita control de tiempo espera
                    delayCoop(180000); //180 segundos -> 3min
                }
                if (RespMdm == 3 || RespMdm == 9) {
                    Debug_WriteString("-Error intentando llamada-\n\r");
                    MaqStat = SM_User; //Error estableciendo comunicacion => reintento
                    TemStat = SM_NOP;
                    delayCoop(0);
                }
                break;
            case (SM_WaitingEndOfCall):
                if (RespMdm == 3 || RespMdm == 9) {
                    Debug_WriteString("-Finalizando llamada-\n\r");
                    MaqStat = SM_EndOfCall; //Fin de comunicaciones
                    TemStat = SM_NOP;
                    delayCoop(0);
                }
                break;
            default:
                Debug_WriteString("-Condicion erronea-\n\r");
                MaqStat = MaqPrev;
                TemStat = SM_NOP;
                delayCoop(0);
                break;
        }
    } else {
        MaqStat = MaqPrev;
    }
    FlagModem=0; //Finaliza proceso a requerimiento de modem
}

void initvars(void) {
    /*No JTAG Pines RB10 y RB11 son I/O*/
    DDPCONbits.JTAGEN = 0;

    /*Configura pines de la unidad CPU & Modulo GSM*/
    PORTSetPinsDigitalOut(IOPORT_D, ACCESS_MODEM);
    PORTSetPinsDigitalOut(IOPORT_E, ON_MODEM);
    PORTSetPinsDigitalOut(IOPORT_E, RESET_MODEM);
    PORTSetPinsDigitalOut(IOPORT_E, AMP_ON);
    PORTSetPinsDigitalOut(STATUS);
    PORTSetPinsDigitalOut(IOPORT_D, STATUS2);
    PORTSetPinsDigitalOut(IOPORT_B, CHG);

    PORTSetPinsDigitalOut(IOPORT_G, ISD4003_SELECT);
    PORTSetPinsDigitalOut(IOPORT_G, ISD4003_CLK);
    PORTSetPinsDigitalIn(IOPORT_G, ISD4003_MISO);
    PORTSetPinsDigitalOut(IOPORT_G, ISD4003_MOSI);

    PORTSetPinsDigitalIn(IOPORT_D, USUARIO);
    PORTSetPinsDigitalIn(IOPORT_D, ALM1);
    PORTSetPinsDigitalIn(IOPORT_B, ALM);
    PORTSetPinsDigitalIn(IOPORT_B, PM_MODEM);

    PORTSetPinsAnalogIn(IOPORT_B, VBAT | CSOLAR);

    PORTClearBits(IOPORT_E, RESET_MODEM);
    PORTClearBits(IOPORT_E, ON_MODEM);
    PORTSetBits(IOPORT_E, AMP_ON);
    PORTSetBits(IOPORT_B, CHG);
    PORTClearBits(IOPORT_D, ACCESS_MODEM);
    PORTClearBits(STATUS);
    PORTClearBits(IOPORT_D, STATUS2);

    MaqStat = SM_Available; //No hay proceso pendiente
    MaqPrev = SM_Available; //No hubo proceso anterior
    TemStat = SM_NOP; //No hay temporizacion pendiente
    SelTimers = 0; //Ningun timer seleccionado
    delay = 0; //Valor inicializado
    delay2 = 0; //Valor inicializado
    delay10us = 0; //Inicializa control de 10*nuS
    Bandera = 0; //Inicializa caracteres de control
    ControlB = 0;
    general0 = 0; //Lampara de estado
    general1 = 0; //Control reporte
    PanelMax = 0; //Valor maximo de panel (V)

    CallRetry = 3;

    FlagAlarm = 0; //Registro alarma
    FlagReport = 0; //Registro reporte
    FlagModem=0; //No hay requerimiento desde modem
    FlagAnalog=0; //Evita calculo constante de variables

    //Recupera datos desde la EEprom
    delay = DataEEReadArray(numvoz, 15, 20);
    delay = DataEEReadArray(numsms, 40, 20);
    delay = DataEEReadArray(serie, 1, 10);
    delay = DataEEReadArray(tiempoSMS, 80, 5);
    //   delay = DataEEReadArray(dataWay, 65, 2);
    //   delay = DataEEReadArray(apnGPRS, 90, 30);
    //   delay = DataEEReadArray(userGPRS, 130, 30);
    //   delay = DataEEReadArray(passGPRS, 170, 30);
    //   delay = DataEEReadArray(ipGPRS, 210, 30);
    //   delay = DataEEReadArray(portGPRS, 250, 10);
}

void detecta_origenRESET() {
    short respuesta = 0;
    char contador = 0;
    char tempstring[80];
    int bit_pos[20] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};

    if (mGetPORFlag()) { //15
        mClearPORFlag();
        GenStat |= 0b1000000000000000;
    }
    if (mGetBORFlag()) { //14
        mClearBORFlag();
        GenStat |= 0b0100000000000000;
    }
    if (mGetMCLRFlag()) { //13
        mClearMCLRFlag();
        GenStat |= 0b0010000000000000;
    }
    if (mGetSWRFlag()) { //12
        mClearSWRFlag();
        GenStat |= 0b0001000000000000;
    }
    if (mGetWDTOFlag()) { //11
        mClearWDTOFlag();
        GenStat |= 0b0000100000000000;
    }
    if (mGetCMRFlag()) { //10
        mClearCMRFlag();
        GenStat |= 0b0000010000000000;
    }
    if (mGetSLEEPFlag()) { //9
        mClearSLEEPFlag();
        GenStat |= 0b0000001000000000;
    }
    if (mGetIDLEFlag()) { //8
        mClearIDLEFlag();
        GenStat |= 0b0000000100000000;
    }
    if (mGetVREGSFlag()) { //7
        mClearVREGSFlag();
        GenStat |= 0b0000000010000000;
    }

    GenStat &= 0b1111111110000000;
    if (GenStat != 0) {
        for (contador = 8; contador < 16; contador++) {
            respuesta = GenStat & bit_pos[contador];
            if (respuesta) {
                sprintf(tempstring, "\r\nReset en bit:%u\r\n", contador);
                Debug_WriteString(tempstring);
            }
        }
    }
}
