#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>							// for kbhit()

#include "mkMidi.h"
#define     AUDIO				0			// realtime audio output
#define     RENDER              1			// rendering without audio output

/****************************************************************************************
 *
 * c test application	command: midiplayer dev file.mid [render] (dev=15=FMSYNTH)
 *
 ****************************************************************************************/
int main(int argc, char **argv)
{	
	if(argc < 3) {
		printf("%s  dev  file.mid  [render]\n",argv[0]);
		return 1;
	}
	
	// create tracks and fill
	struct BUF *trk1, *trk2, *drum;
	//BYTE syxdata[] = {0xF0, 0x01, 0x02, 0x03, 0x1F, 0x7F, 0xF7};
	BYTE syxdata1[] = {0xF0, 0x00, 0x01, 0x02, 0x03, 0x1F, 0x7F, 0xF7};	
	BYTE syxdata2[] = {0xF0, 0x00, 0x01, 0x02, 0x03, 0x1F, 0x7F, 0xF7, 0xF0, 0x00, 0x01, 0x02, 0x03, 0x1F, 0x7F, 0xF7};	
					   
	// song properties
	int ppqn	= 96;						// pulses/ticks per quarter note
	int tempo 	= 300000;					// time in microsec for quarter note

	trk1 = newTRK();
	writeSYX(trk1, syxdata1);
	
	writeMSG(trk1, ppqn * 2, 0x91, 60, 90);		// note on on channel 1 91 3C 5A
	writeMSG(trk1, ppqn * 2, 0x91, 60, 0);
	writeMSG(trk1, ppqn * 1, 0x91, 60, 90);
	writeMSG(trk1, ppqn * 1, 0x91, 60, 90);
	writeMSG(trk1, ppqn * 0, 0x91, 60, 90);
	writeMSG(trk1, ppqn * 4, 0x91, 60, 0);
	writeMSG(trk1, ppqn * 2, 0x91, 60, 90);
	
	writeMSG(trk1, ppqn * 4, 145, 72, 90);		//  91 48 5A
	writeMSG(trk1, ppqn * 1, 145, 72, 90);
	writeMSG(trk1, ppqn * 1, 145, 73, 90);
	writeTMP(trk1, tempo);
	writeSYX(trk1, syxdata2);
	writeMSG(trk1, ppqn * 4, 145, 72, 0);
	writeMSG(trk1, ppqn * 2, 145, 73, 0);
	writeMSG(trk1, ppqn * 1, 145, 72, 90);
	writeMSG(trk1, ppqn * 0, 145, 73, 90);
	writeMSG(trk1, ppqn * 4, 145, 72, 0);
	writeMSG(trk1, ppqn * 0, 145, 73, 0);
	
/*
	drum = newTRK();
	writeMSG(drum, ppqn * 4, 0x99, 40, 127);
	writeMSG(drum, ppqn * 2, 0x99, 40, 127);
	writeMSG(drum, ppqn * 1, 0x89, 40, 0);
	writeMSG(drum, ppqn * 0, 0x89, 45, 0);
*/
	
	// create memory SMF from internal tracks and write to midi file
	struct BUF *mf1 = newSMF(ppqn);
	if (mf1) writeSMF("test.mid", mf1);
	freeBUF(mf1);
	
	struct BUF *smf = loadSMF(argv[2]);	
	
	if(!smf) {
		printf("wrong midi format\n");
		return 0;
	}
	
	// open midi device 
	int dev = atoi(argv[1]);	
	HMIDIOUT devOut = openOut(dev);						// default modus: audio
	printf("device: %i ", dev);
	
	// check if render mode desire
	if(argc == 4 && strcmp(argv[3], "render") == 0 ) 
		setAudioModeOut(devOut, RENDER);
		
	// start player
	struct USRTIME ts, tp;						// to calculate song and playing time
	printf("start playing ...\n");
	if(openPlayer(smf, devOut, &ts)) {
		//printf("songtime: %.2d:%.2d:%.2d\n",ts->hh, ts->mm, ts->ss);
		printf("songtime: %.2d:%.2d:%.2d:%.6d\n",ts.hh, ts.mm, ts.ss, ts.ms);
		while (statusPlayer()) {
			getPlayTime(&tp);
#ifndef MIDIVIEW
			//printf("playtime: %.2d:%.2d:%.2d\r",tp->hh, tp->mm, tp->ss);
			printf("playtime: %.2d:%.2d:%.2d:%.6d\r",tp.hh, tp.mm, tp.ss, tp.ms);
#endif
			if(kbhit()) break;
		}
		closePlayer();
	}
	
	closeOut(devOut);
	freeBUF(smf);	
	freeTRKs();
	printf("\nready ...\n");
}