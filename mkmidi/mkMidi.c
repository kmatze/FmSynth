// mkMidi.c by ma.ke. 2021
// library to create and play midifile (type 0, 1) on windows midi devices,  
// specially: use internal fmsynth (adlibemu) to play or render to wave file
//   	- without warranty
//   	- use at your own risk
//   	- do what ever you want with this
//   	- be happy
//
//	features:
// 		- create, save, load and play midifile type 0 or 1
// 		- render midi file through fmsynth to wav file 
//		- provide running mode by reading midi file
//      - complete channel messages
//		- meta event tempo
//		- sysex data (F0 ... data ... F7)
//      - midi output interface or internal fmsynth (adlibemu) inkl. sysex
// 		- midi input interface 
//
//  limitations:
// 		- time devision only ppqn
//		- without other meta events except tempo
//      - sysex request not supported

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include <time.h>      			 	/* clock_t, clock, CLOCKS_PER_SEC */

#include "mkMidi.h"
#include "fmsynth.h"

#define VERSION "1.02.09"
#define VERSDATUM "09-02-2021"

#pragma pack(push, 1)
struct MTHD {
	DWORD 	id;				// id 
	DWORD 	hdrl;			// header length
	WORD	fmt;			// midi format
	WORD	trks;			// number of tracks
	WORD	ppqn;			// ppqn
};
#pragma pack(pop)

struct MTRK {
	DWORD 		time;		// absolute time of track 
	char		*ptr;		// event data pointer
	BYTE		lastStatus;	// rember last status byte
};

enum EVENTS {
	EVT_MTHD		= 0x4D546864,		// midi header ID	: 4 byte ("MThd")
	EVT_MTRK		= 0x4D54726B,		// track header ID	: 4 byte ("MTrk")
	EVT_EOT			= 0xFF2F00,			// end of track		: FF 2F 00
	EVT_SYX			= 0xF0,				// sysex			: F0 length message
	EVT_INST		= 0xFF04,			// meta instrument	: FF 04 len instrumentname
	EVT_TMP			= 0xFF5103,			// tempo			: FF 51 03 tt tt tt (24 bit value)
};

// global variable for player environment
struct PLAYER {
	HANDLE		threadHdl;				// player thread handle,  NULL=stop
	struct MTHD *mhd;					// midi header data
	struct BUF	*stream;				// playing stream
	HMIDIOUT 	dev;					// midi device, if FMSYNTH playing through fmsynth
	BYTE		audiomode;				// AUDIO or RENDER
	// 
	int 		tempo;					// midi tempo in millisec
	int         songtime;				// song time in ms
	int 		playtime;				// play time in ms
	float 		bpm;					// BPM beats per minute
	float 		ms_per_tick;		 	// ms per tick  (ppqn in midi header data *mhd)
} player;

struct PCMDATA {
	char    *data;						// memory pointer
	long    size;						// caculated size in bytes 
	long    cnt;   						// byte counter   
} pcm;

/****************************************************************************************
 *
 * c functions for handle midi data
 *
 ****************************************************************************************/
 
DWORD swap32(DWORD x) {
    return (x>>24)               | 
		   ((x<<8) & 0x00FF0000) |
		   ((x>>8) & 0x0000FF00) |
		   (x<<24);
}
WORD swap16(WORD x) {
	return (x>>8)|(x<<8);
}

/****************************************************************************************
 *
 * c functions for handling TRACK buffers to creating own midifiles 
 *
 ****************************************************************************************/

 // initialize linklist
struct BUF *head 	= NULL;
struct BUF *current = NULL;

// get number of tracks
int getNumTRK() {
	int i = 0;
	struct BUF *trk = head;							// start from the first link
	if(!trk) return i;							    // list is empty
	while(trk) {									// navigate through list  	 		 					
		i++;										// and count
		trk = trk->next;							// go to next link
	}
	return i;
}

// create new track
struct BUF* newTRK() {
	if( getNumTRK() >= 0xFFFF ) return NULL;					    // more then 65535‬
	struct BUF *link = (struct BUF*) malloc(sizeof(struct BUF));	// create a link
	link->mem 	= malloc(sizeof(char) * BUFSIZE);
	link->len 	= BUFSIZE;
	link->cnt 	= 0;
	link->next 	= head;								// point it to old first node
	head = link;									// point first to new first node
	return link;
}

// clear memory for all tracks
void freeTRKs() {
	struct BUF *current = head;		   				// start from beginning
	if(!current) return;							// list is empty
	while(current) {								// navigate through list and
		struct BUF *trk = current;
		current = current->next;					// go to next link
		free(trk->mem);								// clear current track
		free(trk);
	}
	head = current = NULL;							// initialize linklist
	return;
}

/****************************************************************************************
 *
 * c functions to handling general buffers (writing and read)
 *
 ****************************************************************************************/

// write byte to buffer
//
void writeBYTE(struct BUF *buf, BYTE value)	{
	if ( buf->cnt + 1 >= buf->len ) {			// dynamic buffer size
		buf->len += BUFSIZE;
		buf->mem = realloc(buf->mem, sizeof(BYTE) * buf->len); 
	}
	buf->mem[buf->cnt++] = value;
	return;
}

// read byte from buffer
//
BYTE readBYTE(char *ptr) { 
	BYTE byte = *(DWORD*)ptr;
	return byte; 
}

// write variable byte length to buffer (1 ... 4)
//
void writeVAL(struct BUF *buf, DWORD value, int bytes) {
	BYTE b[4];
	memcpy(b, &value, sizeof(DWORD));
	if (bytes > 3) writeBYTE(buf, b[0]);
	if (bytes > 2) writeBYTE(buf, b[1]);
	if (bytes > 1) writeBYTE(buf, b[2]); 	
	if (bytes > 0) writeBYTE(buf, b[3]);
	return;
}

// read variable byte length from buffer (1 ... 4)
//
int readVAL(char *ptr, int bytes) {
	BYTE  	b[4] = {0, 0, 0, 0};
	int 	value = 0;
	if (bytes > 3) b[3] = readBYTE(ptr+3);
	if (bytes > 2) b[2] = readBYTE(ptr+2);
	if (bytes > 1) b[1] = readBYTE(ptr+1);	
	if (bytes > 0) b[0] = readBYTE(ptr);
	memcpy(&value, b, 4);
	return value;
}

// write variable length quantity to buffer
//
void writeVLQ(struct BUF *buf, DWORD value)	{
   DWORD buffer = value & 0x7F;  
   while ( (value >>= 7) ) {
     buffer <<= 8;
     buffer |= ((value & 0x7F) | 0x80);
   }
   while (TRUE) {
	  unsigned char *bytes = (unsigned char *) (&buffer);
	  writeBYTE(buf, bytes[0]);
	  if (buffer & 0x80)
          buffer >>= 8;
      else
          break;
   }
   return;
}

// read variable byte length from buffer (1 ... 4)
//
DWORD readVLQ(unsigned char* buf, int* bytesread) {
	DWORD var = 0;
	unsigned char c;
	*bytesread = 0;

	do {
		c = buf[(*bytesread)++];
		var = (var << 7) + (c & 0x7f);
	} while(c & 0x80);

	return var;
}
 
/****************************************************************************************
 *
 * c functions specialy for writing MIDI data to TRACK buffer
 *
 ****************************************************************************************
 *      ppqn = pulses (ticks) per quarter note			default:	  96
 *       bpm = beats per minute	(1 beat = 1/4 note)		default:	 120
 *     tempo = micro sec per quarter note 				default: 500.000
 * 			   											( = 60.000.000 microsec / bpm )
 * 
 * deltatime = ticks 													deltatime
 *       e.g. ppqn = 96 (default): 	1/1   note = ppqn *  4	= 96 *  4 = 384 ticks
 * 									1/2   note = ppqn *  2	= 96 *  2 = 192 ticks
 * 									1/4   note = ppqn *  1	= 96 *  1 =  96 ticks
 * 									1/8   note = ppqn /  2	= 96 /  2 =  48 ticks
 * 									1/16  note = ppqn /  4	= 96 /  4 =  24 ticks
 * 									1/32  note = ppqn /  8	= 96 /  8 =  12 ticks
 * 									1/64  note = ppqn / 16  = 96 / 16 =   6 ticks
 * 									1/128 note = ppqn / 32  = 96 / 32 =   3 ticks
 * ****************************************************************************************/

 // write MIDI events to TRK (status byte 0x8n - 0xEn + databyte1 + databyte2)
//
void writeMSG(struct BUF *trk, int timediv, BYTE status, BYTE data1, BYTE data2) {
	writeVLQ(trk, timediv);
	writeBYTE(trk, status);
	writeBYTE(trk, data1);
	if((status & 0xf0) == 0xc0) return;			// 2. data byte not needed
	if((status & 0xf0) == 0xd0) return;			// 2. data byte not needed
	writeBYTE(trk, data2);
	return;
}

// get size of sysex data string
//
int getSyxSize(BYTE data[]) {
	for(int i = 0; i < SYSEXMAX; i++) {
		if(data[i] == 0xF7) return i+1;
	}
	return 0;
}

// write SYSEX data to TRK 
// the last byte of byte string must be 0xF7 (EOX)
//
void writeSYX(struct BUF *trk, BYTE data[]) {
	int size = getSyxSize(data);
	if(size) {
		writeVLQ(trk, 0);									// meta event always with timediv=0
		writeVAL(trk, swap32(EVT_SYX), 1); 
		writeVLQ(trk, size);
		for( int i = 0; i < size; i++) writeBYTE(trk, data[i]);
	}
	return;
}

// write meta event TEMPO to TRK ( FF 51 03 tt tt tt )
//
void writeTMP(struct BUF *trk, int microsec) {
	writeVLQ(trk, 0);					// meta event always with timediv=0
	writeVAL(trk, swap32(EVT_TMP), 3);
	writeVAL(trk, swap32(microsec), 3);
	return;
}

/****************************************************************************************
 *
 * c functions for midifile and SMF buffer
 *
 ****************************************************************************************/

// position to next event from given track
// and calculate the absolute time from this event
//
void next_event(struct MTRK *mtrk) {
	int bytes;
	DWORD tdiv = readVLQ((char*)mtrk->ptr, &bytes);		// read event tdiv
	mtrk->ptr  += bytes;
	mtrk->time += tdiv;
	return;
}

// read properties from midi header,
// fill the header structure,
// check if valid midi file
//
struct MTHD *get_MThd(struct BUF *smf) {
	struct MTHD *mthd = malloc(sizeof(struct MTHD));
	if(!smf) return NULL;
	void *ptr = smf->mem;
	mthd->id    = swap32(*(DWORD*)ptr); ptr += sizeof(DWORD);	// id
	mthd->hdrl  = swap32(*(DWORD*)ptr);	ptr += sizeof(DWORD);	// header length
	mthd->fmt   = swap16(*(WORD*)ptr); 	ptr += sizeof(WORD);	// midi format
	mthd->trks  = swap16(*(WORD*)ptr); 	ptr += sizeof(WORD);	// number of tracks
	mthd->ppqn  = swap16(*(WORD*)ptr); 	ptr += sizeof(WORD);	// ppqn
	if(mthd->id != EVT_MTHD) 			return NULL;			// none midi file
	return mthd;
}

// create standard SMF buffer from internal tracks (only type 0 or type 1)
//
struct BUF* newSMF(int ppqn) {
	//initialize buffer for midi file
	struct BUF *smf = (struct BUF*) malloc(sizeof(struct BUF));
	smf->mem = malloc(sizeof(char) * BUFSIZE);
	smf->len = BUFSIZE;
	smf->cnt = 0;
	
	// MIDI FILE HEADER create
	int fmt = 0;
	int trkCnt = getNumTRK();	
	if (trkCnt < 1) return NULL;				// no track data
	if (trkCnt > 1) fmt = 1;					// if more than one track file type = 1
	writeVAL(smf, swap32(EVT_MTHD), 4);			// ID,			  4 byte ("MThd")
	writeVAL(smf, swap32(6), 4);				// HEADER LENGTH, 4 byte 
	writeVAL(smf, swap32(fmt), 2);				// FORMAT,		  2 byte (midi file type = 0, 1, or 2)
	writeVAL(smf, swap32(trkCnt), 2);			// TRACK COUNTER  2 byte (number of tracks = 1 - 65535)
	writeVAL(smf, swap32(ppqn), 2); 			// DIVISION,	  2 byte (ticks per quarter-note)
	
	// TRACK  
	struct BUF *trk = head;		   				// start from beginning
	while( trk ) {
		writeVAL(smf, swap32(EVT_MTRK), 4);		// ID,			  4 byte ("MTrk")
		writeVAL(smf, swap32(trk->cnt + 4), 4);	// TRACK LENGTH,  4 byte length (track size + 4 byte footer)
		for( int i = 0; i < trk->cnt; i++)  	// TRACKDATA,     n byte track stream
			writeBYTE(smf, trk->mem[i]);
		writeVLQ(smf, 0);						// meta event always with timediv=0	
		writeVAL(smf, swap32(EVT_EOT), 3);		// end of track,  3 byte	
		trk = trk->next;						// next track
	}	
	return smf;
}

// write midi file from SMF buffer to file system
//
int writeSMF(char *filename, struct BUF *smf) {
	struct MTHD *mthd = get_MThd(smf);	
	if(!mthd) return FALSE;								// none valid SMF buffer
	FILE *fp = fopen(filename, "w");
	fwrite(smf->mem, smf->cnt, 1, fp);
	fclose(fp);
	return TRUE;
}

// load midi file to SMF buffer
//
struct BUF* loadSMF(char *filename)	{	
	struct BUF *smf = (struct BUF*) malloc(sizeof(struct BUF));
	FILE       *fp = fopen(filename, "rb");
	if(!fp) return NULL;
	fseek(fp, 0, SEEK_END);
	smf->len = ftell(fp);
	smf->cnt = 0;
	fseek(fp, 0, SEEK_SET);
	smf->mem = (char *)malloc(smf->len);
	fread(smf->mem, smf->len, 1, fp);
	fclose(fp);	
	
	// check if midi format 2
	struct MTHD *mthd = get_MThd(smf);
	if(mthd->fmt == 2) {
		freeBUF(smf);
		return NULL;
	}
	
	return smf;
}

// clear memory for SMF buffer
//
void freeBUF(struct BUF *smf) {
	if (!smf) return;
	free(smf->mem);
	free(smf);
	return;
} 

/****************************************************************************************
 *
 * c functions for FMSYNTH: setting audio mode and rendering to wave file
 *
 ****************************************************************************************/
 
// write wav file from wav stream
//
void writeFMWav(char *filename) {
	FILE*   fp =  fopen(filename, "wb");
	int  subChunk1Size  = 16;
	long subChunk2Size  = pcm.size;
	long chunkSize      = 4 + (8 + subChunk1Size) + (8 + subChunk2Size);
	
	// Create WAVE header
	unsigned char hdr[44];
	
	hdr[ 0]='R'; hdr[ 1]='I'; hdr[ 2]='F'; hdr[ 3]='F';						// 4 ChunkID
	hdr[ 8]='W'; hdr[ 9]='A'; hdr[10]='V'; hdr[11]='E';						// 4 Format	
	hdr[12]='f'; hdr[13]='m'; hdr[14]='t'; hdr[15]=' ';						// 4 SubChunk1ID
	hdr[36]='d'; hdr[37]='a'; hdr[38]='t'; hdr[39]='a';						// 4 SubChunk2ID

	*(int*)&hdr[ 4]		= chunkSize;										// 4 ChunkSize
	*(int*)&hdr[16] 	= subChunk1Size;									// 4 SubChunk1Size
	*(short*)&hdr[20] 	= 1;               									// 2 Audioformat 	1=PCM
	*(short*)&hdr[22] 	= NUM_CHANNELS;               						// 2 Channels 		1=mono,2=stereo
	*(int*)&hdr[24] 	= SAMPLE_RATE;     									// 4 SampleRate
	*(int*)&hdr[28] 	= SAMPLE_RATE * NUM_CHANNELS * BYTES_PER_SAMPLE; 	// 4 ByteRate
	*(short*)&hdr[32] 	= NUM_CHANNELS * BYTES_PER_SAMPLE;               	// 2 BlockAlign
	*(short*)&hdr[34] 	= 8 * BYTES_PER_SAMPLE;              				// 2 BitsPerSample
	*(int*)&hdr[40] 	= subChunk2Size;									// 4 SubChunk2Size					

	fwrite(hdr, 44, 1, fp);													// write wav header out to file
    fwrite(pcm.data, 1, pcm.size, fp);										// write pcm data out to file
	fclose(fp);
}

// render wav for msec
//
void renderFMWav(int msec) {
	float sec  			= (float)msec / 1000000;						// calculate sec
	int frames          = SAMPLE_RATE * sec;							//           frames
	long renderSize  	= frames * BYTES_PER_SAMPLE * NUM_CHANNELS;		//           bytes
	char *p 			= pcm.data + pcm.cnt;							// position to end of saved data
	FMGetSample(p, renderSize);											// get ata
	pcm.cnt    		   += renderSize;									// actual saved data
}


/****************************************************************************************
 *
 * c midi device functions
 *
 ****************************************************************************************/

// open midi out device or fmsynth
// 
HMIDIOUT openOut(int dev) {
	HMIDIOUT d = 0;
	if(dev == FMSYNTH) {
		FMOpenSynth();
		AudioInit();
		AudioMode(AUDIO);
		d = (HMIDIOUT)FMSYNTH;	
	} else {
		midiOutOpen(&d, dev, 0, 0, CALLBACK_NULL);
		player.audiomode = AUDIO;
	}
	return d;
}
// reset midi out device or fmsynth
// 
void resetOut(HMIDIOUT dev) {
	if(dev == (HMIDIOUT)FMSYNTH) {
		//AudioMode(RENDER);
		FMReset();
		//AudioMode(AUDIO);
	} else {	
		midiOutReset(dev);
		// sysex strings for midi resets
		BYTE resetGM[] 	= {0xF0, 0x7E, 0x7F, 0x09, 0x01, 0xF7};	
		BYTE resetGM2[] = {0xF0, 0x7E, 0x7F, 0x09, 0x03, 0xF7};
		BYTE resetGS[] 	= {0xF0, 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x7F, 0x00, 0x41, 0xF7};
		BYTE resetXG[]  = {0xF0, 0x43, 0x10, 0x4C, 0x00, 0x00, 0x7E, 0x00, 0xF7};	
		send_syx(resetGM,  dev);
		send_syx(resetGM2, dev);
		send_syx(resetGS,  dev);
		send_syx(resetXG,  dev);
	}
	return; 
}

// set audio mode for fmsynth
// 
void setAudioModeOut(HMIDIOUT dev, BYTE mode) {
	switch (mode) {
		case RENDER:
			if(dev == (HMIDIOUT)FMSYNTH)
				player.audiomode = RENDER;
			else 
				player.audiomode = AUDIO;
			break;
		case AUDIO:
		default:
			player.audiomode = AUDIO; 
			break;
	}
	AudioMode(player.audiomode);
}

// close midi out device or fmsynth
// 
int closeOut(HMIDIOUT dev) {
	if(player.dev == dev && player.threadHdl) return 0;	// player active on dev
	if(dev == (HMIDIOUT)FMSYNTH) {
		AudioClose();
		FMCloseSynth();
	} else {	
		midiOutClose(dev);
	}
	return 1; 
}

// QUEUE definition for midiIn message
//
struct { 
	int write; 
	int read; 
} msgQPos = { 0, 0 };

struct { 
	DWORD time;
	DWORD data;
} msgQ[QUEUEMAX];

// callback function for midi in device
// 
void CALLBACK cbMidiIn(HMIDIIN midi_in, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	switch ( wMsg ) {
		case MIM_OPEN:
		case MIM_CLOSE:
		case MIM_MOREDATA:
		case MIM_ERROR:
		case MIM_LONGDATA:
		case MIM_LONGERROR:
		// Received regular MIDI message and write in msgQueue
		case MIM_DATA:
			// dwParam1: midi message
			msgQ[msgQPos.write].data = dwParam1;
			// dwParam2: time stamp
			msgQ[msgQPos.write].time   = dwParam2;
			if ( msgQPos.write++ >= QUEUEMAX ) msgQPos.write = 0;
			break;
		default:
			break;
	}
}

// open midi in device
// 
HMIDIIN openIn(int dev) {
	HMIDIIN d = 0;
	midiInOpen(&d, dev, (DWORD)(cbMidiIn), (DWORD) NULL, CALLBACK_FUNCTION);
	midiInStart(d);
	return d;
}

// close midi in device
// 
void closeIn(HMIDIIN dev) {
	midiInStop(dev);
	midiInClose(dev);
	return;
}

// read midi in message
//
int getInData(DWORD *time, DWORD *data) {
	if (msgQPos.read != msgQPos.write) { 
		*time = msgQ[msgQPos.read].time;
		*data = msgQ[msgQPos.read].data;
		if (msgQPos.read++ >= QUEUEMAX) msgQPos.read = 0;
		return 1;
	}
	return 0;
}

/****************************************************************************************
 *
 * c functions for playing midi stream
 *
 ****************************************************************************************/

#define T_SEC	   		1000000			// microseconds
#define T_MIN	  	   60000000
#define T_HOUR  	 3600000000

struct USRTIME getTime(int ms) {	
	struct USRTIME t;
	t.ms = ms;
	t.hh = t.ms / T_HOUR;	t.ms %= T_HOUR;
	t.mm = t.ms / T_MIN;	t.ms %= T_MIN;
	t.ss = t.ms / T_SEC; 	t.ms %= T_SEC;
	return t;
}
 
void send_msg(DWORD msg, HMIDIOUT dev) {
	if(dev == (HMIDIOUT)FMSYNTH) {
		FMMidiMsg(msg);
	} else {
		midiOutShortMsg(dev, msg);
	}
	
	//void getChnStatus(struct MIDICHN *chn) { 
	
#ifdef MIDIVIEW 
	int b0 = (BYTE) ((msg & 0x000000FF)>>0);
	int b1 = (BYTE) ((msg & 0x0000FF00)>>8);
	int b2 = (BYTE) ((msg & 0x00FF0000)>>16);
	printf("MSG: %02X %02X %02X\n", b0, b1, b2);
#endif
}

void send_syx(BYTE data[], HMIDIOUT dev) {
	unsigned char *syxstr; 
	int size = getSyxSize(data);
	
#ifdef MIDIVIEW	
	printf("SYX:(%2i)", size);
#endif

	if(!size) return;
	
	if(data[0] == EVT_SYX) {									// startbyte is 0xF0
		syxstr 	= malloc(sizeof(unsigned char) * size);
		for(int i = 0; i < size; i++) syxstr[i] = data[i];
	} else {													// startbyte must be 0xF0
		size++;
		syxstr 	= malloc(sizeof(unsigned char) * size);
		syxstr[0] 	= 0xF0;
		for(int i = 1; i < size; i++) syxstr[i] = data[i-1];
	}
	
	if(dev == (HMIDIOUT)FMSYNTH) {
		FMMidiSyx(size, syxstr);
	} else {
		MIDIHDR	midiHdr;
		UINT    err;
		HANDLE 	hBuffer = GlobalAlloc(GHND, size); 			// allocate buffer for string
		if (hBuffer) {
			midiHdr.lpData = (LPBYTE)GlobalLock(hBuffer);
			if (midiHdr.lpData) {
				midiHdr.dwBufferLength = size;
				midiHdr.dwFlags = 0;				// Flags must be set to 0
				midiOutPrepareHeader(dev, &midiHdr, sizeof(MIDIHDR));
				memcpy(midiHdr.lpData, syxstr, size);
				midiOutLongMsg(dev, &midiHdr, sizeof(MIDIHDR));
				midiOutUnprepareHeader(player.dev, &midiHdr, sizeof(MIDIHDR));
				GlobalUnlock(hBuffer);
			}
			GlobalFree(hBuffer);
		}
	}
	
#ifdef MIDIVIEW	
	for (int i=0; i<size; i++) printf(" %02X", readBYTE(syxstr+i));
	printf("\n");
#endif
}


// create stream buffer of midi events from SMF buffer
//
struct BUF* getMidiStream(struct BUF *smf) {
	//initialize buffer for midistream
	struct BUF *stream = (struct BUF*) malloc(sizeof(struct BUF));
	stream->mem  = malloc(sizeof(char) * BUFSIZE);
	stream->len  = BUFSIZE;
	stream->cnt  = 0;
	
	// calculate songtime	
	// 		tempo: 	micro sec per quarter note
	// 		ppqn:	pulses (ticks) per quarter note
	struct PLAYER *p = &player;
	p->songtime 	 = 0;									// songtime in microsec
	p->tempo		 = 500000;								// default tempo value, can change with meta tempo in track
	p->ms_per_tick   = (float)p->tempo / p->mhd->ppqn;		// ms per tick
	p->bpm			 = T_MIN / (float)p->tempo;				// BPM beats per minute
	
	// mix all tracks to one midi event stream	
	//
	// initialize trackmixer
	int trks = player.mhd->trks;
	struct MTRK mtrk[trks];
	char *ptr  = smf->mem + sizeof(struct MTHD);
	int  i;												// track number
	int  id, size, end;

	for (i = 0; i < trks; i++ ) {
		ptr += 0;		id = swap32(readVAL(ptr,4));	// track ID
		ptr += 4;		size = swap32(readVAL(ptr,4));	// track size
		ptr += 4;		mtrk[i].ptr	= ptr;				// track start pointer			
		ptr += size-4;	end = swap32(readVAL(ptr,4));	// track end

		if(!(id  & EVT_MTRK)) return NULL;				// none valid track begin
		if(!(end & EVT_EOT )) return NULL;				// none valid track end
		
		ptr += 4;										// pos to next track
		mtrk[i].time = 0;								// init time value for track
		mtrk[i].lastStatus = 0;							// init last status of track
		next_event(&mtrk[i]);							// pos to first Event in track
	}
	
	// loop foreach event
	int   songTicks	= 0;								// absolute song time in ticks
	int   readTrks	= trks;								// number of reading tracks
	int   eTicks, eTrk, eTdiv;							// closest event absolute time in ticks, track, tdiv in ticks
	float eTdivTime;									// tdiv in ms		
	BYTE b0, b1, b2;
	int len, bytes;

	int evtCnt = 0;
	do {
		//find closest event
		eTicks = 0xFFFFFFFF;
		for (i = 0; i < trks; i++) {					// for every track
			if(mtrk[i].ptr != NULL)	{					// is there data in track
				if(eTicks > mtrk[i].time) {				// merge closest event
					eTicks = mtrk[i].time;				//
					eTrk  = i;							//
		}	}	}

		// process closest event		
		eTdiv		 = eTicks - songTicks;				// event tdiv
		songTicks 	 = eTicks;							// whole song time in ticks
		ptr			 = mtrk[eTrk].ptr;					// pointer to current event
		b0			 = readBYTE(ptr++);					// get next byte
		
		// calculate absolute songtime in microsec
		eTdivTime    = eTdiv * p->ms_per_tick;
		p->songtime += eTdivTime;	
		
		if ( b0 < 0xF0 ) {								// status byte = channel message
			if(!(b0 & 0x80)) {								// channel message running mode
				b1 = b0;											// first data byte
				b0 = mtrk[eTrk].lastStatus;							// get last status byte
				b2 = 0;
				if(!((b0 & 0xf0) == 0xc0 || (b0 & 0xf0) == 0xd0))	// 2. data byte is needed
					b2 = readBYTE(ptr++);
				writeMSG(stream, eTdiv, b0, b1, b2);

			} else {										// channel message normal mode
				mtrk[eTrk].lastStatus = b0;
				b1 = readBYTE(ptr++);
				b2 = 0;
				if(!((b0 & 0xf0) == 0xc0 || (b0 & 0xf0) == 0xd0))	// 2. data byte is needed
					b2 = readBYTE(ptr++);
				writeMSG(stream, eTdiv, b0, b1, b2);
			}
			mtrk[eTrk].ptr = ptr;
			next_event(&mtrk[eTrk]);
		} 
		
		if ( b0 == 0xF0) {						// sysex data found
			len  = readVLQ(ptr, &bytes);			// read len of data
			writeVLQ(stream, eTdiv);				// write tdiv
			writeBYTE(stream, b0);					// write sysex meta type
			writeVLQ(stream, len);					// write vlq of bytes
			ptr += bytes; 
			for(int i = 0; i < len; i++) writeBYTE(stream, readBYTE(ptr++));
			mtrk[eTrk].ptr = ptr;
			next_event(&mtrk[eTrk]);
			mtrk[eTrk].lastStatus = 0;
		}
		
		if ( b0 == 0xFF ) {						// start byte meta event
			int t0, t1, t2;
			b1 = readBYTE(ptr++);					// meta type
			switch (b1) {
				case 0x00:								// META sequence number
					ptr += 3;								// 1 byte len, 2 byte Data
					mtrk[eTrk].ptr = ptr;					// ignore and skip complete META
					next_event(&mtrk[eTrk]);				// 
					break;
				case 0x01:								// META text event
				case 0x02:								// META copyright notice
				case 0x03:								// META sequence/track name
				case 0x04:								// META instrument name
				case 0x05:								// META lyrics
				case 0x06:								// META marker
				case 0x07:								// META cue point
				case 0x09:								// META device(port) name
				case 0x7F:								// META sequencer spezific
					len  = readVLQ(ptr, &bytes);			// read len of data
					ptr += bytes + len; 					// whole length
					mtrk[eTrk].ptr = ptr; 					// ignore and skip complete META
					next_event(&mtrk[eTrk]);
					break;
				case 0x20:								// META midi channel prefix
				case 0x21:								// META midi port
					ptr +=2;								// 1 byte len, 1 byte data
					mtrk[eTrk].ptr = ptr;					// ignore and skip complete META
					next_event(&mtrk[eTrk]);
					break;
				case 0x2f: 								// META end of track
					mtrk[eTrk].ptr = NULL;					// mark track is end
					--readTrks;								// reduce the number of reading tracks
					break;
				case 0x51: 								// META tempo
					writeVLQ(stream, 0);					//write complete meta event to stream
					writeBYTE(stream, b0);					//
					writeBYTE(stream, b1);					//
					// calculate new tempo
					t0  = readBYTE(ptr+1); 					// 1. data byte
					t1  = readBYTE(ptr+2);					// 2. data byte
					t2  = readBYTE(ptr+3);					// 3. data byte
					p->tempo = (t0 << 16) | (t1 << 8) | t2;
					p->ms_per_tick	= (float)p->tempo / p->mhd->ppqn;	// ms per tick
					p->bpm			= T_MIN / (float)p->tempo;			// BPM beats per minute
					//
					len = 4;								// 1byte len + 3 byte data
					for( int i = 0; i < len; i++)			// write event data
						writeBYTE(stream, readBYTE(ptr++));
					mtrk[eTrk].ptr = ptr;
					next_event(&mtrk[eTrk]);
					break;
				case 0x54:								// META smtp offset
					ptr += 6;								//  1 byte len + 5 byte data
					mtrk[eTrk].ptr = ptr;					// ignore and skip complete META
					next_event(&mtrk[eTrk]);
					break;
				case 0x58:								// META time signature
					ptr += 5;								//  1 byte len + 4 byte data
					mtrk[eTrk].ptr = ptr;					// ignore and skip complete META
					next_event(&mtrk[eTrk]);
					break;
				case 0x59:								// META key signature
					ptr += 3;								//  1 byte len + 4 byte data
					mtrk[eTrk].ptr = ptr;					// ignore and skip complete META
					next_event(&mtrk[eTrk]);
					break;
				default:								// UNKNOWN!!!
					printf("UNKNOWN event %i: 0x%2X\n", evtCnt, b1);
					return NULL;
					break;
			}
		} 
	} while (readTrks > 0);								// no more events

	return stream;
}

// call back / thread function to play midistream
//
DWORD WINAPI cb_player(void *data) {
	struct PLAYER *p = data;
	DWORD midiMsg;
	char *ptr	  = p->stream->mem;						// midi stream
	char *ptrLast = p->stream->mem + p->stream->cnt;	// last memory adress of stream
	int len, bytes;
	BYTE b0, b1, b2;

	// timing tools for playing
	float nextEvtTime, tdivTime, currentTime, startTime = (float)clock();
	unsigned int tdiv;
	
	p->songtime 	 = 0;									// songtime in microsec
	p->tempo		 = 500000;								// default tempo value, can change with meta tempo in track
	p->ms_per_tick   = (float)p->tempo / p->mhd->ppqn;		// ms per tick
	p->bpm			 = T_MIN / (float)p->tempo;				// BPM beats per minute

	struct USRTIME t;
	
	// loop foreach event
	int evtCnt = 0;
	do {
		evtCnt++;
		tdiv         = readVLQ(ptr, &bytes);					// get event tdiv
		tdivTime     = p->ms_per_tick * tdiv;					// calculate eent duration
		p->playtime += tdivTime;								// calculate new absolute playtime in microsecs		
		
#ifdef MIDIVIEW 
		t = getTime(p->playtime);
		printf("[%4i] ", evtCnt);
		printf("%.2d:%.2d:%.2d:%.6d ", t.hh, t.mm, t.ss, t.ms);
		printf("- tdiv: %4i ", tdiv);		
#endif	

		// check if render or audio mode
		if (p->audiomode == RENDER ) {
			if (tdivTime > 0) renderFMWav(tdivTime);			// render samples from FMSYNTH
		} else {
			nextEvtTime = p->playtime /1000;					// calculate next Event start in millisec	
			while((float)clock() - startTime < nextEvtTime);	// wait until next event start time
		}
				
		ptr += bytes;
		b0 = readBYTE(ptr++);								// get next byte
		if ( b0 < 0xF0 ) {									// status byte = channel message
			b1 = readBYTE(ptr++);
			b2 = 0;
			if(!((b0 & 0xf0) == 0xc0 || (b0 & 0xf0) == 0xd0)) {	// 2. data byte is needed
				b2 = readBYTE(ptr++);
				midiMsg = (b2 << 16) | (b1 << 8) | b0;
			} else {
				midiMsg = (b1 << 8) | b0;
			}
			send_msg(midiMsg, player.dev);							// send midi msg
		} 
		
		if ( b0 == 0xF0 ) {								// byte for sysex data
			len  = readVLQ(ptr, &bytes);				// read len of data
			ptr += bytes; 
			send_syx(ptr, player.dev);
			ptr += len;
		}
		
		if ( b0 == 0xFF ) {								// byte for meta start
			b1 = readBYTE(ptr++);						// byte for meta type
			if (b1 == 0x51) {								// meta TEMPO
				ptr++;											// skip len of data
				b0  = readBYTE(ptr++); 
				b1  = readBYTE(ptr++);
				b2  = readBYTE(ptr++);
				p->tempo = (b0 << 16) | (b1 << 8) | b2;
				p->ms_per_tick	= (float)p->tempo / p->mhd->ppqn;	// ms per tick
				p->bpm			= T_MIN / (float)p->tempo;			// BPM beats per minute
#ifdef MIDIVIEW 
				printf("TMP: %6i, ppqn:%4i\n",p->tempo, p->mhd->ppqn);
#endif
			}
		}
		
	} while (ptr < ptrLast && p->threadHdl);							// while more events
	
	closePlayer();
	return 1;
}

// initialize midi player and play midistream, get USRTIME, set audiomode
// 
int openPlayer(struct BUF *smf, HMIDIOUT dev, struct USRTIME *songtime) {
	if(player.threadHdl) return 0;									// player is active
	player.dev = dev;

	// proof midi file header
	player.mhd = get_MThd(smf);
	if(!player.mhd) return 0;	 									// no valid midifile	
	player.tempo 		= 500000;									// default value ms per beat

	// generate playable midi stream from smf file/memory
	freeBUF(player.stream);
	player.stream	= getMidiStream(smf);
	if(!player.stream) {
		printf("error convert midifile to midistream\n");
		return 0;
	}

	*songtime = getTime(player.songtime);							// get songtime
	
	// check if render mode
	if (player.audiomode == RENDER) {
		AudioMode(RENDER);
		// allocate memory for rendering
		float sec  	= (float)player.songtime / 1000000;
		pcm.size 	= (long)(BYTES_PER_SAMPLE * NUM_CHANNELS * SAMPLE_RATE * sec);
		free(pcm.data);
		pcm.data 	= (unsigned char *)malloc(sizeof(unsigned char) * pcm.size);
		pcm.cnt 	= 0;
	} 


	// initialize the callback player function
	player.threadHdl	= CreateThread(NULL, 0, cb_player, &player, 0, NULL);
	if(!player.threadHdl) return 0;
	SetThreadPriority(player.threadHdl, HIGH_PRIORITY_CLASS);
	return 1;
}

HANDLE statusPlayer() { return player.threadHdl; }

void getPlayTime(struct USRTIME *time) { 
	*time = getTime(player.playtime);
}

void closePlayer() { 
	if (player.audiomode == RENDER) {			// FMSYNTH in render mode
		writeFMWav("render.wav");
		AudioMode(AUDIO);
	};
	// close player
	player.threadHdl = NULL;
	player.playtime = player.songtime = 0;
	resetOut(player.dev);
}
