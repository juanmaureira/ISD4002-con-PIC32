#include <p32xxxx.h>
#include <plib.h>			// Peripheral Library
#include "pines.h"
#include "Definiciones.h"

// initialize the ADC for single conversion, select input pins

void init_ADC() {
#define PARAM1  ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON 
#define PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_ON | ADC_SAMPLES_PER_INT_4 | ADC_ALT_BUF_ON | ADC_ALT_INPUT_OFF
#define PARAM3  ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15
#define PARAM4  ENABLE_AN0_ANA | ENABLE_AN1_ANA| ENABLE_AN2_ANA| ENABLE_AN3_ANA
#define PARAM5  SKIP_SCAN_AN2 | SKIP_SCAN_AN3 | SKIP_SCAN_AN4 | SKIP_SCAN_AN5 | SKIP_SCAN_AN6 | SKIP_SCAN_AN7 | SKIP_SCAN_AN8 | SKIP_SCAN_AN9 |\
									SKIP_SCAN_AN10 |SKIP_SCAN_AN11 | SKIP_SCAN_AN12 | SKIP_SCAN_AN13 | SKIP_SCAN_AN14 | SKIP_SCAN_AN15 
    SetChanADC10(ADC_CH0_NEG_SAMPLEA_NVREF);
    OpenADC10(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5);
    mAD1ClearIntFlag();
    EnableADC10();
}

void leeAnalogos() {
    char i=0;

    AdoffSet = 8 * ((~ReadActiveBufferADC10() &0x01));
    Vbat = ReadADC10(AdoffSet); // read the result of channel 0 conversion from the idle buffer
    Csolar = ReadADC10(AdoffSet + 1); // read the result of channel 1 conversion from the idle buffer
    mAD1ClearIntFlag();

    vbat_f = Vbat * 0.0359;
    csolar_f = Csolar * 0.0359;

    if(Csolar>PanelMax)PanelMax=Csolar; //Busca constantemente la mayor lectura

    if (vbat_f < 14,4)PORTSetBits(IOPORT_B, CHG);
    if (vbat_f > 14,9)PORTClearBits(IOPORT_B, CHG);

}

void MaxVPanel(){
    csolar_f = PanelMax * 0.0359; //Calcula valor flotante de panel para reporte (Vmax)
}
