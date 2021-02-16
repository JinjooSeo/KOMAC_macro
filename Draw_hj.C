void Draw_hj()
{
        TFile * filename = new TFile("beamtest_conv_hj_2.root","read");
        TTree * tree_tdc = (TTree*) filename->Get("tree_TDC");
        TTree * tree_adc = (TTree*) filename->Get("tree_ADC");

        // initialize Branches name //

	UInt_t tdc_length = 999;       
	UShort_t tdc_ch[2048] = {999,};                                                              
        UInt_t tdc_value[2048] = {999,};  
        UInt_t tdc_buf_cnt = 999;    
	UInt_t tdc_triggerID = 999;                                                                    
        UInt_t tdc_header_cnt = 999;             
	UInt_t tdc_data_cnt = 999;                                                                     
        UInt_t tdc_trailer_cnt = 999;                                                                                                                                                                       
        UInt_t tdc_order_value[2048] = {999,};                                                                                                                                                                       
        UInt_t tdc_order_length = 999;                                                                                                                                                                       

	UInt_t adc_length = 999;                         
	UShort_t adc_ch[2048] = {999,};                                                               
        UInt_t adc_value[2048] = {999,};           
	UInt_t adc_buf_cnt = 999;                                                                     
        UInt_t adc_triggerID = 999;            
	UInt_t adc_header_cnt = 999;                                                                   
        UInt_t adc_data_cnt = 999;       
	UInt_t adc_trailer_cnt = 999; 
        UInt_t adc_order_value[2048] = {999,};                                                                                                                                                                       
        UInt_t adc_order_length = 999;                                                                                                                                                                       

	// set Branches address //

	tree_tdc -> SetBranchAddress("tdc_length",&tdc_length);
        tree_tdc -> SetBranchAddress("tdc_ch",tdc_ch);
        tree_tdc -> SetBranchAddress("tdc_value",tdc_value);
        tree_tdc -> SetBranchAddress("tdc_buf_cnt",&tdc_buf_cnt);
        tree_tdc -> SetBranchAddress("tdc_triggerID",&tdc_triggerID);
        tree_tdc -> SetBranchAddress("tdc_header_cnt",&tdc_header_cnt);
        tree_tdc -> SetBranchAddress("tdc_data_cnt",&tdc_data_cnt);
        tree_tdc -> SetBranchAddress("tdc_trailer_cnt",&tdc_trailer_cnt);
        tree_tdc -> SetBranchAddress("tdc_order_value",tdc_order_value);
        tree_tdc -> SetBranchAddress("tdc_order_length",&tdc_order_length);

        tree_adc -> SetBranchAddress("adc_length",&adc_length);
        tree_adc -> SetBranchAddress("adc_ch",adc_ch);
        tree_adc -> SetBranchAddress("adc_value",adc_value);
        tree_adc -> SetBranchAddress("adc_buf_cnt",&adc_buf_cnt);
        tree_adc -> SetBranchAddress("adc_triggerID",&adc_triggerID);
        tree_adc -> SetBranchAddress("adc_header_cnt",&adc_header_cnt);
        tree_adc -> SetBranchAddress("adc_data_cnt",&adc_data_cnt);
        tree_adc -> SetBranchAddress("adc_trailer_cnt",&adc_trailer_cnt);
        tree_adc -> SetBranchAddress("adc_order_value",adc_order_value);
        tree_adc -> SetBranchAddress("adc_order_length",&adc_order_length);


	// Make histogram //
	
	TH1D * h_tdc0 = new TH1D("ht0","h_tdc0",500,-10,10000);
	TH1D * h_tdc1 = new TH1D("ht1","h_tdc1",500,-10,10000);
	TH1D * h_tdc2 = new TH1D("ht2","h_tdc2",500,-10,10000);
	TH1D * h_tdc3 = new TH1D("ht3","h_tdc3",500,-10,10000);

	TH1D * h_adc0 = new TH1D("ha0","h_adc0",200,-10,1000);
	TH1D * h_adc1 = new TH1D("ha1","h_adc1",200,-10,1000);
	TH1D * h_adc2 = new TH1D("ha2","h_adc2",200,-10,1000);
	TH1D * h_adc3 = new TH1D("ha3","h_adc3",200,-10,1000);

	TH1D * h_tdc_h_t_cnt = new TH1D("h_t_h-t","tdc header-trailer count",10,-5,5);			//tdc의 header count - trailer count
	TH1D * h_adc_h_t_cnt = new TH1D("h_a_h-t","adc header-trailer count",10,-5,5);			//adc의 header count - trailer count
	TH1D * h_trig_dif = new TH1D("h_trig_dif","tdc&adc trigger difference",100,-600,0);			//tdc, adc의  trigger difference

	TH2D * h_tdc_header_buf_diff = new TH2D("h_t_h_b_d","tdc header count / buf",10,0,10,10,0,10);	//tdc 한 버퍼에 들어간 header count
	TH2D * h_tdc_trailer_buf_diff = new TH2D("h_t_t_b_d","tdc trailer count / buf",10,0,10,10,0,10);	//tdc 한 버퍼에 들어간 trailer count

        TH2D * h_adc_header_buf_diff = new TH2D("h_a_h_b_d","adc header count / buf",10,0,10,10,0,10);	//adc 한 버퍼에 들어간 header count
        TH2D * h_adc_trailer_buf_diff = new TH2D("h_a_t_b_d","adc trailer count / buf",10,0,10,10,0,10);	//adc 한 버퍼에 들어간 trailer count

        TH2D * h_header_dif_buf = new TH2D("h_h_d_b","header count difference in smae buf",20,-10,10,10,0,10);		//tdc,adc 의 한 버퍼에 들어간 header count의 차이
        TH2D * h_trailer_dif_buf = new TH2D("h_t_d_b","trailer count difference in same buf",20,-10,10,10,0,10);		//tdc,adc 의 한 버퍼에 들어간 trailer count의 차이

	TH1D * h_tdc_err_buf = new TH1D("h_t_e_b","tdc error buf",5,-1,4);
	TH1D * h_adc_err_buf = new TH1D("h_a_e_b","adc error buf",5,-1,4);

	TH1D * h_tdc_data = new TH1D("h_tdc_data","tdc data / buf",110,-10,100);
	TH1D * h_tdc_data_clone = (TH1D*) h_tdc_data -> Clone("h_tdc_data_clone");
	TH1D * h_tdc_data_tmp = new TH1D("h_tdc_data_tmp","tdc data_tmp / buf",110,-10,100);
	TH1D * h_adc_data = new TH1D("h_adc_data","adc data / buf",260,-10,250);
	TH1D * h_adc_data_clone = (TH1D*) h_adc_data -> Clone("h_adc_data_clone");
	TH1D * h_adc_data_tmp = new TH1D("h_adc_data_tmp","adc data_tmp / buf",260,-10,250);

	TH1D * h_tdc0_cut = new TH1D("h_tdc0_cut","tdc0 value after cut",10000,0,10000);
	TH1D * h_tdc1_cut = new TH1D("h_tdc1_cut","tdc1 value after cut",10000,0,10000);
	TH1D * h_tdc2_cut = new TH1D("h_tdc2_cut","tdc2 value after cut",10000,0,10000);
	TH1D * h_tdc3_cut = new TH1D("h_tdc3_cut","tdc3 value after cut",10000,0,10000);

	TH1D * h_adc0_cut = new TH1D("h_adc0_cut","adc0 value after cut",1000,0,1000);
	TH1D * h_adc1_cut = new TH1D("h_adc1_cut","adc1 value after cut",1000,0,1000);

	TH2D * h_tdc_adc_cor = new TH2D("h_tdc_adc_cor","tdc&adc corelation",80,-2,2,50,-1,1);

	TH1D * h_tdc_cut_differ = new TH1D("h_tdc_cut_differ","tdc0-tdc1",159,-2,2);
	// set drawing option //

	bool Draw_tdc = false;					//tdc값 그림
	bool Draw_adc = false;					//adc값 그림
	bool Draw_header_trailer_cnt = false;			//header 와 trailer값 그림
	bool Draw_triggerID_dif = false;			//triggerID 차이 그림
	bool Draw_buf_cnt_dif = false;				//한 buf에 들어간 header와 trailer수 차이 그림
	bool Draw_header_trailer_dif_buf = false;		//tdc,adc의 한 버퍼에 들어간 header / trailer수 비교 그림
	bool Draw_HDT_error = true;
	bool Draw_data_cnt = false;
	bool Draw_conv = false;
	bool Draw_cut = false;

	// set extra-value //
	
	int tdc_buf_cnt_init = 0;
	int tdc_buf_cnt_next = 0;
	int tdc_header_cnt_init = 0;
	int tdc_header_cnt_next = 0;
	int tdc_trailer_cnt_init = 0;
	int tdc_trailer_cnt_next = 0;

        int adc_buf_cnt_init = 0;
        int adc_buf_cnt_next = 0;
        int adc_header_cnt_init = 0;
        int adc_header_cnt_next = 0;
        int adc_trailer_cnt_init = 0;
        int adc_trailer_cnt_next = 0;

	int header_cnt_dif = 0;
	int trailer_cnt_dif = 0;

	int tdc_err_buf_num = 0;
	int adc_err_buf_num = 0;

	double tdc0_cut_value = -999;
	double tdc1_cut_value = -999;
	double tdc2_cut_value = -999;
	double tdc3_cut_value = -999;

	double adc0_cut_value = -999;
	double adc1_cut_value = -999;


// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
	for( int k = 0; k < tree_tdc->GetEntries(); k++)
	{
		tree_tdc->GetEntry(k);
		tree_adc->GetEntry(k);
		for( int i = 0; i < tdc_length; i++)
		{		
			if(tdc_ch[i] == 0)
			{
				h_tdc0 -> Fill(tdc_value[i]);
			}

                        if(tdc_ch[i] == 1)
                        {
                                h_tdc1 -> Fill(tdc_value[i]);
                        }
                       
		       	if(tdc_ch[i] == 2)
                        {
                                h_tdc2 -> Fill(tdc_value[i]);
                        }
                       
		       	if(tdc_ch[i] == 3)
                        {
                                h_tdc3 -> Fill(tdc_value[i]);
                        }

		}

		for(int i = 0; i < adc_length; i++)
		{
			if(adc_ch[i] == 0)
			{
				h_adc0->Fill(adc_value[i]);
			}
                        if(adc_ch[i] == 1)
                        {
                                h_adc1->Fill(adc_value[i]);
                        }

                        if(adc_ch[i] == 2)
                        {
                                h_adc2->Fill(adc_value[i]);
                        }
					
                        if(adc_ch[i] == 3)
                        {
                                h_adc3->Fill(adc_value[i]);
                        }
		}

		int triggerID_differ = tdc_triggerID - adc_triggerID;
		h_trig_dif -> Fill(triggerID_differ);
		
		tdc_header_cnt_init = tdc_header_cnt;
		tdc_trailer_cnt_init = tdc_trailer_cnt;
		tdc_buf_cnt_init = tdc_buf_cnt; 

                adc_header_cnt_init = adc_header_cnt;
                adc_trailer_cnt_init = adc_trailer_cnt;
                adc_buf_cnt_init = adc_buf_cnt;

		tree_tdc->GetEntry(k+1);
		tree_adc->GetEntry(k+1);

		tdc_header_cnt_next = tdc_header_cnt;
		tdc_buf_cnt_next = tdc_buf_cnt;
                tdc_trailer_cnt_next = tdc_trailer_cnt;

		adc_header_cnt_next = adc_header_cnt;
                adc_buf_cnt_next = adc_buf_cnt;
                adc_trailer_cnt_next = adc_trailer_cnt;

		int tdc_header_cnt_differ = tdc_header_cnt_next - tdc_header_cnt_init;
		int tdc_trailer_cnt_differ = tdc_trailer_cnt_next - tdc_trailer_cnt_init;
		int tdc_buf_cnt_differ = tdc_buf_cnt_next - tdc_buf_cnt_init;
		int t_h_t_cnt = tdc_header_cnt_differ - tdc_trailer_cnt_differ; 

		int adc_header_cnt_differ = adc_header_cnt_next - adc_header_cnt_init;
                int adc_trailer_cnt_differ = adc_trailer_cnt_next - adc_trailer_cnt_init;
                int adc_buf_cnt_differ = adc_buf_cnt_next - adc_buf_cnt_init;
		int a_h_t_cnt = adc_header_cnt_differ - adc_trailer_cnt_differ; 

		if(k != tree_tdc->GetEntries())
		{
			h_tdc_header_buf_diff->Fill(tdc_header_cnt_differ,tdc_buf_cnt_differ);
			h_tdc_trailer_buf_diff->Fill(tdc_trailer_cnt_differ,tdc_buf_cnt_differ);

			h_adc_header_buf_diff->Fill(adc_header_cnt_differ,adc_buf_cnt_differ);
               		h_adc_trailer_buf_diff->Fill(adc_trailer_cnt_differ,adc_buf_cnt_differ);
		}

		header_cnt_dif = tdc_header_cnt_differ - adc_header_cnt_differ;
		trailer_cnt_dif = tdc_trailer_cnt_differ - adc_trailer_cnt_differ;

		h_header_dif_buf -> Fill(header_cnt_dif,tdc_buf_cnt_differ);
		h_trailer_dif_buf->Fill(trailer_cnt_dif,tdc_buf_cnt_differ);
		h_tdc_h_t_cnt -> Fill(t_h_t_cnt);
		h_adc_h_t_cnt -> Fill(a_h_t_cnt);

                tree_tdc->GetEntry(k);
                tree_adc->GetEntry(k);

		if(tdc_length == 4 && adc_length ==16)
		{
                for(int i = 0; i < tdc_order_length; i++)
	                {
        	                if (tdc_order_value[0] == 3)
				{
					for( int j = 0; j < adc_order_length; j++)
					{
						if(adc_order_value[0] ==3)
						{
							for(int tevt = 0; tevt < tdc_length; tevt++)
							{
								if(tdc_ch[tevt] == 0)
								{
									h_tdc0_cut->Fill(tdc_value[tevt]);
									tdc0_cut_value = tdc_value[tevt];
								}

								if(tdc_ch[tevt] == 1)
                                                                {
									h_tdc1_cut->Fill(tdc_value[tevt]);
									tdc1_cut_value = tdc_value[tevt];
                                                                }
								if(tdc_ch[tevt] == 2)
                                                                {
									h_tdc2_cut->Fill(tdc_value[tevt]);
                                                                        tdc2_cut_value = tdc_value[tevt];
                                                                }

                                                                if(tdc_ch[tevt] == 3)
                                                                {
									h_tdc3_cut->Fill(tdc_value[tevt]);
                                                                        tdc3_cut_value = tdc_value[tevt];
                                                                	break;
								}



							}

							for(int aevt = 0; aevt < adc_length; aevt++)
							{
								if(adc_ch[aevt] == 0)
								{
									adc0_cut_value = log(adc_value[aevt]);
									h_adc0_cut->Fill(adc_value[aevt]);
								}

								if(adc_ch[aevt] == 1)
                                                                {
                                                                        adc1_cut_value = log(adc_value[aevt]);
                                                                        h_adc1_cut->Fill(adc_value[aevt]);
                                                                        break;
                                                                }

							}
							double tdc_cut_differ01 = (tdc0_cut_value - tdc1_cut_value)/40;
							int tdc_cut_differ23 = (tdc2_cut_value + tdc3_cut_value)/2;
							double adc_cut_differ01 = adc0_cut_value - adc1_cut_value;
							double tdc_real_value = (tdc_cut_differ01);
							h_tdc_cut_differ -> Fill(tdc_real_value);
							h_tdc_adc_cor -> Fill(tdc_real_value,adc_cut_differ01);
							break;
						}
					}
				break;
				}
			}


		}

	
	
	}

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //


	if(Draw_HDT_error)
	{
	for(int k = 0; k < tree_tdc->GetEntries(); k++)
	{
		tree_tdc->GetEntry(k);
		tdc_err_buf_num = 0;
		for(int i = 0; i < tdc_order_length; i++)
		{
			if (tdc_order_value[0] != 3)
			{
				tdc_err_buf_num++;
				//cout << "Header error :" << tdc_order_value[i] << "  buf cnt : " << k << endl;
				break;
			}
			if(tdc_order_value[tdc_order_length-1] !=1)
			{
				tdc_err_buf_num++;
				//cout << "Trailer error :" << tdc_order_value[tdc_order_length-1] << "  buf cnt : " << k << endl;
				break;
			}
			if(tdc_order_value[i] == 3)
			{
				if(tdc_order_value[i+1] !=2)
				{
					tdc_err_buf_num++;
					//cout << "HD error :" << tdc_order_value[i] << "  buf cnt : " << k << endl;
					break;
				}
			}
			if(tdc_order_value[i] == 2)
			{
				if(tdc_order_value[i+1] ==3 )
				{
					tdc_err_buf_num++;
					//cout << "DT error :" << tdc_order_value[i] << "  buf cnt : " << k << endl;
					break;
				}
			}
			if(tdc_order_value[i] == 1)
			{
				if(tdc_order_value[i+1] !=3)
				{
					if(i+1 == tdc_order_length)
					{
						break;
					}
					tdc_err_buf_num++;
					//cout << "TH error :" << tdc_order_value[i] << "   k value : " << k << endl;
					break;
				}
			}
		}
	h_tdc_err_buf -> Fill(tdc_err_buf_num);
	}




        for(int k = 0; k < tree_adc->GetEntries(); k++)
        {
                tree_adc->GetEntry(k);
		adc_err_buf_num = 0;
                for(int i = 0; i < adc_order_length; i++)
                {
                        if (adc_order_value[0] != 3)
                        {
				adc_err_buf_num++;
                                //cout << "Header error :" << adc_order_value[i] << "   k value : " << k << endl;
                                break;
                        }
                        if(adc_order_value[adc_order_length-1] !=1)
                        {
				adc_err_buf_num++;
     				//cout << "Trailer error :" << adc_order_value[tdc_order_length-1] << "   k value : " << k << endl;
                                break;
                        }
                        if(adc_order_value[i] == 3)
                        {
                                if(adc_order_value[i+1] !=2)
                                {
					adc_err_buf_num++;
   					//cout << "HD error :" << adc_order_value[i] << "   k value : " << k << endl;
                                        break;
                                }
                        }
                        if(adc_order_value[i] == 2)
                        {
                                if(adc_order_value[i+1] ==3 )
                                {
					adc_err_buf_num++;
 					//cout << "DT error :" << adc_order_value[i] << "   k value : " << k << endl;
                                        break;
                                }
                        }
                        if(adc_order_value[i] == 1)
                        {
                                if(adc_order_value[i+1] !=3)
                                {
                                        if(i+1 == adc_order_length)
                                        {
                                                break;
                                        }
					adc_err_buf_num++;
  					//cout << "TH error :" << adc_order_value[i] << "   k value : " << k << endl;
                                        break;
                                }
                        }

	        }
		h_adc_err_buf -> Fill(adc_err_buf_num);
        	}
	}
	



// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
	int tmp_k = 0;
	for(int k = 0; k < tree_tdc->GetEntries(); k++)
	{
		tree_tdc->GetEntry(k);
		tree_adc->GetEntry(k);
	
		int loop_cnt = 0;

		for(int i = 0; i<tdc_length; i++)
		{
			if(tdc_ch[i] >3)
			{
				loop_cnt++;
				if ( loop_cnt == 1)
				{
					tmp_k = k;
					tree_tdc->GetEntry(tmp_k);
					tree_adc->GetEntry(tmp_k);
					int tdc_data_cnt_differ_tmp = tdc_length;
					int adc_data_cnt_differ_tmp = adc_length;
                			h_tdc_data_tmp->Fill(tdc_data_cnt_differ_tmp);
                			h_adc_data_tmp->Fill(adc_data_cnt_differ_tmp);
				}
			}
		}
                int tdc_data_cnt_differ = tdc_length;
                int adc_data_cnt_differ = adc_length;
		
		
                h_tdc_data->Fill(tdc_data_cnt_differ);
                h_adc_data->Fill(adc_data_cnt_differ);
                
		h_tdc_data_clone->Fill(tdc_data_cnt_differ);
                h_adc_data_clone->Fill(adc_data_cnt_differ);
	}









// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
	if(Draw_tdc)
	{
		TCanvas * c1 = new TCanvas("c1","c1",800,600);
		c1->Divide(2,2);
		c1->cd(1);
		h_tdc0->Draw();
		c1->cd(2);
		h_tdc1->Draw();
                c1->cd(3);
	        h_tdc2->Draw();
   	  	c1->cd(4);
       		h_tdc3->Draw();
	}
        if(Draw_adc)
        {
                TCanvas * c2 = new TCanvas("c2","c2",800,600);
                c2->Divide(2,2);
                c2->cd(1);
                h_adc0->Draw();
                c2->cd(2);
                h_adc1->Draw();
                c2->cd(3);
                h_adc2->Draw();
                c2->cd(4);
                h_adc3->Draw();
        }

	if(Draw_header_trailer_cnt)
	{
		TCanvas * c3 = new TCanvas("c3","c3",1000,500);
		c3->Divide(2,1);
		c3->cd(1);
		gPad->SetLogy();
		h_tdc_h_t_cnt -> Draw();
		c3->cd(2);
		gPad->SetLogy();
		h_adc_h_t_cnt -> Draw();	
	}

	if(Draw_triggerID_dif)
	{
		TCanvas * c4 = new TCanvas("c4","c4",1000,500);
		h_trig_dif->Draw();
	}

	if(Draw_buf_cnt_dif)
	{
		TCanvas * c5 = new TCanvas("c5","c5",1200,800);
		c5->Divide(2,2);
		c5->cd(1);
		h_tdc_header_buf_diff->ProjectionX()->Draw();
		c5->cd(2);
		h_tdc_trailer_buf_diff->ProjectionX()->Draw();
                c5->cd(3);
                h_adc_header_buf_diff->ProjectionX()->Draw();
                c5->cd(4);
                h_adc_trailer_buf_diff->ProjectionX()->Draw();
	}

	if(Draw_header_trailer_dif_buf)
	{
		TCanvas *c6 = new TCanvas("c6","c6",1000,500);
		c6->Divide(2,1);
		c6->cd(1);
		h_header_dif_buf->ProjectionX()->Draw();
                c6->cd(2);
		h_trailer_dif_buf->ProjectionX()->Draw();
	}

	if(Draw_HDT_error)
	{
		TCanvas *c7 = new TCanvas("c7","c7",1000,500);
		c7->Divide(2,1);
		c7->cd(1);
		gPad->SetLogy();
		h_tdc_err_buf->Draw();
		c7->cd(2);
		gPad->SetLogy();
		h_adc_err_buf->Draw();
	}

	if(Draw_data_cnt)
	{
		h_tdc_data_clone->Add(h_tdc_data_tmp,-1);
        	h_adc_data_clone->Add(h_adc_data_tmp,-1);

		TCanvas *c8 = new TCanvas("c8","c8",800,600);
		c8 ->Divide(2,2);
		c8->cd(1);
		gPad->SetLogy();
		h_tdc_data_clone->Draw();
		c8->cd(2);
		gPad->SetLogy();
		h_adc_data_clone->Draw();
		c8->cd(3);
                gPad->SetLogy();
                h_tdc_data->Draw();
                c8->cd(4);
                gPad->SetLogy();
                h_adc_data->Draw();

	}

	if(Draw_conv)
	{
		TCanvas *c9 = new TCanvas("c9","c9",1800,600);
		c9->Divide(3,1);
		c9->cd(1);
		h_tdc_adc_cor->SetYTitle("log(ADC0/ADC1)");
		h_tdc_adc_cor->SetXTitle("TDC0-TDC1(ns)");
	        h_tdc_adc_cor->Draw("colz");
		c9->cd(2);
		h_tdc_adc_cor->ProfileX()->Draw();
		c9->cd(3);
		h_tdc_cut_differ->SetXTitle("TDC0-TDC1(ns)");
		h_tdc_cut_differ->Draw();
	}

		
	if(Draw_cut)
	{
		TCanvas *c10 = new TCanvas("c10","c10",800,600);
		c10->Divide(2,2);
		c10->cd(1);	
		h_tdc0_cut->Draw();
	        c10->cd(2);
                h_tdc1_cut->Draw();
                c10->cd(3);
                h_tdc2_cut->Draw();
                c10->cd(4);
                h_tdc3_cut->Draw();
	}

}

