#include "TFile.h"
#include "TTree.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

void VME_Conv(){
  TFile* itf = new TFile("beamtest_2.root");
  TTree* itr = (TTree*)itf->Get("BinaryOut");
  Int_t         nLoop;
  Int_t         tdcLength;
  Int_t         adcLength;
  ULong64_t     tdcData[2048];
  ULong64_t     adcData[2048];
  Long64_t      unix_time;

  itr->SetBranchAddress("ReadCycle"     ,&nLoop        );
  itr->SetBranchAddress("ntdc"          ,&tdcLength    );
  itr->SetBranchAddress("nadc"          ,&adcLength    );
  itr->SetBranchAddress("tdc"           ,tdcData       );
  itr->SetBranchAddress("adc"           ,adcData       );
  itr->SetBranchAddress("unix_time"     ,&unix_time    );
  
  TFile* otf = new TFile("beamtest_2_cov.root","recreate");
  TTree* ttr = new TTree("ttr","TDCTree");
  TTree* atr = new TTree("atr","ADCTree");
  ULong64_t tevt = 0;
  ULong64_t aevt = 0;
  Int_t aDL  = 0;// DataLength;
  Int_t tDL  = 0;// DataLength;
  UShort_t ADCC[2048]={0};
  UShort_t TDCC[2048]={0};
  UInt_t ADC[2048]={0};
  UInt_t TDC[2048]={0};
  
  UInt_t astatus = 0;
  UInt_t tstatus = 0;
  Int_t TT = 0;
  Int_t aTOff    = 0;/// adc Trigger offset
  Int_t tTOff    = 0;/// tdc Trigger offset
  
  ttr->Branch("tevt"   ,&tevt    ,"tevt/l");
  ttr->Branch("tDL"    ,&tDL     ,"tDL/I");
  ttr->Branch("TDCC"   ,TDCC     ,"TDCC[tDL]/s");	      
  ttr->Branch("TDC"    ,TDC      ,"TDC[tDL]/i");
  ttr->Branch("tstatus",&tstatus ,"tstatus/i");
  ttr->Branch("TT"     ,&TT      ,"TT/I");
  ttr->Branch("tTOff"  ,&tTOff   ,"tTOff/I");

  atr->Branch("aevt"   ,&aevt   ,"aevt/l");
  atr->Branch("aDL"    ,&aDL    ,"aDL/I");
  atr->Branch("ADCC"   ,&ADCC   ,"ADCC[aDL]/s");	      
  atr->Branch("ADC"    ,ADC     ,"ADC[aDL]/i");
  atr->Branch("astatus",&astatus,"astatus/i");
  atr->Branch("aTOff"  ,&aTOff  ,"aTOff/I");
  
  Int_t tCount;
  Int_t aCount;
  
  tDL = 0;
  aDL = 0;
  tevt = -1;
  Int_t stopflag = 0;

  //// Trigger number offset
  itr->GetEntry(0);
  for( int ip = 0; ip < tdcLength; ip++){
    Int_t wheader = (tdcData[ip] >> 27) & 0x1F ;
    if( wheader == 8 ){
      tTOff = (int)((tdcData[ip] >> 5) & 0x3FFFFF);
      break;
    }
  }
  for( int ip = 0; ip < adcLength; ip++){
    Int_t wheader = (adcData[ip] >> 24 ) & 0x07;
    if( wheader == 4 ){
      aTOff = (int)((adcData[ip] & 0xFFFFFF));
    }
  }
  Int_t tl = 0; 
  std::cout<< "TDC Trigger offset : " << tTOff << " ADC Triger offset : " << aTOff << std::endl; 

  for( int ievt = 0; ievt < itr->GetEntries(); ievt++ ){
    itr->GetEntry(ievt);
    tDL = 0;
    std::cout<< "ReadFile" << std::endl;
    std::cout<< "TDC : " ;
    for( int ip = 0; ip < tdcLength; ip++){
      Int_t wheader = (tdcData[ip] >> 27) & 0x1F ;      
      switch ( wheader ){
      case 0:
	tl = (tdcData[ip] >> 26) & 0x01;
	TDC[tDL] = (UInt_t)(tdcData[ip] & 0x1FFFFF);
	TDCC[tDL]= (UShort_t)(tdcData[ip] >> 21) & 0x1F;
	std::cout<< "Data " << tl << " " << TDC[tDL] << " " << TDCC[tDL] ;	
	// Set Data
	tDL++;
	break;
      case 8:
	if( tDL != 0 ){
	  tstatus = 1;
	  ttr->Fill();
	  tstatus = 0; 
	  tDL = 0;
	  TT  = -1;
	}
	tCount = (int)((tdcData[ip] >> 5) & 0x3FFFFF);
	std::cout<< "THeader " << tCount << " ";
	tevt++;
	tstatus = 0;
	while( tevt < tCount ){
	  //stopflag = 1;
	  tstatus = 2;
	  ttr->Fill();
	  tevt++;
	  //TT = 0;
	}
	break;
      case 17:
	TT = (Int_t)(tdcData[ip] & 0x7FFFFFF);
	std::cout<< "TT " << TT << " " ;
	break;
      case 16:
	//std::cout<< "Trailer" << std::endl;
	break;
      default :
	std::cout<< "Unknown " << wheader << " "; 
      }      
    }
    ttr->Fill();
    
    std::cout<< "\n";    
    std::cout << "ADC : ";    
    //aDL = 0;
    for( int ip = 0; ip < adcLength; ip++){
      Int_t wheader = (adcData[ip] >> 24 ) & 0x07;
      switch ( wheader ){
      case 0:
	std::cout<< "Data ";
	// SetData 
	ADC[aDL] = (UInt_t)(adcData[ip] & 0x0FFF);
	ADCC[aDL]= (UShort_t)((adcData[ip] >> 17) & 0x0F);
	aDL++;
	break;
      case 2:
	std::cout<< "AHeader " ;
	astatus = 1;
	break;
      case 4:
	aCount = (int)((adcData[ip] & 0xFFFFFF));
	std::cout<< "Trailer " << aCount << " " ;	
	if( aevt < aCount){
	  while( aevt < aCount ){
	    astatus = 2;
	    atr->Fill();
	    aevt++;
	  }
	  astatus = 3;
	  atr->Fill();
	  aevt++;
	  astatus = 0; 
	}else{
	  atr->Fill();
	  aevt++;
	  astatus = 0;
	}
	/// Initialization of Data
	//if( aDL != 16 ){ std::cout<< aDL << std::endl;getchar(); }
	aDL = 0;
	break;
      default :
	std::cout<< "Unknown  " << wheader << " ";
      }      
    }
		//atr->Fill();
    std::cout<< "\n";
    if( stopflag ){
      getchar();
      stopflag = 0;
    }    
  }

		std::cout<< "TDC Trigger offset : " << tTOff << " ADC Triger offset : " << aTOff << std::endl;
  ttr->Write();
  atr->Write();
  otf->Close();
}


