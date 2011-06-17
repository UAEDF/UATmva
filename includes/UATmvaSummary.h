#ifndef __UATmvaSummary_H__
#define __UATmvaSummary_H__

#include "../includes/UATmvaConfig.h"

#include <TFile.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>

#include <TString.h>
#include <string>
#include <vector>
using namespace std;


class UATmvaSummary_t {
  public:
  UATmvaSummary_t()  ;
  UATmvaSummary_t(TString,TString,TString)  ;
  ~UATmvaSummary_t() ; 

  TString             BaseName;
  TString             ExtName;  
  TString             TmvaName;

  TH2F*               CorrMtxS;
  TH2F*               CorrMtxB;

  TH1F*               D2Train;
  TH1F*               D2Test ;

  TH1F*               STrain ;
  TH1F*               BTrain ;
  TH1F*               STest  ;
  TH1F*               BTest  ;


  TH1D*               Cut; 
  TH1D*               Sign; 
  TH1D*               Limit; 

};


class UATmvaSummary {

  private:

  
  Bool_t                    InitDone  ;
  vector<UATmvaSummary_t*>  vUASummary ;

  public:

  UATmvaSummary(){ InitDone = false ;}
  virtual ~UATmvaSummary();

  Bool_t IsInit() { return InitDone ; }

  void Init ( UATmvaConfig& );
  void Print( );
  void Plots( );

  void PlotEpoch( int );
  void PlotOvertrain( int );





};


#endif

