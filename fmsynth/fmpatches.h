// fmpatches.h
// for fmsynth.c
// by ma.ke. 2020
//   	- without warranty
//   	- use at your own risk
//   	- do what ever you want with this
//   	- be happy
//
// default ADLIB sound data = timbre structure
// the data bytes are:
//  [0,1] AM/VIB/EG/KSR/Multiple bits for carrier and modulator respectively
//  [2,3] KSL/Attenuation settings    for carrier and modulator respectively
//  [4,5] Attack and decay rates      for carrier and modulator respectively
//  [6,7] Sustain and release rates   for carrier and modulator respectively
//  [8,9] Wave select settings        for carrier and modulator respectively
//  [10]  Feedback/connection bits    for the channel (also stereo/pan bits)
//
//  [11]  Percussion voice number     instrument=0,BD=6,SD=7,TT=8,CY=9,HH=10
//	[12]  Number of notes to transpose timbre, signed
//	[13]  For percussive instruments: pitch (MIDI Note 0-127 - GP35..GP87)
//	[14]  reserved
//	[15]  reserved
//

unsigned char SOUND[256][16] =
{
//  general midi compatible instruments sound data (collection ma.ke.)
//
// 	   0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15 	= array
//  0x20  0x23  0x40  0x43  0x60  0x63  0x80  0x83  0xE0  0xE3  0xC0             pitch              = register
	   1,    1,  143,    6,  242,  242,  244,  247,    0,    0,   56,    0,    0,    0,    0,    0, // GM1:AcouGrandPiano
       1,    1,   75,    0,  242,  242,  244,  247,    0,    0,   56,    0,    0,    0,    0,    0, // GM2:BrightAcouGrand
       1,    1,   73,    0,  242,  242,  244,  246,    0,    0,   56,    0,    0,    0,    0,    0, // GM3:ElecGrandPiano
     129,   65,   18,    0,  242,  242,  247,  247,    0,    0,   54,    0,    0,    0,    0,    0, // GM4:Honky-tonkPiano
       1,    1,   87,    0,  241,  242,  247,  247,    0,    0,   48,    0,    0,    0,    0,    0, // GM5:Rhodes Piano
       1,    1,  147,    0,  241,  242,  247,  247,    0,    0,   48,    0,    0,    0,    0,    0, // GM6:Chorused Piano
       1,   22,  128,   14,  161,  242,  242,  245,    0,    0,   56,    0,    0,    0,    0,    0, // GM7:Harpsichord
       1,    1,  146,    0,  194,  194,  248,  248,    0,    0,   58,    0,    0,    0,    0,    0, // GM8:Clavinet
      12,    1,   92,    0,  246,  243,  244,  245,    0,    0,   48,    0,    0,    0,    0,    0, // GM9:Celesta
       7,   17,  151,  128,  243,  242,  242,  241,    0,    0,   50,    0,    0,    0,    0,    0, // GM10:Glockenspiel
      23,    1,   33,    0,   84,  244,  244,  244,    0,    0,   50,    0,    0,    0,    0,    0, // GM11:Music box
     152,  129,   98,    0,  243,  242,  246,  246,    0,    0,   48,    0,    0,    0,    0,    0, // GM12:Vibraphone
      24,    1,   35,    0,  246,  231,  246,  247,    0,    0,   48,    0,    0,    0,    0,    0, // GM13:Marimba
      21,    1,  145,    0,  246,  246,  246,  246,    0,    0,   52,    0,    0,    0,    0,    0, // GM14:Xylophone
      69,  129,   89,  128,  211,  163,  243,  243,    0,    0,   60,    0,    0,    0,    0,    0, // GM15:Tubular Bells
       3,  129,   73,  128,  117,  181,  245,  245,    1,    0,   52,    0,    0,    0,    0,    0, // GM16:Dulcimer
     113,   49,  146,    0,  246,  241,   20,    7,    0,    0,   50,    0,    0,    0,    0,    0, // GM17:Hammond Organ
     114,   48,   20,    0,  199,  199,   88,    8,    0,    0,   50,    0,    0,    0,    0,    0, // GM18:Percussive Organ
     112,  177,   68,    0,  170,  138,   24,    8,    0,    0,   52,    0,    0,    0,    0,    0, // GM19:Rock Organ
      35,  177,  147,    0,  151,   85,   35,   20,    1,    0,   52,    0,    0,    0,    0,    0, // GM20:Church Organ
      97,  177,   19,  128,  151,   85,    4,    4,    1,    0,   48,    0,    0,    0,    0,    0, // GM21:Reed Organ
      36,  177,   72,    0,  152,   70,   42,   26,    1,    0,   60,    0,    0,    0,    0,    0, // GM22:Accordion
      97,   33,   19,    0,  145,   97,    6,    7,    1,    0,   58,    0,    0,    0,    0,    0, // GM23:Harmonica
      33,  161,   19,  137,  113,   97,    6,    7,    0,    0,   54,    0,    0,    0,    0,    0, // GM24:Tango Accordion
       2,   65,  156,  128,  243,  243,  148,  200,    1,    0,   60,    0,    0,    0,    0,    0, // GM25:Acoustic Guitar1
       3,   17,   84,    0,  243,  241,  154,  231,    1,    0,   60,    0,    0,    0,    0,    0, // GM26:Acoustic Guitar2
      35,   33,   95,    0,  241,  242,   58,  248,    0,    0,   48,    0,    0,    0,    0,    0, // GM27:Electric Guitar1
       3,   33,  135,  128,  246,  243,   34,  248,    1,    0,   54,    0,    0,    0,    0,    0, // GM28:Electric Guitar2
       3,   33,   71,    0,  249,  246,   84,   58,    0,    0,   48,    0,    0,    0,    0,    0, // GM29:Electric Guitar3
      35,   33,   74,    5,  145,  132,   65,   25,    1,    0,   56,    0,    0,    0,    0,    0, // GM30:Overdrive Guitar
      35,   33,   74,    0,  149,  148,   25,   25,    1,    0,   56,    0,    0,    0,    0,    0, // GM31:Distorton Guitar
       9,  132,  161,  128,   32,  209,   79,  248,    0,    0,   56,    0,    0,    0,    0,    0, // GM32:Guitar Harmonics
      33,  162,   30,    0,  148,  195,    6,  166,    0,    0,   50,    0,    0,    0,    0,    0, // GM33:Acoustic Bass
      49,   49,   18,    0,  241,  241,   40,   24,    0,    0,   58,    0,    0,    0,    0,    0, // GM34:Electric Bass 1
      49,   49,  141,    0,  241,  241,  232,  120,    0,    0,   58,    0,    0,    0,    0,    0, // GM35:Electric Bass 2
      49,   50,   91,    0,   81,  113,   40,   72,    0,    0,   60,    0,    0,    0,    0,    0, // GM36:Fretless Bass
       1,   33,  139,   64,  161,  242,  154,  223,    0,    0,   56,    0,    0,    0,    0,    0, // GM37:Slap Bass 1
      33,   33,  139,    8,  162,  161,   22,  223,    0,    0,   56,    0,    0,    0,    0,    0, // GM38:Slap Bass 2
      49,   49,  139,    0,  244,  241,  232,  120,    0,    0,   58,    0,    0,    0,    0,    0, // GM39:Synth Bass 1
      49,   49,   18,    0,  241,  241,   40,   24,    0,    0,   58,    0,    0,    0,    0,    0, // GM40:Synth Bass 2
      49,   33,   21,    0,  221,   86,   19,   38,    1,    0,   56,    0,    0,    0,    0,    0, // GM41:Violin
      49,   33,   22,    0,  221,  102,   19,    6,    1,    0,   56,    0,    0,    0,    0,    0, // GM42:Viola
     113,   49,   73,    0,  209,   97,   28,   12,    1,    0,   56,    0,    0,    0,    0,    0, // GM43:Cello
      33,   35,   77,  128,  113,  114,   18,    6,    1,    0,   50,    0,    0,    0,    0,    0, // GM44:Contrabass
     241,  225,   64,    0,  241,  111,   33,   22,    1,    0,   50,    0,    0,    0,    0,    0, // GM45:Tremulo Strings
       2,    1,   26,  128,  245,  133,  117,   53,    1,    0,   48,    0,    0,    0,    0,    0, // GM46:Pizzicato String
       2,    1,   29,  128,  245,  243,  117,  244,    1,    0,   48,    0,    0,    0,    0,    0, // GM47:Orchestral Harp
      16,   17,   65,    0,  245,  242,    5,  195,    1,    0,   50,    0,    0,    0,    0,    0, // GM48:Timpany
      33,  162,  155,    1,  177,  114,   37,    8,    1,    0,   62,    0,    0,    0,    0,    0, // GM49:String Ensemble1
     161,   33,  152,    0,  127,   63,    3,    7,    1,    1,   48,    0,    0,    0,    0,    0, // GM50:String Ensemble2
     161,   97,  147,    0,  193,   79,   18,    5,    0,    0,   58,    0,    0,    0,    0,    0, // GM51:Synth Strings 1
      33,   97,   24,    0,  193,   79,   34,    5,    0,    0,   60,    0,    0,    0,    0,    0, // GM52:SynthStrings 2
      49,  114,   91,  131,  244,  138,   21,    5,    0,    0,   48,    0,    0,    0,    0,    0, // GM53:Choir Aahs
     161,   97,  144,    0,  116,  113,   57,  103,    0,    0,   48,    0,    0,    0,    0,    0, // GM54:Voice Oohs
     113,  114,   87,    0,   84,  122,    5,    5,    0,    0,   60,    0,    0,    0,    0,    0, // GM55:Synth Voice
     144,   65,    0,    0,   84,  165,   99,   69,    0,    0,   56,    0,    0,    0,    0,    0, // GM56:Orchestra Hit
      33,   33,  146,    1,  133,  143,   23,    9,    0,    0,   60,    0,    0,    0,    0,    0, // GM57:Trumpet
      33,   33,  148,    5,  117,  143,   23,    9,    0,    0,   60,    0,    0,    0,    0,    0, // GM58:Trombone
      33,   97,  148,    0,  118,  130,   21,   55,    0,    0,   60,    0,    0,    0,    0,    0, // GM59:Tuba
      49,   33,   67,    0,  158,   98,   23,   44,    1,    1,   50,    0,    0,    0,    0,    0, // GM60:Muted Trumpet
      33,   33,  155,    0,   97,  127,  106,   10,    0,    0,   50,    0,    0,    0,    0,    0, // GM61:French Horn
      97,   34,  138,    6,  117,  116,   31,   15,    0,    0,   56,    0,    0,    0,    0,    0, // GM62:Brass Section
     161,   33,  134,  131,  114,  113,   85,   24,    1,    0,   48,    0,    0,    0,    0,    0, // GM63:Synth Brass 1
      33,   33,   77,    0,   84,  166,   60,   28,    0,    0,   56,    0,    0,    0,    0,    0, // GM64:Synth Brass 2
      49,   97,  143,    0,  147,  114,    2,   11,    1,    0,   56,    0,    0,    0,    0,    0, // GM65:Soprano Sax
      49,   97,  142,    0,  147,  114,    3,    9,    1,    0,   56,    0,    0,    0,    0,    0, // GM66:Alto Sax
      49,   97,  145,    0,  147,  130,    3,    9,    1,    0,   58,    0,    0,    0,    0,    0, // GM67:Tenor Sax
      49,   97,  142,    0,  147,  114,   15,   15,    1,    0,   58,    0,    0,    0,    0,    0, // GM68:Baritone Sax
      33,   33,   75,    0,  170,  143,   22,   10,    1,    0,   56,    0,    0,    0,    0,    0, // GM69:Oboe
      49,   33,  144,    0,  126,  139,   23,   12,    1,    1,   54,    0,    0,    0,    0,    0, // GM70:English Horn
      49,   50,  129,    0,  117,   97,   25,   25,    1,    0,   48,    0,    0,    0,    0,    0, // GM71:Bassoon
      50,   33,  144,    0,  155,  114,   33,   23,    0,    0,   52,    0,    0,    0,    0,    0, // GM72:Clarinet
     225,  225,   31,    0,  133,  101,   95,   26,    0,    0,   48,    0,    0,    0,    0,    0, // GM73:Piccolo
     225,  225,   70,    0,  136,  101,   95,   26,    0,    0,   48,    0,    0,    0,    0,    0, // GM74:Flute
     161,   33,  156,    0,  117,  117,   31,   10,    0,    0,   50,    0,    0,    0,    0,    0, // GM75:Recorder
      49,   33,  139,    0,  132,  101,   88,   26,    0,    0,   48,    0,    0,    0,    0,    0, // GM76:Pan Flute
     225,  161,   76,    0,  102,  101,   86,   38,    0,    0,   48,    0,    0,    0,    0,    0, // GM77:Bottle Blow
      98,  161,  203,    0,  118,   85,   70,   54,    0,    0,   48,    0,    0,    0,    0,    0, // GM78:Shakuhachi
      98,  161,  153,    0,   87,   86,    7,    7,    0,    0,   59,    0,    0,    0,    0,    0, // GM79:Whistle
      98,  161,  147,    0,  119,  118,    7,    7,    0,    0,   59,    0,    0,    0,    0,    0, // GM80:Ocarina
      34,   33,   89,    0,  255,  255,    3,   15,    2,    0,   48,    0,    0,    0,    0,    0, // GM81:Lead 1 squareea
      33,   33,   14,    0,  255,  255,   15,   15,    1,    1,   48,    0,    0,    0,    0,    0, // GM82:Lead 2 sawtooth
      34,   33,   70,  128,  134,  100,   85,   24,    0,    0,   48,    0,    0,    0,    0,    0, // GM83:Lead 3 calliope
      33,  161,   69,    0,  102,  150,   18,   10,    0,    0,   48,    0,    0,    0,    0,    0, // GM84:Lead 4 chiff
      33,   34,  139,    0,  146,  145,   42,   42,    1,    0,   48,    0,    0,    0,    0,    0, // GM85:Lead 5 charang
     162,   97,  158,   64,  223,  111,    5,    7,    0,    0,   50,    0,    0,    0,    0,    0, // GM86:Lead 6 voice
      32,   96,   26,    0,  239,  143,    1,    6,    0,    2,   48,    0,    0,    0,    0,    0, // GM87:Lead 7 fifths
      33,   33,  143,  128,  241,  244,   41,    9,    0,    0,   58,    0,    0,    0,    0,    0, // GM88:Lead 8 brass
     119,  161,  165,    0,   83,  160,  148,    5,    0,    0,   50,    0,    0,    0,    0,    0, // GM89:Pad 1 new age
      97,  177,   31,  128,  168,   37,   17,    3,    0,    0,   58,    0,    0,    0,    0,    0, // GM90:Pad 2 warm
      97,   97,   23,    0,  145,   85,   52,   22,    0,    0,   60,    0,    0,    0,    0,    0, // GM91:Pad 3 polysynth
     113,  114,   93,    0,   84,  106,    1,    3,    0,    0,   48,    0,    0,    0,    0,    0, // GM92:Pad 4 choir
      33,  162,  151,    0,   33,   66,   67,   53,    0,    0,   56,    0,    0,    0,    0,    0, // GM93:Pad 5 bowedpad
     161,   33,   28,    0,  161,   49,  119,   71,    1,    1,   48,    0,    0,    0,    0,    0, // GM94:Pad 6 metallic
      33,   97,  137,    3,   17,   66,   51,   37,    0,    0,   58,    0,    0,    0,    0,    0, // GM95:Pad 7 halo
     161,   33,   21,    0,   17,  207,   71,    7,    1,    0,   48,    0,    0,    0,    0,    0, // GM96:Pad 8 sweep
      58,   81,  206,    0,  248,  134,  246,    2,    0,    0,   50,    0,    0,    0,    0,    0, // GM97:FX 1 rain
      33,   33,   21,    0,   33,   65,   35,   19,    1,    0,   48,    0,    0,    0,    0,    0, // GM98:FX 2 soundtrack
       6,    1,   91,    0,  116,  165,  149,  114,    0,    0,   48,    0,    0,    0,    0,    0, // GM99:FX 3 crystal
      34,   97,  146,  131,  177,  242,  129,   38,    0,    0,   60,    0,    0,    0,    0,    0, // GM100:FX 4 atmosphere
      65,   66,   77,    0,  241,  242,   81,  245,    1,    0,   48,    0,    0,    0,    0,    0, // GM101:FX 5 brightness
      97,  163,  148,  128,   17,   17,   81,   19,    1,    0,   54,    0,    0,    0,    0,    0, // GM102:FX 6 goblins
      97,  161,  140,  128,   17,   29,   49,    3,    0,    0,   54,    0,    0,    0,    0,    0, // GM103:FX 7 echoes
     164,   97,   76,    0,  243,  129,  115,   35,    1,    0,   52,    0,    0,    0,    0,    0, // GM104:FX 8 sci-fi
       2,    7,  133,    3,  210,  242,   83,  246,    0,    1,   48,    0,    0,    0,    0,    0, // GM105:Sitar
      17,   19,   12,  128,  163,  162,   17,  229,    1,    0,   48,    0,    0,    0,    0,    0, // GM106:Banjo
      17,   17,    6,    0,  246,  242,   65,  230,    1,    2,   52,    0,    0,    0,    0,    0, // GM107:Shamisen
     147,  145,  145,    0,  212,  235,   50,   17,    0,    1,   56,    0,    0,    0,    0,    0, // GM108:Koto
       4,    1,   79,    0,  250,  194,   86,    5,    0,    0,   60,    0,    0,    0,    0,    0, // GM109:Kalimba
      33,   34,   73,    0,  124,  111,   32,   12,    0,    1,   54,    0,    0,    0,    0,    0, // GM110:Bagpipe
      49,   33,  133,    0,  221,   86,   51,   22,    1,    0,   58,    0,    0,    0,    0,    0, // GM111:Fiddle
      32,   33,    4,  129,  218,  143,    5,   11,    2,    0,   54,    0,    0,    0,    0,    0, // GM112:Shanai
       5,    3,  106,  128,  241,  195,  229,  229,    0,    0,   54,    0,    0,    0,    0,    0, // GM113:Tinkle Bell
       7,    2,   21,    0,  236,  248,   38,   22,    0,    0,   58,    0,    0,    0,    0,    0, // GM114:Agogo Bells
       5,    1,  157,    0,  103,  223,   53,    5,    0,    0,   56,    0,    0,    0,    0,    0, // GM115:Steel Drums
      24,   18,  150,    0,  250,  248,   40,  229,    0,    0,   58,    0,    0,    0,    0,    0, // GM116:Woodblock
      16,    0,  134,    3,  168,  250,    7,    3,    0,    0,   54,    0,    0,    0,    0,    0, // GM117:Taiko Drum
      17,   16,   65,    3,  248,  243,   71,    3,    2,    0,   52,    0,    0,    0,    0,    0, // GM118:Melodic Tom
       1,   16,  142,    0,  241,  243,    6,    2,    2,    0,   62,    0,    0,    0,    0,    0, // GM119:Synth Drum
      14,  192,    0,    0,   31,   31,    0,  255,    0,    3,   62,    0,    0,    0,    0,    0, // GM120:Reverse Cymbal
       6,    3,  128,  136,  248,   86,   36,  132,    0,    2,   62,    0,    0,    0,    0,    0, // GM121:Guitar FretNoise
      14,  208,    0,    5,  248,   52,    0,    4,    0,    3,   62,    0,    0,    0,    0,    0, // GM122:Breath Noise
      14,  192,    0,    0,  246,   31,    0,    2,    0,    3,   62,    0,    0,    0,    0,    0, // GM123:Seashore
     213,  218,  149,   64,   55,   86,  163,   55,    0,    0,   48,    0,    0,    0,    0,    0, // GM124:Bird Tweet
      53,   20,   92,    8,  178,  244,   97,   21,    2,    0,   58,    0,    0,    0,    0,    0, // GM125:Telephone
      14,  208,    0,    0,  246,   79,    0,  245,    0,    3,   62,    0,    0,    0,    0,    0, // GM126:Helicopter
      38,  228,    0,    0,  255,   18,    1,   22,    0,    1,   62,    0,    0,    0,    0,    0, // GM127:Applause/Noise
       0,    0,    0,    0,  243,  246,  240,  201,    0,    2,   62,    0,    0,    0,    0,    0, // GM128:Gunshot
//
//  general midi compatible drums sound data
//
// 	   0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15 	= array
//  0x20  0x23  0x40  0x43  0x60  0x63  0x80  0x83  0xE0  0xE3  0xC0             pitch              = register
	0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP1: default GP37
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP2:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP3:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP4:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP5:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP6:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP7:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP8:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP9:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP10:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP11:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP12:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP13:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP14:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP15:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP16:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP17:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP18:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP19:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP20:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP21:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP22:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP23:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP24:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP25:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP26:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP27:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP28:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP29:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP30:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP31:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP32:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP33:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP34:	

//********************************************************************************************************************************
      16,   17,   68,    0,  248,  243,  119,    6,    2,    0,   56,    0,    0,   33,    0,    0, // ! 35: Ac Bass Drum
	0x00, 0x00, 0x00, 0x00, 0xF6, 0xF6, 0x0C, 0x06, 0x00, 0x00, 0x04, 0x00, 0x00,   45, 0x00, 0x00, // ! 36: Bass Drum
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00,   57, 0x00, 0x00, // ! 37: Side Kick
	0x00, 0x00, 0x00, 0x00, 0xFC, 0xFA, 0x05, 0x17, 0x02, 0x00, 0x0E, 0x00, 0x00,   48, 0x00, 0x00, // ! 38: Acoustic Snare
    0x30, 0x3A, 0x00, 0x00, 0x9F, 0xE8, 0x0F, 0xFA, 0x00, 0x00, 0x0E, 0x00, 0x00,   59, 0x00, 0x00, // ! 39: Hand Clap
	0x00, 0x00, 0x00, 0x00, 0xFC, 0xFA, 0x05, 0x17, 0x02, 0x00, 0x0E, 0x00, 0x00,   60, 0x00, 0x00, // ! 40: Electric Snare
       0,    0,    0,    0,  246,  246,   12,    6,    0,    0,   52,    0,    0,   45,    0,    0, // ! 41: Low Floor Tom
    0x2E, 0x06, 0x05, 0x02, 0xF5, 0x99, 0xF1, 0xF9, 0x00, 0x03, 0x00, 0x00, 0x00,   97, 0x00, 0x00, // ! 42: Closed High Hat
	   0,    0,    0,    0,  246,  246,   12,    6,    0,    0,   52,    0,    0,   45,    0,    0, // ! 43: High Floor Tom  
	0x0C, 0x12, 0x00, 0x05, 0xF6, 0x7B, 0x08, 0x47, 0x00, 0x02, 0x0A, 0x00, 0x00,   76, 0x00, 0x00, // ! 44: Pedal High Hat
       0,    0,    0,    0,  246,  246,   12,    6,    0,    0,   52,    0,    0,   50,    0,    0, // ! 45: Low Tom   
    0x0B, 0x11, 0x00, 0x00, 0xF6, 0xEC, 0x02, 0x53, 0x00, 0x02, 0x0A, 0x00, 0x00,   47, 0x00, 0x00, // ! 46: Open High Hat	
       0,    0,    0,    0,  246,  246,   12,    6,    0,    0,   52,    0,    0,   55,    0,    0, // ! 47: Low-Mid Tom
       0,    0,    0,    0,  246,  246,   12,    6,    0,    0,   52,    0,    0,   57,    0,    0, // ! 48: High-Mid Tom
//    0xA4, 0x03, 0x02, 0x80, 0xB2, 0x97, 0xA2, 0xD4, 0x02, 0x01, 0x0E, 0x00, 0x00, 0x26, 0x00, 0x00, // ? 49: Crash Cymbal 1
      38,   30,    0,    0,  224,  255,  241,   49,    0,    0,    0,    0,    0,   60,    0,    0, // ? 49: Crash Cymbal 1 
       0,    0,    0,    0,  246,  246,   12,    6,    0,    0,   52,    0,    0,   60,    0,    0, // ! 50: High Tom
//    0x0E, 0x07, 0x08, 0x4A, 0xF8, 0xF4, 0x42, 0xE4, 0x00, 0x03, 0x0E, 0x00, 0x00, 0x50, 0x00, 0x00, // ? 51: Ride Cymbal 1
	  38,   30,    3,    0,  224,  255,  240,   49,    0,    0,    8,    0,    0,   60,    0,    0, // ? 51: Ride Cymbal 1
//    0x0F, 0xD7, 0x00, 0x03, 0xF5, 0x9F, 0x30, 0x02, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x54, 0x00, 0x00, // ? 52: Chinese Cymbal
	  60,   17,    5,    0,  115,   98,  178,  147,    0,    0,   10,    0,    0,   60,    0,    0, // ? 52: Chinese Cymbal
//    0x0E, 0x07, 0x00, 0x40, 0xE4, 0xF5, 0xE4, 0xE5, 0x03, 0x01, 0x06, 0x00, 0x00,   31, 0x00, 0x00, // ? 53: Ride Bell
	  24,   19,   79,    8,  242,  242,   97,  116,    0,    0,    8,    0,    0,   60,    0,    0, // ? 53: Ride Bell
//    0x2E, 0x06, 0x09, 0x02, 0xF5, 0x87, 0xF1, 0xF7, 0x00, 0x03, 0x00, 0x00, 0x00,   98, 0x00, 0x00, // ? 54: Tambourine
	 178,   49,    2,    0,   98,  117,  210,  213,    0,    0,   14,    0,    0,   60,    0,    0, // ? 54: Tambourine
//    0x2E, 0x0E, 0x0E, 0x00, 0xBA, 0xB4, 0x10, 0xF4, 0x02, 0x02, 0x0E, 0x00, 0x00, 0x3F, 0x00, 0x00, // ? 55: Splash Cymbal
	  42,   17,   10,    7,  115,   98,  177,  146,    3,    0,   10,    0,    0,   60,    0,    0, // ? 55: Splash Cymbal
      17,   16,   69,    8,  248,  243,   55,    5,    2,    0,   56,    0,    0,   83,    0,    0, // ! GP56:Cow Bell
      14,  208,    0,    0,  246,  159,    0,    2,    0,    3,   62,    0,    0,  108,    0,    0, // ! GP57:Crash Cymbal 2
    0x80, 0x00, 0x00, 0x00, 0xFF, 0xF5, 0x00, 0xF7, 0x01, 0x01, 0x0C, 0x00, 0x00,   31, 0x00, 0x00, // ! 58: Vibraslap
    0x1E, 0x0E, 0x00, 0x00, 0xF0, 0xF7, 0xF0, 0xB6, 0x00, 0x00, 0x0E, 0x00, 0x00,   21, 0x00, 0x00, // ! 59: Ride Cymbal 2
       6,    2,   11,    0,  245,  245,   12,    8,    0,    0,   54,    0,    0,   60,    0,    0, // ! 60: High Bongo
       1,    2,    0,    0,  250,  200,  191,  151,    0,    0,   55,    0,    0,   65,    0,    0, // ! 61: Low Bongo
       1,    1,   81,    0,  250,  250,  135,  183,    0,    0,   54,    0,    0,   59,    0,    0, // ! 62: Mute High Conga
       1,    2,   84,    0,  250,  248,  141,  184,    0,    0,   54,    0,    0,   51,    0,    0, // ! 63: Open High Conga
       1,    2,   89,    0,  250,  248,  136,  182,    0,    0,   54,    0,    0,   45,    0,    0, // ! 64: Low Conga
       1,    0,    0,    0,  249,  250,   10,    6,    3,    0,   62,    0,    0,   71,    0,    0, // ! 65: High Timbale
       0,    0,  128,    0,  249,  246,  137,  108,    3,    0,   62,    0,    0,   60,    0,    0, // ! 66: Low Timbale
    0x0E, 0x0E, 0x00, 0x00, 0xF8, 0xF8, 0xF6, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00,   71, 0x00, 0x00, // ! 67: High Agogo
    0x0E, 0x0E, 0x00, 0x00, 0xF8, 0xF8, 0xF6, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00,   73, 0x00, 0x00, // ! 68: Low Agogo
      14,    0,   64,    8,  118,  119,   79,   24,    0,    2,   62,    0,    0,   65,    0,    0, // ! 69: Cabasa
      14,    3,   64,    0,  200,  155,   73,  105,    0,    2,   62,    0,    0,   73,    0,    0, // ! 70: Maracas
     215,  199,  220,    0,  173,  141,    5,    5,    3,    0,   62,    0,    0,   53,    0,    0, // ! 71: Short Whistle
     215,  199,  220,    0,  168,  136,    4,    4,    3,    0,   62,    0,    0,   41,    0,    0, // ! 72: Long Whistle
     128,   17,    0,    0,  246,  103,    6,   23,    3,    3,   62,    0,    0,   86,    0,    0, // ? 73: Short Guiro
     128,   17,    0,    9,  245,   70,    5,   22,    2,    3,   62,    0,    0,   86,    0,    0, // ? 74: Long Guiro
    0x13, 0x08, 0x40, 0x00, 0xFB, 0xE8, 0xFF, 0xFF, 0x00, 0x00, 0x08, 0x00, 0x00,   36, 0x00, 0x00, // ! 75: Claves
    0x27, 0x12, 0x22, 0x00, 0xFA, 0xF6, 0xF2, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x00,   65, 0x00, 0x00, // ! 76: High Wood Block
    0x27, 0x12, 0x22, 0x00, 0xFA, 0xF6, 0xF2, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x00,   53, 0x00, 0x00, // ! 77: Low Wood Block
       1,    2,   88,    0,  103,  117,  231,    7,    0,    0,   48,    0,    0,   74,    0,    0, // ! 78: Mute Cuica
      65,   66,   69,    8,  248,  117,   72,    5,    0,    0,   48,    0,    0,   60,    0,    0, // ! 79: Open Cuica
//    0x26, 0x27, 0x44, 0x40, 0xF1, 0xF7, 0xF0, 0xFC, 0x00, 0x00, 0x06, 0x00, 0x00,   81, 0x00, 0x00, // ! 80: Mute Triangle
	   8,    9,  192,    0,  241,  252,  240,   22,    1,    0,    0,    0,    0,   60,    0,    0, // ! 80: Mute Triangle 
//    0x26, 0x27, 0x44, 0x40, 0xF1, 0xF5, 0xF0, 0xF5, 0x00, 0x00, 0x06, 0x00, 0x00,  101, 0x00, 0x00, // ! 81: Open Triangle
	  12,    7,  207,   15,  241,  252,  240,    4,    2,    0,    0,    0,    0,   60,    0,    0, // ! 81: Open Triangle
//********************************************************************************************************************************
	0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP82: default GP37
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP83:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP84:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP85:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP86:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP87:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP88:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP89:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP90:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP91:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP92:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP93:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP94:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP95:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP96:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP97:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP98:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP99:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP100:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP101:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP102:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP103:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP104:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP105:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP106:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP107:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP108:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP109:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP110:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP111:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP112:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP113:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP114:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP115:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP116:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP117:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP118:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP119:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP120:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP121:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP122:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP123:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP124:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP125:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP126:
    0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP127:
	0x16, 0x11, 0x08, 0x02, 0xF1, 0xF9, 0xFB, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, // GP128:
};
