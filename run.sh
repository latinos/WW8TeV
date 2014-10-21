if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./run.sh NJETS"
    echo "  "
    exit -1
fi

FILEPATH="root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/amassiro/RunI/trees/tree_skim_wwmin_09Jan2014/";

LUMINOSITY=19.468


NJETS=$1

SYSTEMATICS="       \
nominals            \
JER_down            \
JER_up              \
electronResolution  \
electronScale_down  \
electronScale_up    \
jetEnergyScale_down \
jetEnergyScale_up   \
metResolution       \
metScale_down       \
metScale_up         \
muonResolution      \
muonScale_down      \
muonScale_up        \
"

CHANNELS="EMu MuE"
#EE MuE EMu MuMu "

MERGES="DF"
# DF SF All


SAMPLES="          \
DataRun2012_Total  \
ggWWto2L           \
WWTo2L2Nu          \
WWTo2L2Nu_pow      \
WWTo2L2Nu_mcnlo    \
GamGamWW           \
WZ                 \
ZZ                 \
TTbar              \
TW                 \
DY                 \
WgammaStar         \
WgammaNoStar       \
DYtautau           \
Zgamma             \
HWW125             \
VVV                \
WJets              \
WJetsFakes_Total   \
"

MERGELIST=" \
$SAMPLES    \
WW          \
WW_pow      \
WW_mc       \
Wgamma      \
"


#rm -rf rootfiles/${NJETS}jet

# Loop

for SYSTEMATIC in $SYSTEMATICS; do

    for CHANNEL in $CHANNELS; do
	
	for SAMPLE in $SAMPLES; do 
	
	    root -l -b -q "runLatinosTreeScript.C(\"$FILEPATH\",$LUMINOSITY,$NJETS,\"$CHANNEL\",\"$SAMPLE\",\"$SYSTEMATIC\")"
  
	done

	OUTPATH=rootfiles/${NJETS}jet/${SYSTEMATIC}/${CHANNEL}

        # Merge some MC files
	hadd ${OUTPATH}/WW.root     ${OUTPATH}/ggWWto2L.root     ${OUTPATH}/WWTo2L2Nu.root 
	hadd ${OUTPATH}/WW_pow.root     ${OUTPATH}/ggWWto2L.root     ${OUTPATH}/WWTo2L2Nu_pow.root 
	hadd ${OUTPATH}/WW_mc.root     ${OUTPATH}/ggWWto2L.root     ${OUTPATH}/WWTo2L2Nu_mcnlo.root
	hadd ${OUTPATH}/Wgamma.root ${OUTPATH}/WgammaNoStar.root ${OUTPATH}/WgammaStar.root ${OUTPATH}/GamGamWW.root 
	
    done

    MERGEPATH=rootfiles/${NJETS}jet/${SYSTEMATIC}/
    for MERGE in $MERGES; do
	mkdir ${MERGEPATH}/${MERGE}/
	MERGESRC=""
	[ $MERGE == "DF" ] && MERGESRC="EMu MuE"
	[ $MERGE == "SF" ] && MERGESRC="EE MuMu"
	[ $MERGE == "All" ] && MERGESRC="EMu MuE EE MuMu"
	for MSAMPLE in $MERGELIST; do
	    MFILES=""
	    for SRC in $MERGESRC; do
		MFILES="$MFILES ${MERGEPATH}${SRC}/${MSAMPLE}.root"
	    done
	    hadd ${MERGEPATH}/${MERGE}/${MSAMPLE}.root $MFILES
	done
    done

done


