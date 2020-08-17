#include <p32xxxx.h>
#include <plib.h>
#include "pines.h"
#include "dee_emulation_pic32.h"
#include "Definiciones.h"

//Servicio a requerimiento de interrupciones
void Usuario() {
    char intento = 4; //Numero de intentos para registro modem
    char tempstring[100]; //String para comandos a modem
    char dato = 0; //Variable para recepcion datos

    //Informacion para depuracion
    Debug_WriteString("-Leyendo EEPROM-\n\r");
    delay = DataEEReadArray(numvoz, 15, 20);
    delay = DataEEReadArray(numsms, 40, 20);
    delay = DataEEReadArray(serie, 1, 10);

    //Error de comunicaciones
    if (MaqStat == SM_CallError || MaqStat == SM_EndOfCall) {
        intento = fin_call();
        if (MaqStat == SM_CallError)cc_mensaje(1); //Mensaje error de comunicaciones
        if (MaqStat == SM_EndOfCall)cc_mensaje(2); //Mensaje fin de comunicaciones
        TemStat = SM_NOP;
        MaqStat = SM_Available;
        Debug_WriteString("-Llamada finalizada-\n\r");
        mINT3ClearIntFlag(); //Asegura bandera limpia
        //ConfigINT3(FALLING_EDGE_INT | EXT_INT_ENABLE | EXT_INT_PRI_4);
        mINT3IntEnable(1);
        //INTEnableSystemMultiVectoredInt();
    }

    //Iniciando comunicaciones
    if (MaqStat == SM_User) {
        mINT3IntEnable(0);
        cc_mensaje(0); //Mensaje comunicacion establecida
        inicia_modem();
        Debug_WriteString("-Iniciando llamada-\n\r");
        do {
            cc_mensaje(0); //Mensaje comunicacion establecida
            dato = GM862_checkNetwork();
            intento--;
        } while (intento > 0 && dato != 4);
        if (CallRetry != 0) {
            CallRetry--;
            GM862_WriteString("ATD");
            sprintf(tempstring, "%s;\r", numvoz);
            GM862_WriteString(tempstring);
            Debug_WriteString("-Llamando-\n\r");
            Debug_WriteString("-Tiempo espera conexion: 25s-\n\r");
            TemStat = SM_SET;
            delayCoop(25000);
            MaqStat = SM_InitiatingCall; //En espera por conexion 25 segundos
            sprintf(tempstring, "-Intentos restantes:%u \n\r", CallRetry);
            Debug_WriteString(tempstring);
        } else {
            MaqStat = SM_CallError;
            TemStat = SM_NOP;
        }
    }
}

void ReporteAlarma(){
    char intento = 4; //Numero de intentos para registro modem
    char tempstring[100]; //String para comandos a modem
    char dato = 0; //Variable para recepcion datos
    int tiempoSMSint=0;

    inicia_modem();
    Debug_WriteString("-Iniciando SMS-\n\r");
    tiempoSMSint=atoi(tiempoSMS);
    delayTrap(tiempoSMSint);
    do {
        dato = GM862_checkNetwork();
        intento--;
    } while (intento > 0 && dato != 4);

    MaxVPanel();
    if (FlagAlarm != 0 || MaqStat == SM_Alarm) {
        Debug_WriteString("-Reportando alarma-\n\r");
        sprintf(tempstring, "ALM:%3.1f,%3.1f,%s;",vbat_f,csolar_f,serie);
        GM862_sms(numsms,tempstring);
    }
    if (FlagReport != 0 || MaqStat == SM_Report) {
        Debug_WriteString("-Reportando estado-\n\r");
        sprintf(tempstring, "RPT:%3.1f,%3.1f,%s;",vbat_f,csolar_f,serie);
        GM862_sms(numsms,tempstring);
        PanelMax=0;   //Reinicia control panel
    }
    FlagReport=0; //Reinicia flag reporte
    FlagAlarm=0;  //Reinicia flag alarma
    intento = fin_call();
    TemStat = SM_NOP;
    MaqPrev = SM_Available;
    MaqStat = SM_Available;
    mINT3ClearIntFlag();
    mINT3IntEnable(1);
}
