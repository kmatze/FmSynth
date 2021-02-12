#--------------------------------------------------------------------
# variables
#
namespace eval fmsynth {
    variable version    "fmsynth 0.3 - 09.02.2021"	; # well yes, with some iterations ;-)
	variable sms		{ }     	    ; # holds the text pointer for sms editor
	variable volume     127   			; # standard volume for note
	variable balance     64             ; # standard balance value
	variable delay        0             ; # standard stereo delay value
	variable device      15				; # standard midi device (fmsynth)
	variable channel      0         	; # standard midi channel for keyboard
	variable key     	"key off"		; # name of pressed key
	variable soundBank	"INST"			; # active sound bank: INST, DRUM, USER
	variable filetype   "ibk"			; # status radio buttons for file dialog: sbi ibk sms
	variable sbi        { }				; # sbi pointer for sound data
	variable ibk        { }				; # ibk pointer for user sound bank
	variable midChnVol  127				; # CC_VOL		7
    variable midChnBal   64				; # CC_BAL  	8
    variable midChnPan   64				; # CC_PAN 	   10
    variable midChnDly    0				; # CC_DELA	   91
	variable Font       {Arial 12}		; # system font for widgets
	variable sms        { }             ; # widget of sms
	variable smsText    { }         	; # widget of sms
	variable smsFont    {Consolas 12}	; # system font for widgets
	variable sbiDisplay { }				; # widget of sbi data
	variable sbiHelp    { }				; # widget of sbi help line
	variable sbiData    { }			    ; # sound data for sbi display
	variable sbiProofW  "empty" 		; # current sbi parameter widget to proof
	variable sbiProofV    0 			; # current sbi parameter value  to proof
	variable mmToogle     1				; # flag to view midi monitor
	variable mm		    { }				; # widget of midi monitor
	variable mmDisplay  { }             ; # widget of midi monitor data
	variable mmFont     {Consolas 10 {bold}}	; # font for midi monitor
	variable midiSynth  { }				; # widget of midi synth channel display
	variable fmSynth    { }				; # widget of fm   synth channel display
	variable player     { }				; # widget of midi player
	variable gmNote     { C C# D D# E F F# G G# A A# B } 
	# definition for listbox sounds
	variable ibkUSER {}
	variable ibkINST {
		"001 Acoustic Grand Piano"    "002 Bright Acoustic Piano"   "003 Electric Grand Piano"   "004 Honky-tonk Piano"
	    "005 Electric Piano 1"        "006 Electric Piano 2"        "007 Harpsichord"            "008 Clavinet"
		"009 Celesta"                 "010 Glockenspiel"            "011 Music Box"              "012 Vibraphone"
		"013 Marimba"                 "014 Xylophone"               "015 Tubular Bells"          "016 Dulcimer"
		"017 Drawbar Organ"           "018 Percussive Organ"        "019 Rock Organ"             "020 Church Organ"
		"021 Reed Organ"              "022 Accordion"               "023 Harmonica"              "024 Tango Accordion"
		"025 Acoustic Guitar (nylon)" "026 Acoustic Guitar (steel)" "027 Electric Guitar (jazz)" "028 Electric Guitar (clean)"
		"029 Electric Guitar (muted)" "030 Overdriven Guitar"       "031 Distortion Guitar"      "032 Guitar harmonics"
		"033 Acoustic Bass"           "034 Electric Bass (finger)"  "035 Electric Bass (pick)"   "036 Fretless Bass"
		"037 Slap Bass 1"             "038 Slap Bass 2"             "039 Synth Bass 1"           "040 Synth Bass 2"
		"041 Violin"                  "042 Viola"                   "043 Cello"                  "044 Contrabass"
		"045 Tremolo Strings"         "046 Pizzicato Strings"       "047 Orchestral Harp"        "048 Timpani"
		"049 String Ensemble 1"       "050 String Ensemble 2"       "051 Synth Strings 1"        "052 Synth Strings 2"
		"053 Choir Aahs"              "054 Voice Oohs"              "055 Synth Voice"            "056 Orchestra Hit"
		"057 Trumpet"                 "058 Trombone"                "059 Tuba"                   "060 Muted Trumpet"
		"061 French Horn"             "062 Brass Section"           "063 Synth Brass 1"          "064 Synth Brass 2"
		"065 Soprano Sax"             "066 Alto Sax"                "067 Tenor Sax"              "068 Baritone Sax"
		"069 Oboe"                    "070 English Horn"            "071 Bassoon"                "072 Clarinet"
		"073 Piccolo"                 "074 Flute"                   "075 Recorder"               "076 Pan Flute"
		"077 Blown Bottle"            "078 Shakuhachi"              "079 Whistle"                "080 Ocarina"
		"081 Lead 1 (square)"         "082 Lead 2 (sawtooth)"       "083 Lead 3 (calliope)"      "084 Lead 4 (chiff)"
		"085 Lead 5 (charang)"        "086 Lead 6 (voice)"          "087 Lead 7 (fifths)"        "088 Lead 8 (bass + lead)"
		"089 Pad 1 (new age)"         "090 Pad 2 (warm)"            "091 Pad 3 (polysynth)"      "092 Pad 4 (choir)"
		"093 Pad 5 (bowed)"           "094 Pad 6 (metallic)"        "095 Pad 7 (halo)"           "096 Pad 8 (sweep)"
		"097 FX 1 (rain)"             "098 FX 2 (soundtrack)"       "099 FX 3 (crystal)"         "100 FX 4 (atmosphere)"
		"101 FX 5 (brightness)"       "102 FX 6 (goblins)"          "103 FX 7 (echoes)"          "104 FX 8 (sci-fi)"
		"105 Sitar"                   "106 Banjo"                   "107 Shamisen"               "108 Koto"
		"109 Kalimba"                 "110 Bag pipe"                "111 Fiddle"                 "112 Shanai"
		"113 Tinkle Bell"             "114 Agogo"                   "115 Steel Drums"            "116 Woodblock"
		"117 Taiko Drum"              "118 Melodic Tom"             "119 Synth Drum"             "120 Reverse Cymbal"
		"121 Guitar Fret Noise"       "122 Breath Noise"            "123 Seashore"               "124 Bird Tweet"
		"125 Telephone Ring"          "126 Helicopter"              "127 Applause"               "128 Gunshot"
	}
	variable ibkDRUM {
		"001 <empty>" "002 <empty>" "003 <empty>" "004 <empty>" "005 <empty>" "006 <empty>" "007 <empty>" "008 <empty>"
		"009 <empty>" "010 <empty>" "011 <empty>" "012 <empty>" "013 <empty>" "014 <empty>" "015 <empty>" "016 <empty>"
		"017 <empty>" "018 <empty>" "019 <empty>" "020 <empty>" "021 <empty>" "022 <empty>" "023 <empty>" "024 <empty>"
		"025 <empty>" "026 <empty>" "027 <empty>" "028 <empty>" "029 <empty>" "050 <empty>" "031 <empty>" "032 <empty>"
		"033 <empty>" "034 <empty>"
		"035 Bass Drum 2"             "036 Bass Drum 1"             "037 Side Stick"             "038 Snare Drum 1"
		"039 Hand Clap"               "040 Snare Drum 2"            "041 Low Tom 2"              "042 Closed Hi-hat"
		"043 Low Tom 1"               "044 Pedal Hi-hat"            "045 Mid Tom 2"              "046 Open Hi-hat"
		"047 Mid Tom 1"               "048 High Tom 2"              "049 Crash Cymbal 1"         "050 High Tom 1"
		"051 Ride Cymbal 1"           "052 Chinese Cymbal"          "053 Ride Bell"              "054 Tambourine"
		"055 Splash Cymbal"           "056 Cowbell"                 "057 Crash Cymbal 2"         "058 Vibra Slap"
		"059 Ride Cymbal 2"           "060 High Bongo"              "061 Low Bongo"              "062 Mute High Conga"
		"063 Open High Conga"         "064 Low Conga"               "065 High Timbale"           "066 Low Timbale"
		"067 High Agogo"              "068 Low Agogo"               "069 Cabasa"                 "070 Maracas"
		"071 Short Whistle"           "072 Long Whistle"            "073 Short Guiro"            "074 Long Guiro"
		"075 Claves"                  "076 High Wood Block"         "077 Low Wood Block"         "078 Mute Cuica"
		"079 Open Cuica"              "080 Mute Triangle"           "081 Open Triangle"          
		"082 <empty>" "083 <empty>" "084 <empty>" "085 <empty>" "086 <empty>" "087 <empty>" "088 <empty>" 
		"089 <empty>" "090 <empty>" "091 <empty>" "092 <empty>" "093 <empty>" "094 <empty>" "095 <empty>" "096 <empty>" 
		"097 <empty>" "098 <empty>" "099 <empty>" "100 <empty>" "101 <empty>" "102 <empty>" "103 <empty>" "104 <empty>" 
		"105 <empty>" "106 <empty>" "107 <empty>" "108 <empty>" "109 <empty>" "110 <empty>" "111 <empty>" "112 <empty>"
		"113 <empty>" "114 <empty>" "115 <empty>" "116 <empty>" "117 <empty>" "118 <empty>" "119 <empty>" "120 <empty>"
		"121 <empty>" "122 <empty>" "123 <empty>" "124 <empty>" "125 <empty>" "126 <empty>" "127 <empty>" "128 <empty>"
	} 
	variable help {
		"register20: AM, VIB, EG-TYP, KSR, MULTI"
	}
	#--------------------------------------------------------------------
	# midi control
	#
	proc sendMidiMsg {status data1 data2} {
		set cMidiIN "OrangeRed"; set cMidiOUT "DarkBlue"
		$fmsynth::device msg $status $data1 $data2
		set cmd [fmsynth::getbitfield $status 11110000]
		set chn [fmsynth::getbitfield $status 00001111]
		# CTRLCHG		0xB
		#	CC_VOL        7
		#	CC_BAL		  8
		#	CC_PAN		 10
		#	CC_DELAY	 91
		set descr ""; set type ""
		if {$cmd ==  8} { append descr "| NOTEOFF [format "%2i %3i %3i" $chn $data1 $data2]"; set type NOTEOFF }
		if {$cmd ==  9}	{ append descr "| NOTEON  [format "%2i %3i %3i" $chn $data1 $data2]"; set type NOTEON  }
		if {$cmd == 11} { append descr "| CTRLCHG [format "%2i " $chn]"
						  if {$data1 ==  7} { append descr "VOL "; set type CCVOL }
						  if {$data1 ==  8} { append descr "BAL "; set type CCBAL }
						  if {$data1 == 10} { append descr "PAN "; set type CCPAN }
						  if {$data1 == 91} { append descr "DLY "; set type CCDLY }
						  append descr [format "%3i" $data2]
	   }
		set mmMsg [format " msg 0x%02X 0x%02X 0x%02X %s" $status $data1 $data2 $descr]
		fmsynth::tkPrintMidi "$mmMsg" $type
	}
	
	#    one voice par as sysex data
    #    SOX  man-id channel parameter value EOX
    #    0xF0  0x43    chn      par     val  0xF7
	proc sendMidiSyxPar {par val} {
		$fmsynth::device syx 0xF0 0x43 $fmsynth::channel $par $val 0xF7
		set mmMsg [format " syx 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X" 0xF0 0x43 $fmsynth::channel $par $val 0xF7] 
		fmsynth::tkPrintMidi "$mmMsg" SYSEX
	}
	
	#--------------------------------------------------------------------
	# synth functions
	#
	proc init { } {
		set fmsynth::device   [mk::open out $fmsynth::device]
		
		set fmsynth::sbi 	  [$fmsynth::device sbi new]
		$fmsynth::sbi set [list "<empty>" 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]
	   
	   set fmsynth::ibk 	  [$fmsynth::device ibk new]
		for {set i 0} {$i < 128} {incr i} {
			$fmsynth::ibk set $i [$fmsynth::sbi get]
			lappend fmsynth::ibkUSER [format "%03i <empty>" $i]
		}
	}
	
	proc setSoundBank { lbox sbk } {
		if {$sbk == "INST" } { $lbox configure -listvariable fmsynth::ibkINST }
		if {$sbk == "DRUM" } { $lbox configure -listvariable fmsynth::ibkDRUM }
		if {$sbk == "USER" } { $lbox configure -listvariable fmsynth::ibkUSER }
		set fmsynth::soundBank $sbk
	}
	
	proc setSound {lbox} {
		set sound [$lbox curselection]
		set name  [lrange [$lbox get $sound] 1 end]
	
		if {$fmsynth::soundBank == "DRUM"} {set sound [expr $sound + 128]}
		if {$fmsynth::soundBank == "USER"} {
			# read and set sound from ibk
			$fmsynth::sbi set [$fmsynth::ibk get $sound]
		} else {
			#read and set sound from internal soundBank
			set reg  [lrange [$fmsynth::device sound $sound] 1 end]
			$fmsynth::sbi set "[list $name] $reg"
		}
		$fmsynth::sbi use $fmsynth::channel
		set fmsynth::sbiData [$fmsynth::sbi get]

		refreshSoundData
	}
	
	proc refreshSoundData {} {
		#create register data list
		set d $fmsynth::sbiDisplay
		$d.name delete 0 end; $d.name insert 0 [lindex $fmsynth::sbiData 0]
		# fill register bytes and calculate parameter values
		set reg [lrange $fmsynth::sbiData 1 end]
			set val [lindex $reg 0]
				$d.20val     delete 0 end; $d.20val     insert 0 $val
				$d.20multi   delete 0 end; $d.20multi   insert 0 [fmsynth::getbitfield $val 00001111]
				$d.20ksr 	 delete 0 end; $d.20ksr     insert 0 [fmsynth::getbitfield $val 00010000]
				$d.20egtype  delete 0 end; $d.20egtype  insert 0 [fmsynth::getbitfield $val 00100000]
				$d.20vib     delete 0 end; $d.20vib	    insert 0 [fmsynth::getbitfield $val 01000000]
				$d.20am      delete 0 end; $d.20am 	    insert 0 [fmsynth::getbitfield $val 10000000]
			set val [lindex $reg 1]
				$d.23val     delete 0 end; $d.23val     insert 0 $val
				$d.23multi   delete 0 end; $d.23multi   insert 0 [fmsynth::getbitfield $val 00001111]
				$d.23ksr 	 delete 0 end; $d.23ksr     insert 0 [fmsynth::getbitfield $val 00010000]
				$d.23egtype  delete 0 end; $d.23egtype  insert 0 [fmsynth::getbitfield $val 00100000]
				$d.23vib     delete 0 end; $d.23vib	    insert 0 [fmsynth::getbitfield $val 01000000]
				$d.23am      delete 0 end; $d.23am 	    insert 0 [fmsynth::getbitfield $val 10000000]	
			set val [lindex $reg 2]
				$d.40val     delete 0 end; $d.40val     insert 0 $val
				$d.40tl 	 delete 0 end; $d.40tl      insert 0 [fmsynth::getbitfield $val 00111111]
				$d.40ksl     delete 0 end; $d.40ksl     insert 0 [fmsynth::getbitfield $val 11000000]
			set val [lindex $reg 3]
				$d.43val     delete 0 end; $d.43val     insert 0 $val
				$d.43tl 	 delete 0 end; $d.43tl      insert 0 [fmsynth::getbitfield $val 00111111]
				$d.43ksl     delete 0 end; $d.43ksl     insert 0 [fmsynth::getbitfield $val 11000000]
			set val [lindex $reg 4]
				$d.60val     delete 0 end; $d.60val     insert 0 $val
				$d.60decay 	 delete 0 end; $d.60decay   insert 0 [fmsynth::getbitfield $val 00001111]
				$d.60attack  delete 0 end; $d.60attack  insert 0 [fmsynth::getbitfield $val 11110000]
			set val [lindex $reg 5]
				$d.63val     delete 0 end; $d.63val     insert 0 $val
				$d.63decay 	 delete 0 end; $d.63decay   insert 0 [fmsynth::getbitfield $val 00001111]
				$d.63attack  delete 0 end; $d.63attack  insert 0 [fmsynth::getbitfield $val 11110000]
			set val [lindex $reg 6]
				$d.80val     delete 0 end; $d.80val     insert 0 $val
				$d.80release delete 0 end; $d.80release insert 0 [fmsynth::getbitfield $val 00001111]
				$d.80sustain delete 0 end; $d.80sustain insert 0 [fmsynth::getbitfield $val 11110000]
			set val [lindex $reg 7]
				$d.83val     delete 0 end; $d.83val     insert 0 $val
				$d.83release delete 0 end; $d.83release insert 0 [fmsynth::getbitfield $val 00001111]
				$d.83sustain delete 0 end; $d.83sustain insert 0 [fmsynth::getbitfield $val 11110000]
			set val [lindex $reg 8]
				$d.e0val     delete 0 end; $d.e0val     insert 0 $val
				$d.e0wave    delete 0 end; $d.e0wave    insert 0 [fmsynth::getbitfield $val 00000011]
			set val [lindex $reg 9]
				$d.e3val     delete 0 end; $d.e3val     insert 0 $val
				$d.e3wave    delete 0 end; $d.e3wave    insert 0 [fmsynth::getbitfield $val 00000011]
			set val [lindex $reg 10]
				$d.c0val     delete 0 end; $d.c0val     insert 0 $val
				$d.c0connect delete 0 end; $d.c0connect insert 0 [fmsynth::getbitfield $val 00000001]
				$d.c0fb      delete 0 end; $d.c0fb      insert 0 [fmsynth::getbitfield $val 00001110]
				$d.c0stereo configure -state normal
				$d.c0stereo  delete 0 end; $d.c0stereo  insert 0 [fmsynth::getbitfield $val 00110000]
				$d.c0stereo configure -state disable
			
			$d.pvval configure -state normal
			$d.tpval configure -state normal
			$d.r1val configure -state normal
			$d.r2val configure -state normal
			$d.pvval delete 0 end; $d.pvval insert 0 [lindex $reg 11]
			$d.tpval delete 0 end; $d.tpval insert 0 [lindex $reg 12]
			$d.dpval delete 0 end; $d.dpval insert 0 [lindex $reg 13]			
			$d.r1val delete 0 end; $d.r1val insert 0 [lindex $reg 14]
			$d.r2val delete 0 end; $d.r2val insert 0 [lindex $reg 15]
			$d.pvval configure -state disable
			$d.tpval configure -state disable
			$d.r1val configure -state disable
			$d.r2val configure -state disable
	}
	
	proc copyToClipboard {} {
		#create sound as c list
		set reg  [lrange $fmsynth::sbiData 1 end]	
		set name [lindex $fmsynth::sbiData 0]
		set CList "   "
		foreach byte $reg { append CList " [format "0x%02X," $byte]" }
		append CList " // $name\n"
		clipboard append $CList
	}
	 
	proc soundToUserBank {} {
		set ready ""
		set c [toplevel .patch]
		wm title .patch "copy sound to ..."
			frame $c.list -relief raised -padx 5 -pady 5 -bd 2
			scrollbar $c.list.scr -command "$c.list.lbx yview"
			listbox   $c.list.lbx -width 40 -height 16 -yscroll "$c.list.scr set" -listvariable fmsynth::ibkUSER
			pack $c.list.lbx -side left	-fill x
			pack $c.list.scr -side right -fill y
			bind $c.list.lbx <ButtonRelease-1> { set ready "ready" }
			bind $c.list.lbx <Any-Enter>	   { focus -force %W }
		pack $c.list
		set oldFocus [focus]
		grab .patch
		focus .patch
		#update
		tkwait variable ready
		set patch [$c.list.lbx curselection]

		$fmsynth::ibk set $patch $fmsynth::sbiData
		
		puts "$patch [$fmsynth::ibk get $patch]"
		
		set name [lindex [$fmsynth::ibk get $patch] 0]
		set fmsynth::ibkUSER [lreplace $fmsynth::ibkUSER $patch $patch [format "%03i %s" $patch $name]]
		
		destroy .patch
		focus $oldFocus
	}
	
	proc file { operation type } {
		if {$operation == "load"} {set fDialog "tk_getOpenFile"}
		if {$operation == "save"} {set fDialog "tk_getSaveFile"}
		
		if {$type == "ibk"} {
			set extension { {{Sound Bank} {.ibk} }}
			set FileName [$fDialog -filetypes $extension]
			if {$FileName == ""} { return }
			if {$operation == "load"} {
				$fmsynth::ibk load $FileName
				set sounds [$fmsynth::ibk list]
				set i 0
				set fmsynth::ibkUSER {}
				foreach s $sounds {
					lappend fmsynth::ibkUSER "[format "%03i %s" $i $s]"
					incr i
			} }
			if {$operation == "save"} { $fmsynth::ibk save $FileName}
		}
		if {$type == "sbi"} {
			set extension { {{Instrument} {.sbi} }}
			set FileName [$fDialog -filetypes $extension]
			if {$FileName == ""} { return }
			if {$operation == "load"} {
				$fmsynth::sbi load $FileName
				$fmsynth::sbi use $fmsynth::channel
				set fmsynth::sbiData [$fmsynth::sbi get]
				refreshSoundData
			}
			if {$operation == "save"} {
				$fmsynth::sbi set  $fmsynth::sbiData
				$fmsynth::sbi save $FileName
			}
		}
		if {$type == "sms"} {
			set extension { {{Simple Music Script} {.sms} }}
			set FileName [$fDialog -filetypes $extension]
			if {$FileName == ""} { return }
			if {$operation == "load"} {
				set fileid [open $FileName r]
				set data [read $fileid]
				close $fileid
				$fmsynth::smsText delete 1.0 end
				$fmsynth::smsText insert end $data
			}
			if {$operation == "save"} {
				set data [$fmsynth::smsText get 1.0 {end -1c}]
				set fileid [open $FileName w]
				puts -nonewline $fileid $data
				close $fileid
			}
		}
	} 

	#--------------------------------------------------------------------
	# widgets
	#
	proc keyboard {c} {
		set whiteKeys [expr 7*7 + 3]
		set x0 5; set y0 5      		;# top left corner to start
		set y1 100              		;# length of white keys
		set y05 [expr $y1*.67]  		;# length of black keys
		set dx 18               		;# width of white keys
		set dx2 [expr $dx/2]  			;# offset of black keys
		
		frame $c -padx 5 -pady 5
			canvas $c.c -bg brown -height [expr $y1+4] -width [expr ($dx+1)*$whiteKeys + $x0] 
			$c config -cursor hand2 		;# so we see the single finger that plays

			for {set o 0} {$o < 11} {incr o} {
				for {set key 0 } {$key < 12} {incr key} {
					set n [expr $o*12 + $key]
					if {$n < 128} {
						# check if inside of piano range
						if { $n >=21 && $n <= 108 } {
							if {$key == 1 || $key == 3 || $key == 6 || $key == 8 || $key == 10} {
								set x [expr {$x0 - $dx*.35}]
								set id [$c.c create rect $x $y0 [expr {$x + $dx*0.65}] $y05 -fill black -tag black]
							} else {
								set id [$c.c create rect $x0 $y0 [expr {$x0+$dx}] $y1 -fill white]
								incr x0 $dx; incr x0 1
							}
							$c.c bind $id <1>               "fmsynth::tkNoteON  $c.c [expr $id]"
							$c.c bind $id <ButtonRelease-1> "fmsynth::tkNoteOFF $c.c [expr $id]"
			} } } }
			$c.c raise black ;# otherwise half-hidden by next white key
			pack $c.c
		return $c
	}

	proc keydisplay {c} {
		frame $c -padx 5 -pady 5
			label  $c.display  -textvariable fmsynth::key -padx 5 -pady 5 -bd 2 -font {Arial 28 {bold}}
			button $c.btnDrum -text "DRUM" -width 9	-font $fmsynth::Font		
			bind   $c.btnDrum <ButtonPress-1>   "fmsynth::tkDrum on"
			bind   $c.btnDrum <ButtonRelease-1> "fmsynth::tkDrum off"
			pack   $c.display -side left  -fill both -expand 1
			pack   $c.btnDrum -side right -fill y
		return $c
	}
	
	proc filedialog {c} {
		frame $c -padx 5 -pady 5
			radiobutton $c.rSBI -text "*.sbi" -variable fmsynth::filetype -value "sbi"
			radiobutton $c.rIBK -text "*.ibk" -variable fmsynth::filetype -value "ibk"
			radiobutton $c.rSMS -text "*.sms" -variable fmsynth::filetype -value "sms"
			button $c.btnLOAD -text "LOAD" -width 6 -command {fmsynth::file load $fmsynth::filetype}
			button $c.btnSAVE -text "SAVE" -width 6 -command {fmsynth::file save $fmsynth::filetype}
			pack   $c.rSBI $c.rIBK $c.rSMS -side left -padx 5
			pack   $c.btnSAVE $c.btnLOAD -side right
		return $c
	}
	
	proc synth {c} {
		frame $c -padx 5 -pady 5
			frame $c.titel -relief raised -padx 5 -pady 5 -bd 2
				label  $c.titel.lbl     -textvariable fmsynth::soundBank -width 4 -font $fmsynth::Font -anchor w
				button $c.titel.btnInst -text "INST" -width 6 -command "fmsynth::setSoundBank $c.list.lbx INST"
				button $c.titel.btnDrum -text "DRUM" -width 6 -command "fmsynth::setSoundBank $c.list.lbx DRUM"
				button $c.titel.btnUser -text "USER" -width 6 -command "fmsynth::setSoundBank $c.list.lbx USER"
				pack $c.titel.btnUser $c.titel.btnDrum $c.titel.btnInst -side right 
				pack $c.titel.lbl     -side right -fill x -expand 1
			frame $c.list -relief raised -padx 5 -pady 5 -bd 2
				scrollbar $c.list.scr -command "$c.list.lbx yview"
				listbox   $c.list.lbx -width 40 -height 10 -yscroll "$c.list.scr set" -listvariable fmsynth::ibkINST
				pack $c.list.lbx -side left	-fill x
				pack $c.list.scr -side right -fill y
				bind $c.list.lbx <ButtonRelease-1> "fmsynth::setSound $c.list.lbx"
				bind $c.list.lbx <Any-Enter>	   { focus -force %W }
			frame $c.data -relief raised -padx 5 -bd 2
				# color settings
				set cMod LightBlue3; set cCar NavajoWhite3; set cOpt LightGoldenrod1; set cHlp DarkSeaGreen1
				set d [frame  $c.data.f]
					label $d.sbi   -text "Patch-Name:" -width 11 -anchor w; grid $d.sbi   -row 0 -column 0 -columnspan 3
					entry $d.name  -justify center -width 31; 			    grid $d.name -row 0  -column 3 -columnspan 10
					
					label $d.til10 -text "REG" -width  3; grid $d.til10 -row 1 -column 0
					label $d.til9  -text "VAL" -width  3; grid $d.til9  -row 1 -column 1
					label $d.til8  -text "->"  -width  2; grid $d.til8  -row 1 -column 2
					label $d.til7  -text "D7"  -width  3; grid $d.til7  -row 1 -column 3
					label $d.til6  -text "D6"  -width  3; grid $d.til6  -row 1 -column 4
					label $d.til5  -text "D5"  -width  3; grid $d.til5  -row 1 -column 5
					label $d.til4  -text "D4"  -width  3; grid $d.til4  -row 1 -column 6
					label $d.til3  -text "D3"  -width  3; grid $d.til3  -row 1 -column 7
					label $d.til2  -text "D2"  -width  3; grid $d.til2  -row 1 -column 8
					label $d.til1  -text "D1"  -width  3; grid $d.til1  -row 1 -column 9
					label $d.til0  -text "D0"  -width  3; grid $d.til0  -row 1 -column 10
				
					label $d.20lbl -text "20" -bg $cMod -width 3; grid $d.20lbl -row 2  -column 0
						entry $d.20val     -justify center -width 3  -validate focus -vcmd "fmsynth::check %W %V %s %P sbi  0 255 {reg20: AM, VIB, EG-TYP, KSR, MULTI}"
							grid  $d.20val     -row 2  -column 1
						entry $d.20am      -justify center -width 3	 -validate focus -vcmd "fmsynth::check %W %V %s %P par  4   1 {reg20: AM Tremolo (Amplitude vibrato)}"
							grid  $d.20am      -row 2  -column 3
						entry $d.20vib     -justify center -width 3	 -validate focus -vcmd "fmsynth::check %W %V %s %P par  3   1 {reg20: VIB Frequency vibrato}"	
							grid  $d.20vib 	   -row 2  -column 4
						entry $d.20egtype  -justify center -width 3	 -validate focus -vcmd "fmsynth::check %W %V %s %P par  2   1 {reg20: EG Sound Sustaining}"	
							grid  $d.20egtype  -row 2  -column 5
						entry $d.20ksr 	   -justify center -width 3	 -validate focus -vcmd "fmsynth::check %W %V %s %P par  1   1 {reg20: KSR Envelope scaling}"	
							grid  $d.20ksr     -row 2  -column 6
						entry $d.20multi   -justify center -width 15 -validate focus -vcmd "fmsynth::check %W %V %s %P par  0  15 {reg20: MULTI Frequ. Multiplication Factor}"	
							grid  $d.20multi   -row 2  -column 7  -columnspan 4
					label $d.23lbl -text "23" -bg $cCar -width 3; grid $d.23lbl -row 3 -column 0
						entry $d.23val     -justify center -width 3  -validate focus -vcmd "fmsynth::check %W %V %s %P sbi  1 255 {reg23: AM, VIB, EG-TYP, KSR, MULTI}"
							grid  $d.23val     -row 3  -column 1
						entry $d.23am	   -justify center -width 3	 -validate focus -vcmd "fmsynth::check %W %V %s %P par  9   1 {AM: Tremolo (Amplitude vibrato)}"
							grid  $d.23am      -row 3  -column 3
						entry $d.23vib     -justify center -width 3  -validate focus -vcmd "fmsynth::check %W %V %s %P par  8   1 {VIB: Frequency vibrato}"	
							grid  $d.23vib     -row 3  -column 4
						entry $d.23egtype  -justify center -width 3	 -validate focus -vcmd "fmsynth::check %W %V %s %P par  7   1 {EG-TYP: Sound Sustaining}"
							grid  $d.23egtype  -row 3  -column 5
						entry $d.23ksr	   -justify center -width 3	 -validate focus -vcmd "fmsynth::check %W %V %s %P par  6   1 {KSR: Envelope scaling}"
							grid  $d.23ksr     -row 3  -column 6
						entry $d.23multi   -justify center -width 15 -validate focus -vcmd "fmsynth::check %W %V %s %P par  5  15 {MULTI: Frequency Multiplication Factor}"	
							grid  $d.23multi   -row 3  -column 7  -columnspan 4
					
					label $d.40lbl -text "40" -bg $cMod -width 3; grid $d.40lbl -row 4  -column 0
						entry $d.40val 	   -justify center -width 3  -validate focus -vcmd "fmsynth::check %W %V %s %P sbi  2 255 {reg40: KSL, TL}"	
							grid  $d.40val     -row 4  -column 1
						entry $d.40ksl 	   -justify center -width 7  -validate focus -vcmd "fmsynth::check %W %V %s %P par 11   3 {KSL: Key Scale Level}"		
							grid  $d.40ksl     -row 4  -column 3  -columnspan 2
						entry $d.40tl      -justify center -width 23 -validate focus -vcmd "fmsynth::check %W %V %s %P par 10  63 {TL: Total output level}"		
							grid  $d.40tl      -row 4  -column 5  -columnspan 6
					label $d.43lbl -text "43" -bg $cCar -width 3; grid $d.43lbl -row 5  -column 0
						entry $d.43val     -justify center -width 3  -validate focus -vcmd "fmsynth::check %W %V %s %P sbi  3 255 {reg43: KSL, TL}"	
							grid  $d.43val     -row 5  -column 1
						entry $d.43ksl     -justify center -width 7  -validate focus -vcmd "fmsynth::check %W %V %s %P par 13   3 {KSL: Key Scale Level}"		
							grid  $d.43ksl     -row 5  -column 3  -columnspan 2
						entry $d.43tl 	   -justify center -width 23 -validate focus -vcmd "fmsynth::check %W %V %s %P par 12  63 {TL: Total output level}"	
							grid  $d.43tl      -row 5  -column 5  -columnspan 6					
					
					label $d.60lbl -text "60" -bg $cMod -width 3; grid $d.60lbl -row 6  -column 0
						entry $d.60val     -justify center -width 3  -validate focus -vcmd "fmsynth::check %W %V %s %P sbi  4 255 {reg60: AR,DR}"	
							grid  $d.60val     -row 6  -column 1    
						entry $d.60attack  -justify center -width 15 -validate focus -vcmd "fmsynth::check %W %V %s %P par 15  15 {AR: Envelope Generator: Attack Rate}"		
							grid  $d.60attack  -row 6  -column 3  -columnspan 4
						entry $d.60decay   -justify center -width 15 -validate focus -vcmd "fmsynth::check %W %V %s %P par 14  15 {DR: Envelope Generator: Decay Rate}"		
							grid  $d.60decay   -row 6  -column 7  -columnspan 4
					label $d.63lbl -text "63" -bg $cCar -width 3; grid $d.63lbl -row 7  -column 0
						entry $d.63val 	   -justify center -width 3  -validate focus -vcmd "fmsynth::check %W %V %s %P sbi  5 255 {reg63: AR,DR}"	
							grid  $d.63val     -row 7  -column 1
						entry $d.63attack  -justify center -width 15 -validate focus -vcmd "fmsynth::check %W %V %s %P par 17  15 {AR: Envelope Generator: Attack Rate}"	
							grid  $d.63attack  -row 7  -column 3  -columnspan 4
						entry $d.63decay   -justify center -width 15 -validate focus -vcmd "fmsynth::check %W %V %s %P par 16  15 {DR: Envelope Generator: Decay Rate}"	
							grid  $d.63decay   -row 7  -column 7  -columnspan 4
					
					label $d.80lbl -text "80" -bg $cMod  -width 3; grid $d.80lbl -row 8  -column 0
						entry $d.80val     -justify center -width 3  -validate focus -vcmd "fmsynth::check %W %V %s %P sbi  6 255 {reg80: SL,RR}"	
							grid  $d.80val     -row 8  -column 1
						entry $d.80sustain -justify center -width 15 -validate focus -vcmd "fmsynth::check %W %V %s %P par 19  15 {SL: Envelope Generator: Sustain Level}"	
							grid  $d.80sustain -row 8  -column 3  -columnspan 4
						entry $d.80release -justify center -width 15 -validate focus -vcmd "fmsynth::check %W %V %s %P par 18  15 {RR: Envelope Generator: Release Rate}"	
							grid  $d.80release -row 8  -column 7  -columnspan 4
					label $d.83lbl -text "83" -bg $cCar -width 3; grid $d.83lbl -row 9  -column 0
						entry $d.83val     -justify center -width 3  -validate focus -vcmd "fmsynth::check %W %V %s %P sbi  7 255 {reg83: SL,RR}"	
							grid  $d.83val     -row 9  -column 1
						entry $d.83sustain -justify center -width 15 -validate focus -vcmd "fmsynth::check %W %V %s %P par 21  15 {SL: Envelope Generator: Sustain Level}"	
							grid  $d.83sustain -row 9  -column 3  -columnspan 4
						entry $d.83release -justify center -width 15 -validate focus -vcmd "fmsynth::check %W %V %s %P par 20  15 {RR: Envelope Generator: Release Rate}"	
							grid  $d.83release -row 9  -column 7  -columnspan 4					
					
					label $d.e0lbl -text "E0" -bg $cMod -width 3; grid $d.e0lbl -row 10 -column 0
						entry $d.e0val     -justify center -width 3  -validate focus -vcmd "fmsynth::check %W %V %s %P sbi  8   3 {regE0: WS}"
							grid  $d.e0val     -row 10 -column 1
						entry $d.e1        -state disabled -width 23  
							grid  $d.e1        -row 10 -column 3  -columnspan 6
						entry $d.e0wave    -justify center -width 7	 -validate focus -vcmd "fmsynth::check %W %V %s %P par 22   3 {WS: Waveform Select}"
							grid  $d.e0wave    -row 10 -column 9  -columnspan 2	
					label $d.e3lbl -text "E3" -bg $cCar -width 3; grid  $d.e3lbl -row 11 -column 0
						entry $d.e3val     -justify center -width 3  -validate focus -vcmd "fmsynth::check %W %V %s %P sbi  9   3 {regE3: WS}"	
							grid  $d.e3val     -row 11 -column 1
						entry $d.e2        -state disabled -width 23 	 
							grid  $d.e2        -row 11 -column 3  -columnspan 6
						entry $d.e3wave    -justify center -width 7	 -validate focus -vcmd "fmsynth::check %W %V %s %P par 24   3 {WS: Waveform Select}"
							grid  $d.e3wave    -row 11 -column 9  -columnspan 2					
					
					label $d.c0lbl -text "C0" -bg $cMod -width 3; grid $d.c0lbl -row 12 -column 0
						entry $d.c0val     -justify center -width 3  -validate focus -vcmd "fmsynth::check %W %V %s %P sbi 10  63 {regC0: FB, C}"	
							grid  $d.c0val     -row 12 -column 1
						entry $d.e3        -state disabled -width 7  
							grid  $d.e3        -row 12 -column 3  -columnspan 2
						entry $d.c0stereo  -justify center -width 7	
							grid  $d.c0stereo  -row 12 -column 5  -columnspan 2
						entry $d.c0fb      -justify center -width 11 -validate focus -vcmd "fmsynth::check %W %V %s %P par 27   7 {FB: Feedback strength}"	
							grid  $d.c0fb      -row 12 -column 7  -columnspan 3
						entry $d.c0connect -justify center -width 3	 -validate focus -vcmd "fmsynth::check %W %V %s %P par 26   1 {C: Connection 0=FM, 1=AM}"
							grid  $d.c0connect -row 12 -column 10

					label $d.pvlbl -text "PV" -bg $cOpt -width 3; grid $d.pvlbl -row 13 -column 0
						entry $d.pvval     -justify center -state disabled -width 3	
							grid  $d.pvval     -row 13 -column 1
						label $d.lbl1      -text "percussion voice (not used)" -width 30 -anchor w
							grid $d.lbl1       -row 13 -column 3 -columnspan 9
					label $d.tplbl -text "TP" -bg $cOpt  -width 3; grid $d.tplbl -row 14 -column 0
						entry $d.tpval     -justify center -state disabled -width 3	
							grid  $d.tpval     -row 14 -column 1
						label $d.lbl2      -text "notes to transppose (not used)" -width 30 -anchor w
							grid $d.lbl2       -row 14 -column 3 -columnspan 9
					label $d.dplbl -text "DP" -bg $cOpt -width 3; grid $d.dplbl -row 15 -column 0
						entry $d.dpval     -justify center -width 3 -validate focus -vcmd "fmsynth::check %W %V %s %P sbi 13 127  {regDP: Drum pitch}"	
							grid  $d.dpval     -row 15 -column 1
						label $d.lbl3      -text "percussion pitch" -width 30 -anchor w 
							grid $d.lbl3       -row 15 -column 3 -columnspan 9
					label $d.r1lbl -text "R1" -bg $cOpt -width 3; grid $d.r1lbl -row 16 -column 0
						entry $d.r1val     -justify center -state disabled -width 3	
							grid  $d.r1val     -row 16 -column 1
						label $d.lbl4      -text "reserved" -width 10 -anchor w 
							grid $d.lbl4       -row 16 -column 3 -columnspan 3
						label $d.lbl50     -text "REG" -width 3 -bg $cMod
						    grid $d.lbl50      -row 16 -column 8
						label $d.lbl5      -text "modulator" -width 8  -bg $cHlp -anchor center
							grid $d.lbl5       -row 16 -column 9 -columnspan 2
					label $d.r2lbl -text "R2" -bg $cOpt -width 3; grid $d.r2lbl -row 17 -column 0
						entry $d.r2val     -justify center -state disabled -width 3	
							grid  $d.r2val     -row 17 -column 1
						label $d.lbl6      -text "reserved" -width 10 -anchor w 
							grid $d.lbl6       -row 17 -column 3 -columnspan 3
						label $d.lbl70     -text "REG" -width 3 -bg $cCar
						    grid $d.lbl70      -row 17 -column 8
						label $d.lbl7      -text "carrier" -width 8 -bg $cHlp -anchor center
							grid $d.lbl7       -row 17 -column 9 -columnspan 2
							
					label $d.help  -width 44 -bg $cHlp -anchor w; grid $d.help -row 18 -column 0 -columnspan 11

					label  $c.data.lbl      -text "copy patch to:"
					button $c.data.btnUser  -text "USER" -width 6 -command fmsynth::soundToUserBank
					button $c.data.btnClip  -text "CLIP" -width 6 -command fmsynth::copyToClipboard
							
				pack $d
				pack $c.data.btnClip $c.data.btnUser $c.data.lbl -side right
				set  fmsynth::sbiDisplay $d
				set  fmsynth::sbiHelp $d.help

			frame $c.midi -relief raised -padx 5 -pady 5 -bd 2
				scale  $c.midi.vol -orient horizontal -from 0 -to 127 -length 80 -command "fmsynth::tkCC vol" -label "CHN Volume"
				scale  $c.midi.bal -orient horizontal -from 0 -to 127 -length 80 -command "fmsynth::tkCC bal" -label "CHN Balance"
				scale  $c.midi.dly -orient horizontal -from 0 -to 127 -length 80 -command "fmsynth::tkCC dly" -label "CHN Delay"
				pack   $c.midi.vol  $c.midi.bal $c.midi.dly -side left -fill x
				$c.midi.vol set 127; $c.midi.bal set 64; $c.midi.dly set 0
				
			pack $c.titel -fill x 
			pack $c.list  -fill x 
			pack $c.data  -fill both -expand 1
			pack $c.midi  -fill x
			fmsynth::setSoundBank $c.list.lbx INST
			$c.list.lbx selection set 0 
			fmsynth::setSound $c.list.lbx
			
		return $c
	}

	proc sms {c} {
		set fmsynth::sms $c
		frame $c -padx 5 -pady 5
			frame $c.titel -relief raised -padx 5 -pady 5 -bd 2
				label  $c.titel.lbl  -text "SMS-Editor" -padx 5 -font $fmsynth::Font
				button $c.titel.btn1 -text "button1"    -padx 5
				button $c.titel.btn2 -text "button2"    -padx 5
				button $c.titel.btn3 -text "button3" 	-padx 5
				pack $c.titel.btn3 -side right				
				pack $c.titel.lbl $c.titel.btn1 $c.titel.btn2 -side left -fill x
			frame $c.sms -relief raised -padx 5 -pady 5 -bd 2
				scrollbar $c.sms.scry -command "$c.sms.txt yview"
				scrollbar $c.sms.scrx -command "$c.sms.txt xview" -orient horizontal
				set fmsynth::smsText [text $c.sms.txt  -yscroll "$c.sms.scry set"  -xscroll "$c.sms.scrx set" -font $fmsynth::smsFont -wrap none -undo 1]
				pack $c.sms.scry -side right  -fill y
				pack $c.sms.scrx -side bottom -fill x
				pack $c.sms.txt  -side left   -fill both -expand 1
				bind $fmsynth::smsText  <Any-Enter>  { focus -force %W }
			frame $c.synth -relief raised -padx 5 -pady 5 -bd 2
				canvas $c.synth.midi -width 115 -height 20 -bg green -relief raised -bd 2
					set fmsynth::midiSynth $c.synth
					#
					# lxo = line width  offset, lx  = line width, ls  = line space
					# lyo = line height offset, ly  = line height 
					set lxo 5; set lx  4; set ls 7
					set lyo 4; set ly 16
					for { set i 0 } { $i < 16 } { incr i } { 
						set x [expr ($i * $ls + $lxo)]
						set y $lyo 
						set color "black"
						if {$i == 9} {set color "white"}
						$c.synth.midi create rectangle $x $y [expr ($x + $lx)] [expr ($y + $ly)]  -fill $color -width 0 -tag "bar$i"
						fmsynth::tkMidiChnChange $i 0
					}	
				frame $c.synth.player
					set fmsynth::player $c.synth.player
					
					label  $c.synth.player.totaltime -width 8 -text "00:00:00" -padx 5 -font {Consolas 12}
					label  $c.synth.player.playtime  -width 8 -text "00:00:00" -padx 5 -font {Consolas 12 {bold}}
					label  $c.synth.player.l1        -width 2
					button $c.synth.player.btn1      -text "PLAY"  -width 6 -command "fmsynth::tkPlay audio"
					button $c.synth.player.btn2      -text "WAVE"  -width 6 -command "fmsynth::tkPlay render"
					button $c.synth.player.btn3      -text "STOP"  -width 6 -command "fmsynth::tkStop" -state disabled
					pack   $c.synth.player.totaltime $c.synth.player.playtime $c.synth.player.l1 -side left
					pack   $c.synth.player.btn3 $c.synth.player.btn2 $c.synth.player.btn1 -side right
				
				label $c.synth.notes -text 20 -bg green -relief raised -bd 2 -width 3 -anchor center -font {Consolas 11 {bold}}
				label $c.synth.l1    -width 3
				label $c.synth.v0    -text  0 -bg green -relief raised -bd 2 -width 1 -anchor center -font {Consolas 11}
				label $c.synth.v1    -text  1 -bg green -relief raised -bd 2 -width 1 -anchor center -font {Consolas 11}
				label $c.synth.v2    -text  2 -bg green -relief raised -bd 2 -width 1 -anchor center -font {Consolas 11}
				label $c.synth.v3    -text  3 -bg green -relief raised -bd 2 -width 1 -anchor center -font {Consolas 11}
				label $c.synth.v4    -text  4 -bg green -relief raised -bd 2 -width 1 -anchor center -font {Consolas 11}
				label $c.synth.v5    -text  5 -bg green -relief raised -bd 2 -width 1 -anchor center -font {Consolas 11}
				label $c.synth.v6    -text  6 -bg green -relief raised -bd 2 -width 1 -anchor center -font {Consolas 11}
				label $c.synth.v7    -text  7 -bg green -relief raised -bd 2 -width 1 -anchor center -font {Consolas 11}
				label $c.synth.v8    -text  8 -bg green -relief raised -bd 2 -width 1 -anchor center -font {Consolas 11}
				pack $c.synth.midi $c.synth.notes $c.synth.l1  -side left
				pack $c.synth.v0 $c.synth.v1 $c.synth.v2 $c.synth.v3 $c.synth.v4 $c.synth.v5 $c.synth.v6 $c.synth.v7 $c.synth.v8 -side left
				pack $c.synth.player -side right -fill x
	
			pack $c.titel -fill x
			pack $c.synth -side bottom -fill x
			pack $c.sms   -fill both -expand 1
		return $c
	} 
	
	proc mm {c} {
		set cMidiIN "OrangeRed"; set cMidiOUT "DarkBlue"
		frame $c -padx 5 -pady 5
			frame $c.titel -relief raised -padx 5 -pady 5 -bd 2
				label  $c.titel.lbl      -text "MONITOR"       -padx 5 -font $fmsynth::Font
				button $c.titel.btn1     -text "CLEAR"         -padx 5 -command "fmsynth::mmClear"
				pack $c.titel.lbl  -side left
				pack $c.titel.btn1 -side right
			frame $c.mm -relief raised -padx 5 -pady 5 -bd 2
				scrollbar $c.mm.scry -command "$c.mm.txt yview"
				scrollbar $c.mm.scrx -command "$c.mm.txt xview" -orient horizontal
				set fmsynth::mmDisplay [text $c.mm.txt  -width 41 -yscroll "$c.mm.scry set"  -xscroll "$c.mm.scrx set" -font $fmsynth::Font -wrap none -undo 1]
				pack $c.mm.scry -side right  -fill y
				pack $c.mm.scrx -side bottom -fill x
				pack $c.mm.txt  -side left   -fill y -expand 1
				$fmsynth::mmDisplay tag configure NOTEOFF -foreground ForestGreen
				$fmsynth::mmDisplay tag configure NOTEON  -foreground DarkGreen
				$fmsynth::mmDisplay tag configure CCVOL   -foreground DarkRed
				$fmsynth::mmDisplay tag configure CCBAL   -foreground DarkMagenta
				$fmsynth::mmDisplay tag configure CCPAN   -foreground DarkMagenta
				$fmsynth::mmDisplay tag configure CCDLY   -foreground DarkGoldenrod
				$fmsynth::mmDisplay tag configure SYSEX   -foreground MediumVioletRed
				$fmsynth::mmDisplay configure -background PaleGoldenrod -font $fmsynth::mmFont -state disabled
			pack $c.titel     -fill x
			#pack $c.synth     -fill x
			pack $c.mm        -side bottom -fill y -expand 1
		return $c
	}

	proc mmClear {} {
		$fmsynth::mmDisplay configure -state normal
		$fmsynth::mmDisplay delete 1.0 end
		$fmsynth::mmDisplay configure -state disabled
	}
	
	proc check {w evt oldval val typ nr max help } {
		if {$evt == "focusin" && $fmsynth::sbiProofW == "empty"} {
			set fmsynth::sbiProofW $w
			set fmsynth::sbiProofV $val
			if {$typ == "par"} {$fmsynth::sbiHelp configure -text "$help, value: 0-$max"}
			if {$typ == "sbi"} {
				if {$nr == 13} { append help ", value: 0-$max" }
				$fmsynth::sbiHelp configure -text "$help"
			}
			$w configure -bg DarkSeaGreen1
			return 1
		}
		#
		if {$evt == "focusout" && $fmsynth::sbiProofW == $w} {
			if {$val > $max || $val < 0} { 
				$w configure -bg salmon
				focus $w
				return 0
			} 
			$w configure -bg white
			set fmsynth::sbiProofW "empty"
			$fmsynth::sbiHelp configure -text ""
			#
			# update sound data only value change
			#
			if {$fmsynth::sbiProofV == $val} {return 1}
			if {$typ == "par"} {
			    sendMidiSyxPar $nr $val
				set fmsynth::sbiData [$fmsynth::device inst 0]
				set fmsynth::sbiData [lreplace $fmsynth::sbiData 0 0 [$fmsynth::sbiDisplay.name get]]
			}
			if {$typ == "sbi"} {
				incr nr
				set fmsynth::sbiData [lreplace $fmsynth::sbiData $nr $nr $val]
			}
			$fmsynth::sbi set $fmsynth::sbiData
			refreshSoundData
			return 1
		}	
		return 1
	}
	
	proc tkNoteON {c id} {
		$c move $id 1 1
		set note 	[expr $id+20]
		set key  	[expr $note%12]
		set keyName	[lindex $fmsynth::gmNote $key]
		set oct  	[expr $note/12]
		fmsynth::sendMidiMsg [expr 0x90+$fmsynth::channel] $note $fmsynth::volume
		set fmsynth::key "($note) $keyName$oct" 
	}
	proc tkNoteOFF {c id} {
		$c move $id -1 -1
		set note [expr $id+20]
		fmsynth::sendMidiMsg [expr 0x80+$fmsynth::channel] $note 0
		set fmsynth::key "key off"
	 }

	 proc tkCC {cc val} {
		# CTRLCHG		0xB
		#	CC_VOL        7
		#	CC_BAL		  8
		#	CC_PAN		 10
		#	CC_DELAY	 91
		if {$cc == "vol"} {fmsynth::sendMidiMsg [expr 0xB0+$fmsynth::channel]  7 $val}
		if {$cc == "bal"} {fmsynth::sendMidiMsg [expr 0xB0+$fmsynth::channel]  8 $val}
		if {$cc == "dly"} {fmsynth::sendMidiMsg [expr 0xB0+$fmsynth::channel] 91 $val}		
	}
	
	proc tkDrum {status} {
		set drumKey [lindex [$fmsynth::sbi get] 14]
		if {$status == "on"}  {fmsynth::sendMidiMsg [expr 0x90+$fmsynth::channel] $drumKey 127}
		if {$status == "off"} {fmsynth::sendMidiMsg [expr 0x80+$fmsynth::channel] $drumKey   0}
	}
	
	proc tkMidiChnChange {chn volume} {
		# lxo = line width  offset, lx  = line width, ls  = line space
		# lyo = line height offset, ly  = line height 
		# global lxo lx ls
		# global lyo ly
		set lxo 5; set lx  4; set ls 7
		set lyo 4; set ly 16
		#set x [expr ($i * $ls + $lxo)]
		set x [expr ($chn * $ls + $lxo)]
		set y [expr ($lyo + $ly)]
		set value [expr ($ly * $volume / 127)]
		$fmsynth::midiSynth.midi coords "bar$chn" $x $y [expr ($x + $lx)] [expr ($y - $value)] 
	}
	
	# voice{note vol midichn};
	proc tkFmVoiceStatus { } {
		set status [$fmsynth::device status]
		# read fmvoice status
		set fm [lindex $status 0]
		set voice 0
		for {set i 0} {$i < 9} {incr i} {
			# set color ForestGreen
			# if {[lindex $fm $i] > 0 } {set color green}
			set color DarkOrange4
			if {[lindex $fm $i] > 0 } {set color DarkOrange}
			$fmsynth::midiSynth.v$i configure -bg $color
		}
	
		# read midi channel status
		set midi [lindex $status 1]
		# puts "$midi"
		set voices 0; set chn 0
		foreach {vol notes} $midi {
			if {$notes == 0} {set vol 0}
			fmsynth::tkMidiChnChange $chn $vol
			incr voices $notes
			incr chn
		}			
		$fmsynth::midiSynth.notes configure -text $voices
		
		# read playing time
		set cmd [lindex [$fmsynth::player.btn3 configure -state ] 4]
		if {$cmd == "normal"} {
			set playtime [string range [mk::play time] 0 7]
			$fmsynth::player.playtime  configure -text $playtime
			if {$playtime == "00:00:00"} {fmsynth::tkStop}
		} 

		after 50 fmsynth::tkFmVoiceStatus
	}
	
	proc tkPrintMidi {data type} {
		$fmsynth::mmDisplay configure -state normal
		$fmsynth::mmDisplay insert 1.0 "$data\n" $type
		$fmsynth::mmDisplay configure -state disabled
	}

	proc tkPlay {cmd} {
		set extension { {{Midi File} {.mid} }}
		set FileName [tk_getOpenFile -filetypes $extension]
		if {$FileName == ""}  { return }
		if {$cmd == "audio"}  { set action "playing: "}
		if {$cmd == "render"} { set action "rendering: "}
		wm title   . "$fmsynth::version   $action $FileName"
		set totaltime [mk::play start [$fmsynth::device hdl] load $FileName $cmd]
		$fmsynth::player.totaltime configure -text [string range $totaltime 0 7]
		$fmsynth::player.btn1 configure -state disabled
		$fmsynth::player.btn2 configure -state disabled
		$fmsynth::player.btn3 configure -state normal
		return 
	}	
	
	proc tkStop { } {
		wm title   . "$fmsynth::version"
		catch {mk::play stop}
		$fmsynth::player.totaltime configure -text "00:00:00"
		$fmsynth::player.playtime  configure -text "00:00:00"
		$fmsynth::player.btn1 configure -state normal
		$fmsynth::player.btn2 configure -state normal
		$fmsynth::player.btn3 configure -state disabled
	}
	
	#--------------------------------------------------------------------
	# bit tools
	#
	proc bitstream2dec {bs} {
		set size [string length $bs]
		if {$size > 8}  {error "max. 8 bits required"}
		set res 0
		for {set i 0} {$i < $size} {incr i} {
			set pos [expr $size-1 - $i]
			set bit [string range $bs $pos $pos]
			if {$bit != 0 && $bit != 1} {error "invalid binary value"}
			if {$bit == 1}              {set res [expr int(pow(2,$i)*$bit + $res )]}
		}
		return $res
	}
	
	proc setbitfields2byte {args} {
		if {[llength $args] < 1} {error "not enough args"}
		foreach bitfield [lrange $args 0 end] {append res $bitfield}
		if {[string length $res] != 8}  {error "8 bits required"}
		return [fmsynth::bitstream2dec $res]
	}
	
	proc getbitfield {byte bitmask} {
		set mask  [fmsynth::bitstream2dec $bitmask]
		set first [string first 1 $bitmask]
		set last  [string last  1 $bitmask]
		binary scan  [binary format c $byte] B* bitstream			
		set bitfield [string range $bitstream $first $last]
		set bitvalue [fmsynth::bitstream2dec $bitfield]
	}
}

#--------------------------------------------------------------------
# main
#
package require mk
fmsynth::init

wm title   . $fmsynth::version
wm minsize . 1300 800
wm maxsize . 1300 800

frame .main1
	fmsynth::mm       .main1.mm
	fmsynth::sms	  .main1.sms
	fmsynth::synth    .main1.synth
	pack .main1.synth  -side right -fill y
	pack .main1.mm     -side left  -fill y    -expand 1
	pack .main1.sms    -side left  -fill both -expand 1	
pack .main1 -fill both -expand 1

frame .main2
	fmsynth::keyboard 		.main2.kbd 
	frame 					.main2.f1
		fmsynth::filedialog     .main2.f1.file
		fmsynth::keydisplay		.main2.f1.display
		pack .main2.f1.display .main2.f1.file -side bottom -fill x -expand 1 
	pack .main2.kbd     -side left  
	pack .main2.f1		-side left -fill both -expand 1
pack .main2 -side bottom -fill x -expand 1

fmsynth::tkFmVoiceStatus

#bind all 	<Button-1>					{ focus -force %W }
#bind all 	<Any-Enter>					{ focus -force %W }

