#include <plib.h>			// Peripheral Library
#include <stdlib.h>                     //
#include "pines.h"                      //Definiciones de pines
#include "dee_emulation_pic32.h"        //Emulacion de EEPROM
#include "Definiciones.h"               //Definiciones generales

void Servicio() {
    char i;
    int dato = 0;
    int basura = 0;
    char contador = 0;
    char seleccion = 0;
    char tempstring[30];

    DisableIntU1RX;
    cc_mensaje(3);
    while (1) {
        Debug_WriteString("\n\rMenu servicio Yelcho r3\n\r");
        Debug_WriteString("A-Informacion del poste\n\r");
        Debug_WriteString("B-Tunel a modem\n\r");
        Debug_WriteString("C-On/Off modem\n\r");
        Debug_WriteString("D-Mensaje Chipcorder\n\r");
        Debug_WriteString("E-Probar lineas\n\r");
        Debug_WriteString("F-Numero para voz\n\r");
        Debug_WriteString("G-Numero para SMS\n\r");
        Debug_WriteString("H-Espera para SMS\n\r");
        Debug_WriteString("K-Numero de serie\n\r");
        Debug_WriteString("L-Niveles de tension\n\r");
        Debug_WriteString("T-Envio reporte\n\r");
        Debug_WriteString("Q-Salir\n\r");
        while (!UARTReceivedDataIsAvailable(UART2))ClearWDT();
        seleccion = UARTGetDataByte(UART2);
        switch (seleccion) {
            case('T'):
                Debug_WriteString("\n\rEnviando reporte\n\r");
                MaqStat = SM_Report;
                ReporteAlarma();
                MaqStat = SM_Available;
                break;
            case('F'):
                Debug_WriteString("\n\rNumero para Voz\n\r");
                delay = DataEEReadArray(numvoz, 15, 20);
                sprintf(tempstring, "\n\rNumero actual: %s\n\r", numvoz);
                Debug_WriteString(tempstring);
                Debug_WriteString("Ingrese nuevo:\n\r");
                contador = 0;
                while (contador < 20) {
                    while (!UARTReceivedDataIsAvailable(UART2));
                    seleccion = UARTGetDataByte(UART2);
                    if ((seleccion == 0x0A) || (seleccion == 0x0D)) {
                        for (i = contador; i < 20; i++)numvoz[i] = '\0';
                        contador = 20;
                    } else {
                        numvoz[contador] = seleccion;
                        contador++;
                    }
                }
                delay = DataEEWriteArray(numvoz, 15, 20);
                seleccion = 0;
                sprintf(tempstring, "\n\rEEPROM: %u\n\r", delay);
                Debug_WriteString(tempstring);
                break;
            case('G'):
                Debug_WriteString("\n\rNumero para SMS\n\r");
                delay = DataEEReadArray(numsms, 40, 20);
                sprintf(tempstring, "\n\rNumero actual: %s\n\r", numsms);
                Debug_WriteString(tempstring);
                Debug_WriteString("Ingrese nuevo:\n\r");
                contador = 0;
                while (contador < 20) {
                    while (!UARTReceivedDataIsAvailable(UART2));
                    seleccion = UARTGetDataByte(UART2);
                    if ((seleccion == 0x0A) || (seleccion == 0x0D)) {
                        for (i = contador; i < 20; i++)numsms[i] = '\0';
                        contador = 20;
                    } else {
                        numsms[contador] = seleccion;
                        contador++;
                    }
                }
                delay = DataEEWriteArray(numsms, 40, 20);
                seleccion = 0;
                sprintf(tempstring, "\n\rEEPROM: %u\n\r", delay);
                Debug_WriteString(tempstring);
                break;
            case('H'):
                Debug_WriteString("\n\rTiempo para SMS (en milisegundos)\n\r");
                delay = DataEEReadArray(tiempoSMS, 80, 5);
                sprintf(tempstring, "\n\rValor actual: %s\n\r", tiempoSMS);
                Debug_WriteString(tempstring);
                Debug_WriteString("Ingrese nuevo:\n\r");
                contador = 0;
                while (contador < 5) {
                    while (!UARTReceivedDataIsAvailable(UART2));
                    seleccion = UARTGetDataByte(UART2);
                    if ((seleccion == 0x0A) || (seleccion == 0x0D)) {
                        for (i = contador; i < 20; i++)tiempoSMS[i] = '\0';
                        contador = 5;
                    } else {
                        tiempoSMS[contador] = seleccion;
                        contador++;
                    }
                }
                delay = DataEEWriteArray(tiempoSMS, 80, 5);
                seleccion = 0;
                sprintf(tempstring, "\n\rEEPROM: %u\n\r", delay);
                Debug_WriteString(tempstring);
                break;
            case('A'):
                delay = DataEEReadArray(numvoz, 15, 20);
                delay = DataEEReadArray(serie, 1, 10);
                delay = DataEEReadArray(tiempoSMS, 80, 5);

                Debug_WriteString("\n\rInformacion\n\r");
                sprintf(tempstring, "Serie No.: %s\n\r", serie);
                Debug_WriteString(tempstring);
                sprintf(tempstring, "Llamada de voz: %s\n\r", numvoz);
                Debug_WriteString(tempstring);
                sprintf(tempstring, "Numero para SMS: %s\n\r", numsms);
                Debug_WriteString(tempstring);
                sprintf(tempstring, "Tiempo espera para SMS: %sms\n\r", tiempoSMS);
                Debug_WriteString(tempstring);
                break;
            case('B'):
                PORTClearBits(IOPORT_E, AMP_ON); //Habilita AMPlificador
                Debug_WriteString("\n\rTunel a modem\n\r");
                while (dato != 27) {
                    if (UARTReceivedDataIsAvailable(UART1)) {
                        dato = UARTGetDataByte(UART1);
                        UARTSendDataByte(UART2, dato);
                        ClearWDT();
                    }
                    if (UARTReceivedDataIsAvailable(UART2)) {
                        dato = UARTGetDataByte(UART2);
                        UARTSendDataByte(UART1, dato);
                        ClearWDT();
                    }
                }
                dato = 0;
                PORTSetBits(IOPORT_E, AMP_ON); //Deshabilita AMPlificador
                break;
            case('C'):
                if (PORTReadBits(IOPORT_B, PM_MODEM) == 0) //Chequea linea Monitor
                {
                    Debug_WriteString("\n\rModem apagado, activando\n\r");
                    PORTSetBits(IOPORT_E, ON_MODEM); //Toggle a linea Encendido
                    delayTrap(2000);
                    PORTClearBits(IOPORT_E, ON_MODEM);
                } else {
                    Debug_WriteString("\n\rModem encendido, apagando\n\r");
                    PORTSetBits(IOPORT_E, ON_MODEM); //Toggle a linea Encendido
                    delayTrap(2000);
                    PORTClearBits(IOPORT_E, ON_MODEM);
                }
                delayTrap(3000);
                if (PORTReadBits(IOPORT_B, PM_MODEM) == 0)Debug_WriteString("\n\rModem apagado\n\r");
                else Debug_WriteString("\n\rModem activado\n\r");
                break;
            case('D'):
                PORTClearBits(IOPORT_E, AMP_ON); //Habilita AMPlificador
                while (seleccion != 27) {
                    seleccion = 0;
                    Debug_WriteString("\n\rPrueba de Mensajes\n\r");
                    Debug_WriteString("A-Comunicacion establecida\n\r");
                    Debug_WriteString("B-Error de comunicaciones\n\r");
                    Debug_WriteString("C-Fin de comunicaciones\n\r");
                    Debug_WriteString("D-Mantenimiento habilitado\n\r");
                    while (!UARTReceivedDataIsAvailable(UART2));
                    seleccion = UARTGetDataByte(UART2);
                    switch (seleccion) {
                        case('A'):
                            cc_mensaje(0);
                            seleccion = 0;
                            break;
                        case('B'):
                            cc_mensaje(1);
                            seleccion = 0;
                            break;
                        case('C'):
                            cc_mensaje(2);
                            seleccion = 0;
                            break;
                        case('D'):
                            cc_mensaje(3);
                            seleccion = 0;
                            break;
                        default:
                            Debug_WriteString("Finalizado\n\r");
                            seleccion = 27;
                            break;
                    }
                }
                PORTSetBits(IOPORT_E, AMP_ON); //Habilita AMPlificador
                break;
            case('K'):
                delay = DataEEReadArray(serie, 1, 10);
                sprintf(tempstring, "\n\rNumero actual: %s\n\r", serie);
                Debug_WriteString(tempstring);
                Debug_WriteString("Ingrese nuevo:\n\r");
                contador = 0;
                //                while (contador < 10) {
                //                    while (!UARTReceivedDataIsAvailable(UART2));
                //                    serie[contador] = UARTGetDataByte(UART2);
                //                    contador++;
                //                }
                while (contador < 10) {
                    while (!UARTReceivedDataIsAvailable(UART2));
                    seleccion = UARTGetDataByte(UART2);
                    if ((seleccion == 0x0A) || (seleccion == 0x0D)) {
                        for (i = contador; i < 10; i++)serie[i] = '\0';
                        contador = 10;
                    } else {
                        serie[contador] = seleccion;
                        contador++;
                    }
                }
                delay = DataEEWriteArray(serie, 1, 10);
                sprintf(tempstring, "\n\rEEPROM: %u\n\r", delay);
                Debug_WriteString(tempstring);
                break;
            case('E'):
                while (seleccion != 27) {
                    seleccion = 0;
                    Debug_WriteString("\n\rSeleccione linea\n\r");
                    Debug_WriteString("A-Toggle Amplificador\n\r");
                    Debug_WriteString("B-Toggle Reset\n\r");
                    Debug_WriteString("C-Toggle Canal audio\n\r");
                    Debug_WriteString("D-Toggle Led Estado CPU\n\r");
                    Debug_WriteString("E-Monitor Usuario\n\r");
                    Debug_WriteString("F-Monitor Alarma\n\r");
                    while (!UARTReceivedDataIsAvailable(UART2));
                    seleccion = UARTGetDataByte(UART2);
                    switch (seleccion) {
                        case('A'):
                            Debug_WriteString("Toggle control amp\n\r");
                            PORTToggleBits(IOPORT_E, AMP_ON);
                            if (PORTReadBits(IOPORT_E, AMP_ON) == 0)Debug_WriteString("Activa\n\r");
                            else Debug_WriteString("No Activa\n\r");
                            seleccion = 0;
                            break;
                        case('B'):
                            Debug_WriteString("Toggle reset modulo GSM\n\r");
                            PORTToggleBits(IOPORT_E, RESET_MODEM);
                            if (PORTReadBits(IOPORT_E, RESET_MODEM) == 0)Debug_WriteString("No Activa\n\r");
                            else Debug_WriteString("Activa\n\r");
                            seleccion = 0;
                            break;
                        case('C'):
                            Debug_WriteString("Toggle canal audio GSM\n\r");
                            PORTToggleBits(IOPORT_D, ACCESS_MODEM);
                            if (PORTReadBits(IOPORT_D, ACCESS_MODEM) == 0)Debug_WriteString("No Activa\n\r");
                            else Debug_WriteString("Activa\n\r");
                            seleccion = 0;
                            break;
                        case('D'):
                            Debug_WriteString("Toggle led estado CPU\n\r");
                            mPORTBToggleBits(STATUS);
                            if (PORTReadBits(STATUS) == 0)Debug_WriteString("No Activa\n\r");
                            else Debug_WriteString("Activa\n\r");
                            seleccion = 0;
                            break;
                        case('E'):
                            Debug_WriteString("Prueba de usuario\n\r");
                            while (seleccion != 27) {
                                seleccion = UARTGetDataByte(UART2);
                                if (PORTReadBits(IOPORT_D, USUARIO) == 0)Debug_WriteString("Linea activa    \r");
                                else Debug_WriteString("Linea no activa \r");
                            }
                            Debug_WriteString("\n\r");
                            seleccion = 0;
                            break;
                        case('F'):
                            Debug_WriteString("Prueba de alarma\n\r");
                            while (seleccion != 27) {
                                seleccion = UARTGetDataByte(UART2);
                                if (PORTReadBits(IOPORT_B, ALM) == 0)Debug_WriteString("Linea activa    \r");
                                else Debug_WriteString("Linea no activa \r");
                            }
                            Debug_WriteString("\n\r");
                            seleccion = 0;
                            break;
                        default:
                            Debug_WriteString("Finalizado\n\r");
                            seleccion = 27;
                            break;
                    }
                }
                break;
            case('Q'):
                Debug_WriteString("\n\rFijando condiciones iniciales...\n\r");
                if (PORTReadBits(IOPORT_B, PM_MODEM) != 0) //Chequea linea Monitor
                {
                    PORTSetBits(IOPORT_E, ON_MODEM); //Toggle a linea Encendido
                    delayTrap(1500);
                    PORTClearBits(IOPORT_E, ON_MODEM);
                }
                off_modem();
                PORTClearBits(IOPORT_E, RESET_MODEM);
                PORTClearBits(IOPORT_E, ON_MODEM);
                PORTSetBits(IOPORT_E, AMP_ON);
                PORTSetBits(IOPORT_D, ACCESS_MODEM);
                PORTSetBits(STATUS);
                Debug_WriteString("\n\rEmpaquetando EEPROM...\n\r");
                delay = PackEE();
                sprintf(tempstring, "\n\rResultado EEPROM: %u\n\r", delay);
                Debug_WriteString(tempstring);
                SoftReset();
                break;

            case('L'):
                leeAnalogos();
                Debug_WriteString("Niveles de tension\n\r");
                sprintf(tempstring, "Bateria: %3.1f\n\r", vbat_f);
                Debug_WriteString(tempstring);
                sprintf(tempstring, "Celda solar: %3.1f\n\r", csolar_f);
                Debug_WriteString(tempstring);
                break;

            default:
                Debug_WriteString("\n\rFuncion no soportada!!\n\r");
                break;
        }
        seleccion = 0;
    }
}
