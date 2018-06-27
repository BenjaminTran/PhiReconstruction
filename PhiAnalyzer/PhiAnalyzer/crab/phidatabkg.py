import os
from WMCore.Configuration import Configuration
config = Configuration()

try:
    with open( 'PhiVarStore.dat', 'r' ) as fle:
        counter = int( fle.readline() )
except FileNotFoundError:
    counter = 0

config.section_("General")
config.General.workArea = 'crab_dir/Phi_BDTDataBkg_pPb_JL' + str(counter)
config.General.requestName = 'PhiBDTDataBkg_2016pPb_JL' + str(counter)

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
#config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = os.path.expandvars('$CMSSW_BASE/src/PhiAnalyzer/PhiAnalyzer/python/phikaontree_cfg.py')

config.section_("Data")
config.Data.inputDBS = 'phys03'
config.Data.ignoreLocality = True
#config.Data.primaryDataset = 'MinBias_TuneZ2star_7TeV_pythia6'
config.Data.inputDataset = '/PAHighMultiplicity2/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.totalUnits = 500000
config.Data.unitsPerJob = 50000
config.Data.outLFNDirBase = '/store/group/phys_heavyions/btran/Phi/BDT'
config.Data.useParent = True
config.Data.publication = False
config.Data.publishDBS = 'phys03'
config.Data.outputDatasetTag = 'PhiBDTDataBkg_JL' + str(counter)

config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
#config.Site.whitelist = ['T2_US_MIT']
config.Site.whitelist = ['T2_US_Vanderbilt']

with open( 'PhiVarStore.dat', 'w' ) as fle:
    counter = counter + 1
    fle.write( str(counter) )

