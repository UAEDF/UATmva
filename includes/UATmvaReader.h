#ifndef __UATmvaReader_H__
#define __UATmvaReader_H__

#include "../includes/UATmvaConfig.h"
#include "../includes/UATmvaTree.h"


#include "TMVA/Reader.h"
using namespace TMVA;

#include <TString.h>
#include <string>
#include <vector>
using namespace std;


class UATmvaReader_t {
  public:
  UATmvaReader_t(){ TmvaFile = NULL ; TmvaReader = NULL; }
  ~UATmvaReader_t(){ TmvaFile->Close(); delete TmvaFile ; delete TmvaReader ; }
   
  TString             TmvaName ;
  TFile*              TmvaFile ;
  Reader*             TmvaReader ; 
};


class UATmvaReader {

  private:

  
  Bool_t                   InitDone  ;
  string                   NAME ;
  UATmvaReader_t*          UAReader ;

  Int_t nbins ;
  Float_t minBin, maxBin;


//  void Train(); 
//  void Plot();
 
  public:

  UATmvaReader(){ InitDone = false ; nbins = 1 ; minBin=-1.1 ; maxBin=1.1 ;}
  virtual ~UATmvaReader(){;}

  Bool_t IsInit() { return InitDone ; }

  void Do    ( UATmvaConfig&, UATmvaTree&);
  void DoMLP ( UATmvaConfig&, UATmvaTree&);



};


#endif

