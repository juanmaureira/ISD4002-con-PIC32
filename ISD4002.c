//#include <p32xxxx.h>
#include "plib.h"
#include "pines.h"
#include "funciones.h"
//////////// Driver for ISD4002 Voice Record/Playback Chip //////////////
////                                                                 ////
////  init_isd()                 Initializes the chip for operation. ////
////                                                                 ////
////  record_message(address)    Records a message at the given      ////
////                             address.  Stop_message must be      ////
////                             called to stop recording            ////
////                                                                 ////
////  play_message(address)      Plays a message at the given        ////
////                             address.  Stop_message must be      ////
////                             called to stop playing.             ////
////                                                                 ////
////  stop_message()             Stops the current operation and     ////
////                             returns the isd pointer value of    ////
////                             where the action stopped.           ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////
//#include "pines.h"

/* CP0.Count counts at half the CPU rate */
#define TICK_HZ (80000000 / 2)

#define  POWER_UP    0X2000
#define  SET_PLAY    0XE000
#define  PLAY        0XF000
#define  SET_RECORD  0XA000
#define  RECORD      0XB000
#define  SET_MC      0XE800
#define  MESSAGE_CUE 0XF800
#define  STOP        0X3000
#define  POWER_DOWN  0X1000
#define  READ_INT    0X3000

typedef enum {
    playing, recording, stopped
} isd_mode;
isd_mode current_isd_mode; // global variable for state of isd chip

//this function writes a 16 bit number to the isd chip

void write_isd(short value) {
    short i;

    mPORTGClearBits(ISD4003_SELECT); // enable the device
    for (i = 0; i < 16; i++) {
        OutputBitShiftRight(ISD4003_MOSI, &value, 0);
        delay_us(50);
        mPORTGSetBits(ISD4003_CLK);
        delay_us(20);
        mPORTGClearBits(ISD4003_CLK);
        delay_us(50);
    }
    mPORTGSetBits(ISD4003_SELECT); // disable the device
    delay_ms(1);
}

//Esta funcion lee y escribe 16 bits al ISD4002

long read_isd(short valueRISD) {
    short retval;
    short i;

    mPORTGClearBits(ISD4003_SELECT); //Habilita ISD4002
    for (i = 0; i < 16; i++) { // lectura isd4003
        InputBitShiftRight(&retval, ISD4003_MISO);
        OutputBitShiftRight(ISD4003_MOSI, &valueRISD, 0);
        delay_us(25);
        mPORTGSetBits(ISD4003_CLK);
        delay_us(50);
        mPORTGClearBits(ISD4003_CLK);
        delay_us(25);
    }

    mPORTGSetBits(ISD4003_SELECT); //Deshabilita el ISD4002
    delay_us(25);
    return (retval);
}

//Inicializa el ISD

void init_isd(void) {
    mPORTGSetBits(ISD4003_SELECT);
    mPORTGClearBits(ISD4003_MOSI);
    mPORTGClearBits(ISD4003_MISO);
    mPORTGSetBits(ISD4003_CLK);
    current_isd_mode = stopped;
}

//Detiene la reproducci�n y apaga el ISD

long stop_message(void) {
    short address = 0;

    address = read_isd(STOP);
    address >>= 2;
    address &= 0x07ff;
    write_isd(POWER_DOWN);
    current_isd_mode = stopped;
    return (address);
}

//Graba un mensaje en el ISD

void record_message(short address) {
    short data_out;

    current_isd_mode = recording;
    read_isd(POWER_UP); // turn on chip
    delay_ms(50);

    read_isd(POWER_UP);
    delay_ms(100);

    data_out = SET_RECORD | address; // set to record
    read_isd(data_out);

    read_isd(RECORD); // record the message
}

//Reproduce un mensaje

void play_message(short address) {
    short data_out;

    current_isd_mode = playing;
    read_isd(POWER_UP); // turn on chip
    delay_ms(50);

    data_out = SET_PLAY | address; // set to play
    read_isd(data_out);

    read_isd(PLAY); // play the message
}


//Mensajes de operaci�n Yelcho

void cc_mensaje(short y) {
    mPORTEClearBits(AMP_ON); //Habilita AMPlificador
    switch (y) {
        case 0:
            Debug_WriteString("-Mensaje Comunicacion establecida-\n\r");
            play_message(0x0000); //Mensaje comunicacion establecida
            delay_ms(11000);
            stop_message();
            break;
        case 1:
            Debug_WriteString("-Mensaje Error de comunicaciones-\n\r");
            play_message(0x0078); //Mensaje error de comunicaciones
            delay_ms(12000);
            stop_message();
            mPORTESetBits(AMP_ON); //Apaga amplificador
            break;
        case 2:
            Debug_WriteString("-Mensaje Fin de comunicaciones-\n\r");
            play_message(0x00F0); //Mensaje Fin de comunicaciones
            delay_ms(8000);
            stop_message();
            mPORTESetBits(AMP_ON); //Apaga amplificador
            break;
        case 3:
            Debug_WriteString("-Mensaje Mantenimiento habilitado-\n\r");
            play_message(0x0168); //Mensaje mantenimiento
            //delay_ms(8000);
            //stop_message();
            break;
        default:
            mPORTESetBits(AMP_ON); //Deshabilita AMPlificador
            break;
    }
}
