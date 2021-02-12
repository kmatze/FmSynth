FmSynth ...

- ... by ma.ke. 2021
- ... is a hobby project with no warrenty
- ... use at your own risk
- ... do what ever you want with this
- ... be happy ;-)

![FmSynth](/fmsynth.GIF)
	  
**core features FMSYNTH:**
- fmsynth.c / fmsynth.h in directory fmsynth
	- library to use adlibemu (Ken Silverman) as midi synth on **windows**
- adlibemu can use as voice or midi synthesizer
- voice: 9 voices, provide complete register writing (drum mode not implented)
- midi : dynamic voice handling to use 16 midichannels
		 max. 9 polyphonic
- full stereo and delay

- support for sound patches
	- SBI, IBK
	- default MIDI soundbank with 128 "GM" instruments (SBI) + 128 "GM" drums (SBI)

- MIDI implementation: 
	- channel messages:
		- NOTEOFF       0x8
		- NOTEON        0x9
		- CTRLCHG       0xB
		- CC_VOL          7
		- CC_BAL          8 (same as pan)
		- CC_PAN         10 (same as balance)
		- CC_DELAY       91
		- PRGCHG        0xC
		- PITCHBEND     0xE
	- internal instrument (soundblaster timbre with 16 bytes) for midi channel can set with PRGCHG
	- system exclusive for modify midi channel instrument
		- one voice parameter per sysex msg or 
		- complete voicedata  per sysex msg 

- C-API (well documented, see source code fmsynth.c and fmsynth.h)
	- API WINDOWS 	audio
	- API FMSYNTH 	general
	- API FMSYNTH 	midi support
	- API FMSYNTH 	sound patches
	
**extended features - MIDI:**
- mkMidi.c / mkMidi.h in directory mkmidi
	- library to create and play midifile (type 0, 1) on **windows** midi devices,  
	- specially: use internal fmsynth (adlibemu) to play or render to wave file
- create, save, load and play midifile type 0 or 1
- render midi file through fmsynth to a wav file 
- provide running mode by reading midi file
- complete channel messages
- meta event tempo
- sysex data (F0 ... data ... F7)
- provide midi output interface or internal fmsynth (adlibemu) inkl. sysex
- provide midi input interface (WIP)

- limitations:
	- time devision only ppqn
	- without other meta events except tempo
	- sysex request not supported

**extended features - TCL/TK:**
- mk.c         
	- wrapper library to use mkMidi.c to create and play midifile (type 0, 1) on **windows** midi devices
	- specially: fmsynth (adlibemu)
	
- fmsynth.tcl 
	- GUI for FMSYNTH
	
- commands:
	- mk::help     view this help
	- mk::list     get device list
	- mk::open     open device -> get new cmd for midi device or fmsynth 
	- mk::track    create and processing midi tracks -> get new command
	- mk::play     play / render midi file
	- every command can show help informations: cmd ?
	
- **bonus:** 
	- collection of instrument and bank files I've found over the internet
	  see directory instruments or unzip instruments.zip
	- documentation see directory doc
	- sample midi files in directory smf
	- sample rendered file ![FmSynth](/Hier_kommt_Alex.mp3)

**todo:**

- implement a **simple music script** language (SMS)
- bug fixes

TRY IT ;-)

I hope you have fun.

Greetings - kmatze (aka ma.ke.) - 12.02.2021




