void VME_Conv_hj()
{
	TFile * file_in = new TFile("beamtest_2.root","READ");
	TTree * tree_in = (TTree*) file_in -> Get("BinaryOut");

	ULong64_t ReadCycle;
	Int_t ntdc;
	Int_t nadc;
	ULong64_t tdc[2048];
	ULong64_t adc[2048];
	ULong64_t unix_time;

	tree_in -> SetBranchAddress("ReadCycle",&ReadCycle); 
	tree_in -> SetBranchAddress("ntdc",&ntdc); 
	tree_in -> SetBranchAddress("nadc",&nadc); 
	tree_in -> SetBranchAddress("tdc",tdc); 
	tree_in -> SetBranchAddress("adc",adc); 
	tree_in -> SetBranchAddress("unix_time",&unix_time); 

	TFile * file_make = new TFile("beamtest_2_conv.root","RECREATE");
	TTree * tree_tdc = new TTree("tree_TDC","TDCTree");
	TTree * tree_adc = new TTree("tree_ADC","ADCTree");

	Int_t tdc_length = 999;
	Int_t tdc_order_length = 999;
	UShort_t tdc_ch[2048] = {999,};
	UInt_t tdc_value[2048] = {999,};
	UInt_t tdc_buf_cnt = 999;
	Int_t tdc_triggerID = 999;
	Int_t tdc_header_cnt = 999;
	Int_t tdc_data_cnt = 999;
	Int_t tdc_trailer_cnt = 999;
	Int_t tdc_evt_num = 999;
	Int_t tdc_order_value[2048] = {999,};

        Int_t adc_length = 999;
        Int_t adc_order_length = 999;
        UShort_t adc_ch[2048] = {999,};
        UInt_t adc_value[2048] = {999,};
        UInt_t adc_buf_cnt = 999;
	Int_t adc_triggerID = 999;
	Int_t adc_header_cnt = 999;
	Int_t adc_data_cnt = 999;
	Int_t adc_trailer_cnt = 999;
	Int_t adc_evt_num = 999;
	Int_t adc_order_value[2048] = {999,};


	tree_tdc -> Branch("tdc_length", &tdc_length, "tdc_length/i");
	tree_tdc -> Branch("tdc_order_length", &tdc_order_length, "tdc_order_length/i");
	tree_tdc -> Branch("tdc_ch", tdc_ch, "tdc_ch[tdc_length]/s");
	tree_tdc -> Branch("tdc_value", tdc_value, "tdc_value[tdc_length]/i");
	tree_tdc -> Branch("tdc_buf_cnt", &tdc_buf_cnt, "tdc_buf_cnt/i");
	tree_tdc -> Branch("tdc_triggerID", &tdc_triggerID, "tdc_triggerID/i");
	tree_tdc -> Branch("tdc_header_cnt", &tdc_header_cnt, "tdc_header_cnt/i");
	tree_tdc -> Branch("tdc_data_cnt", &tdc_data_cnt, "tdc_data_cnt/i");
	tree_tdc -> Branch("tdc_trailer_cnt", &tdc_trailer_cnt, "tdc_trailer_cnt/i");
	tree_tdc -> Branch("tdc_evt_num", &tdc_evt_num, "tdc_evt_num/i");
	tree_tdc -> Branch("tdc_order_value", tdc_order_value, "tdc_order_value[tdc_order_length]/i");

        tree_adc -> Branch("adc_length", &adc_length, "adc_length/i");
        tree_adc -> Branch("adc_order_length", &adc_order_length, "adc_order_length/i");
        tree_adc -> Branch("adc_ch", adc_ch, "adc_ch[adc_length]/s");
        tree_adc -> Branch("adc_value", adc_value, "adc_value[adc_length]/i");
        tree_adc -> Branch("adc_buf_cnt", &adc_buf_cnt, "adc_buf_cnt/i");
	tree_adc -> Branch("adc_triggerID", &adc_triggerID, "adc_triggerID/i");
        tree_adc -> Branch("adc_header_cnt", &adc_header_cnt, "adc_header_cnt/i");
        tree_adc -> Branch("adc_data_cnt", &adc_data_cnt, "adc_data_cnt/i");
        tree_adc -> Branch("adc_trailer_cnt", &adc_trailer_cnt, "adc_trailer_cnt/i");
	tree_adc -> Branch("adc_evt_num", &adc_evt_num, "adc_evt_num/i");
	tree_adc -> Branch("adc_order_value", adc_order_value, "adc_order_value[adc_order_length]/i");


	// // // // // // // // // // // // // // // // // // // // // // //
	tdc_buf_cnt = 0;
	tdc_header_cnt = 0;
	tdc_data_cnt = 0;
	tdc_trailer_cnt = 0;

	for (int k = 3; k < tree_in->GetEntries(); k++)
//	for (int k = 3; k < 10; k++)
	{
		tree_in->GetEntry(k);
		tdc_buf_cnt++;
		tdc_length = 0;
		tdc_order_length = 0;
		for (int i = 0; i < ntdc; i++)
		{
			int addr = (tdc[i] >> 27) & 0x1F;
			cout << bitset<32>(tdc[i]) << endl;
			switch(addr)
			{
				case 8 :
				tdc_triggerID = (UInt_t)((tdc[i] >> 5) & 0xFFFF);
				tdc_header_cnt++;
				tdc_order_value[tdc_order_length] = 3;
				tdc_order_length++;
				break;

				case 0 :
				tdc_ch[tdc_length] = (UInt_t)((tdc[i] >> 21) & 0x1F); 
				tdc_value[tdc_length] = (UShort_t)(tdc[i] & 0x1FFFFF);
				tdc_length++;
				tdc_data_cnt++;
				tdc_order_value[tdc_order_length] = 2;
				tdc_order_length++;
				break;

				case 16 :
				tdc_trailer_cnt++;
				tdc_order_value[tdc_order_length] = 1;
				tdc_order_length++;
				break;
			
			}

		}
		tree_tdc->Fill();

	}
	tree_tdc->Write();

	adc_buf_cnt = 0;
        adc_header_cnt = 0;
        adc_data_cnt = 0;
        adc_trailer_cnt = 0;
	int a,b = 0;
	//for (int k = 3; k < 10; k++)
	for (int k = 3; k < tree_in->GetEntries(); k++)
	{
		tree_in->GetEntry(k);
		adc_buf_cnt++;
                adc_length = 0;
		adc_order_length = 0;
		for(int i = 0; i < nadc; i++)
		{
			int addr = (adc[i] >> 24) & 0x07;
			switch(addr)
			{
				case 2 :
				adc_header_cnt++;
				adc_order_value[adc_order_length] = 3;
				adc_order_length++;
			 	break;		
				
				case 0 :
                                adc_ch[adc_length] = (UInt_t)((adc[i] >> 17) & 0x1F);
                                adc_value[adc_length] = (UShort_t)(adc[i] & 0xFFF);
				adc_length++;
				adc_data_cnt++;
				adc_order_value[adc_order_length] = 2;
				adc_order_length++;
				break;

				case 4 :
                                adc_triggerID = (UInt_t)(adc[i] & 0xFFFFFF);
				adc_trailer_cnt++;
				adc_order_value[adc_order_length] = 1;
				adc_order_length++;
				break;
			}
				
		}
	        tree_adc->Fill();
	}
        tree_adc->Write();

	file_make->Close();
}
