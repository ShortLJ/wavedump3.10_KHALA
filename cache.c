int WriteOutputFilesx742(WaveDumpConfig_t *WDcfg, WaveDumpRun_t *WDrun, CAEN_DGTZ_EventInfo_t *EventInfo, CAEN_DGTZ_X742_EVENT_t *Event)
{
	int gr,ch, j, ns;
	char trname[10], flag = 0; 
	int datamax, datamin, isample;

	if( WDcfg->OutFileFlags & OFF_ROOT) {
		for (gr=0;gr<(WDcfg->Nch/8);gr++) {
			if (Event->GrPresent[gr]) {
				for(ch=0; ch<9; ch++) {
					int Size = Event->DataGroup[gr].ChSize[ch];
					if (Size <= 0) {
						continue;
					}
					if(!WDrun->foutROOT){
						char fname[100];
						if (!(WDcfg->UserSetNameFlag))          // Added by LJ
							sprintf(fname, "%swave.root", path);          // Added by LJ
						else if (WDcfg->UserSetNameFlag)          // Added by LJ
							sprintf(fname, "%s%s.root", path, WDcfg->UserSetName);          // Added by LJ
						WDrun->foutROOT = new TFile(fname,"recreate");
					}
					/////////////zerosup
					datamax=Event->DataGroup[gr].DataChannel[ch][0];
					datamin=Event->DataGroup[gr].DataChannel[ch][0];
					for (isample=1; isample<WDcfg->RecordLength-20; isample++){
						if(Event->DataGroup[gr].DataChannel[ch][isample]>datamax) datamax=Event->DataGroup[gr].DataChannel[ch][isample];
						if(Event->DataGroup[gr].DataChannel[ch][isample]<datamin) datamin=Event->DataGroup[gr].DataChannel[ch][isample];
					}
					if(datamax-datamin<WDcfg->PHthreshold) continue;
					////////////
					uint32_t BinHeader[6];
					BinHeader[0] = (WDcfg->Nbit == 8) ? Size + 6*sizeof(*BinHeader) : Size*4 + 6*sizeof(*BinHeader);
					BinHeader[1] = gr;		//EventInfo->BoardId;
					//BinHeader[2] = gr;		//EventInfo->Pattern;
					BinHeader[2] = Event->DataGroup[gr].StartIndexCell;	 //gr;		//EventInfo->Pattern;
					BinHeader[3] = ch;
					BinHeader[4] = EventInfo->EventCounter;
					BinHeader[5] = EventInfo->TriggerTimeTag;

					if( WDcfg->OutFileFlags & OFF_HEADER) {
						// Write the Channel Header
						if(fwrite(BinHeader, sizeof(*BinHeader), 6, WDrun->foutaio) != 6) {
							// error writing to file
							fclose(WDrun->foutaio);
							WDrun->foutaio= NULL;
							return -1;
						}
					}
					ns = (int)fwrite( Event->DataGroup[gr].DataChannel[ch] , 1 , Size*4, WDrun->foutaio) / 4;
					if (ns != Size) {
						// error writing to file
						fclose(WDrun->foutaio);
						WDrun->foutaio= NULL;
						return -1;
					}

					if (WDrun->SingleWrite) {
						fclose(WDrun->foutaio);
						WDrun->foutaio= NULL;
					}
					//} // closing if ch==0,1
			}
		}
	}
} // closig if(WDcfg->AllInOneFlag)

//	printf("\rEventInfo->EventCounter: %u",EventInfo->EventCounter);	// not tested

	return 0;

}


