// mkMidiFile.c
// library to create and play midifile (type 0, 1) on windows
// by ma.ke. 2020
//   	- without warranty
//   	- use at your own risk
//   	- do what ever you want with this
//   	- be happy
//
//	features:
// 		- create, save, load and play midifile type 0 or 1
//		- provide running mode by reading midi file
//      - complete channel messages
//		- meta event tempo
//		- sysex data (F0 ... data ... F7)
//      - midi output interface or internal fmsynth (adlibemu)
// 		- midi input interface 
//
//  limitations:
// 		- time devision only ppqn
//		- without other meta events except tempo
//      - sysex request not supported
//
#include <windows.h>

#define FMSYNTH	  0x0F					// pseudo midi device number for internal fmsynth

//#define MIDIVIEW  	 1					// view played midi data
#define BUFSIZE 	16					// smallest unit of memory buffer
#define SYSEXMAX 	128					// max. size of sysex data string
#define QUEUEMAX	64					// midi message queue size for callback midi in

struct BUF {						
	char    	*mem;					// memory buffer
	int			len;					// length of allocated memory
	int 		cnt;					// write byte counter of mem
	struct BUF 	*next; 					// link to next track (linked list)
};

struct USRTIME {
	BYTE		hh;						// hours
	BYTE    	mm;						// minutes
	BYTE    	ss;						// seconds
	DWORD     	ms;						// milli seconds
};

/****************************************************************************************
 *
 * c api
 *
 ****************************************************************************************/

// functions for track/memory file (incl. link list)
//
struct 	BUF* newTRK();									// create new track
void 	freeTRKs();										// clear memory of all tracks

// functions for midi file
//
struct 	BUF* newSMF(int ppqn);							// create new SMF buffer
int 	writeSMF(char *filename, struct BUF *smf);		// write SMF buffer to file
struct 	BUF* loadSMF(char *filename);					// load file to SMF buffer
void 	freeBUF(struct BUF *smf);						// clear memory

// functions for devices
HMIDIOUT 	openOut(int dev);							// open midi output
void 		resetOut(HMIDIOUT dev);						// reset midi output (all channel note off)
void 		setAudioModeOut(HMIDIOUT dev, BYTE mode);	// set audioMode for device - AUDIO or RENDER(only for FMSYNTH)
int 		closeOut(HMIDIOUT dev);						// close midi output
void 		send_msg(DWORD msg, HMIDIOUT dev);			// send midi message
void 		send_syx(BYTE data[], HMIDIOUT dev);		// send sysex data
HMIDIIN 	openIn(int dev);							// open midi input
void 		closeIn(HMIDIIN dev);						// close midi input
int 		getInData(DWORD *time, DWORD *data);		// get midi in data

// functions for wav stream rendering (only for FMSYNTH)
void 	setFMAudio();									// set audio mode 
void 	setFMRender();									// set render mode
void   	renderFMWav(int msec);							// render wav stream
void   	writeFMWav(char *name); 						// write wave file


// functions for midi file player
//
extern struct PLAYER player;															// player environment
int 	openPlayer(struct BUF *smf, HMIDIOUT dev, struct USRTIME *t);					// open midi player
HANDLE	statusPlayer();																	// get handle of player
void  	getPlayTime(struct USRTIME *t); 												// get playing time
void	closePlayer();																	// close midi player

// functions for writing midi data to track
//
void 	writeMSG(struct BUF *trk, int timediv, BYTE status, BYTE data1, BYTE data2);
void 	writeSYX(struct BUF *trk, BYTE data[]);
void 	writeTMP(struct BUF *trk, int microsec);
