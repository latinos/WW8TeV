void runLatinosTreeScript(TString  rootPath      = "",
			  Double_t luminosity    = 12.103,
			  Int_t    jetChannel    = 1,
			  TString  flavorChannel = "MuE",
			  TString  theSample     = "DYtautau",
			  TString  systematic    = "nominals",
			  Bool_t   verbose       = false)
{
  gInterpreter->LoadMacro("LatinosTreeScript.C+");

  LatinosTreeScript(rootPath, luminosity, jetChannel, flavorChannel, theSample, systematic, verbose);
}
