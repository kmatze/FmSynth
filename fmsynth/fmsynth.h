#include <windows.h>

/* Definitions for SBTimbre Bank File 
** Jamie O'Connell - 91.01.13 
** Modified 92.06.28 JWO - It seems CL is now using the 12th byte as a 
**                         percussion voice indicator                  
** Modified 93.07.02 JWO - Added transpos parameter to IBK timbre      
** Modified 93.10.11 JWO - Added Percussion Pitch  
** Modified 20.10.23 MK  - Added union struct to use a byte or it's bit fields                 
*/
#define IBKEXT   	"IBK"
#define SBIEXT   	"SBI"
#define SBISIG   	"SBI\x1A"		// 4 chars long
#define IBKSIG   	"IBK\x1A"     	// 4 chars long
#define MAN_ID   	0x43			// manufacturer-id

typedef enum {
	// voice parameter
	MOD_MULTI 		=  0,	// Frequency Multiplication Factor
	MOD_KSR			=  1,	// Envelope scaling (KSR)
	MOD_EGTYPE		=  2,	// Sound Sustaining 
	MOD_VIBRATO		=  3,	// Frequency vibrato
	MOD_TREMOLO		=  4,	// Tremolo (Amplitude vibrato)		
	CAR_MULTI		=  5,	// Frequency Multiplication Factor
	CAR_KSR			=  6,	// Envelope scaling (KSR)
	CAR_EGTYPE		=  7,	// Sound Sustaining
	CAR_VIBRATO		=  8,	// Frequency vibrato
	CAR_TREMOLO		=  9,	// Tremolo (Amplitude vibrato)
	MOD_VOLUME		= 10,	// Output level
	MOD_KSL			= 11,	// Key Scale Level 	
	CAR_VOLUME		= 12,	// Output level
	CAR_KSL			= 13,	// Key Scale Level	
	MOD_EGD			= 14,	// Envelop Generator: Decay Rate
	MOD_EGA			= 15,	// Envelop Generator: Attack Rate	
	CAR_EGD			= 16,	// Envelop Generator: Decay Rate
	CAR_EGA			= 17,	// Envelop Generator: Attack Rate	
	MOD_EGR			= 18,	// Envelop Generator: Release Rate
	MOD_EGS			= 19,	// Envelop Generator: Sustain Level
	CAR_EGR			= 20,	// Envelop Generator: Release Rate
	CAR_EGS			= 21,	// Envelop Generator: Sustain Level
	MOD_WAVE		= 22,	// Waveform Select
	MOD_UNUSED1		= 23,	// unused
	CAR_WAVE		= 24,	// Waveform Select
	CAR_UNUSED1		= 25,	// unused
	MOD_ALGORITHM	= 26,	// Algorithm (0 = FM, 1 = AM)
	MOD_FEEDBACK	= 27,	// Feedback strength
	MOD_UNUSED2		= 28,	// not used	
	// fm global parameter
}FM_VOICE_PAR;

// Packed Timbre Parameters
#pragma pack(push, 1)
typedef struct {			// 16 Bytes each
	// register 20,23 -> AM, VIB, SUS, KSR, MUL
	union 	{	BYTE reg20;			//  modulator (operator1)	
	struct 	{	BYTE MODmulti:4;		// Frequency Multiplication Factor
				BYTE MODksr:1;			// Envelope scaling (KSR)
				BYTE MODegtype:1;		// Sound Sustaining 
				BYTE MODvibrato:1;		// Frequency vibrato
				BYTE MODtremolo:1;		// Tremolo (Amplitude vibrato)		
	}; };
	union 	{	BYTE reg23;			// carrier (operator2)
	struct 	{	BYTE CARmulti:4;		// Frequency Multiplication Factor
				BYTE CARksr:1;			// Envelope scaling (KSR)
				BYTE CARegtype:1;		// Sound Sustaining
				BYTE CARvibrato:1;		// Frequency vibrato
				BYTE CARtremolo:1;		// Tremolo (Amplitude vibrato)
	}; };
	
	// register 40,43 -> KSL, TL	
	union 	{ 	BYTE reg40;			// modulator (operator1)
	struct  {	BYTE MODvolume:6;		// Output level
				BYTE MODksl:2;			// Key Scale Level 	
	}; };
	union 	{	BYTE reg43;			// carrier (operator2)
	struct 	{ 	BYTE CARvolume:6;		// Output level
				BYTE CARksl:2;			// Key Scale Level	
	}; };	

	// register 60,63 -> Attack/Decay	
	union 	{ 	BYTE reg60;			// modulator (operator1)
	struct  {	BYTE MODegd:4;		// Envelop Generator: Decay Rate
				BYTE MODega:4;		// Envelop Generator: Attack Rate	
	}; };
	union 	{ 	BYTE reg63;			// carrier (operator2)
	struct  {	BYTE CARegd:4;		// Envelop Generator: Decay Rate
				BYTE CARega:4;		// Envelop Generator: Attack Rate	
	}; };
	
	// register 80,83 -> Sustain/Release	
	union 	{ 	BYTE reg80;			// modulator (operator1)
	struct  {	BYTE MODegr:4;		// Envelop Generator: Release Rate
				BYTE MODegs:4;		// Envelop Generator: Sustain Level
	}; };
	union 	{ 	BYTE reg83;			// carrier (operator2)
	struct  {	BYTE CARegr:4;		// Envelop Generator: Release Rate
				BYTE CARegs:4;		// Envelop Generator: Sustain Level
	}; };
	
	// register E0,E3 -> Wave Select	
	union 	{ 	BYTE regE0;			// modulator (operator1)
	struct  {	BYTE MODwave:2;			// Waveform Select
				BYTE MODunused1:6;		// unused
	}; };
	union 	{ 	BYTE regE3;			// carrier (operator2)
	struct  {	BYTE CARwave:2;			// Waveform Select
				BYTE CARunused1:6;		// unused
	}; };

	// register C0    -> Feed Back (only for operator 1)
	union 	{ 	BYTE regC0;			// modulator (operator1)
	struct  {	BYTE MODalgorithm:1;	// Algorithm (0 = FM, 1 = AM)
				BYTE MODfeedback:3;		// Feedback strength
				BYTE MODunused2:4;		// not used	
	}; };
		
								// The following was originally reserved...CL uses 
								// the next byte the same way we do: BD=6,SD=7,TT=8
								// CY=9,HH=10                                      
    BYTE percvoc;   			// Percussion voice number                    : JWO
    char transpos;  			// Number of notes to transpose timbre, signed: JWO
    BYTE dpitch;    			// percussion pitch: MIDI Note 0 - 127        : JWO
    BYTE rsv[2];    			// unsused - so far
} SBTIMBRE;  
#pragma pack(pop)

// SBI Instrument file
typedef struct {
	char     sig[4];			// signature: "SBI\x1A"
    char     name[32];          // instrument name: NUL Terminated
    SBTIMBRE snd;               // instrument parameters
} SBIFMT;

// IBK Instrument Bank File
typedef struct {				// 3204 Bytes (0x0C83)
	char     sig[4];            // signature: "IBK\x1A"
    SBTIMBRE snd[128];          // Instrument block
    char     name[128][9];      // name block: NUL terminated strings
} IBKFMT;

// API WINDOWS AUDIO
//
#define NUM_CHANNELS            2			// stereo
#define SAMPLE_RATE         44100			// samplereate per second
#define BYTES_PER_SAMPLE        2 			// bit depth in bytes, here 16 bits
#define     AUDIO				0			// mode for realtime audio output
#define     RENDER              1			// mode for rendering without audio output
void AudioInit();
void AudioMode(BYTE modus);					// AUDIO or RENDER
void AudioClose();

// API FMSYNTH general
//
struct ADLVOICE {	// need for dynamic voice management for midi play
	BYTE midichn;		// midi channel
	BYTE note;			// active note
	int  cnt;			// note count, if zero voice free
} ADLVoice[9];

struct MIDICHN {	// properties for midi channel
	SBTIMBRE 	*sound;			// sound data		(16 bytes)
	BYTE  		bal;          	// bal   value  	(0=left ... 127=right)	
	BYTE  		pan;          	// pan   value  	(0=left ... 127=right)
	BYTE  		delay;        	// samples to delay (0      ... 255)
	BYTE  		vol;			// channel volume	(0 		... 127)
	BYTE        noteCounter;	// number of active notes
	BYTE        lastNoteVolume; // volume of last recieved note
} MIDIChn[16];

void  		FMOpenSynth();
void  		FMCloseSynth();
void 		FMGetSample(void *stream, long size);
void  		FMWrite(int reg, int data);
void  		FMReset();
void  		FMKeyOn(int voice, int fnum);
void  		FMKeyOff(int voice);
void  		FMVolume(int voice, int vol);
void  		FMEffect(int voice, BYTE bal, BYTE pan, int delay, int mainvol);
void 		FMVoice(int voice, SBTIMBRE *inst);
void 		FMVoicePar(SBTIMBRE *t, BYTE par, BYTE val);
SBTIMBRE* 	FMGetTimbre(BYTE sound);

// API FMSYNTH midi support
//
void		FMMidiInit();
void  		FMMidiMsg(DWORD msg);
void 		FMMidiSyx(int size, BYTE data[]); 
int 		FMMidiSetINST(BYTE chn, SBTIMBRE *data) ;
SBTIMBRE* 	FMMidiGetINST(BYTE chn);

// API FMSYNTH sound patches
//
// SBI: sound blaster instrument
SBIFMT* 	FMNewSBI();
int 		FMLoadSBI(SBIFMT* sbi, char fileName[]);
int     	FMSaveSBI(SBIFMT *sbi, char fileName[]);
// IBK: soundblaster instrument bank (128 instruments)
IBKFMT* 	FMNewIBK();
int         FMLoadIBK(IBKFMT* ibk, char fileName[]);
int     	FMSaveIBK(IBKFMT *ibk, char fileName[]);
int 		FMSetIBK(IBKFMT *ibk, int type);						// replace internal soundset (instrument or drum)




