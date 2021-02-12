/****************************************************************
 *
 * simple test program for adlibsynth
 * compile: gcc/tcc testfmsynth.c fmsynth.c adlibemu.c -lwinmm
 * 
 ****************************************************************/
 
#include <conio.h>			// for kbhit()
#include <stdio.h>
#include "fmsynth.h"

int midi(BYTE bStatus, BYTE bData1, BYTE bData2) {
		FMMidiMsg( (bData2 << 16) | (bData1 << 8) | bStatus );
}

int sysex(int size, BYTE data[]) {
	FMMidiSyx(size, data);
}

int main(int argc, char *argv[]) {
	int bStatus, bData1, bData2;
	DWORD msg;
	BYTE tune = 0, patch = 0;
	
	//IBKFMT *ibk = FMLoadIBK("Custom_Drums.IBK");
	//if(ibk) FMSetIBK(ibk, 1);
	FMOpenSynth();
	AudioInit();
	AudioMode(AUDIO);

/*
	patch = 0;
	midi(0xC1, patch, 0);			// program change for channel 1
	midi(0xB1, 10, 0);				// cc channel 1 - pan = left
	midi(0xB1, 91, 255);			// cc channel 1 - samples to delay
	midi(0xB1,  7, 127);			// cc channel 1 - channel volume
	
	midi(0xB9, 10, 127);			// cc channel 9 - pan = right
	midi(0xB9, 91, 127);			// cc channel 1 - samples to delay
	midi(0xB9,  7, 127);			// cc channel 1 - channel volume

	//sysex string =  SOX  man-id chn   par     val EOX
	BYTE syxdata[] = {0xF0, 0x43, 0x01, MOD_EGA, 3, 0xF7};		
	
	
	// melody test
	printf("melody test ...\n");
	while (1) {
		midi(0x91, 69-12, 127);		// noteon channel 1, A4 = 440Hz -1 octave, volume
		Sleep(2000);
		sysex(6, syxdata);
		midi(0x81, 69-12, 0);		// noteoff channel 1
		if(kbhit()) break;
	}
*/
	// drum test
	printf("drum test ...\n");
	int c;
	midi(0xB9, 10, 0);			// cc channel 9 - pan = right
	for(int i = 35; i<82; i++) {
		// drum test
		printf("mididrum[%3i]",i);
		midi(0x99, i, 127);		// noteon channel 9, key i, volume
		Sleep(100);
		midi(0x99, i, 0);		// noteoff channel 9
		c = getchar();	
	}

	AudioClose();
	FMCloseSynth();

	return(0);
}
