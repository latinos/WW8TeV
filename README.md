WW8TeV
======

WW cross section at 8TeV


Code updated to include the dymva and some small modifications on the systematics propagation.

 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 # 0. Get the code 
 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 cvs co -d WW8TeV calderona/WW8TeV

 cd WW8TeV

 cvs update -dP

 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 # 1. Produce the histograms  
 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 ./run.sh 0

./run.sh 1

./run.sh 2

./inclusive.sh

 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 # 2. Print DY scale, top scale and WW cross section 
 # 
 # njet = {0, 1, 2, 3} 
 # 
 # ichannel = {-1, All, SF, OF, MuMu, EE, EMu, MuE} 
 # 
 # luminosity = 19468 # # printLevel = {SILENT, NOTE, DEBUG} 
 # 
 # drawTheXS = false 
 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 root -l runXS.C

 # To compute the WW cross section removing the "XYZ" cut
 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 root -l -b -q runXS.C(0,All,19468,\"XYZ\",1)

 # To compute the WW cross section at "XYZ" cut level
 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 root -l -b -q runXS.C(0,All,19468,\"XYZ\",0)

 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 # 3. Draw distributions  
 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 root -l drawDistributions.
