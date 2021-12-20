#ifndef V1742_Hit_h
#define V1742_Hit_h

#include <TROOT.h>
#include "TObject.h"
using namespace std ;

struct V1742_Hit{
	public:
		//UInt_t EventSize;
		UInt_t BoardId;
		UInt_t Group;
		UInt_t StartIndexCell;
		UInt_t Channel;
		UInt_t EventNumber;
		UInt_t TrigTimeTag;		

		UInt_t RecordLength;
		Float_t *Waveform;

		Float_t pedestal=-1;
		Float_t rPH=-1;
		Float_t QDC=-1;
		Float_t QDCShort=-1;
		Float_t fineTimestamp=-1;
		Float_t CalibE=-1;
		Float_t PSD;

};

#endif
