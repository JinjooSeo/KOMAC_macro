void DrawTime(){
  std::string filename ="beamtest_2_cov.root";

  TFile* itf= new TFile(filename.c_str());
  TTree* itr = (TTree*)itf->Get("ttr");
  ULong64_t tevt;
  Int_t tDL;
  UShort_t TDCC[2048];
  UInt_t   TDC[2048];
  UInt_t   tstatus;
  Int_t    TT;
  Int_t    tTOff;
  
  itr->SetBranchAddress("tevt"      ,&tevt  ); 
  itr->SetBranchAddress("tDL"       ,&tDL    );    
  itr->SetBranchAddress("TDCC"      ,TDCC    );
  itr->SetBranchAddress("TDC"       ,TDC     );
  itr->SetBranchAddress("tstatus"   ,&tstatus);
  itr->SetBranchAddress("TT"        ,&TT     );    
  itr->SetBranchAddress("tTOff"     ,&tTOff  );    

  TTree* atr = (TTree*)itf->Get("atr");
  Int_t aDL;
  UShort_t ADCC[2048];
  UInt_t   ADC[2048];
  atr->SetBranchAddress("aDL"       ,&aDL    );    
  atr->SetBranchAddress("ADCC"      ,ADCC    );
  atr->SetBranchAddress("ADC"       ,ADC     );

  TH1D* hisTime[16];
  for( int i = 0; i< 16; i++){
    hisTime[i] = new TH1D(Form("hisTime_%d",i),Form("hisTime_%d",i), 1600,0,16000);
  }
  TH1D* hisDeltaT[8];  
  for( int i = 0; i< 8; i++){
    hisDeltaT[i] = new TH1D(Form("hisDeltaT_%d",i),"his",6000,-1.5,1.5);
  }

  TH2D* higSTT     = new TH2D("hisTT","hisTT",2000,2000,8000,2000,2000,8000);
  TH2D* hisMT     = new TH2D("hisMT","hisMT",16,0,16,20,0,20);
  for( int ievt = 0; ievt  < itr->GetEntries(); ievt++){
    itr->GetEntry(ievt);
    Int_t tmpHit[16]={0};
    Double_t tmpTime[16]={0};
    Double_t tmpST[8]={0};
    Int_t    tmpNST[8]={0};
    for( int ip = 0; ip < tDL; ip++){
      if( TDCC[ip] < 0 || TDCC[ip] > 15 ){ continue; }
      tmpHit[TDCC[ip]]++;
      //if( tmpHit[TDCC[ip]] == 1 ){
      tmpTime[TDCC[ip]] = TDC[ip];
      hisTime[TDCC[ip]]->Fill( TDC[ip] );
      //}
    }
    // if( tmpHit[0] > 0 && tmpHit[1] > 0 ){
    /*
      if( tmpHit[0] >0 && tmpHit[1] >0 && tmpHit[2]>0 && tmpHit[3] > 0 && tmpHit[4] > 0 && tmpHit[5] > 0){      
      Double_t t[3]={0};
      for( int i = 0; i< 3; i++ ){
      t[i] =( tmpTime[i*2] - tmpTime[i*2+1] )/2./4.;
      }
      for( int i = 0; i< 3; i++ ){
      hisDeltaT[i]->Fill( t[i] );
      }
      
      hisTT->Fill( tmpTime[0], tmpTime[1]); 
      }
      }*/
    for( int i = 0; i< 16; i++){
      //if( tmpHit[i] > 0 ){
      //	hisTime[i]->Fill(i,tmpTime[i]);
      //}
      hisMT->Fill( i, tmpHit[i] );
    }
    for( int i = 0; i< 8; i++){
      if( tmpHit[i*2]   != 0 &&
	  tmpHit[i*2+1] != 0 ){
	tmpNST[i] = 1;
	tmpST[i] = (tmpTime[i*2] + tmpTime[i*2+1])/2*0.025;
	}
    }
    for( int i = 0; i< 8; i++){
      if( tmpNST[i] == 1 && tmpNST[0] == 1){
	      for(int m = 0; m<8; m++){
		      if(ADC[m]>200)
		      {
	hisDeltaT[i]->Fill( tmpST[i] - tmpST[0] );
		      }
	      }
      }    
    }
  }    
 
  TCanvas* can = new TCanvas("can","can",900,400);
  can->Divide(2,1);
  for( int i = 0; i< 2; i++){
    can->cd(i+1);
    gPad->SetGridx();
    gPad->SetGridy();
    //hisMT->Draw("colz");
    //gPad->SetLogy();
    hisDeltaT[i]->Draw();
    //hisDeltaT[i]->Fit("gaus","","",-1,1);
    //hisTT->Draw("colz");
  }
  //hisTT->Draw("colz");
  
  TH1D* hisADC[16];
  for( int i = 0; i< 16; i++){
    hisADC[i] = new TH1D(Form("hisADC_%d",i),Form("hisADC_%d",i),
			 1000,0,1000);    
  }

  for( int ievt = 0; ievt < atr->GetEntries(); ievt++){
    atr->GetEntry(ievt);
    for (int iDL = 0; iDL < aDL; iDL++){
      hisADC[ADCC[iDL]]->Fill(ADC[iDL]);
    }    
  }
  
  
  TCanvas* can1 = new TCanvas("can1","can1",1800,400);
  can1->Divide(4,1);  
  for( int i = 0; i< 4; i++){
    can1->cd(i+1);
    //gPad->SetLogy();
    hisADC[i]->Draw();
  }
 TCanvas* can2 = new TCanvas("can2","can2",1800,400);
  can2->Divide(4,1);
  for( int i = 0; i< 4; i++){
    can2->cd(i+1);
    //gPad->SetLogy();
    hisTime[i]->Draw();
  }
  /*
  TCanvas* can3 = new TCanvas("can3","can3",800,800);
  hisMT->Draw("col");
*/
  }
