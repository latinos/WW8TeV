#include "DY.C"


TString directory = "rootfiles.2014Selection.4L.NewMC";


//------------------------------------------------------------------------------
//
// standAloneDY
//
//------------------------------------------------------------------------------
void standAloneDY(Int_t  njet          = 0,
		  Bool_t useDataDriven = true,
		  Int_t  printLevel    = 2,
		  Bool_t drawR         = true)
{
  Double_t dyYield;
  Double_t dyStatError;
  Double_t dySystError;
  Double_t dyScaleFactor;

  DY(dyYield,
     dyStatError,
     dySystError,
     dyScaleFactor,
     njet,
     "SF",
     directory,
     useDataDriven,
     printLevel,
     drawR);
}
