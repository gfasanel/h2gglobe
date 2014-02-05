#!/bin/bash

export DISPLAY=""

dir="my_syst_v1"
#mkdir $dir

## Il tune e' rimandato => questa sistematica teorica sui Tprime
##tunes="TuneD6T TuneD6TUEOFF TuneP0 TuneP0UEOFF TuneProPT0 TuneProPT0UEOFF TuneProQ20 TuneProQ20UEOFF TuneUEOFF TuneZ2Star"
## tunes="TuneP0 TuneProPT0 TuneUEOFF TuneZ2Star TuneD6T TuneProQ20"

##for tune in $tunes; do
##    rm jetanalysis/tmp_mydatafile_giuseppe_Tprime_store.dat{,.pevents}
##    ./mk_fitter.py -i jetanalysis/mydatafile_giuseppe_Tprime_store.dat -n 2 -l ${tune} -o ${dir}/Tprime_${tune}/sub &&  \
##     	./submit_fitter.py -q 8nh -d ${dir}/Tprime_${tune}
##done

## dir="yr3_systematics_v1"
## mkdir $dir

jecUncs="nominal jecUp jecDown jerCentral jerUp jerDown"

for syst in $jecUncs; do
    rm jetanalysis/tmp_mydatafile_giuseppe_tth_store_jec_btag.dat{,.pevents}
    rm -r ${dir}/tthTprimetag_${syst}/sub*
    ./mk_fitter.py -i jetanalysis/mydatafile_giuseppe_tth_store_jec_btag.dat -n 2 -l ${syst} -o ${dir}/tthTprimetag_${syst}/sub && \
	./submit_fitter.py -q 1nh -d ${dir}/tthTprimetag_${syst}
done

comment1()
{
#JER JEC
#jecUncs="jerCentral"
jecUncs="nominal jecUp jecDown jerCentral jerUp jerDown"

for syst in $jecUncs; do
    rm jetanalysis/tmp_mydatafile_giuseppe_Tprime_store_jec_btag.dat{,.pevents}
    rm -r ${dir}/Tprimetag_${syst}/sub*
    ./mk_fitter.py -i jetanalysis/mydatafile_giuseppe_Tprime_store_jec_btag.dat -n 2 -l ${syst} -o ${dir}/Tprimetag_${syst}/sub && \
	./submit_fitter.py -q 1nh -d ${dir}/Tprimetag_${syst}
done
}

comment1 ()
{
#BTAG
btagEff=" noBtagSF nominalBtagSF shiftBtagEffUp_bc shiftBtagEffDown_bc shiftBtagEffUp_l shiftBtagEffDown_l"

for systb in $btagEff; do
    rm jetanalysis/tmp_mydatafile_giuseppe_Tprime_store_jec_btag.dat{,.pevents}
    rm -r ${dir}/Tprimetag_${systb}/sub*
    ./mk_fitter.py -i jetanalysis/mydatafile_giuseppe_Tprime_store_jec_btag.dat -n 2 -l ${systb} -o ${dir}/Tprimetag_${systb}/sub && \
	./submit_fitter.py -q 1nh -d ${dir}/Tprimetag_${systb}
done
}