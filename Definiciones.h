/*Estados m�quina*/
#define SM_User                  1
#define SM_InitiatingCall        2
#define SM_WaitingForConnection  3
#define SM_IncomingCall          4
#define SM_WaitingEndOfCall      5
#define SM_EndOfCall             6
#define SM_Registered            7
#define SM_Unregistered          8
#define SM_Available             9
#define SM_Service               10
#define SM_CallError             11
#define SM_Modem                 12
#define SM_Report                13
#define SM_Alarm                 14

#define	SM_SET                   101
#define	SM_WAIT                  102
#define	SM_DONE                  103
#define	SM_NOP                   104

/*Generales*/
#define FOSC 80E6

char CallRetry;
char FlagAlarm;
char FlagReport;
char FlagModem;
char FlagAnalog;
char numvoz[20]; //Numero voz
char numsms[20]; //Numero sms
char serie[10]; //Numero de serie poste
char tiempoSMS[5]; //Tiempo de espera para env�o SMS
char Bandera; //Bandera para caracteres de control USART
short TemStat;
short MaqStat;
short MaqPrev; //Estado de maquina previo
short SelTimers; //Habilita temporizadores 1, 2 o ambos
short delay2; //Delay utilizado por rutina no cooperativa
short GenStat; //Variable para regstro eventos generales
short timeout; //Variable para control tiempo fuera
short delay10us; //Variable para decenas de uS
short general0; //Variable para led estado
int general1; //Variable para control de reportes 24Hr
int AdoffSet;
int PanelMax;
int intVarCoop; //Variable interna rutina cooperativa
int delay; //Delay utilizado por rutina cooperativa
int Vbat;
int Csolar;
int ControlB;
float csolar_f; //Tension celda solar
float vbat_f; //Tension bateria


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
void OutputBitShiftRight(short pin, short *cmd, short bool);
void OutputByteShiftRight(short pin, short cmd, short qty, short bool);
void InputBitShiftRight(short *cmd, short pin);
void InputByteShiftRight(short cmd, short qty, short pin);
