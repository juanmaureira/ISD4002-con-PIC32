#include <plib.h>			// Peripheral Library
#include "Definiciones.h"               //Definiciones generales

void Debug_WriteString(char *string);
void GM862_WriteString(char *string);

//void output_low(int port){
//    mPORTGClearBits(port);
//}
//
//void output_high(int port){
//    mPORTGSetBits(port);
//}

//char input(int bits){
//  unsigned int value = PORTReadBits(IOPORT_G, bits);
//  return value;
//}

void GM862_WriteString(char *string) {
    while (*string != '\0') {
        while (!UARTTransmitterIsReady(UART1));
        UARTSendDataByte(UART1, *string);
        string++;
        while (!UARTTransmissionHasCompleted(UART1));
    }
}

void Debug_WriteString(char *string) {
    while (*string != '\0') {
        putcUART2(*string);
        string++;
    }
}

void Debug_WriteByte(char *CharByte) {
    while (!UARTTransmitterIsReady(UART2));
    UARTSendDataByte(UART2, *CharByte);
    while (!UARTTransmissionHasCompleted(UART2));
}

void OutputBitShiftRight(short pin, short *cmd, short bool) {
    short mask;
    mask = *cmd & 0x01;
    if (mask == 1)
        mPORTGSetBits(pin);
    else
        mPORTGClearBits(pin);
    if (bool == 0)
        *cmd >>= 1 & 0x7F;
    else
        *cmd >>= 1 & 0xFF;
}

void OutputByteShiftRight(short pin, short cmd, short qty, short bool) {
    short i;
    for (i = 0; i < (8 * qty); i++)
        OutputBitShiftRight(pin, &cmd, bool);
}

void InputBitShiftRight(short *cmd, short pin) {
    *cmd = PORTReadBits(IOPORT_G, pin);
    *cmd >>= 1;
}

void InputByteShiftRight(short cmd, short qty, short pin) {
    short i;
    for (i = 0; i < (8 * qty); i++)
        InputBitShiftRight(&cmd, pin);
}