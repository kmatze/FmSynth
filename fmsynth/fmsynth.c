	/********************************************************************************************
// fmsynth.c / fmsynth.h
// library to use adlibemu (Ken Silverman) as midi synth on windows
// by ma.ke. 2021
//   	- without warranty
//   	- use at your own risk
//   	- do what ever you want with this
//   	- be happy
//
//	features:
//  =========
//		- adlibemu can use as voice or midi synthesizer
//			- voice: 9 voices, provide complete register writing (drum mode not implented)
//			- midi : dynamic voice handling to use 16 midichannels
//					 max. 9 polyphonic
//			- full stereo and delay
//
// 		- support for sound patches
//			- SBI, IBK
//			- default MIDI soundbank with 128 "GM" instruments (SBI) + 128 "GM" drums (SBI)
//
//		- MIDI implementation: 
//			- channel messages:
// 				NOTEOFF 			0x8
// 				NOTEON 				0x9
//				CTRLCHG				0xB
//					CC_VOL            7
//					CC_BAL			  8 -----+ 
//					CC_PAN			 10 -----+
//					CC_DELAY		 91
//				PRGCHG				0xC
// 				PITCHBEND			0xE
//			- internal instrument (soundblaster timbre with 16 bytes) for midi channel can set with PRGCHG
//			- system exclusive for modify midi channel instrument
//				- one voice parameter per sysex msg or 
//				- complete voicedata  per sysex msg 
//
//		- C-API (well documented, see source code fmsynth.c and fmsynth.h)
//			- API WINDOWS 	audio
//			- API FMSYNTH 	general
//			- API FMSYNTH 	midi support
//			- API FMSYNTH 	sound patches
********************************************************************************************/

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "adlibemu.h"
#include "fmsynth.h"
#include "fmpatches.h"			// sound set with standard patches (SOUND[256][16])	

#define VERSION "1.02.09"
#define VERSDATUM "09-02-2021"

// MIDI definitions
//
#define NOTEOFF 				0x8
#define NOTEON 					0x9
#define CTRLCHG					0xB
#define PITCHBEND				0xE
#define     CC_VOL            	  7
#define     CC_BAL				  8
#define     CC_PAN			 	 10
#define     CC_DELAY		 	 91
#define PRGCHG					0xC

#define PITCHBEND_MAX_HALFTONE      2
#define PITCHBEND_MAX_VALUE		16384

// ADLIB definitions
//
#define MIN_REGISTER        0
#define MAX_REGISTER        256
#define ADLBASEFREQ         440		// basis frequency for audio = A4 = 440 Hz 
BYTE ADLVOICES 				= 9;	// standard = 9 voices (6 in drum mode, not implemented)
int  ADLCnt    				= 0;	// note counter to define the older of note
BYTE ADLVoiceOffset[9] 		= {0, 1, 2, 8, 9, 10, 16, 17, 18};

struct ADLFNUM {	// need to convert midi note number to block and frequency
	int  freq;			// frequnecy of note
	int block;			// block (octave?)
	int fnum;			// f-number 0 ... 1023
} ADLFnum[128];

// copy from midiplayer by wetspot2
static int tbl_fmVol[] = {
    63, 55, 48, 43, 40, 37, 35, 34,
    32, 31, 29, 28, 27, 26, 26, 25,
    24, 23, 23, 22, 21, 21, 20, 20,
    19, 19, 18, 18, 18, 17, 17, 16,
    16, 16, 15, 15, 15, 14, 14, 14,
    13, 13, 13, 13, 12, 12, 12, 12,
    11, 11, 11, 11, 10, 10, 10, 10,
    9, 9, 9, 9, 9, 8, 8, 8,
    8, 8, 8, 7, 7, 7, 7, 7,
    7, 6, 6, 6, 6, 6, 6, 5,
    5, 5, 5, 5, 5, 5, 5, 4,
    4, 4, 4, 4, 4, 4, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 0,
};

/****************************************************************
 *
 * simple windows sound streaming code for adlib synthesizer
 * tiny audio library
 *
 ****************************************************************/

// declarations for audio output
//
void AudioInit();
void AudioPlay(int buffers);
static void CALLBACK AudioProc( HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance,  
								  DWORD_PTR dwParam1, DWORD_PTR dwParam2 );
 
#define 	SOUNDBUFFERS  		   8  		// minimum: 4 for noncracked sound
#define     BUFFERTIME           100		// 1/100 part of second

HWAVEOUT 	WaveOut;
WAVEHDR  	WaveOutHdr[SOUNDBUFFERS];
void     	*soundbuf[SOUNDBUFFERS];
BYTE 		AudioEndFlag = FALSE;			// end flag for audio callback function
long     	waveBufLen   = 0;

BYTE FMsynthactive 		 = FALSE;			// fmsynth active flag
int  FMDELAYFACTOR       = 0xFF;			// delay factor for voices

// ---------------------------------------------------------------------------------
// Function: AudioOutProc
//     Description:  	callback function for waveOutOpen function
//
static void CALLBACK AudioOutProc( HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance,  
								  DWORD_PTR dwParam1, DWORD_PTR dwParam2 )
{
	if(!AudioEndFlag) AudioPlay(1);
};


// ---------------------------------------------------------------------------------
// Function: AudioInit
//     Description:  	initialize audio output
//
void AudioInit() {
	long samplerate 	= SAMPLE_RATE;
	long numspeakers 	= NUM_CHANNELS;
	long bytespersample = BYTES_PER_SAMPLE;

	WAVEOUTCAPS WaveOutCaps;
	PCMWAVEFORMAT mywaveformat;
	waveOutGetDevCaps(0,&WaveOutCaps,sizeof(WaveOutCaps));
	if (!(WaveOutCaps.dwFormats&0xaaa)) numspeakers 	= 1;
	if (!(WaveOutCaps.dwFormats&0xccc)) bytespersample 	= 1;
	if (!(WaveOutCaps.dwFormats&0xf00)) samplerate 		= 22050;
	if (!(WaveOutCaps.dwFormats&0xff0)) samplerate 		= 11025;

	mywaveformat.wf.wFormatTag 		= WAVE_FORMAT_PCM;
	mywaveformat.wf.nChannels 		= numspeakers;
	mywaveformat.wf.nSamplesPerSec 	= samplerate;
	mywaveformat.wf.nAvgBytesPerSec = samplerate*numspeakers*bytespersample;
	mywaveformat.wf.nBlockAlign 	= numspeakers*bytespersample;
	mywaveformat.wBitsPerSample 	= (bytespersample<<3);
	
//	waveOutReset(WaveOut);
	waveOutOpen(&WaveOut,WAVE_MAPPER,(void *)&mywaveformat,(DWORD)AudioOutProc, 0, CALLBACK_FUNCTION);

	waveBufLen = bytespersample*numspeakers*samplerate / BUFFERTIME;
	for(int i = 0; i < SOUNDBUFFERS; i++) {
		soundbuf[i]                     = (unsigned char *)malloc(sizeof(unsigned char)*waveBufLen);
		WaveOutHdr[i].lpData 			= soundbuf[i];
		WaveOutHdr[i].dwBufferLength 	= waveBufLen;
		WaveOutHdr[i].dwBytesRecorded 	= 0;
		WaveOutHdr[i].dwUser 			= 0;
		WaveOutHdr[i].dwFlags 			= 0;
		WaveOutHdr[i].dwLoops 			= 1;
		waveOutPrepareHeader(WaveOut,&WaveOutHdr[i],sizeof(WAVEHDR));
	}
}

// ---------------------------------------------------------------------------------
// Function: AudioMode
//     Parameter:		mode
//     Description:  	switch between AUDIO and RENDER 
//
void AudioMode(BYTE modus) {
	if(modus == AUDIO) {
		AudioEndFlag = FALSE;
		AudioPlay(SOUNDBUFFERS);
	}
	if(modus == RENDER) {
		AudioEndFlag = TRUE;
		waveOutReset(WaveOut);
	}
}

// ---------------------------------------------------------------------------------
// Function: AudioPlay
//     Description:  	fill the next wave output buffer(s) with
//                   	current sound stream from synthesizer 
//
void AudioPlay(int buffers) {
	static int i = 0;
	while (buffers-- > 0) {
		adlibgetsample(soundbuf[i],waveBufLen);
		waveOutWrite( WaveOut,&WaveOutHdr[i],sizeof(WAVEHDR));
		if (++i >= SOUNDBUFFERS) i = 0;
	}
}

// ---------------------------------------------------------------------------------
// Function: AudioClose 
//      Description: 	close audio output 
//
void AudioClose() {
	AudioEndFlag = TRUE;
	waveOutReset(WaveOut);
	waveOutClose(WaveOut);
	for(int i = 0; i < SOUNDBUFFERS; i++) {
		waveOutUnprepareHeader(WaveOut,&WaveOutHdr[i],sizeof(WAVEHDR));
	}
}

/****************************************************************
 *
 * functions for adlib synthesizer
 *
 ****************************************************************/

// --------------------------------------------------------------------------------- 
// Function: openSynth 
//      Description:    initialize adlib synthesizer 
//                      (OPL2 mode, 9 channels, no drum)
//
void FMOpenSynth() {
	if(FMsynthactive) return;
	adlibinit(SAMPLE_RATE, NUM_CHANNELS, BYTES_PER_SAMPLE);
	FMReset();
	FMsynthactive = TRUE;
}

// ---------------------------------------------------------------------------------
// Function: closeSynth 
//      Description: 	shutdown adlib synthesizer and audio 
//
void FMCloseSynth() {
	if (!FMsynthactive) return;
	FMsynthactive = FALSE;
}
 
// ---------------------------------------------------------------------------------
// Function: FMMidiToFreq 
//      Parameters:     note  	- midi note to convert
//      Description:    return frequency (Hz) of a midi note
//
int FMMidiToFreq(int note) {
	double frequency = pow(2., (note - 69.) / 12.) * 440.;
	double freq_scaled = frequency / 49716. * pow(2., 20.);
	return (int)freq_scaled;
}

// ---------------------------------------------------------------------------------
// Function: FMFreqToFnum 
//      Parameters:     freq  	- freq to convert
//                      block	- adress for calulatet block
//						fnum	- adress for f-number 
//      Description:    return block and frequency number for adlib synthesizer
//						for given frequency
//						only for internal use
//
//  F-Number = Music Frequency * 2^(20-Block) / 49716 Hz 
//
//	The OPL2 creates sound using two operator FM-synthesis. 
//	Each operator can be set to run at some multiple (1/2, 1-10, 12, 15)
//  of the base frequency, allowing you to get rich harmonic sounds.
//	It also allows you to set _both_ operators to the same harmonic, 
//  allowing you to achieve a frequency of up to 15 times the maximum 
//  base frequency.  Without the multiplier, you're either limited 6000 Hz
//  With it, you can go well above the top end of human hearing.
//
//  adlib fnum (frequency number) 	0 ... 1023
//  addlib block 					0 ... 7
//
void FMFreqToFnum(int freq, int *block, int *fnum) {
    int bl = -1;
    int fn =  0;
    do {
        bl++;
		fn = freq / pow(2., bl);
    } while (fn >= 1024 && bl < 7);
	*block = bl;
	*fnum  = fn;
}
 
// ---------------------------------------------------------------------------------
// Function: FMGetSample
//      Parameters:		stream			- pointer to stream
//                      size 			- size of stream
//      Description:    render data
//
void FMGetSample(void *stream, long size) {
	adlibgetsample(stream, size);
}
// ---------------------------------------------------------------------------------
// Function: FMWrite
//      Parameters:     reg  - adlib register
//                      data - data to write
//     Description:		write data to adlib register
//
void FMWrite(int reg, int data) {
	adlib0(reg, data);
}

// ---------------------------------------------------------------------------------
// Function: FMReset
//     Description:		quick and dirty sound card reset 
//                  	(zeros all registers)
//
void FMReset() {
	for(int i = 0; i < ADLVOICES; i++) 
		FMKeyOff(i);
	Sleep(1000);
	
	for(int i = MIN_REGISTER; i < MAX_REGISTER; i++) 
	   FMWrite(i, 0);  				// zero all registers
   
	FMWrite(0x01, 0x20); 			// test off, wave select enable(!)
	
	for(int i=0;i< ADLVOICES;i++) {	// init adlib voices
		ADLVoice[i].midichn = 0;
		ADLVoice[i].note    = 0;
		ADLVoice[i].cnt     = 0;		// 0 for inactive voice
		lvol[i] = rvol[i]   = 1; 		// Volume multiplier (float) on left/right speaker = pan
		lplc[i] = rplc[i] 	= 0;		// Samples to delay          on left/right speaker = reverb
	}
	
	for(int i = 0;i < 128;i++) {	// generate block and fnum for every midi note
		int block, fnum;
		int freq = FMMidiToFreq(i);
		FMFreqToFnum(freq, &block, &fnum);
		ADLFnum[i].freq 	= freq;
		ADLFnum[i].block 	= block;
		ADLFnum[i].fnum 	= fnum;
	}
	
	for(int i=0;i<16;i++) {			// init midi channels with default values
		MIDIChn[i].sound    	  = (SBTIMBRE*)SOUND[0]; // instrument 	= patch 0
		MIDIChn[i].pan    		  = 63; 				 // pan 			= middle
		MIDIChn[i].delay   		  = 0; 					 // delay 		= off (0 samples)
		MIDIChn[i].vol  		  = 127; 				 // volume 		= max
		MIDIChn[i].noteCounter	  = 0;					 // number of active notes
		MIDIChn[i].lastNoteVolume = 0; 					 // volume of last recieved note
	}
	ADLCnt   = 0;		// reset note counter
}

// ---------------------------------------------------------------------------------
// Function: FMKeyOn 
//      Parameters:     voice  - which voice to turn on
//                      note   - midi note nummber
//      Description:    turns on a voice of specfied midi note
//
void FMKeyOn(int voice, int note) {
	FMWrite(0xA0+voice, ADLFnum[note].fnum & 0xff);
	int tmp = ADLFnum[note].fnum >> 8 | ADLFnum[note].block << 2 | 0x20 ; 	
	FMWrite(0xB0+voice, tmp);
}

// ---------------------------------------------------------------------------------
// Function: FMKeyOff 
//      Parameters:     voice - voice to turn off
//      Description:    turns off the specified voice
//
void FMKeyOff(int voice) {
	FMWrite(0xA0+voice, 0);
	FMWrite(0xB0+voice, 0);	
}

// ---------------------------------------------------------------------------------
// Function: FMSetFreq 
//      Parameters:     voice  - which voice
//                      freq   - frequency to set
//      Description:    set frequency for a adlib voice
//
void FMSetFreq(int voice, int freq) {
	int block, fnum;
	FMFreqToFnum(freq, &block, &fnum);
	FMWrite(0xA0+voice, fnum & 0xff);
	int tmp = fnum >> 8 | block << 2 | 0x20 ; 	
	FMWrite(0xB0+voice, tmp);
}

// ---------------------------------------------------------------------------------
// Function: FMVolume 
//      Parameters:     voice 	- voice to set volume of
//                      vol 	- volume value
//      Description:    sets the volume of a voice to the specified
//                      value in the range (0-63)
//
void FMVolume(int voice, int vol)
{
	FMWrite(0x43 + ADLVoiceOffset[voice], vol);
}

// ---------------------------------------------------------------------------------
// Function: FMEffect 
//      Parameters:     voice 	- voice to set effect
//						pan 	- left ... right
//                      delay 	- samples to delay
//						mainvol - main volume of voice 0 ... 127
//      Description:    set effect of a voice 
//
void FMEffect(int voice, BYTE bal, BYTE pan, int delay, int mainvol)
{
	// set balance/pan effect for voice
	// bal position: 0 = left, 63/64 = middle, 127 = right
	// pan position: 0 = left, 63/64 = middle, 127 = right
	// Volume multiplier on left/right speaker = float 0 ... 1
	
	// pan = pan > bal? pan : bal;				// must to be check the difference between pan and balance!

	lvol[voice] = rvol[voice] = 1.0;			// pan position = middle (default)
	lplc[voice] = rplc[voice] = 0;				// default 0 samples to delay
	if(pan < 64) {								// pan position = left
		rvol[voice] = (float)pan / 63;	
		if(delay > 0) {
			rvol[voice] = rvol[voice] * 0.7 + 0.2;
			rplc[voice] = delay * FMDELAYFACTOR;
		}
	} else {
		lvol[voice] = (float)(127 - pan) / 63;	// pan position = right
		if(delay > 0) {
			lvol[voice] = lvol[voice] * 0.7 + 0.2;
			lplc[voice] = delay  * FMDELAYFACTOR;
	}	}

	// transform to main volume
	lvol[voice] = lvol[voice] / 127 * mainvol; 
	rvol[voice] = rvol[voice] / 127 * mainvol;
}

// ---------------------------------------------------------------------------------
// Function: FMVoice
//      Parameters:     voice - voice to load instrument
//						inst  - instrument
//      Description:    set voice intrument
//
// melody
//		Voice		   0     1     2     3     4     5     6     7     8
// 		Operator 1     1     2     3     7     8     9    13    14    15
//        offset 1  0x00  0x01  0x02  0x08  0x09  0x0A  0x10  0x11  0x12
// 		Operator 2     4     5     6    10    11    12    16    17    18
//        offet  2  0x03  0x04  0x05  0x0B  0x0C  0x0D  0x13  0x14  0x15
//
void FMVoice(int voice, SBTIMBRE *instrument) {
	unsigned char *inst = (unsigned char*)instrument;
	
	// voice specific
	FMWrite(0xa0+voice,0);				//   fnumber, lower 8 bits 
	FMWrite(0xb0+voice,0);				// 	 keyon bit, block number, fnum hi bits
	FMWrite(0xc0+voice,inst[10]);		//   feedBack modulation factor, additive

	voice = ADLVoiceOffset[voice];
	// operator 1 = modulator
	FMWrite(0x20+voice,inst[0]);		//	 tremolo, vibrato, sustain, KSR, frequency multiplication factor 
	FMWrite(0x40+voice,inst[2]);		//   key scale level, output level  
	FMWrite(0x60+voice,inst[4]);		//   attack rate, decay rate   
	FMWrite(0x80+voice,inst[6]);		//   sustain level, release rate
	FMWrite(0xe0+voice,inst[8]);		//   waveform select
	// operator 2 = carrier
	FMWrite(0x23+voice,inst[1]);		//	 tremolo, vibrato, sustain, KSR, frequency multiplication factor
	FMWrite(0x43+voice,inst[3]);		//   key scale level, output level
	FMWrite(0x63+voice,inst[5]);		//   attack rate, decay rate 	
	FMWrite(0x83+voice,inst[7]);		//   sustain level, release rate
	FMWrite(0xe3+voice,inst[9]);		//   waveform select 
}

// ---------------------------------------------------------------------------------
// Function: FMVoicePar
//      Parameters:     t  		- timbre pointer  
//						par   	- parameter (FM_VOICE_PAR)
//						val		- value
//      Description:    modify sound parameter for timbre
//
void FMVoicePar(SBTIMBRE *t, BYTE par, BYTE val) {
	BYTE reg    = 0;						// register to update
	BYTE regVal = 0;						// with value
	
	switch (par) {
		// register 0x20 + 0x23 (modulator + carrier)
		case MOD_MULTI: 	t->MODmulti 	= val; reg=0x20; regVal=t->reg20; break;	// Frequency Multiplication Factor
		case MOD_KSR: 		t->MODksr 		= val; reg=0x20; regVal=t->reg20; break;	// Envelope scaling (KSR)
		case MOD_EGTYPE: 	t->MODegtype 	= val; reg=0x20; regVal=t->reg20; break;	// Sound Sustaining 
		case MOD_VIBRATO: 	t->MODvibrato 	= val; reg=0x20; regVal=t->reg20; break;	// Frequency vibrato
		case MOD_TREMOLO: 	t->MODtremolo	= val; reg=0x20; regVal=t->reg20; break;	// Tremolo (Amplitude vibrato)		
		case CAR_MULTI: 	t->CARmulti		= val; reg=0x23; regVal=t->reg23; break;	// Frequency Multiplication Factor
		case CAR_KSR: 		t->CARksr		= val; reg=0x23; regVal=t->reg23; break;	// Envelope scaling (KSR)
		case CAR_EGTYPE: 	t->CARegtype	= val; reg=0x23; regVal=t->reg23; break;	// Sound Sustaining
		case CAR_VIBRATO: 	t->CARvibrato	= val; reg=0x23; regVal=t->reg23; break;	// Frequency vibrato
		case CAR_TREMOLO: 	t->CARtremolo	= val; reg=0x23; regVal=t->reg23; break;	// Tremolo (Amplitude vibrato)
		
		// register 0x40 + 0x43 (modulator + carrier)
		case MOD_VOLUME: 	t->MODvolume	= val; reg=0x40; regVal=t->reg40; break;	// Output level
		case MOD_KSL: 		t->MODksl		= val; reg=0x40; regVal=t->reg40; break;	// Key Scale Level 	
		case CAR_VOLUME: 	t->CARvolume	= val; reg=0x43; regVal=t->reg43; break;	// Output level
		case CAR_KSL: 		t->CARksl		= val; reg=0x43; regVal=t->reg43; break;	// Key Scale Level	
		
		// register 0x60 + 0x63 (modulator + carrier)
		case MOD_EGD: 		t->MODegd		= val; reg=0x60; regVal=t->reg60; break;	// Envelop Generator: Decay Rate
		case MOD_EGA: 		t->MODega		= val; reg=0x60; regVal=t->reg60; break;	// Envelop Generator: Attack Rate	
		case CAR_EGD: 		t->CARegd		= val; reg=0x63; regVal=t->reg63; break;	// Envelop Generator: Decay Rate
		case CAR_EGA: 		t->CARega		= val; reg=0x63; regVal=t->reg63; break;	// Envelop Generator: Attack Rate	
		
		// register 0x80 + 0x83 (modulator + carrier)
		case MOD_EGR: 		t->MODegr		= val; reg=0x80; regVal=t->reg80; break;	// Envelop Generator: Release Rate
		case MOD_EGS: 		t->MODegs		= val; reg=0x80; regVal=t->reg80; break;	// Envelop Generator: Sustain Level
		case CAR_EGR: 		t->CARegr		= val; reg=0x83; regVal=t->reg83; break;	// Envelop Generator: Release Rate
		case CAR_EGS: 		t->CARegs		= val; reg=0x83; regVal=t->reg83; break;	// Envelop Generator: Sustain Level
		
		// register 0xE0 + 0xE3 (modulator + carrier)
		case MOD_WAVE: 		t->MODwave		= val; reg=0xE0; regVal=t->regE0; break;	// Waveform Select
		case CAR_WAVE: 		t->CARwave		= val; reg=0xE3; regVal=t->regE0; break;	// Waveform Select
		
		// register 0xC0 (modulator)
		case MOD_ALGORITHM: t->MODalgorithm	= val; reg=0xC0; regVal=t->regC0; break;	// Algorithm (0 = FM, 1 = AM)
		case MOD_FEEDBACK: 	t->MODfeedback	= val; reg=0xC0; regVal=t->regC0; break;	// Feedback strength
		
		default:				   							  				  break; 	// all other parameters
	}
	
	return;
}

// ---------------------------------------------------------------------------------
// Function: FMGetTimbre
//      Parameters:	    sound 	- sound number 0 ... 255
//      Description:    get timbre data of sound 
//
SBTIMBRE* FMGetTimbre(BYTE sound) {
	return (SBTIMBRE*)SOUND[sound];
}

/****************************************************************
 *
 * midi functions for adlib synthesizer
 *
 ****************************************************************/
 
// ---------------------------------------------------------------------------------
// Function: FMMidiNoteOn (internal)
//      Parameters:     chn   - midi channel
//                      note  - note
//                      vol   - volume
//      Description:    play midi note
//						only for internal use
//
void FMMidiNoteOn(BYTE chn, BYTE note, BYTE vol) {
	if (note < 21 || note > 108)  return; 		// outside of piano range (21 ... 108)
	
	int cnt0  = ADLVoice[0].cnt;				// old of note
	int v0    = 0;								// holds the oldest or free voice
	SBTIMBRE *instrument;						// for getting instrument

	int mainvolume = MIDIChn[chn].vol;				
	int fmvolume   = tbl_fmVol[MIDIChn[chn].vol]; 	// convert midi volume to fm volume

	for(int v = 0; v < ADLVOICES; v++) {		// search free or oldest voice
		if(ADLVoice[v].cnt == 0) {					// voice is free, use this voice
			v0 = v; v  = ADLVOICES;             	// set v to max voice
		} else {									// voice is active
			if(ADLVoice[v].cnt < cnt0) {			// found oldest voice
				cnt0 = ADLVoice[v].cnt;
				v0   = v;
	}	}	}

	ADLVoice[v0].midichn 		 = chn;			// merge voice to use
	ADLVoice[v0].note 	 		 = note;		//       note
	ADLVoice[v0].cnt 	 		 = ++ADLCnt;	//       old of  note - add 1 to active notes
	
	MIDIChn[chn].noteCounter	+= 1;			// number of active notes
	MIDIChn[chn].lastNoteVolume  = vol; 		// volume of last recieved note
	
	if(chn == 9) {								// midi GM drum channel
		instrument = (SBTIMBRE*)SOUND[note+128];	// get drum data
		note = instrument->dpitch;					// get key (frequency/pitch) of drum (byte 13)
	} else {									// midi GM instrument channel
		instrument = MIDIChn[chn].sound;		// get instrument data for channel
	}
	
	FMVoice(v0, instrument);															// voice: load instrument
	FMEffect(v0, MIDIChn[chn].bal, MIDIChn[chn].pan, MIDIChn[chn].delay, mainvolume);	// set effects
	FMVolume(v0, fmvolume);																// set fmvolume
	FMKeyOn(v0, note);																	// play note
}

// ---------------------------------------------------------------------------------
// Function: FMMidiPitchBend (internal)
//      Parameters:     chn	   - midi channel
//                      data1  - msb
//						data2  - lsb
//      Description:    control pitch bend for channel
//
// #define PITCHBEND_MAX_HALFTONE      2
// #define PITCHBEND_MAX_VALUE		16384     // 0 = 8192

void FMMidiPitchBend(BYTE chn, BYTE data1, BYTE data2) {
	for(int v = 0; v < ADLVOICES; v++) {		// search active voices for midi channel
		if(ADLVoice[v].midichn == chn) {			// active voice 
			int freq_delta;
			float freq_bend;
			int note = ADLVoice[v].note;
			
			int freq = ADLFnum[note].freq;
			int pb_value = ( data2 << 7 | data1) - 8192;
	
			if (pb_value > 0) freq_delta = ADLFnum[note + 2].freq - freq;
			if (pb_value < 0) freq_delta = freq - ADLFnum[note - 2].freq;
			
			freq_bend = freq + (freq_delta / 8192.0 * pb_value);
			
			FMSetFreq(v, (int)freq_bend);
	}	}
}

// ---------------------------------------------------------------------------------
// Function: FMMidiNoteOff (internal)
//      Parameters:     chn   - midi channel
//                      note  - note
//      Description:    stop playing midi note
//						only for internal use
//

void FMMidiNoteOff(BYTE chn, BYTE note) {
	if (MIDIChn[chn].noteCounter > 0) MIDIChn[chn].noteCounter -= 1;	// reduce active notes on midi channel
	for(int v=0; v < ADLVOICES; v++) {				// search playing note
		if(ADLVoice[v].cnt > 0) { 					// this is a used voice
			if(ADLVoice[v].midichn == chn) {		// found the midi channel
				if(ADLVoice[v].note == note) {		// found the note
					FMKeyOff(v);					// turn key off
					ADLVoice[v].midichn = 0;		// reset parameter and set voice free
					ADLVoice[v].note 	= 0;
					ADLVoice[v].cnt 	= 0;
					break;
	}	}	}	}
}

// ---------------------------------------------------------------------------------
// Function: FMMidiMsg 
//      Parameters:     msg   - midi message as DWORD
//      Description:    process midi data
//
void FMMidiMsg(DWORD msg) {
	// unsigned char  bStatus = (BYTE) ((msg & 0x000000FF)>>0);
	BYTE chn 	= (BYTE) ((msg & 0x0000000F)>>0);
	BYTE cmd 	= (BYTE) ((msg & 0x000000F0)>>4);
	BYTE bData1	= (BYTE) ((msg & 0x0000FF00)>>8);
	BYTE bData2	= (BYTE) ((msg & 0x00FF0000)>>16);
	
	// SBTIMBRE *t;
	
	switch (cmd) {
		case PRGCHG: 				// change instrument
			if (chn != 9) MIDIChn[chn].sound   = (SBTIMBRE*)SOUND[bData1];
			break;
		case NOTEOFF: 				// note off
			FMMidiNoteOff(chn, bData1);
			break;
		case NOTEON:				// note on
			if(bData2 == 0) {		// alternative note off: note on with volume 0
				FMMidiNoteOff(chn, bData1);
				break;
			}
			FMMidiNoteOn(chn, bData1, bData2);
			break;
		case PITCHBEND:				// pitchbend data
			FMMidiPitchBend(chn, bData1, bData2);
			break;
		case CTRLCHG:
			switch (bData1) {
				case CC_VOL:			MIDIChn[chn].vol   = bData2; break;
				//case CC_BAL:    		MIDIChn[chn].bal   = bData2; break;
				case CC_BAL:    		MIDIChn[chn].pan   = bData2; break;
				case CC_PAN:    		MIDIChn[chn].pan   = bData2; break;
				case CC_DELAY:  		MIDIChn[chn].delay = bData2; break;
			}
			break;
		default:		
			break; // all other channel messages
	}
	return;
}	

// ---------------------------------------------------------------------------------
// Function: FMMidiSyx
//      Parameters:     size   - size of sysex string  
//						data   - sysex string   
//									a) one voice par
//									   SOX  man-id channel parameter value EOX
//									   0xF0  0x43    chn      par     val  0xF7
//									b) complete parameter list (=17 bytes)
//									   SOX  man-id channel par01 ... par17 EOX
//									   0xF0  0x43    chn   val01 ... val17 0xF7
//      Description:    process sysex data to modify voice parameter
//

void FMMidiSyx(int size, BYTE data[]) {
	if (size != 6 && size !=21) 	return;				// wrong sysex length
	if (data[1] != MAN_ID) 			return;				// wrong manufacturer-id
	BYTE chn = data[2];
	if (chn > 15)       			return;				// wrong midi channel
	SBTIMBRE *timbre = MIDIChn[chn].sound;				// get timbre data for midi channel
	
	if (size == 6) {									// one voice par 
		FMVoicePar(timbre, data[3], data[4]); 			// timbre pointer, parameter, value 
	} else {											// complete parameter list 
		for(int par = 0; par < 17; par++) {					// parameter foreach 
			FMVoicePar(timbre, par, data[par+3]); 		// channel, parameter, value 
		}
	}
	
	for(int v = 0; v < ADLVOICES; v++) {				// search in active voices
		if(ADLVoice[v].midichn == chn) {					// current midi channel
			FMVoice(v, timbre);									// and update voice data
	} 	}
	
}

// ---------------------------------------------------------------------------------
// Function: FMMidiSetINST
//      Parameters:     chn		- midi channel 0 ... 15
//						data	- byte list of 16 elements
//      Description:    set sound data for midi channel
//
int FMMidiSetINST(BYTE chn, SBTIMBRE *data) {
	if(chn > 15) return 0;
	MIDIChn[chn].sound = data;
	return 1;
}

// ---------------------------------------------------------------------------------
// Function: FMMidiGetINST
//      Parameters:	    chn 	- midi channel 0 ... 15
//      Description:    get sound data 
//
SBTIMBRE* FMMidiGetINST(BYTE chn) {
	if(chn > 15) return NULL;
	return MIDIChn[chn].sound;
}

/****************************************************************
 *
 * support for sound patches
 *
 ****************************************************************/

// ---------------------------------------------------------------------------------
// Function: FMnewSBI
//      Description:    create pointer to a SBIFMT structure
//
SBIFMT* FMNewSBI()
{
   SBIFMT *sbi = calloc(1, sizeof(SBIFMT));
   memcpy(sbi->sig,  SBISIG, 4);
   return sbi;
}

// ---------------------------------------------------------------------------------
// Function: FMLoadSBI
//      Parameters:     sbi      - pointer to SBIFMT structure
//                      fileName - name of .SBI file
//      Description:    loads .SBI and create pointer to a SBIFMT structure
//
int FMLoadSBI(SBIFMT* sbi, char fileName[])
{
   FILE *fp;
   if ((fp = fopen(fileName, "rb")) == NULL) 	return 0;
   // read the data
   fread(sbi, sizeof(SBIFMT), 1, fp);
   fclose(fp);
   char sig[4];
   memcpy(sig, sbi->sig, 3);
   sig[3]=0;
   if(strcmp(sig, SBIEXT) != 0)          		return 0;
   return 1;
}

// ---------------------------------------------------------------------------------
// Function: FMSaveSBI
//      Parameters:     pointer to a sbi structure, fileName
//      Description:    write .SBI file
//
int FMSaveSBI(SBIFMT *sbi, char fileName[])
{
   FILE *fp;
   if ((fp = fopen(fileName, "wb")) == NULL) return 0;
   // write the data
   fwrite(sbi, sizeof(SBIFMT), 1, fp);
   fclose(fp);
   return 1;
}

// ---------------------------------------------------------------------------------
// Function: FMnewIBK
//      Description:    create pointer to a IBKFMT structure
//
IBKFMT* FMNewIBK()
{
   IBKFMT *ibk = calloc(1, sizeof(IBKFMT));
   memcpy(ibk->sig,  IBKSIG, 4);
   return ibk;
}
/*
#define IBKEXT   "IBK"
#define SBIEXT   "SBI"
#define SBISIG   "SBI\x1A"		// 4 chars long
#define IBKSIG   "IBK\x1A"     	// 4 chars long
*/ 
// ---------------------------------------------------------------------------------
// Function: FMLoadIBK
//      Parameters:     ibk      - pointer to IBKFMT structure
//                      fileName - name of .IBK file
//      Description:    load .IBK and create pointer to a IBKFMT structure
//
int FMLoadIBK(IBKFMT* ibk, char fileName[])
{
   FILE *fp;
   if ((fp = fopen(fileName, "rb")) == NULL) 	return 0;
   // read the data
   fread(ibk, sizeof(IBKFMT), 1, fp);
   fclose(fp);
   char sig[4];
   memcpy(sig, ibk->sig, 3);
   sig[3]=0;
   if(strcmp(sig, IBKEXT) != 0)          		return 0;
   return 1;
}

// ---------------------------------------------------------------------------------
// Function: FMSetIBK
//      Parameters:     ibk 	- pointer to a IBKFMT structure
//						type	- bank type (0 = instrument, 1 = drum)
//      Description:    replace standard soundset 
//
int FMSetIBK(IBKFMT *ibk, int type) {
	int offset = (type > 0) ? 128 : 0;
	for (int inst=0; inst < 128; inst++) {
		BYTE *p = (BYTE*)&ibk->snd[inst];
		for (int i = 0; i < 16; i++) SOUND[offset + inst][i] = p[i];
	}
	
}

// ---------------------------------------------------------------------------------
// Function: FMSaveIBK
//      Parameters:     pointer to a ibk structure, fileName
//      Description:    write .IBK file
//
int FMSaveIBK(IBKFMT *ibk, char fileName[])
{
   FILE *fp;
   if ((fp = fopen(fileName, "wb")) == NULL) return 0;
   // write the data
   fwrite(ibk, sizeof(IBKFMT), 1, fp);
   fclose(fp);
   return 1;
}

