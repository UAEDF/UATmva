

#include "../includes/UATmvaClassification.h"

#include <iostream>



// ---------------------------- Init()

void UATmvaClassification::Do( UATmvaConfig& Cfg, UATmvaTree& T) {

   if ( Cfg.GetTmvaType() == "CUT" )    DoCUT(Cfg,T);
   if ( Cfg.GetTmvaType() == "ANN" )    DoMLP(Cfg,T);
   if ( Cfg.GetTmvaType() == "BDT" )    DoBDT(Cfg,T);
   if ( Cfg.GetTmvaType() == "LH" )     DoLH(Cfg,T);
   if ( Cfg.GetTmvaType() == "PDERS" )  DoPDERS(Cfg,T);
   if ( Cfg.GetTmvaType() == "PDEFoam" )  DoPDEFoam(Cfg,T);


   if ( Cfg.GetTmvaType() == "XML" )  cout << "[UATmvaClassification::Do] XML external weight --> Nothing to train !" << endl;

}

void UATmvaClassification::DoMLP( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetTmvaVarNumRemove() ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ; 
   for (Int_t nHLayers = Cfg.GetANNHiddenLayersMin() ; nHLayers <= Cfg.GetANNHiddenLayersMax() ; ++nHLayers ) {
   for (Int_t nHNodes  = Cfg.GetANNHiddenNodesMin()  ; nHNodes  <= Cfg.GetANNHiddenNodesMax()  ; ++nHNodes  ) {
    
 
     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_MLP_" << nHLayers << "Layers_" << nHNodes << "Nodes_" << nVarMax << "Var" ;
     if (Cfg.GetTmvaOptim()) Name << "_Optim" ;
     NAME =  Name.str();


     // Create ethod Options
     ostringstream Method;
     Method << "H:!V:NeuronType=tanh:VarTransform=N:NCycles=" << Cfg.GetANNCycles() << ":HiddenLayers=" ;
     //Method << "H:!V:NeuronType=tanh:NCycles=" << Cfg.GetANNCycles() << ":HiddenLayers=" ;
     for ( Int_t iHLayer = 1 ; iHLayer <= nHLayers ; ++iHLayer ) {
       Method << "N+" << nHNodes ; 
       if ( iHLayer != nHLayers ) Method << "," ;
     }

     // Create and Train MVA
     Train(Cfg,T,Name.str(),Method.str(),nVarMax);

     // Making some basic plots
     //Plot(1);
   
   } // Nodes
   } // Layers
   } // Variables

}


void UATmvaClassification::DoBDT( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetTmvaVarNumRemove() ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ;
   
   for( int iBDTNTrees             = 0 ; iBDTNTrees              < (signed) (Cfg.GetBDTNTrees())->size()              ; ++iBDTNTrees              ) {
   for( int iBDTBoostType          = 0 ; iBDTBoostType           < (signed) (Cfg.GetBDTBoostType())->size()           ; ++iBDTBoostType           ) {
//   for( int iBDTAdaBoostR2Loss     = 0 ; iBDTAdaBoostR2Loss      < (signed) (Cfg.GetBDTAdaBoostR2Loss())->size()      ; ++iBDTAdaBoostR2Loss      ) {
   for( int iBDTUseBaggedGrad      = 0 ; iBDTUseBaggedGrad       < (signed) (Cfg.GetBDTUseBaggedGrad())->size()       ; ++iBDTUseBaggedGrad       ) {
   for( int iBDTGradBaggingFraction= 0 ; iBDTGradBaggingFraction < (signed) (Cfg.GetBDTGradBaggingFraction())->size() ; ++iBDTGradBaggingFraction ) {
   for( int iBDTShrinkage          = 0 ; iBDTShrinkage           < (signed) (Cfg.GetBDTShrinkage())->size()           ; ++iBDTShrinkage           ) {
   for( int iBDTSeparationType     = 0 ; iBDTSeparationType      < (signed) (Cfg.GetBDTSeparationType())->size()      ; ++iBDTSeparationType      ) {
   for( int iBDTnCuts              = 0 ; iBDTnCuts               < (signed) (Cfg.GetBDTnCuts())->size()               ; ++iBDTnCuts               ) {
   for( int iBDTPruneMethod        = 0 ; iBDTPruneMethod         < (signed) (Cfg.GetBDTPruneMethod())->size()         ; ++iBDTPruneMethod         ) {
   for( int iBDTPruneStrength      = 0 ; iBDTPruneStrength       < (signed) (Cfg.GetBDTPruneStrength())->size()       ; ++iBDTPruneStrength       ) {
//   for( int iBDTNNodesMax          = 0 ; iBDTNNodesMax           < (signed) (Cfg.GetBDTNNodesMax())->size()           ; ++iBDTNNodesMax           ) {

     // Skip non-meningfull combination

     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_BDT_" ;
     Name << Cfg.GetBDTNTrees()->at(iBDTNTrees) << "Trees_" ; 
     Name << Cfg.GetBDTBoostType()->at(iBDTBoostType) << "_" ; 
     if ( Cfg.GetBDTBoostType()->at(iBDTBoostType) == "Grad" ) {
       Name << Cfg.GetBDTUseBaggedGrad()->at(iBDTUseBaggedGrad) << "Bagged_" ;
       Name << Cfg.GetBDTGradBaggingFraction()->at(iBDTGradBaggingFraction) << "BagFrac_" ;
       Name << Cfg.GetBDTShrinkage()->at(iBDTShrinkage) << "BagShrink_" ;
     }
     Name << Cfg.GetBDTSeparationType()->at(iBDTSeparationType) << "_" ; 
     Name << Cfg.GetBDTnCuts()->at(iBDTnCuts) << "Cuts_" ; 
     Name << Cfg.GetBDTPruneMethod()->at(iBDTPruneMethod) << "_" ; 
     Name << Cfg.GetBDTPruneStrength()->at(iBDTPruneStrength) << "PruneStrength_" ;
//     Name << Cfg.GetBDTNNodesMax()->at(iBDTNNodesMax) << "NodesMax_" ;
     Name << nVarMax << "Var" ;    
     if (Cfg.GetTmvaOptim()) Name << "_Optim" ;
     NAME =  Name.str();

     // Create ethod Options
     ostringstream Method;
     Method << "H:V:NTrees="      << Cfg.GetBDTNTrees()->at(iBDTNTrees)                ;
     Method << ":BoostType="       << Cfg.GetBDTBoostType()->at(iBDTBoostType)          ;
     if ( Cfg.GetBDTBoostType()->at(iBDTBoostType) == "Grad" ) {
       Method << ":UseBaggedGrad="        << Cfg.GetBDTUseBaggedGrad()->at(iBDTUseBaggedGrad)  ;
       Method << ":GradBaggingFraction="  << Cfg.GetBDTGradBaggingFraction()->at(iBDTGradBaggingFraction) ;
       Method << ":Shrinkage="            << Cfg.GetBDTShrinkage()->at(iBDTShrinkage)          ; 
     }
     Method << ":SeparationType="  << Cfg.GetBDTSeparationType()->at(iBDTSeparationType);
     Method << ":nCuts="           << Cfg.GetBDTnCuts()->at(iBDTnCuts)                  ;
     Method << ":PruneMethod="     << Cfg.GetBDTPruneMethod()->at(iBDTPruneMethod)      ;
     Method << ":PruneStrength="   << Cfg.GetBDTPruneStrength()->at(iBDTPruneStrength)  ;
//     Method << ":NNodesMax="        << Cfg.GetBDTNNodesMax()->at(iBDTNNodesMax)          ;

     // Create and Train MVA
     Train(Cfg,T,Name.str(),Method.str(),nVarMax);

     // Making some basic plots
     //Plot(0);

   } // BDTNTrees
   } // BDTBoostType
//   } // BDTAdaBoostR2Loss
   } // BDTUseBaggedGrad
   } // BDTGradBaggingFraction
   } // BDTShrinkage
   } // BDTSeparationType
   } // BDTnCuts
   } // BDTPruneMethod
   } // BDTPruneStrength
//   } // BDTNNodesMax

   } // Variables

}


void UATmvaClassification::DoLH( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetTmvaVarNumRemove() ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ; 
 
     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_Likelihood_" << nVarMax << "Var" ;
     NAME =  Name.str();

     // Create Method Options
     ostringstream Method;
     Method << "H:!V" ;
     if ( Cfg.GetTmvaRespUseSigmoid() ) {
       Method << ":TransformOutput" ;
     } else {
       Method << ":!TransformOutput" ;
     }
     Method << ":PDFInterpol=Spline2:NSmoothSig[0]=30:NSmoothBkg[0]=30:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" ;


     // Create and Train MVA
     Train(Cfg,T,Name.str(),Method.str(),nVarMax);

     // Making some basic plots
     //Plot(0);
   
   } // Variables

}

void UATmvaClassification::DoPDERS( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetTmvaVarNumRemove() ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ; 

     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_PDERS_" << nVarMax << "Var" ;
     NAME =  Name.str();

     // Create Method Options
     ostringstream Method;
     Method << "H:!V:VolumeRangeMode=RMS" ;

     // Create and Train MVA
     Train(Cfg,T,Name.str(),Method.str(),nVarMax);

     // Making some basic plots
     //Plot(0);

   } // Variables

}


void UATmvaClassification::DoPDEFoam( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetTmvaVarNumRemove() ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ;

     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_PDEFoam_" << nVarMax << "Var" ;
     NAME =  Name.str();

     // Create Method Options
     ostringstream Method;
     //Method << "H:!V" ;
     Method << "H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.5333:nActiveCells=500:nSampl=100:nBin=5:Nmin=100:Kernel=None:Compress=T" ;

     // Create and Train MVA
     Train(Cfg,T,Name.str(),Method.str(),nVarMax);

     // Making some basic plots
     //Plot(0);

   } // Variables

}

void UATmvaClassification::DoCUT( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetTmvaVarNumRemove() ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ;

     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_CUT_" << nVarMax << "Var" ;
     NAME =  Name.str();

     // Create Method Options
     ostringstream Method;
     //Method << "H:!V" ;
     Method << "H:!V:" << Cfg.GetCUTOptions() ;

     // Create and Train MVA
     Train(Cfg,T,Name.str(),Method.str(),nVarMax);

     // Making some basic plots
     //Plot(0);

   } // Variables

}


void UATmvaClassification::Train(UATmvaConfig& Cfg, UATmvaTree& T , string Name , string Method , int nVarMax ){

  if ( Cfg.GetTmvaDim() == 1 ) {
    Train1D(Cfg,T,Name,Method,nVarMax,1);
  } else {
    for ( int iDim = 1 ; iDim <= Cfg.GetTmvaDim() ; ++iDim ) {
      ostringstream Name1D;
      Name1D << Name << "_Dim" << iDim ;
      Train1D(Cfg,T,Name1D.str(),Method,nVarMax,iDim);
    }
  }
  
}

void UATmvaClassification::Train1D(UATmvaConfig& Cfg, UATmvaTree& T , string Name , string Method , int nVarMax , int iDim ){

     cout << "[UATmvaClassification::Train()] TmvaName = " << Name   << endl;
     cout << "[UATmvaClassification::Train()] Method   = " << Method << endl;

     // Create UA TMVA Factory Interface
     UAFactory = new UATmvaFactory_t() ; 
     UAFactory->TmvaName = Name;
 
     // Open TMVA output file
     cout << "[UATmvaClassification::Train1D()] Create: " << UAFactory->TmvaName << endl;
     UAFactory->TmvaFile  = TFile::Open("rootfiles/" + UAFactory->TmvaName  + ".root","RECREATE" );
  
     // Create TMVA Factory
     //UAFactory->TmvaFactory = new Factory(UAFactory->TmvaName , UAFactory->TmvaFile ,
      UAFactory->TmvaFactory = new Factory("UATmva" , UAFactory->TmvaFile ,
      "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
    

     cout << "[UATmvaClassification::Train1D()] Factory created !!! "<< endl;
     // Add VariablesCfg.GetTmvaVar())
     Int_t nVar = 0;     
     for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
       if (++nVar <= nVarMax) UAFactory->TmvaFactory->AddVariable(iVar->VarName,iVar->VarType);
     }    
  
     // Weights
     cout << "[UATmvaClassification::Train1D()] Setting weight "<< endl;
     UAFactory->TmvaFactory->SetWeightExpression(Cfg.GetTmvaWeight());
     Double_t SgWeight=1.0;
     Double_t BgWeight=1.0;
     
     // Add Tree
     cout << "[UATmvaClassification::Train1D()] Adding trees "<< endl;
     for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
       // SigTrain
       if(iD->SigTrain ) {
         SgWeight = iD->ScaleFac;
         //if      ( Cfg.GetTestMode() == 1 )
           UAFactory->TmvaFactory->AddSignalTree     ( T.GetTree(iD->NickName) , SgWeight ) ;
         //else if ( Cfg.GetTestMode() == 2 ) 
         //  UAFactory->TmvaFactory->AddSignalTree     ( T.GetTree(iD->NickName) , SgWeight , "Training") ;
       }
       // SigTest (if that mode)
/*
       if(iD->SigTest ) {
         SgWeight = iD->ScaleFac;
         if      ( Cfg.GetTestMode() == 2 )
           UAFactory->TmvaFactory->AddSignalTree     ( T.GetTree(iD->NickName) , SgWeight , "Test") ;
       }
*/
       // BkgdTrain 
       if(iD->BkgdTrain) {
         BgWeight = iD->ScaleFac;
         if ( Cfg.GetTmvaDim() == 1 ) { 
           //if      ( Cfg.GetTestMode() == 1 )
             UAFactory->TmvaFactory->AddBackgroundTree ( T.GetTree(iD->NickName) , BgWeight ) ;
           //else if ( Cfg.GetTestMode() == 2 )
           //  UAFactory->TmvaFactory->AddBackgroundTree ( T.GetTree(iD->NickName) , BgWeight , "Training") ;
         } else {
           if ( (iD->BkgdTrain) == iDim ) {
             //if      ( Cfg.GetTestMode() == 1 )
               UAFactory->TmvaFactory->AddBackgroundTree ( T.GetTree(iD->NickName) , BgWeight ) ;
             //else if ( Cfg.GetTestMode() == 2 )
             //  UAFactory->TmvaFactory->AddBackgroundTree ( T.GetTree(iD->NickName) , BgWeight , "Training" ) ;
           }
         }
       }
       // BkgdTest (if that mode)
/*
       if(iD->BkgdTest) {
         BgWeight = iD->ScaleFac;
         if ( Cfg.GetTestMode() == 2 ) {
           if ( Cfg.GetTmvaDim() == 1 ) { 
             UAFactory->TmvaFactory->AddBackgroundTree ( T.GetTree(iD->NickName) , BgWeight , "Test" ) ;
           } else {  
             if ( (iD->BkgdTrain) == iDim )
               UAFactory->TmvaFactory->AddBackgroundTree ( T.GetTree(iD->NickName) , BgWeight , "Test" ) ;
           }
         }
       }
*/ 
     }
  
     // Set training options
     cout << "[UATmvaClassification::Train1D()] Set training options" << endl;
     TCut Cut = (Cfg.GetTmvaPreCut()).c_str() ;
     UAFactory->TmvaFactory->PrepareTrainingAndTestTree(Cut,"");
  
     // BookMethod
     cout << "[UATmvaClassification::Train1D()] BookMethod" << endl;
     string VarTrans = "";
     if  ( (Cfg.GetTmvaVarTrans())->size() > 0 ) VarTrans = ":VarTransform=";
     for ( int i = 0 ; i < (signed) (Cfg.GetTmvaVarTrans())->size() ; ++i ) {
       VarTrans += (Cfg.GetTmvaVarTrans())->at(i) ;
       if ( i+1 < (signed) (Cfg.GetTmvaVarTrans())->size() ) VarTrans += ";" ; 
     } 
     if  ( (Cfg.GetTmvaVarTrans())->size() > 0 ) Method += VarTrans ;

     cout << "[UATmvaClassification::Train1D()] Method = " << Method << endl;
     if ( Cfg.GetTmvaType() == "CUT" )    UAFactory->TmvaFactory->BookMethod( TMVA::Types::kCuts      , UAFactory->TmvaName , Method );
     if ( Cfg.GetTmvaType() == "ANN" )    UAFactory->TmvaFactory->BookMethod( TMVA::Types::kMLP       , UAFactory->TmvaName , Method );
     if ( Cfg.GetTmvaType() == "BDT" )    UAFactory->TmvaFactory->BookMethod( TMVA::Types::kBDT       , UAFactory->TmvaName , Method );
     if ( Cfg.GetTmvaType() == "LH"  )    UAFactory->TmvaFactory->BookMethod( TMVA::Types::kLikelihood, UAFactory->TmvaName , Method );
     if ( Cfg.GetTmvaType() == "PDERS")   UAFactory->TmvaFactory->BookMethod( TMVA::Types::kPDERS     , UAFactory->TmvaName , Method );
     if ( Cfg.GetTmvaType() == "PDEFoam") UAFactory->TmvaFactory->BookMethod( TMVA::Types::kPDEFoam   , UAFactory->TmvaName , Method );


     // Tmva Optimisation
     cout << "[UATmvaClassification::Train1D()] Optimisation " << endl;
     if (Cfg.GetTmvaOptim()) UAFactory->TmvaFactory->OptimizeAllMethods();

     // Train , Test, Validate
     cout << "[UATmvaClassification::Train1D()] Train NOW " << endl;
     UAFactory->TmvaFactory->TrainAllMethods(); 
     cout << "[UATmvaClassification::Train1D()] Testing NOW " << endl;
     UAFactory->TmvaFactory->TestAllMethods();
     cout << "[UATmvaClassification::Train1D()] Evaluating NOW " << endl;
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
