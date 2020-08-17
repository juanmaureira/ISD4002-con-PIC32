/*
 * Prototipo de funciones
 *
 */

void write_isd(short value);
long read_isd(short valueRISD);
void init_isd(void);
long stop_message(void);
void record_message(short address);
void play_message(short address);
void cc_mensaje(short y);
void delay_cycles(int retardo);
void delay_us(int retardo);
void delay_ms(int retardo);
void OutputBitShiftRight(short pin,short *cmd,short bool);
void OutputByteShiftRight(short pin,short cmd,short qty,short bool);
void InputBitShiftRight(short *cmd, short pin);
void InputByteShiftRight(short cmd, short qty,short pin);




