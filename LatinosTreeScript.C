#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TSystem.h"
#include "TTree.h"
#include <iomanip>
#include <iostream>


const Double_t ZMASS = 91.1876;

const UInt_t numberMetCuts = 5;
const UInt_t numberDYMVACuts = 5;

Double_t MetCut[] = {20, 25, 30, 45, 1000};

Double_t DYMVACut_0j[numberDYMVACuts] = {-0.9, -0.86, -0.6, 0.88, 1000};

Double_t DYMVACut_1j[numberDYMVACuts] = {-0.9, -0.86, -0.6, 0.84, 1000};

Bool_t runAtOviedo = false;
Bool_t runAtIfca   = false;
Bool_t runEOS      = false;


//gROOT->LoadMacro("../utils/TResultsTable.C+");

//------------------------------------------------------------------------------
// LatinosTreeScript
//------------------------------------------------------------------------------
void LatinosTreeScript(TString rootPath,
		       Float_t luminosity,
		       Int_t   jetChannel,
		       TString flavorChannel,
		       TString theSample,
		       TString systematic,
		       Bool_t  verbose)
{
  TH1::SetDefaultSumw2();

  TString path = Form("rootfiles/%s/%djet/%s/", systematic.Data(), jetChannel, flavorChannel.Data());

  gSystem->mkdir(path, kTRUE);

  TFile* output = new TFile(path + theSample + ".root", "recreate");


  // Counting histograms
  //----------------------------------------------------------------------------
  TH1F* hWTrigger     = new TH1F("hWTrigger",     "", 3, 0, 3);
  TH1F* hWMetCut      = new TH1F("hWMetCut",      "", 3, 0, 3);
  TH1F* hWLowMinv     = new TH1F("hWLowMinv",     "", 3, 0, 3);
  TH1F* hWZVeto       = new TH1F("hWZVeto",       "", 3, 0, 3);
  TH1F* hWpMetCut     = new TH1F("hWpMetCut",     "", 3, 0, 3);
  TH1F* hWJetVeto     = new TH1F("hWJetVeto",     "", 3, 0, 3);
  TH1F* hWDeltaPhiJet = new TH1F("hWDeltaPhiJet", "", 3, 0, 3);
  TH1F* hWSoftMuVeto  = new TH1F("hWSoftMuVeto",  "", 3, 0, 3); 
  TH1F* hWExtraLepton = new TH1F("hWExtraLepton", "", 3, 0, 3);
  TH1F* hWPtll        = new TH1F("hWPtll",        "", 3, 0, 3);
  TH1F* hWTopTagging  = new TH1F("hWTopTagging",  "", 3, 0, 3);

  TH1F* hWeffTrigger     = new TH1F("hWeffTrigger",     "", 3, 0, 3);
  TH1F* hWeffMetCut      = new TH1F("hWeffMetCut",      "", 3, 0, 3);
  TH1F* hWeffLowMinv     = new TH1F("hWeffLowMinv",     "", 3, 0, 3);
  TH1F* hWeffZVeto       = new TH1F("hWeffZVeto",       "", 3, 0, 3);
  TH1F* hWeffpMetCut     = new TH1F("hWeffpMetCut",     "", 3, 0, 3);
  TH1F* hWeffJetVeto     = new TH1F("hWeffJetVeto",     "", 3, 0, 3);
  TH1F* hWeffDeltaPhiJet = new TH1F("hWeffDeltaPhiJet", "", 3, 0, 3);
  TH1F* hWeffSoftMuVeto  = new TH1F("hWeffSoftMuVeto",  "", 3, 0, 3); 
  TH1F* hWeffExtraLepton = new TH1F("hWeffExtraLepton", "", 3, 0, 3);
  TH1F* hWeffPtll        = new TH1F("hWeffPtll",        "", 3, 0, 3);
  TH1F* hWeffTopTagging  = new TH1F("hWeffTopTagging",  "", 3, 0, 3);


  // WW level histograms
  //----------------------------------------------------------------------------
  TH1F* hPtLepton1WWLevel       = new TH1F("hPtLepton1WWLevel",       "", 200, 0, 200);
  TH1F* hPtLepton2WWLevel       = new TH1F("hPtLepton2WWLevel",       "", 200, 0, 200);
  TH1F* hPtDiLeptonWWLevel      = new TH1F("hPtDiLeptonWWLevel",      "", 200, 0, 200);
  TH1F* hMinvWWLevel            = new TH1F("hMinvWWLevel",            "", 200, 0, 200);
  TH1F* hMtWWLevel              = new TH1F("hMtWWLevel",              "", 250, 0, 250);
  TH1F* hNJets30WWLevel         = new TH1F("hNJetsPF30WWLevel",       "",  10, 0,  10);
  TH1F* hpfMetWWLevel           = new TH1F("hpfMetWWLevel",           "", 150, 0, 150);
  TH1F* hppfMetWWLevel          = new TH1F("hppfMetWWLevel",          "", 150, 0, 150);
  TH1F* hchMetWWLevel           = new TH1F("hchMetWWLevel",           "", 150, 0, 150);
  TH1F* hpchMetWWLevel          = new TH1F("hpchMetWWLevel",          "", 150, 0, 150);
  TH1F* hpminMetWWLevel         = new TH1F("hpminMetWWLevel",         "", 150, 0, 150);
  TH1F* hDeltaRLeptonsWWLevel   = new TH1F("hDeltaRLeptonsWWLevel",   "",  50, 0,   5);
  TH1F* hDeltaPhiLeptonsWWLevel = new TH1F("hDeltaPhiLeptonsWWLevel", "",  32, 0, 3.2);
  TH1F* hDPhiPtllJetWWLevel     = new TH1F("hDPhiPtllJetWWLevel",     "",  32, 0, 3.2);

  TH1F* h_WWLevel_TightFailEvents = new TH1F("h_WWLevel_TightFailEvents", "", 3, 0 , 3);
  TH1F* h_WWLevel_TightTightEvents = new TH1F("h_WWLevel_TightTightEvents", "", 3, 0 , 3);
  TH1F* h_WWLevel_TightLooseEvents = new TH1F("h_WWLevel_TightLooseEvents", "", 3, 0 , 3);


  // TwoLeptons level histograms
  //----------------------------------------------------------------------------
  TH1F* hPtLepton1TwoLeptonsLevel       = new TH1F("hPtLepton1TwoLeptonsLevel",       "", 200, 0, 200);
  TH1F* hPtLepton2TwoLeptonsLevel       = new TH1F("hPtLepton2TwoLeptonsLevel",       "", 200, 0, 200);
  TH1F* hPtDiLeptonTwoLeptonsLevel      = new TH1F("hPtDiLeptonTwoLeptonsLevel",      "", 200, 0, 200);
  TH1F* hMinvTwoLeptonsLevel            = new TH1F("hMinvTwoLeptonsLevel",            "", 200, 0, 200);
  TH1F* hMtTwoLeptonsLevel              = new TH1F("hMtTwoLeptonsLevel",              "", 250, 0, 250);
  TH1F* hNJets30TwoLeptonsLevel         = new TH1F("hNJetsPF30TwoLeptonsLevel",       "",  10, 0,  10);
  TH1F* hpfMetTwoLeptonsLevel           = new TH1F("hpfMetTwoLeptonsLevel",           "", 150, 0, 150);
  TH1F* hppfMetTwoLeptonsLevel          = new TH1F("hppfMetTwoLeptonsLevel",          "", 150, 0, 150);
  TH1F* hchMetTwoLeptonsLevel           = new TH1F("hchMetTwoLeptonsLevel",           "", 150, 0, 150);
  TH1F* hpchMetTwoLeptonsLevel          = new TH1F("hpchMetTwoLeptonsLevel",          "", 150, 0, 150);
  TH1F* hpminMetTwoLeptonsLevel         = new TH1F("hpminMetTwoLeptonsLevel",         "", 150, 0, 150);
  TH1F* hDeltaRLeptonsTwoLeptonsLevel   = new TH1F("hDeltaRLeptonsTwoLeptonsLevel",   "",  50, 0,   5);
  TH1F* hDeltaPhiLeptonsTwoLeptonsLevel = new TH1F("hDeltaPhiLeptonsTwoLeptonsLevel", "",  32, 0, 3.2);
  TH1F* hDPhiPtllJetTwoLeptonsLevel     = new TH1F("hDPhiPtllJetTwoLeptonsLevel",     "",  32, 0, 3.2);

  TH1F* h_TwoLeptons_TightFailEvents = new TH1F("h_TwoLeptons_TightFailEvents", "", 3, 0 , 3);
  TH1F* h_TwoLeptons_TightTightEvents = new TH1F("h_TwoLeptons_TightTightEvents", "", 3, 0 , 3);
  TH1F* h_TwoLeptons_TightLooseEvents = new TH1F("h_TwoLeptons_TightLooseEvents", "", 3, 0 , 3);



  // Dilepton level differential histograms 
  //----------------------------------------------------------------------------

  //differential in dilepton (eta, pt, ptll, mll, dphill), jets (jetpt1, njet), transverse mass? 2-jet bin?
  
  Double_t pt1bins[8] = {20,40,60,80,100,125,150,200}; 
  Double_t eta1bins[9] = {-2.4,-1.8,-1.2,-0.6,0,0.6,1.2,1.8,2.4};
  Double_t mllbins[9] = {20,40,60,80,100,125,150,175,200};
  Double_t dphibins[13] = {0,0.25,0.5,0.75,1,1.25,1.5,1.75,2,2.25,2.5,2.75,3};
  Double_t ptllbins[8] = {30,40,50,60,70,85,120,150};

  TH1F* hPtLepton1DilepLevel_Diff  = new TH1F("hPtLepton1DilepLevel_Diff",       "", 7, pt1bins);
  TH1F* hEtaLepton1DilepLevel_Diff  = new TH1F("hEtaLepton1DilepLevel_Diff",       "", 8, eta1bins);
  TH1F* hDileptonDilepLevel_Diff  = new TH1F("hDileptonDilepLevel_Diff",       "", 7, ptllbins);
  TH1F* hMinvDilepLevel_Diff  = new TH1F("hMinvDilepLevel_Diff",       "", 8, mllbins);
  TH1F* hDeltaPhiDilepLevel_Diff  = new TH1F("hDeltaPhiDilepLevel_Diff",       "", 12, dphibins);


  // WW level differential histograms 
  //----------------------------------------------------------------------------

  TH1F* hPtLepton1WWLevel_Diff  = new TH1F("hPtLepton1WWLevel_Diff",       "",7, pt1bins);
  TH1F* hEtaLepton1WWLevel_Diff  = new TH1F("hEtaLepton1WWLevel_Diff",       "", 8, eta1bins);
  TH1F* hDileptonWWLevel_Diff  = new TH1F("hDileptonWWLevel_Diff",       "", 7, ptllbins);
  TH1F* hMinvWWLevel_Diff  = new TH1F("hMinvWWLevel_Diff",       "", 8, mllbins);
  TH1F* hDeltaPhiWWLevel_Diff  = new TH1F("hDeltaPhiWWLevel_Diff",       "", 12, dphibins);




  // Data-driven methods: Z+jets
  //----------------------------------------------------------------------------
  TH1F* hNinZevents     [numberDYMVACuts];
  TH1F* hNoutZevents    [numberDYMVACuts];
  TH1F* hNinLooseZevents[numberDYMVACuts];
  TH1F* hMassInZevents  [numberDYMVACuts];
  TH1F* hMassOutZevents [numberDYMVACuts];

  for (Int_t nC=0; nC<numberDYMVACuts; nC++) {
    hNinZevents     [nC] = new TH1F(Form("hNinZevents%.1i",      nC+1 ), "",   3, 0,   3);
    hNoutZevents    [nC] = new TH1F(Form("hNoutZevents%.1i",     nC+1 ), "",   3, 0,   3);
    hNinLooseZevents[nC] = new TH1F(Form("hNinLooseZevents%.1i", nC+1 ), "",   3, 0,   3);
    hMassInZevents  [nC] = new TH1F(Form("hMassInZevents%.1i",   nC+1 ), "", 200, 0, 200);
    hMassOutZevents [nC] = new TH1F(Form("hMassOutZevents%.1i",  nC+1 ), "", 200, 0, 200);

   


  }


  // Data-driven methods: Top
  //----------------------------------------------------------------------------
  TH1F* hTopTaggedEvents            = new TH1F("hTopTaggedEvents",            "", 3, 0, 3);
  TH1F* hNTopControlRegion          = new TH1F("hNTopControlRegion",          "", 3, 0, 3);
  TH1F* hNTopTaggedTopControlRegion = new TH1F("hNTopTaggedTopControlRegion", "", 3, 0, 3);

  TH1F* hbTagDisTopTaggedEvents            = new TH1F("hbTagDisTopTaggedEvents",            "", 300, -10, 20);
  TH1F* hbTagDisNTopControlRegion          = new TH1F("hbTagDisNTopControlRegion",          "", 300, -10, 20);
  TH1F* hbTagDisNTopTaggedTopControlRegion = new TH1F("hbTagDisNTopTaggedTopControlRegion", "", 300, -10, 20);


  //----------------------------------------------------------------------------
  // Input files
  //----------------------------------------------------------------------------
  TString filesPath;

  if (runAtOviedo) filesPath = " /hadoop/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/";
  if (runAtIfca)   filesPath = "/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/Systematics2013_nominals_fromMaiko/"; 
  if (runEOS)      filesPath = "root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/amassiro/RunI/trees/tree_skim_wwmin_09Jan2014/";

  if (rootPath != "") filesPath = rootPath;

  TChain* tree = new TChain("latino", "latino");

  if (theSample == "DataRun2012_Total") {

    tree->Add(filesPath + "/data/" + "latino_RunA_892pbinv.root");
    tree->Add(filesPath + "/data/" + "latino_RunB_4404pbinv.root");
    tree->Add(filesPath + "/data/" + "latino_RunC_7032pbinv.root");
    tree->Add(filesPath + "/data/" + "latino_RunD_7274pbinv.root");
  }

  else if (theSample == "WJetsFakes_Total_old") {
    tree->Add("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/Moriond13/latino_LooseLoose_19.5fb.root");

 }
  else if (theSample == "WJetsFakes_Total") {
      
    tree->Add(filesPath + "/wjets/" + "latino_RunA_892pbinv_LooseLoose.root");
    tree->Add(filesPath + "/wjets/" + "latino_RunB_4404pbinv_LooseLoose.root");
    tree->Add(filesPath + "/wjets/" + "latino_RunC_7032pbinv_LooseLoose.root");
    tree->Add(filesPath + "/wjets/" + "latino_RunD_7274pbinv_LooseLoose.root");
    tree->Add(filesPath + "/wjets/" + "latino_082_WGstarToElNuMad.root");
    tree->Add(filesPath + "/wjets/" + "latino_083_WGstarToMuNuMad.root");
    tree->Add(filesPath + "/wjets/" + "latino_084_WGstarToTauNuMad.root");
    tree->Add(filesPath + "/wjets/" + "latino_085_WgammaToLNuG.root");
    tree->Add(filesPath + "/wjets/" + "latino_086_ZgammaToLLuG.root");

  }
  else if (theSample == "ggWWto2L") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_001_GluGluToWWTo4L.root");
  }
  else if (theSample == "WWTo2L2Nu") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_000_WWJets2LMad.root");
  }
  else if (theSample == "WWTo2L2Nu_pow") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_006_WWJets2LPowheg.root");
  }
  else if (theSample == "WWTo2L2Nu_nonSkim_pow") {
    tree->Add("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/DiferentialXSection/NoSkim_puW_effW_triggW/latino006.root");
  }
  else if (theSample == "WWTo2L2Nu_mcnlo") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_002_WWto2L2NuMCatNLO.root");
  }
  else if (theSample == "WWTo2L2Nu_mcnlo_nnll") {
    tree->Add("/gpfs/csic_projects/cms/calderon/WWGEN/nnllResummation/latino002_nll_ewk.root");
  }
  else if (theSample == "WWTo2L2Nu_pow_nnll") {
    tree->Add("/gpfs/csic_projects/cms/calderon/WWGEN/nnllResummation/latino006_nll_ewk.root");
  }
  else if (theSample == "WWTo2L2Nu_mad_nnll") {
    tree->Add("/gpfs/csic_projects/cms/calderon/WWGEN/nnllResummation/latino000_nll_ewk.root");
  } else if (theSample == "WZ") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_074_WZJetsMad.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_078_WZTo2L2QMad.root");
  }
  else if (theSample == "ZZ") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_075_ZZJetsMad.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_079_ZZTo2L2QMad.root"); 
  }
  else if (theSample == "TTbar") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_019_TTTo2L2Nu2B.root");
  }
  else if (theSample == "TW") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_011_TtWFullDR.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_012_TbartWFullDR.root");
  }
  else if (theSample == "Top") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_019run_TTTo2L2Nu2B.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_011_TtWFullDR.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_012_TbartWFullDR.root");
  }
  else if (theSample == "DY") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_036_DY10toLLMad.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_037_DY50toLLMad.root");
  }
  else if (theSample == "DYtautau") { // Only for OF channels!!! 
    tree->Add(filesPath + "/templates/" + "latino_DYtt_19.5fb.root"); // CHECK PATH    
  }
  else if (theSample == "Zgamma") { 
    tree->Add(filesPath + "/" + systematic + "/" + "latino_086_ZgammaToLLuG.root");
  }
  else if (theSample == "WgammaNoStar") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_085_WgammaToLNuG.root");
  }
  else if (theSample == "WgammaStar") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_082_WGstarToElNuMad.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_083_WGstarToMuNuMad.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_084_WGstarToTauNuMad.root");
  }
  else if (theSample == "HWW125") { 
    tree->Add(filesPath + "/" + systematic + "/" + "latino_1125_ggToH125toWWTo2LAndTau2Nu.root");
  }
  else if (theSample =="GamGamWW"){
    tree->Add(filesPath + "/" + systematic + "/" + "latino_008_GamGamWW.root");    
  }
  else if (theSample == "WJets") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_080_WJetsToLNuMad.root");
    //CHECK PATH: tree->Add("/gpfs/csic_projects/tier3data/LatinosSkims/ReducedTrees/R53X_S1_V08_S2_V09_S3_V13/MC_LooseLoose/4L/latino_080_WJetsToLNuMad.root");
  }
  else if (theSample == "VVV") {
    tree->Add(filesPath + "/" + systematic + "/" + "latino_088_WWGJets.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_089_WZZJets.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_090_ZZZJets.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_091_WWZJets.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_092_WWWJets.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_093_TTWJets.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_094_TTZJets.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_095_TTWWJets.root");
    tree->Add(filesPath + "/" + systematic + "/" + "latino_096_TTGJets.root");
  }
  else {
    return;
  }


  // Declaration of leaf types
  //----------------------------------------------------------------------------
  Float_t baseW;        tree->SetBranchAddress("baseW"       , &baseW);
  Float_t channel;      tree->SetBranchAddress("channel"     , &channel);
  Float_t chmet;        tree->SetBranchAddress("chmet"       , &chmet);
  Float_t dataset;      tree->SetBranchAddress("dataset"     , &dataset);
  Float_t dphill;       tree->SetBranchAddress("dphill"      , &dphill);
  Float_t dphilljet;    tree->SetBranchAddress("dphilljet"   , &dphilljet);
  Float_t dphilljetjet; tree->SetBranchAddress("dphilljetjet", &dphilljetjet);
  Float_t drll;         tree->SetBranchAddress("drll"        , &drll);
  Float_t effW;         tree->SetBranchAddress("effW"        , &effW);
  Float_t jeteta1;      tree->SetBranchAddress("jeteta1"     , &jeteta1);
  Float_t jeteta2;      tree->SetBranchAddress("jeteta2"     , &jeteta2);
  Float_t jeteta3;      tree->SetBranchAddress("jeteta3"     , &jeteta3);
  Float_t jetpt1;       tree->SetBranchAddress("jetpt1"      , &jetpt1);
  Float_t jetpt2;       tree->SetBranchAddress("jetpt2"      , &jetpt2);
  Float_t jetpt3;       tree->SetBranchAddress("jetpt3"      , &jetpt3);
  Float_t jettche1;     tree->SetBranchAddress("jettche1"    , &jettche1);
  Float_t jettche2;     tree->SetBranchAddress("jettche2"    , &jettche2);
  Float_t mctruth;      tree->SetBranchAddress("mctruth"     , &mctruth);
  Float_t mll;          tree->SetBranchAddress("mll"         , &mll);
  Float_t mpmet;        tree->SetBranchAddress("mpmet"       , &mpmet); 
  Float_t mth;          tree->SetBranchAddress("mth"         , &mth);
  Float_t nbjet;        tree->SetBranchAddress("nbjet"       , &nbjet);
  Float_t nbjettche;    tree->SetBranchAddress("nbjettche"   , &nbjettche);
  Float_t nextra;       tree->SetBranchAddress("nextra"      , &nextra);
  Float_t njet;         tree->SetBranchAddress("njet"        , &njet);
  Float_t nvtx;         tree->SetBranchAddress("nvtx"        , &nvtx);
  Float_t pchmet;       tree->SetBranchAddress("pchmet"      , &pchmet);
  Float_t pfmet;        tree->SetBranchAddress("pfmet"       , &pfmet);
  Float_t ppfmet;       tree->SetBranchAddress("ppfmet"      , &ppfmet);
  Float_t pt1;          tree->SetBranchAddress("pt1"         , &pt1);
  Float_t pt2;          tree->SetBranchAddress("pt2"         , &pt2);
  Float_t eta1;          tree->SetBranchAddress("eta1"         , &eta1);
  Float_t eta2;          tree->SetBranchAddress("eta2"         , &eta2);
  Float_t ch1;          tree->SetBranchAddress("ch1"         , &ch1);
  Float_t ch2;          tree->SetBranchAddress("ch2"         , &ch2);
  Float_t ptll;         tree->SetBranchAddress("ptll"        , &ptll);
  Float_t softtche;     tree->SetBranchAddress("softtche"    , &softtche);
  Float_t trigger;      tree->SetBranchAddress("trigger"     , &trigger);
  Float_t triggW;       tree->SetBranchAddress("triggW"      , &triggW);
  Int_t   bveto;        tree->SetBranchAddress("bveto"       , &bveto);
  Int_t   bveto_ip;     tree->SetBranchAddress("bveto_ip"    , &bveto_ip);
  Int_t   bveto_mu;     tree->SetBranchAddress("bveto_mu"    , &bveto_mu);
  Int_t   bveto_nj30;   tree->SetBranchAddress("bveto_nj30"  , &bveto_nj30);
  Int_t   dphiveto;     tree->SetBranchAddress("dphiveto"    , &dphiveto);
  Int_t   sameflav;     tree->SetBranchAddress("sameflav"    , &sameflav);
  Int_t   zveto;        tree->SetBranchAddress("zveto"       , &zveto);
  UInt_t  event;        tree->SetBranchAddress("event"       , &event);
  UInt_t  lumi;         tree->SetBranchAddress("lumi"        , &lumi);
  UInt_t  run;          tree->SetBranchAddress("run"         , &run);

  Int_t pass2012ICHEP1;  tree->SetBranchAddress("pass2012ICHEP1", &pass2012ICHEP1);
  Int_t pass2012ICHEP2;  tree->SetBranchAddress("pass2012ICHEP2", &pass2012ICHEP2);
  
    Float_t dymva1;          tree->SetBranchAddress("dymva1"         , &dymva1);

  

    Float_t nllW = 1; 
    if ( theSample.Contains("_nnll") && jetChannel == 0)
      tree->SetBranchAddress("nllW", &nllW);

    Float_t fakeW;

  if (theSample.Contains("WJetsFakes"))
    tree->SetBranchAddress("fakeW", &fakeW);

  Float_t puW;
  
  if (!theSample.Contains("WJetsFakes") && !theSample.Contains("Data"))
    tree->SetBranchAddress("puW", &puW);


  // Set the channel
  //----------------------------------------------------------------------------
  Float_t SelectedChannel = -999;

  if      (flavorChannel == "MuMu") SelectedChannel =  0;
  else if (flavorChannel == "EE"  ) SelectedChannel =  1;
  else if (flavorChannel == "EMu" ) SelectedChannel =  2;
  else if (flavorChannel == "MuE" ) SelectedChannel =  3;
  else if (flavorChannel == "OF" )  SelectedChannel =  4;
  else if (flavorChannel == "SF" )  SelectedChannel =  5;
  
  //  else if (flavorChannel == "All" ) SelectedChannel = -1;


  //----------------------------------------------------------------------------
  // Loop
  //----------------------------------------------------------------------------
  for (int ievent=0; ievent<tree->GetEntries(); ievent++) {
    
    tree->GetEntry(ievent);
    
    Double_t efficiencyW = effW * triggW;
    Double_t totalW      = -999;

    
    Double_t mybaseW = 5984.0/999864; 
    //5812.3/539594; // mcnlo 
    //5812.3/1933235; // madgraph (1933232)
    // 5812.3/999864; // powheg (999860)


    if (theSample.Contains("Data"))
      totalW = 1.0;

    else if (theSample.Contains("WJetsFakes")) { 
      totalW = fakeW;
     
      if ( dataset >= 82 && dataset <= 85 ) 
	totalW  = fakeW * (1 + 0.5 * (dataset >= 82 && dataset <= 84)) * baseW * puW * effW * triggW * luminosity;
    
    }

    else if (theSample.Contains("_nnll") ) {
      efficiencyW = puW * effW * triggW *nllW;
      totalW      = (1 + 0.5 * (dataset >= 82 && dataset <= 84)) * mybaseW * efficiencyW * luminosity;
    } 

    else {
      efficiencyW = puW * effW * triggW ;
      totalW      = (1 + 0.5 * (dataset >= 82 && dataset <= 84)) * baseW * efficiencyW * luminosity;
    }

    //cout <<mybaseW << endl;

    //if (theSample.Contains("DYtautau") && dataset != 86) totalW *= (19.4/12.1);


    // Help variables
    //--------------------------------------------------------------------------
    //   Int_t dphiv = ((njet <= 1 && dphiveto) || (njet > 1 && dphilljetjet < 165.*TMath::DegToRad()));

    Int_t dphiv = (njet <= 1 || (njet > 1 && dphilljetjet < 165.*TMath::DegToRad()));

    Float_t metvar = (njet <= 1) ? mpmet : pfmet;

    Float_t jetbin = njet;

    Float_t dyMVA = ( !sameflav || ( (njet!=0 || dymva1>0.88) && (njet!=1 || dymva1>0.84) && ( njet==0 || njet==1 || (pfmet > 45.0)) ) );

	    
    if (njet == 3) jetbin = 2;  


    Int_t vbfsel = ((jetpt3 <= 30 || !(jetpt3 > 30 && ((jeteta1-jeteta3 > 0 && jeteta2-jeteta3 < 0) || (jeteta2-jeteta3 > 0 && jeteta1-jeteta3 < 0)))));


    // The selection begins here
    //--------------------------------------------------------------------------
    if (theSample == "DY" && mctruth == 2 && (flavorChannel == "EMu" || flavorChannel == "MuE")) continue;
    if (run == 201191) continue;
    if (trigger != 1) continue;
    if (pt2 <= 20) continue; // increase the pt of the leptons to further reduce Wjets 
    if (pt1 <= 20) continue; // increase the pt of the leptons to further reduce Wjets 
    if (ch1*ch2 > 0) continue;

    if (dataset == 86 && (flavorChannel == "MuMu" || flavorChannel == "EE")) continue; 

    if ((SelectedChannel == -1) || (channel == SelectedChannel)    || 
	(SelectedChannel == 4 && (channel == 2 || channel == 3) )  || 
	(SelectedChannel == 5 && (channel == 0 || channel == 1) ) ) {



      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //
      // Data-driven methods
      //
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // if (pfmet > 20 && mpmet > 20 && mll > 12 && ptll > 45 && nextra == 0 && (dphiv || !sameflav)) { OLD CUTS
	
      // if( pfmet > 20 && mpmet > 20 && mll > 12&& nextra == 0 && (dphiv || !sameflav) && dyMVA && ptll>30 && (!sameflav || ptll>45) ) { 

      if( pfmet > 20 && mpmet > 20 && mll > 12&& nextra == 0 && (dphiv || !sameflav)  && ptll>30 && (!sameflav || ptll>45) ) { 



	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	// Z+jets
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//if (dphiv && jetbin == jetChannel && bveto_mu && (bveto_ip && (nbjettche == 0 || njet > 3))) { OLD CUTS

	if( jetbin == jetChannel && bveto_mu && bveto_ip==1 && nbjettche==0 && ptll > 45) { 


	  // Loop over the metvar bins
	  //--------------------------------------------------------------------
	  for (size_t mc=0; mc<numberDYMVACuts; mc ++) {
	    
	    if ( jetbin == 0 ) { 

	      if ( dymva1 > DYMVACut_0j[mc] && fabs(mll - ZMASS) < 7.5) {
		hNinLooseZevents[mc]->Fill(1,totalW);
	      }

	      if (dymva1 > DYMVACut_0j[mc] && dymva1 < DYMVACut_0j[mc+1]) {   
		if (fabs(mll - ZMASS) < 7.5) {
		  hNinZevents[mc]   ->Fill(  1, totalW);
		  hMassInZevents[mc]->Fill(mll, totalW);
		}
		else if (fabs(mll - ZMASS) > 15) {  
		  hNoutZevents[mc]   ->Fill(  1, totalW);
		  hMassOutZevents[mc]->Fill(mll, totalW);
		}
	      }
	    }

	    if ( jetbin == 1 ) { 

	      if ( dymva1 > DYMVACut_1j[mc] && fabs(mll - ZMASS) < 7.5) {
		hNinLooseZevents[mc]->Fill(1,totalW);
	      }

	      if (dymva1 > DYMVACut_1j[mc] && dymva1 < DYMVACut_1j[mc+1]) {   
		if (fabs(mll - ZMASS) < 7.5) {
		  hNinZevents[mc]   ->Fill(  1, totalW);
		  hMassInZevents[mc]->Fill(mll, totalW);
		}
		else if (fabs(mll - ZMASS) > 15) {  
		  hNoutZevents[mc]   ->Fill(  1, totalW);
		  hMassOutZevents[mc]->Fill(mll, totalW);
		}
	      }
	    }


	  }
	}
      
      


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	// Top
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//   if (zveto  && metvar > (20 + 25*sameflav)) { OLD CUTS

	if ( zveto && dyMVA ) {

	  
	  // btag_eff denominator
	  if ((jetChannel == 0 && njet == 1 && nbjet == 1) || // jetBProbabilityBJetTags discriminator
	      (jetChannel == 1 && njet == 2 && jettche2 > 2.1) ||
	      (jetChannel == 2)) {

	    hNTopControlRegion->Fill(1, totalW);
	    hbTagDisNTopControlRegion->Fill(jettche2, totalW);
	    
	    // btag_eff numerator
	    if ((jetChannel == 0 && !bveto_nj30) || // bveto_nj30 == tche>2.1 && jetPt[10,30] && softMuonVeto for jetPt<30 
		(jetChannel == 1 && jettche1 > 2.1) ||
		(jetChannel == 2)) {
	      
	      hNTopTaggedTopControlRegion->Fill(1, totalW);
	      hbTagDisNTopTaggedTopControlRegion->Fill(jettche2, totalW);
	    }
	  }
	  
	}

	// Top-tagged events for ttbar estimation
	//----------------------------------------------------------------------
	if (zveto && dyMVA) {

	  if ((jetChannel == 0 && njet == 0 && !bveto) ||
	      (jetChannel == 1 && njet == 1 && bveto && jettche1 > 2.1) || // bveto == bveto_nj30 == tche>2.1 && jetPt[10,30] && softMuonVeto (no pt requirement)
	      (jetChannel == 2)) {
	    
	    hTopTaggedEvents->Fill(1, totalW);
	    hbTagDisTopTaggedEvents->Fill(jettche2, totalW);
	  }
	}
      }


      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //
      // Main analisis
      //
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


  

      hWTrigger   ->Fill(1, totalW); 
      hWeffTrigger->Fill(1, efficiencyW);



	  hPtLepton1TwoLeptonsLevel      ->Fill(pt1,       totalW);
	  hPtLepton2TwoLeptonsLevel      ->Fill(pt2,       totalW);
	  hPtDiLeptonTwoLeptonsLevel     ->Fill(ptll,      totalW);
	  hMinvTwoLeptonsLevel           ->Fill(mll,       totalW);
	  hMtTwoLeptonsLevel             ->Fill(mth,       totalW);
	  hNJets30TwoLeptonsLevel        ->Fill(njet,      totalW);
	  hpfMetTwoLeptonsLevel          ->Fill(pfmet,     totalW);
	  hppfMetTwoLeptonsLevel         ->Fill(ppfmet,    totalW);
	  hchMetTwoLeptonsLevel          ->Fill(chmet,     totalW);
	  hpchMetTwoLeptonsLevel         ->Fill(pchmet,    totalW);
	  hpminMetTwoLeptonsLevel        ->Fill(mpmet,     totalW);
	  hDeltaRLeptonsTwoLeptonsLevel  ->Fill(drll,      totalW);
	  hDeltaPhiLeptonsTwoLeptonsLevel->Fill(dphill,    totalW);
	  hDPhiPtllJetTwoLeptonsLevel    ->Fill(dphilljet, totalW);



	  //if ( (pass2012ICHEP1 && !pass2012ICHEP2) || (!pass2012ICHEP1 && pass2012ICHEP2))    
	    h_TwoLeptons_TightFailEvents ->Fill(1, totalW); 
	  
	  if ( pass2012ICHEP1 && pass2012ICHEP2) 
	    h_TwoLeptons_TightTightEvents ->Fill(1, totalW); 
	  

	  if ( pass2012ICHEP1 || pass2012ICHEP2) 
	    h_TwoLeptons_TightLooseEvents ->Fill(1, totalW); 
	


    if (nextra == 0) {
			
	    hWExtraLepton->Fill(1, totalW);
	    hWeffExtraLepton->Fill(1, efficiencyW);
      
	    if (pfmet > 20 ) { // removed for differential xsec
	   
	      hWMetCut->Fill(1, totalW);
	      hWeffMetCut->Fill(1, efficiencyW);
	
	      if (mll > 12) {
	    
		hWLowMinv->Fill(1, totalW);
		hWeffLowMinv->Fill(1, efficiencyW);

		if (zveto==1 || !sameflav) {
		  
		  hWZVeto->Fill(1, totalW); 
		  hWeffZVeto->Fill(1, efficiencyW); 

		  if (mpmet > 20  && dyMVA ) { 

		
		    hWpMetCut->Fill(1, totalW);
		    hWeffpMetCut->Fill(1, efficiencyW);		  
		    

		    if (dphiv || !sameflav) {


		      hWDeltaPhiJet->Fill(1, totalW);
		      hWeffDeltaPhiJet->Fill(1, efficiencyW);


		      if (bveto_mu) { //--> third soft lepton with pt> 3GeV 
		      
			hWSoftMuVeto->Fill(1, totalW); 
			hWeffSoftMuVeto->Fill(1, efficiencyW);
		     

			 
			if ( ptll>30 && (!sameflav || ptll>45) ) {
			    
			  hWPtll->Fill(1, totalW);
			  hWeffPtll->Fill(1, efficiencyW);
			  

			  if (jetbin == jetChannel) {
			    
			    hWJetVeto->Fill(1, totalW);
			    hWeffJetVeto->Fill(1, efficiencyW);
			    

			    if (bveto_ip==1 &&  nbjettche==0) {
			      
			      hWTopTagging->Fill(1, totalW);
			      hWeffTopTagging->Fill(1, efficiencyW);

		       
			
			  
			  hPtLepton1WWLevel      ->Fill(pt1,       totalW);
			  hPtLepton2WWLevel      ->Fill(pt2,       totalW);
			  hPtDiLeptonWWLevel     ->Fill(ptll,      totalW);
			  hMinvWWLevel           ->Fill(mll,       totalW);
			  hMtWWLevel             ->Fill(mth,       totalW);
			  hNJets30WWLevel        ->Fill(njet,      totalW);
			  hpfMetWWLevel          ->Fill(pfmet,     totalW);
			  hppfMetWWLevel         ->Fill(ppfmet,    totalW);
			  hchMetWWLevel          ->Fill(chmet,     totalW);
			  hpchMetWWLevel         ->Fill(pchmet,    totalW);
			  hpminMetWWLevel        ->Fill(mpmet,     totalW);
			  hDeltaRLeptonsWWLevel  ->Fill(drll,      totalW);
			  hDeltaPhiLeptonsWWLevel->Fill(dphill,    totalW);
			  hDPhiPtllJetWWLevel    ->Fill(dphilljet, totalW);

			  hPtLepton1WWLevel_Diff  ->Fill(pt1,       totalW);
			  hEtaLepton1WWLevel_Diff ->Fill(eta1,      totalW);
			  hDileptonWWLevel_Diff   ->Fill(ptll,      totalW);
			  hMinvWWLevel_Diff       ->Fill(mll,       totalW);
			  hDeltaPhiWWLevel_Diff   ->Fill(dphill,    totalW);


			  //if ( (pass2012ICHEP1 && !pass2012ICHEP2) || (!pass2012ICHEP1 && pass2012ICHEP2))    
			    h_WWLevel_TightFailEvents ->Fill(1, totalW); 
			  
			   if ( (pass2012ICHEP1 && pass2012ICHEP2)) 
			     h_WWLevel_TightTightEvents ->Fill(1, totalW); 
			  
			    if ( pass2012ICHEP1 || pass2012ICHEP2) 
			      h_WWLevel_TightLooseEvents ->Fill(1, totalW); 
	


			}
			  }
		      }		      
		    }
		  }
		}
	      }
	    }
	  }
	}
      
      
  }
  
}
	  
  verbose = true;

  // Print
  //----------------------------------------------------------------------------
  if (verbose) {

    float norm = 1933235;

    /*
    cout << "Dilepton Level TF  " <<  h_TwoLeptons_TightFailEvents   ->GetEntries() << endl;
    cout << "Dilepton Level TT  " <<  h_TwoLeptons_TightTightEvents  ->GetEntries() << endl;
    cout << "Dilepton Level TL  " <<  h_TwoLeptons_TightLooseEvents  ->GetEntries() << endl;

    cout << "Dilepton Level TF  " <<  h_TwoLeptons_TightFailEvents   ->GetSumOfWeights() << endl;
    cout << "Dilepton Level TT  " <<  h_TwoLeptons_TightTightEvents  ->GetSumOfWeights() << endl;
    cout << "Dilepton Level TL  " <<  h_TwoLeptons_TightLooseEvents  ->GetSumOfWeights() << endl;


    cout << "WW Level TF  " <<  h_WWLevel_TightFailEvents   ->GetEntries() << endl;
    cout << "WW Level TT  " <<  h_WWLevel_TightTightEvents  ->GetEntries() << endl;
    cout << "WW Level TL  " <<  h_WWLevel_TightLooseEvents  ->GetEntries() << endl;

    cout << "WW Level TF  " <<  h_WWLevel_TightFailEvents   ->GetSumOfWeights() << endl;
    cout << "WW Level TT  " <<  h_WWLevel_TightTightEvents  ->GetSumOfWeights() << endl;
    cout << "WW Level TL  " <<  h_WWLevel_TightLooseEvents  ->GetSumOfWeights() << endl;

    */

    cout << endl;
    cout << " Expected number of RAW events for " << theSample.Data() << endl;
    cout << " ------------------+-----------" << endl;
    cout << " trigger           | " << hWTrigger    ->GetEntries() << endl;
    cout << " extra lepton veto | " << hWExtraLepton->GetEntries() << endl;
    cout << " MET cut           | " << hWMetCut     ->GetEntries() << endl;
    cout << " low minv cut      | " << hWLowMinv    ->GetEntries() << endl;
    cout << " Z veto            | " << hWZVeto      ->GetEntries() << endl;
    cout << " projected MET cut | " << hWpMetCut    ->GetEntries() << endl;
    cout << " DeltaPhiJet veto  | " << hWDeltaPhiJet->GetEntries() << endl;    
    cout << " soft muon veto    | " << hWSoftMuVeto ->GetEntries() << endl;
    cout << " ptll cut          | " << hWPtll       ->GetEntries() << endl;
    cout << " jet veto          | " << hWJetVeto    ->GetEntries() << endl;
    cout << " top tagging       | " << hWTopTagging ->GetEntries() << endl; 


    /*
   
    cout << hWTrigger    ->GetEntries() << endl;
    cout << hWExtraLepton->GetEntries() << endl;
    cout << hWMetCut     ->GetEntries() << endl;
    cout << hWLowMinv    ->GetEntries() << endl;
    cout << hWZVeto      ->GetEntries() << endl;
    cout << hWpMetCut    ->GetEntries() << endl;
    cout << hWDeltaPhiJet->GetEntries() << endl;    
    cout << hWSoftMuVeto ->GetEntries() << endl;
    cout << hWTopTagging ->GetEntries() << endl;
    cout << hWPtll       ->GetEntries() << endl;
    cout << hWJetVeto    ->GetEntries() << endl;
    


    
    cout << endl;
    */

    /*
    
    if (!theSample.Contains("Data")) {
      cout << endl;
      cout << " Normalized to " << luminosity << " 1/fb" << endl;
      cout << " ------------------+-----------" << endl;
      cout << " trigger           | " << hWTrigger    ->GetSumOfWeights() << endl;
      cout << " extra lepton veto | " << hWExtraLepton->GetSumOfWeights() << endl;
      cout << " MET cut           | " << hWMetCut     ->GetSumOfWeights() << endl;
      cout << " low minv cut      | " << hWLowMinv    ->GetSumOfWeights() << endl;
      cout << " Z veto            | " << hWZVeto      ->GetSumOfWeights() << endl;
      cout << " projected MET cut | " << hWpMetCut    ->GetSumOfWeights() << endl;
      cout << " DeltaPhiJet veto  | " << hWDeltaPhiJet->GetSumOfWeights() << endl;     
      cout << " soft muon veto    | " << hWSoftMuVeto ->GetSumOfWeights() << endl;
      cout << " jet veto          | " << hWJetVeto    ->GetSumOfWeights() << endl;
      cout << " top tagging       | " << hWTopTagging ->GetSumOfWeights() << endl; 

      cout << endl;

          }
 
    */


    //cout << hWTopTagging->GetBinContent(2) << " +- " << hWTopTagging->GetBinError(2) << endl;	 
 }


  // Save the histograms
  //----------------------------------------------------------------------------
  output->cd();
  output->Write("", TObject::kOverwrite);
  output->Close();
  
}
