

TString directory = "rootfiles.2014Selection.4L.NewMC";


//------------------------------------------------------------------------------
//
// standAloneTop
//
//------------------------------------------------------------------------------
void standAloneTop(Int_t  njet          = 1,
		   Bool_t useDataDriven = true,
		   Int_t  printLevel    = 2)
{
  Double_t dyYield;
  Double_t dyStatError;
  Double_t dySystError;
  Double_t dyScaleFactor;

gInterpreter->LoadMacro("DY.C+");
gInterpreter->LoadMacro("Top.C+");

  DY(dyYield,
     dyStatError,
     dySystError,
     dyScaleFactor,
     njet,
     "SF",
     directory,
     useDataDriven,
     printLevel);

  Double_t topYield;
  Double_t topStatError;
  Double_t topSystError;
  Double_t topScaleFactor;

  Top(topYield,
      topStatError,
      topSystError,
      topScaleFactor,
      dyScaleFactor,
      njet,
      "All",
      directory,
      useDataDriven,
      printLevel);
}
