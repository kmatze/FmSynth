// mk.c -> wrapper library to use mkMidi.c to create and play midifile (type 0, 1)
//         on windows midi devices, specially: fmsynth (adlibemu)
// by ma.ke. 2021
//   - without warranty
//   - use it on your own risk
//   - do what ever you want with this
//   - be happy
//
//	commands:
//		mk::help     view this help
//		mk::list     get device list
//		mk::open     open device -> get new cmd for midi device or fmsynth 
//		mk::track    create and processing midi tracks -> get new command
//		mk::play     play / render midi file
//
// 		every command can show help informations: cmd ?

#include <windows.h>
#include <mmsystem.h>
#include <tcl.h>
#include "mkmidi.h"
#include "fmsynth.h"

#define AUDIO  0
#define RENDER 1


#define NS "mk"
#define VERSION "1.02.09"
#define VERSDATUM "09-02-2021"

// tcl definitions and maakros
//
#define STRGLEN				128
#define TCL_MSG(m)      	Tcl_AppendResult(interp, fstring("%s", m), NULL);
#define TCL_ERR(m)      	{ Tcl_AppendResult(interp, fstring("%s", m), NULL); return TCL_ERROR; }

static int hdl_dev  = 0;		// handle counter for devices
static int hdl_trk  = 0;    	//                    tracks
static int hdl_ibk  = 0;    	//                    ibk instrument bank
static int hdl_sbi  = 0;    	//                    sbi instrument file
int trkCounter		= 0;		// number of tracks

HMIDIOUT devOUT 	= NULL;		// hardware midi out
HMIDIOUT devFM  	= NULL;		// internal fmsynth
HMIDIIN  devIN  	= NULL;		// hardware midi in
int trackCounter	= 0;		// number of tracks

/****************************************************************************************
 *
 * c utilities
 *
 ****************************************************************************************/

// generate one string from format with n format strings
const char* fstring( char* fmt, ...) {
    // eg.: fstring("test %s", string);
    static char buf[STRGLEN];
    va_list vl;
    va_start(vl, fmt);
    vsnprintf( buf, sizeof( buf), fmt, vl);
    va_end( vl);
    return buf;
}

/****************************************************************************************
 *
 * tcl procedures for midi devices and smf
 *
 ****************************************************************************************/
//
// command for SBI handle
//
int sbiCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
	if(argc == 1) TCL_ERR("methods: use get set load save close");
	SBIFMT *sbi = (SBIFMT*)clientData;
	
	// use instrument as new patch for channel
	if(strcmp(argv[1], "use")  == 0) {
		if (argc != 3) 										TCL_ERR("method use: chn");
		int chn;
		if (Tcl_GetInt(interp, argv[2], &chn) != TCL_OK) 	return TCL_ERROR;
		if (chn > 15)                      					TCL_ERR("parameter chn: 0...15");
		if(!FMMidiSetINST(chn, &sbi->snd))		TCL_ERR("failure to use instrument");
		return TCL_OK;
	}

	// get instrument data
	if(strcmp(argv[1], "get")  == 0) {
		TCL_MSG(fstring("{%s} ", sbi->name));
		BYTE *p = (BYTE*)&sbi->snd;
		for (int i = 0; i < 16; i++) TCL_MSG(fstring("%i ", p[i])); 
		return TCL_OK;
	}
	
	// set instrument data
	if(strcmp(argv[1], "set")  == 0) {
		if (argc != 3) 											TCL_ERR("method set: timbre(list: name + 16 bytes)");
		char data[16];
		int listArgC, byte;
		char **listArgV;
		if (Tcl_SplitList(interp, argv[2], &listArgC, &listArgV) != TCL_OK) TCL_ERR("trouble with timbre list");
		if (listArgC != 17)					               		TCL_ERR("too view data for timbre list");
		
		memcpy(sbi->name, listArgV[0], 31);

		for (int i = 0; i < 16; i++) {
			if (Tcl_GetInt(interp, listArgV[i+1], &byte) != TCL_OK) return TCL_ERROR;
			if (byte > 255)                      				TCL_ERR(fstring("%i is not a byte value", byte));
			if (i == 13 && byte > 127)							TCL_ERR("drum pitch 0...127");
			data[i] = byte;
		}
		memcpy(&sbi->snd, &data[0], 16);
		return TCL_OK;
	}
	
	// save load instrument data from sbi file
	if (strcmp(argv[1], "load")  == 0) {
		if (argc < 3) 								TCL_ERR("nethod load: SBIfile");
		FMLoadSBI(sbi, argv[2]);
		if(!FMLoadSBI(sbi, argv[2]))				TCL_ERR("failure load SBIfile");
		return TCL_OK;
	}	
	
	// save instrument data to sbi file
	if(strcmp(argv[1], "save")  == 0) {
		if (argc != 3) 								TCL_ERR("parameter save: filename");
		FMSaveSBI(sbi, argv[2]);
		return TCL_OK;
	}
	
	// close handle
	if(strcmp(argv[1], "close")  == 0) {
		free(sbi);
		Tcl_DeleteCommand(interp, argv[0]);
		return TCL_OK;
	}
	
	TCL_ERR("methods: use get set load save close");
}
 
//
// command for IBK handle
//
int ibkCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
	if(argc == 1) TCL_ERR("methods: list use get set load save close");
	IBKFMT *ibk = (IBKFMT*)clientData;
	
	// list instruments
	if(strcmp(argv[1], "list")  == 0) {
		for (int i = 0; i < 128; i++) {
			TCL_MSG(fstring("{%s} ", ibk->name[i]));
		}
		return TCL_OK;
	}
	
	// use instrument as new patch for channel
	if(strcmp(argv[1], "use")  == 0) {
		if (argc != 4) 											TCL_ERR("method use: chn inst");
		int chn, inst;
		if (Tcl_GetInt(interp, argv[2], &chn) != TCL_OK) 		return TCL_ERROR;
		if (Tcl_GetInt(interp, argv[3], &inst)  != TCL_OK) 		return TCL_ERROR;
	    if (chn > 15 || inst > 127)                      		TCL_ERR("parameter patch: 0..15 / inst: 0..127");
		if(!FMMidiSetINST(chn, &ibk->snd[inst]))				TCL_ERR("failure to use instrument");
		return TCL_OK;
	}
	
	// get instrument data 
	if(strcmp(argv[1], "get")  == 0) {
		if (argc != 3) 										TCL_ERR("method get: inst");
		int inst;
		if (Tcl_GetInt(interp, argv[2], &inst)  != TCL_OK) 	return TCL_ERROR;
		if (inst > 127) 								    TCL_ERR("parameter inst: 0...127");
		TCL_MSG(fstring("{%s} ", ibk->name[inst]));
		BYTE *p = (BYTE*)&ibk->snd[inst];
		for (int i = 0; i < 16; i++) TCL_MSG(fstring("%i ", p[i])); 
		return TCL_OK;
	}
	
	// set instrument data
	if(strcmp(argv[1], "set")  == 0) {
		if (argc != 4) 											TCL_ERR("method set: inst timbre(list: name + 16 bytes)");
		char data[16];
		int listArgC, inst, byte;
		char **listArgV;

		if (Tcl_GetInt(interp, argv[2], &inst) != TCL_OK) 		return TCL_ERROR;
		if (inst > 127) 								    	TCL_ERR("parameter inst: 0...127");

		if (Tcl_SplitList(interp, argv[3], &listArgC, &listArgV) != TCL_OK) TCL_ERR("trouble with timbre list");
		if (listArgC != 17)					               		TCL_ERR("too view data for timbre list");
		
		memcpy(ibk->name[inst], listArgV[0], 8);
		
		for (int i = 0; i < 16; i++) {
			if (Tcl_GetInt(interp, listArgV[i+1], &byte) != TCL_OK) return TCL_ERROR;
			if (byte > 255)                      				TCL_ERR(fstring("%i is not a byte value", byte));
			if (i == 13 && byte > 127)							TCL_ERR("drum pitch 0...127");
			data[i] = byte;
		}
		memcpy(&ibk->snd[inst], &data[0], 16);
		return TCL_OK;
	}	

	// load instrument bank from ibk file
	if (strcmp(argv[1], "load")  == 0) {
		if (argc < 3) 								TCL_ERR("method load: IBKfile")
		if(FMLoadIBK(ibk, argv[2]) == 0)			TCL_ERR("failure load IBKfile");
		return TCL_OK;
	}
	
	// save instrument bank to ibk file 
	if(strcmp(argv[1], "save")  == 0) {
		if (argc != 3) 								TCL_ERR("method save: filename");
		FMSaveIBK(ibk, argv[2]);
		return TCL_OK;
	}

	// close handle
	if(strcmp(argv[1], "close")  == 0) {
		free(ibk);
		Tcl_DeleteCommand(interp, argv[0]);
		return TCL_OK;
	}
	
	TCL_ERR("methods: list use get set load save close");
}
 
//
// command for output device handle
//
int midiOutCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
	if (argc == 1) TCL_ERR("method ?argument ...?");
	HMIDIOUT dev = (HMIDIOUT)clientData;

	// get handle for device
	if (strcmp(argv[1], "hdl")  == 0) {
		TCL_MSG(fstring("%d", dev));
		return TCL_OK;
	}

	// send midi message
	if (strcmp(argv[1], "msg")  == 0) {
		if (argc != 5) 								TCL_ERR("method msg: status data1 data2");
		DWORD msg;
		int status, data1, data2;
		if (Tcl_GetInt(interp, argv[2], &status) != TCL_OK) return TCL_ERROR;
		if (Tcl_GetInt(interp, argv[3], &data1)  != TCL_OK) return TCL_ERROR;
		if (Tcl_GetInt(interp, argv[4], &data2)  != TCL_OK) return TCL_ERROR;
		if(status > 0xFF || data1 > 0xFF || data2 > 0xFF) 
			TCL_ERR("at least one value is greater than 0xFF");
		msg = (data2 << 16) | (data1 << 8) | status;
		send_msg(msg, dev);
		return TCL_OK;
	}

	// send sysex data
	if (strcmp(argv[1], "syx")  == 0) {	
		if (argc < 3) 								TCL_ERR("method syx: 0xF0 ?byte ...? 0xF7");
		int b;
		unsigned char *data = malloc(sizeof(unsigned char) * argc); 
		
		int argument = 2, i = 0;
		while(argument < argc) {
			if (Tcl_GetInt(interp, argv[argument], &b) != TCL_OK) return TCL_ERROR;
			if(b >  0xFF) TCL_ERR((fstring("%i is not a byte value\n", b)));
			data[i++] = b;
			if(b == 0xF7) 						break;				// EOX found
			if(argument == argc-1 && b != 0xF7) data[i] = 0xF7;   	// last  byte allways 0xF7
			argument++;
		}
		send_syx(data, dev);
		return TCL_OK;
	}

	// close device
	if (strcmp(argv[1], "close")  == 0) {
		if(!closeOut(dev)) 							TCL_ERR("failure close device");
		if(dev == (HMIDIOUT)FMSYNTH) 
			devFM = NULL;
		else devOUT = NULL;
		Tcl_DeleteCommand(interp, argv[0]);
		return TCL_OK;
	}
	if(dev != (HMIDIOUT)FMSYNTH) 					TCL_ERR("methods(midi): hdl msg syx close");
	
	// create a new handle for sbi
	if (strcmp(argv[1], "sbi")  == 0) {
		SBIFMT *sbi;
		sbi = FMNewSBI();
		if(!sbi)									TCL_ERR("failure create SBI handle");
		char handleName[13 + TCL_INTEGER_SPACE];
		sprintf(handleName, "sbi%d", hdl_sbi++);
		Tcl_CreateCommand(interp, handleName, (Tcl_CmdProc*)sbiCmd,
											  (ClientData*)sbi,
										      (Tcl_CmdDeleteProc *)NULL);
		TCL_MSG(fstring("%s", handleName));
		return TCL_OK;
	}
	
	// load ibk file and create a new handle
	if (strcmp(argv[1], "ibk")  == 0) {
		IBKFMT *ibk;
		ibk = FMNewIBK();
		if(!ibk)									TCL_ERR("failure load IBKFile");
		char handleName[13 + TCL_INTEGER_SPACE];
		sprintf(handleName, "ibk%d", hdl_ibk++);
		Tcl_CreateCommand(interp, handleName, (Tcl_CmdProc*)ibkCmd,
											  (ClientData*)ibk,
										      (Tcl_CmdDeleteProc *)NULL);
		TCL_MSG(fstring("%s", handleName));
		return TCL_OK;
	}

	// get intstrument from midi channel as byte list
	if (strcmp(argv[1], "inst")  == 0) {
		if (argc < 3) 											TCL_ERR("method inst: chnannel");
		int chn;
		unsigned char *instrument;
		if(Tcl_GetInt(interp, argv[2], &chn) != TCL_OK) 		return TCL_ERROR;
		if(chn > 15) 											TCL_ERR("channel value outside range");
		TCL_MSG(fstring("chnInst_%02i", chn));
		instrument = (char*)FMMidiGetINST((BYTE)chn);
		for (int i = 0; i < 16; i++) TCL_MSG(fstring(" %i", instrument[i])); 
		return TCL_OK;
	}	
	
	// get intstrument from internal soundbank as byte list
	if (strcmp(argv[1], "sound")  == 0) {
		if (argc < 3) 											TCL_ERR("method sound: number");
		int sound;
		unsigned char *instrument;
		if(Tcl_GetInt(interp, argv[2], &sound) != TCL_OK) 		return TCL_ERROR;
		if(sound > 255) 										TCL_ERR("sound value outside range");
		TCL_MSG(fstring("sound_%03i", sound));
		instrument = (char*)FMGetTimbre((BYTE)sound);
		for (int i = 0; i < 16; i++) TCL_MSG(fstring(" %i", instrument[i])); 
		return TCL_OK;
	}
	
	// get voice status
	if (strcmp(argv[1], "status")  == 0) {
		int activeNotes, nn, vol, chn;
		
		TCL_MSG("{ ");
		for (int i=0; i<9; i++) {
			TCL_MSG(fstring("%i ", ADLVoice[i].note));
		}
		TCL_MSG("} {");
		for (int i=0; i<16; i++) {
			TCL_MSG(fstring("%i %i ", MIDIChn[i].lastNoteVolume, MIDIChn[i].noteCounter));
		}
		TCL_MSG("}");
		return TCL_OK;
	}
	
	TCL_ERR("methods(fmsynth): hdl msg syx sbi ibk inst sound status close");
}

//
// command for input device handle
//
int midiInCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
	if(argc == 1) TCL_ERR("methods: get close");
	HMIDIIN dev = (HMIDIIN)clientData;

	// get midi message
	if(strcmp(argv[1], "get")  == 0) {
		DWORD time = 0, data = 0; 
		while(getInData(&time, &data)) { 		// read all events from input queue
			// data: midi message
			BYTE status = (BYTE) ((data & 0x000000FF)>> 0);
			BYTE data1  = (BYTE) ((data & 0x0000FF00)>> 8);
			BYTE data2  = (BYTE) ((data & 0x00FF0000)>>16);
			TCL_MSG(fstring("{%i %i %i %i} ", time, status, data1, data2));
		}
		return TCL_OK;
	}

	// close device
	if(strcmp(argv[1], "close")  == 0) {
		closeIn(dev);
		devIN = NULL;
		Tcl_DeleteCommand(interp, argv[0]);
		return TCL_OK;
	}
	
	TCL_ERR("methods: get close");
}

//
// command for track handle
//
int trackCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
	if(argc < 2) TCL_ERR("methods: msg syx tmp close");
	struct BUF *trk = (struct BUF*)clientData;
	
	// event midi message
	if(strcmp(argv[1], "msg")  == 0) {
		if(argc != 6) 								TCL_ERR("method msg: timediv status data1 data2");
		int tdiv, status, data1, data2;
		if(Tcl_GetInt(interp, argv[2], &tdiv)    != TCL_OK) 	return TCL_ERROR;
		if(Tcl_GetInt(interp, argv[3], &status)  != TCL_OK) 	return TCL_ERROR;
		if(Tcl_GetInt(interp, argv[4], &data1)   != TCL_OK) 	return TCL_ERROR;
		if(Tcl_GetInt(interp, argv[5], &data2)   != TCL_OK) 	return TCL_ERROR;
		if(status > 0xFF || data1 > 0xFF || data2 > 0xFF) 
			TCL_ERR("at least one value is greater than 0xFF");
		writeMSG(trk, tdiv, status, data1, data2);
		return TCL_OK;
	}
	
	// event midi sysex data list
	if(strcmp(argv[1], "syx")  == 0) {
		if(argc < 4) 								TCL_ERR("method syx: 0xF0 ?byte ...? 0xF7");
		unsigned char *data = malloc(sizeof(unsigned char) * argc); 
		int argument = 2, i = 0, b;
		while(argument < argc) {
			if(Tcl_GetInt(interp, argv[argument], &b) != TCL_OK) return TCL_ERROR;
			if(b > 0xFF) 							TCL_ERR((fstring("%i is not a byte value\n", b)));
			data[i++] = b;
			if(b == 0xF7) break;									// EOX found
			if(argument == argc-1 && b != 0xF7) data[i] = 0xF7;   	// last  byte allways 0xF7
			argument++;
		}
		writeSYX(trk, data);
		return TCL_OK;
	}
	
	// event midi tempo
	if(strcmp(argv[1], "tmp")  == 0) {
		if (argc != 3) 								TCL_ERR("method tmp: microsec");
		int  microsec;
		if (Tcl_GetInt(interp, argv[2], &microsec)    != TCL_OK) 	return TCL_ERROR;
		writeTMP(trk, microsec);
		return TCL_OK;
	}	
	
	// close track
	if(strcmp(argv[1], "close")  == 0) {
		Tcl_DeleteCommand(interp, argv[0]);
		--trackCounter;
		return TCL_OK;
	}
	
	TCL_ERR("methods: msg syx tmp close");
} 

/****************************************************************************************
 *
 * package mk general tcl procedures
 *
 ****************************************************************************************/
//
// show help screen 
//
 int Help_cmd(ClientData cdata, Tcl_Interp *interp, int argc, char *argv[]) {
	TCL_MSG("tcl library to use midi and adlibemu (as midi controllable fmsynth)\n");
	TCL_MSG("version " VERSION ", " VERSDATUM " (c) ma.ke.\n");
	TCL_MSG("------------------------------------\n");
	TCL_MSG(NS "::help     view this help\n");
	TCL_MSG(NS "::list     get device list\n");
	TCL_MSG(NS "::open     open device\n");
	TCL_MSG(NS "::track    create and processing midi tracks\n");
	TCL_MSG(NS "::play     play midi file (audio or render)\n");
	TCL_MSG(NS "::cmd ?    get help of command\n");
	return TCL_OK;
}

//
// list all midi devices
//
int MidiList_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
	MIDIOUTCAPS moc;
	MIDIINCAPS  mic;
	int i, max;

	// midiOUT devices
	for (i = 0, max = midiOutGetNumDevs(); i < max; i++) {
		midiOutGetDevCaps(i, &moc, sizeof(moc));
		TCL_MSG(fstring("out %i (%s)\n", i, moc.szPname));
	}
	if (max == 0) TCL_MSG("no midi out devices");
	TCL_MSG(fstring("out %i (%s)\n", FMSYNTH, "internal fmsynth"));

    // midiIN devices
	for (i = 0, max = midiInGetNumDevs(); i < max; i++) {
		midiInGetDevCaps(i, &mic, sizeof(mic));
		TCL_MSG(fstring("in  %i (%s)\n", i, mic.szPname));
	}
	if (max == 0) TCL_MSG("no midi in devices");

	return TCL_OK;
}

//
// open a midi device and create a new handle
//
int MidiOpen_cmd(ClientData cdata, Tcl_Interp *interp, int argc, char *argv[]) {
	if (argc != 3) 								TCL_ERR("parameter: [in | out] device");
	int device;
	if(Tcl_GetInt(interp, argv[2], &device)  != TCL_OK) return TCL_ERROR;	

	// open midi output
	if (strcmp(argv[1], "out")  == 0) { 
		HMIDIOUT dev;
		if(device == FMSYNTH) {
			if(devFM)							TCL_ERR("failure fmsynth is active");
			dev = devFM  = openOut(device);
		} else {
			if(devOUT) TCL_ERR("failure hardware midi out is active");
			dev = devOUT = openOut(device);
		}
		if(!dev) 								TCL_ERR("device not available");
		
		char handleName[13 + TCL_INTEGER_SPACE];
		sprintf(handleName, "dev%d", hdl_dev++);
		Tcl_CreateCommand(interp, handleName, (Tcl_CmdProc*)midiOutCmd,
											  (ClientData)dev,
										      (Tcl_CmdDeleteProc *)NULL);
		TCL_MSG(fstring("%s", handleName));
		return TCL_OK;
	}

	// open midi input
	if (strcmp(argv[1], "in")  == 0) {
		HMIDIIN dev;
		if(devIN) 								TCL_ERR("failure hardware midi in is active")
		dev = devIN = openIn(device);				  	
		if(!dev) TCL_ERR("device not available");
		char handleName[13 + TCL_INTEGER_SPACE];
		sprintf(handleName, "dev%d", hdl_dev++);
		Tcl_CreateCommand(interp, handleName, (Tcl_CmdProc*)midiInCmd,
											  (ClientData)dev,
										      (Tcl_CmdDeleteProc *)NULL);
		TCL_MSG(fstring("%s", handleName));
		return TCL_OK;
	}
	
	TCL_ERR("values for port: in, out");
}

//
// create new track for processing, write all tracks to midi file, close all tracks
//
int Track_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
	if (argc == 1) TCL_ERR("commands: new, write, close");
	
	// generate a new track handle
	if(strcmp(argv[1], "new")  == 0) {
		struct BUF *trk = newTRK();
		if(!trk) 								TCL_ERR("failure to create a new track");
		char handleName[13 + TCL_INTEGER_SPACE];
		sprintf(handleName, "trk%d", hdl_trk++);
		Tcl_CreateCommand(interp, handleName, (Tcl_CmdProc*)trackCmd,
											  (ClientData*)trk,
											  (Tcl_CmdDeleteProc *)NULL);
		++trackCounter;
		TCL_MSG(fstring("%s", handleName));
		return TCL_OK;
	}
	
	// write smf from tracks
	if(strcmp(argv[1], "write")  == 0) {
		if(argc != 4) 							TCL_ERR("command write: filename ppqn");
		int ppqn;
		if(Tcl_GetInt(interp, argv[3], &ppqn) != TCL_OK) return TCL_ERROR;		// failure ppqn
		struct BUF *smf = newSMF(ppqn);
		if(!smf) 								TCL_ERR("failure to create smf from internal tracks");
		writeSMF(argv[2], smf);
		freeBUF(smf);
		return TCL_OK;
	}
	//
	if(strcmp(argv[1], "close")  == 0) {
		if(statusPlayer()) 						TCL_ERR("player instance is active");
		if(trackCounter) 						TCL_ERR(fstring("%i tracks active", trackCounter));
		freeTRKs();
		Tcl_DeleteCommand(interp, argv[0]);
		return TCL_OK;
	}
	
	TCL_ERR("commands: new, write, close");
}

//
// play midi file
//
int Play_cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
	if (argc == 1) 								TCL_ERR("commands: start time stop");

	// play from midifile or internal tracks
	if(strcmp(argv[1], "start")  == 0) {
		if(statusPlayer()) 						TCL_ERR("player is active");
		int dev, ppqn;
		struct USRTIME time;
		if(argc != 6) 	TCL_ERR("command start: devhandle [create ppqn | load midifile] [audio | render]");

		// check if device active
		if(Tcl_GetInt(interp, argv[2], &dev)  != TCL_OK) 	 return TCL_ERROR; 	// failure dev handle
        if((HMIDIOUT)dev != devOUT && (HMIDIOUT)dev != devFM)       TCL_ERR("wrong output device"); 
		
		// fill/load midi data
		struct BUF *smf;
		if(strcmp(argv[3], "new")  == 0) {										// use internal tracks
			if(Tcl_GetInt(interp, argv[4], &ppqn) != TCL_OK) return TCL_ERROR;		// failure ppqn	
			smf = newSMF(ppqn);
		} else if (strcmp(argv[3], "load")  == 0)	 							// load smf 								
			smf = loadSMF(argv[4]);
		if(!smf) TCL_ERR("failure to create/load smf")
			
		// set audio mode 
		if (strcmp(argv[5], "audio")   == 0) setAudioModeOut((HMIDIOUT)dev, AUDIO);
		if (strcmp(argv[5], "render")  == 0) setAudioModeOut((HMIDIOUT)dev, RENDER);
		
		// play / render midi file
		if(openPlayer(smf, (HMIDIOUT)dev, &time)) {
			TCL_MSG(fstring("%02i:%02i:%06i", time.mm, time.ss, time.ms));
			return TCL_OK;
		}
		TCL_ERR("failure to initialize player instance");
	}
	
	// time
	if (strcmp(argv[1], "time")  == 0) {
		struct USRTIME time;
		getPlayTime(&time);
		TCL_MSG(fstring("%02i:%02i:%06i", time.mm, time.ss, time.ms));
		return TCL_OK;
	}
	
	// stop
	if (strcmp(argv[1], "stop")  == 0) {
		if(!statusPlayer()) 						TCL_ERR("player is not active");
		closePlayer();
		return TCL_OK;
	}
	
	TCL_ERR("commands: start time stop");
}
	
/****************************************************************************************
 *
 * tcl init procedure
 *
 ****************************************************************************************/

int DLLEXPORT Mk_Init(Tcl_Interp *interp) {
	if (Tcl_InitStubs (interp, TCL_VERSION, 0) 	== NULL)   		{ return TCL_ERROR; }
	if (Tcl_PkgProvide(interp, NS, VERSION) 	== TCL_ERROR) 	{ return TCL_ERROR; }
	Tcl_CreateCommand(interp, NS "::help",	(Tcl_CmdProc*)Help_cmd,      NULL, NULL);	
	Tcl_CreateCommand(interp, NS "::list",	(Tcl_CmdProc*)MidiList_cmd,  NULL, NULL);
	Tcl_CreateCommand(interp, NS "::open",	(Tcl_CmdProc*)MidiOpen_cmd,  NULL, NULL);
	Tcl_CreateCommand(interp, NS "::track",	(Tcl_CmdProc*)Track_cmd,   	 NULL, NULL);
	Tcl_CreateCommand(interp, NS "::play",	(Tcl_CmdProc*)Play_cmd,   	 NULL, NULL);
	return TCL_OK;
}

