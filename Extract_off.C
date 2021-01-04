
void Extract_off()
{
	TFile *file_open = new TFile("beamtest_2.root","read");
	TTree *tree_out_open = (TTree*) file_open->Get("BinaryOut");
	ULong64_t	nLoop;
	Int_t		ntdc;
	Int_t		nadc;
	ULong64_t	tdcData[2048];
	ULong64_t	adcData[2048];
	ULong64_t	unix_time;

	tree_out_open -> SetBranchAddress("ReadCycle",	&nLoop		);
	tree_out_open -> SetBranchAddress("ntdc",	&ntdc		);
	tree_out_open -> SetBranchAddress("nadc",	&nadc		);
	tree_out_open -> SetBranchAddress("tdc",	tdcData		);
	tree_out_open -> SetBranchAddress("adc",	adcData		);
	tree_out_open -> SetBranchAddress("unix_time",	&unix_time	);

	TFile *file_write = new TFile("beamtest_trig_2.root","recreate");
	TTree *tree_out_write = new TTree("tree_out_write","trig_id");
	Int_t TDC_triggerID = -999;
	Int_t ADC_triggerID = -999;
	Int_t Trig_differ = -999;
	Int_t nevt = 9;
	Int_t TriggerID_Offset = 0;
  Int_t adc_index_correction = 0;
  Int_t tdc_index_correction = 0;
  Int_t Sync_Correction = 0;
	Int_t TDC_corrected_triggerID = -999;
  Int_t ADC_corrected_triggerID = -999;
	Int_t Trig_corrected_differ = -999;

	Int_t tdcheader = -999;
	Int_t adcheader = -999;

	tree_out_write->Branch("TDC_triggerID",&TDC_triggerID,"TDC_triggerID/I");
	tree_out_write->Branch("ADC_triggerID",&ADC_triggerID,"ADC_triggerID/I");
	tree_out_write->Branch("Trig_differ",&Trig_differ,"Trig_differ/I");
  tree_out_write->Branch("TDC_corrected_triggerID",&TDC_corrected_triggerID,"TDC_corrected_triggerID/I");
  tree_out_write->Branch("ADC_corrected_triggerID",&ADC_corrected_triggerID,"ADC_corrected_triggerID/I");
	tree_out_write->Branch("Trig_corrected_differ",&Trig_corrected_differ,"Trig_corrected_differ/I");
	tree_out_write->Branch("nevt",&nevt,"Row/I");

	Int_t evt = tree_out_open->GetEntries();
	for(int k = 10; k <evt; k++)
	{
		tree_out_open->GetEntry(k);
		TDC_triggerID = -999;
		ADC_triggerID = -999;
		Trig_differ = -999;
		adcheader = -999;
		tdcheader = -999;

		for(int i = 0; i<ntdc; i++){
			tdcheader = (tdcData[i] >> 27) & 0x1F;
			if (tdcheader == 8){
				TDC_triggerID = ((int)(tdcData[i] >> 5) & 0x3FFFFF);
				break;
			}
		}

		for(int i = 0; i < nadc; i++){
			adcheader = (adcData[i] >> 24) & 0x07;
			if(adcheader == 4){
				ADC_triggerID = (int)((adcData[i] & 0xFFFFFF));
				break;
			}
		}

		Trig_differ = abs(TDC_triggerID-ADC_triggerID);

		if(TDC_triggerID ==-999 || ADC_triggerID ==-999) continue;
		if(TDC_triggerID == 0 || ADC_triggerID == 0) continue;
		if(k==10) TriggerID_Offset = ADC_triggerID - TDC_triggerID;
		Sync_Correction = 0;
    Sync_Correction = (ADC_triggerID - TDC_triggerID) - TriggerID_Offset;
    tdc_index_correction = 0;
    adc_index_correction = 0;
    if (Sync_Correction > 0) tdc_index_correction = Sync_Correction; // ADC-Late case
    if (Sync_Correction < 0) adc_index_correction = -1*Sync_Correction; // TDC-Late case
		//if(adc_index_correction) cout << adc_index_correction << "-----------------------------------------------"<<endl;

		if(k+tdc_index_correction == evt) break;
		if(k+adc_index_correction == evt) break;

		TDC_corrected_triggerID = TDC_triggerID;
		ADC_corrected_triggerID = ADC_triggerID;
		Trig_corrected_differ = -999;

			if (Sync_Correction > 0){
				tdcheader = -999;
				tree_out_open->GetEntry(k+tdc_index_correction);
				for(int i = 0; i<ntdc; i++){
					tdcheader = (tdcData[i] >> 27) & 0x1F;
					if (tdcheader == 8){
						TDC_corrected_triggerID = ((int)(tdcData[i] >> 5) & 0x3FFFFF);
						break;
					}
				}
			}
			if (Sync_Correction < 0){
				adcheader = -999;
				tree_out_open->GetEntry(k+adc_index_correction);
				for(int i = 0; i < nadc; i++){
					adcheader = (adcData[i] >> 24) & 0x07;
					if(adcheader == 4){
						ADC_corrected_triggerID = (int)((adcData[i] & 0xFFFFFF));
						break;
					}
				}
			}
		//cout << k <<"/"<<tree_out_open->GetEntries() << endl;

		if(TDC_corrected_triggerID == -999 || ADC_corrected_triggerID == -999) continue;
		if(TDC_corrected_triggerID == 0 || ADC_corrected_triggerID == 0) continue;

		Trig_corrected_differ = abs(TDC_corrected_triggerID-ADC_corrected_triggerID);

		tree_out_write->Fill();
		nevt++;
	}
	tree_out_write->Write();
	file_write->Close();
}
