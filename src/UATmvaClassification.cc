

#include "../includes/UATmvaClassification.h"

#include <iostream>



// ---------------------------- Init()

void UATmvaClassification::Do( UATmvaConfig& Cfg, UATmvaTree& T) {

   if ( Cfg.GetTmvaType() == "ANN" )  DoMLP(Cfg,T);
   if ( Cfg.GetTmvaType() == "BDT" )  DoBDT(Cfg,T);

}

void UATmvaClassification::DoMLP( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetTmvaVarNumRemove() ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ; 
   for (Int_t nHLayers = Cfg.GetANNHiddenLayersMin() ; nHLayers <= Cfg.GetANNHiddenLayersMax() ; ++nHLayers ) {
   for (Int_t nHNodes  = Cfg.GetANNHiddenNodesMin()  ; nHNodes  <= Cfg.GetANNHiddenNodesMax()  ; ++nHNodes  ) {
    
 
     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_MLP_" << nHLayers << "Layers_" << nHNodes << "Nodes_" << nVarMax << "Var" ;
     NAME =  Name.str();

     // Create ethod Options
     ostringstream Method;
     Method << "H:!V:NeuronType=tanh:VarTransform=N:NCycles=" << Cfg.GetANNCycles() << ":HiddenLayers=" ;
     for ( Int_t iHLayer = 1 ; iHLayer <= nHLayers ; ++iHLayer ) {
       Method << "N+" << nHNodes ; 
       if ( iHLayer != nHLayers ) Method << "," ;
     }

     // Create and Train MVA
     Train(Cfg,T,Name.str(),Method.str(),nVarMax);

     // Making some basic plots
     Plot(1);
   
   } // Nodes
   } // Layers
   } // Variables

}


void UATmvaClassification::DoBDT( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetTmvaVarNumRemove() ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ;
   
   for( int iBDTNTrees         = 0 ; iBDTNTrees         < (signed) (Cfg.GetBDTNTrees())->size()         ; ++iBDTNTrees         ) {
   for( int iBDTBoostType      = 0 ; iBDTBoostType      < (signed) (Cfg.GetBDTBoostType())->size()      ; ++iBDTBoostType      ) {
   for( int iBDTSeparationType = 0 ; iBDTSeparationType < (signed) (Cfg.GetBDTSeparationType())->size() ; ++iBDTSeparationType ) {
   for( int iBDTnCuts          = 0 ; iBDTnCuts          < (signed) (Cfg.GetBDTnCuts())->size()          ; ++iBDTnCuts          ) {
   for( int iBDTPruneMethod    = 0 ; iBDTPruneMethod    < (signed) (Cfg.GetBDTPruneMethod())->size()    ; ++iBDTPruneMethod    ) {
   for( int iBDTPruneStrength  = 0 ; iBDTPruneStrength  < (signed) (Cfg.GetBDTPruneStrength())->size()  ; ++iBDTPruneStrength  ) {
 

     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_BDT_" ;
     Name << Cfg.GetBDTNTrees()->at(iBDTNTrees) << "Trees_" ; 
     Name << Cfg.GetBDTBoostType()->at(iBDTBoostType) << "_" ; 
     Name << Cfg.GetBDTSeparationType()->at(iBDTSeparationType) << "_" ; 
     Name << Cfg.GetBDTnCuts()->at(iBDTnCuts) << "Cuts_" ; 
     Name << Cfg.GetBDTPruneMethod()->at(iBDTPruneMethod) << "_" ; 
     Name << Cfg.GetBDTPruneStrength()->at(iBDTPruneStrength) << "PruneStrength_" ;
     Name << nVarMax << "Var" ;
     NAME =  Name.str();

     // Create ethod Options
     ostringstream Method;
     Method << "H:!V:NTrees="      << Cfg.GetBDTNTrees()->at(iBDTNTrees)                ;
     Method << ":BoostType="       << Cfg.GetBDTBoostType()->at(iBDTBoostType)          ;
     Method << ":SeparationType="  << Cfg.GetBDTSeparationType()->at(iBDTSeparationType);
     Method << ":nCuts="           << Cfg.GetBDTnCuts()->at(iBDTnCuts)                  ;
     Method << ":PruneMethod="     << Cfg.GetBDTPruneMethod()->at(iBDTPruneMethod)      ;
     Method << ":PruneStrength="   << Cfg.GetBDTPruneStrength()->at(iBDTPruneStrength)  ;

     // Create and Train MVA
     Train(Cfg,T,Name.str(),Method.str(),nVarMax);

     // Making some basic plots
     Plot(0);

   } // BDTNTrees
   } // BDTBoostType
   } // BDTSeparationType
   } // BDTnCuts
   } // BDTPruneMethod
   } // BDTPruneStrength

   } // Variables

}

void UATmvaClassification::Train(UATmvaConfig& Cfg, UATmvaTree& T , string Name , string Method , int nVarMax ){

     cout << "[UATmvaClassification::Train()] TmvaName = " << Name   << endl;
     cout << "[UATmvaClassification::Train()] Method   = " << Method << endl;

     // Create UA TMVA Factory Interface
     UAFactory = new UATmvaFactory_t() ; 
     UAFactory->TmvaName = Name;
 
     // Open TMVA output file
     cout << "[UATmvaClassification::DoMLP()] Create: " << UAFactory->TmvaName << endl;
     UAFactory->TmvaFile  = TFile::Open("rootfiles/" + UAFactory->TmvaName  + ".root","RECREATE" );
  
     // Create TMVA Factory
     UAFactory->TmvaFactory = new Factory(UAFactory->TmvaName , UAFactory->TmvaFile ,
      "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
     
     // Add VariablesCfg.GetTmvaVar())
     Int_t nVar = 0;     
     for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
       if (++nVar <= nVarMax) UAFactory->TmvaFactory->AddVariable(iVar->VarName,iVar->VarType);
     }    
  
     // Weights
     UAFactory->TmvaFactory->SetWeightExpression(Cfg.GetTmvaWeight());
     Double_t SgWeight=1.0;
     Double_t BgWeight=1.0;
     
     // Add Tree
     for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
       if(iD->SigTrain ) {
         UAFactory->TmvaFactory->AddSignalTree     ( T.GetTree(iD->NickName) , SgWeight ) ;
       }
       if(iD->BkgdTrain) {
         UAFactory->TmvaFactory->AddBackgroundTree ( T.GetTree(iD->NickName) , BgWeight ) ;
       }
     }
  
     // Set training options
     UAFactory->TmvaFactory->PrepareTrainingAndTestTree("","");
  
     // BookMethod
     if ( Cfg.GetTmvaType() == "ANN" ) UAFactory->TmvaFactory->BookMethod( TMVA::Types::kMLP, UAFactory->TmvaName , Method );
     if ( Cfg.GetTmvaType() == "BDT" ) UAFactory->TmvaFactory->BookMethod( TMVA::Types::kBDT, UAFactory->TmvaName , Method );

     // Train , Test, Validate
     UAFactory->TmvaFactory->TrainAllMethods(); 
     UAFactory->TmvaFactory->TestAllMethods();
     UAFactory->TmvaFactory->EvaluateAllMethods();

     // Clean UAFactory
     delete UAFactory ;

}

void UATmvaClassification::Plot( bool isMLP ){
    cout << "[UATmvaClassification::Plot()] Plotting " << UAFactory->TmvaName << endl;
    string command;

    int iret;
    // Correlations
    command = "root -b -q $ROOTSYS/tmva/test/correlations.C\\(\\\"rootfiles/"+NAME+".root\\\"\\)";
    iret = system(command.c_str());
    command = "mv plots/CorrelationMatrixS.eps plots/CorrelationMatrixS_"+NAME+".eps";
    iret = system(command.c_str());
    command = "mv plots/CorrelationMatrixS.gif plots/CorrelationMatrixS_"+NAME+".gif";
    iret = system(command.c_str());
    command = "mv plots/CorrelationMatrixB.eps plots/CorrelationMatrixB_"+NAME+".eps";
    iret = system(command.c_str());
    command = "mv plots/CorrelationMatrixB.gif plots/CorrelationMatrixB_"+NAME+".gif";
    iret = system(command.c_str());

    // overtraining (test and training)
    command = "root -b -q $ROOTSYS/tmva/test/mvas.C\\(\\\"rootfiles/"+NAME+".root\\\",3\\)";
    iret = system(command.c_str());

    if (isMLP) {
      // epoch plot (test and training)
      command = "root -b -q $ROOTSYS/tmva/test/annconvergencetest.C\\(\\\"rootfiles/"+NAME+".root\\\"\\)";
      iret = system(command.c_str());
      command = "mv plots/annconvergencetest.eps plots/annconvergencetest_"+NAME+".eps";
      iret = system(command.c_str());
      command = "mv plots/annconvergencetest.gif plots/annconvergencetest_"+NAME+".gif";
      iret = system(command.c_str());
    }

    if ( iret != 0 ) {cout << iret;}
 
    // MVA efficiency
    //command = "root -b -q $ROOTSYS/tmva/test/mvaeffs.C\\(\\\"rootfiles/"+NAME+".root\\\"\\)";
    //system(command.c_str());
}
