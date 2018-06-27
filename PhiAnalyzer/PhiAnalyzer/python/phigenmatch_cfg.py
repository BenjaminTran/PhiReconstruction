from PhiAnalyzer.PhiAnalyzer.phianalysis_cfg import process, cms

# process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1) )

#process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(5000))

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
            #'root://cmsxrootd.fnal.gov//store/himc/pPb816Summer16DR/ReggeGribovPartonMC_EposLHC_pPb_4080_4080_DataBS/AODSIM/MB_80X_mcRun2_pA_v4-v2/120000/06FE3920-490B-E711-A5AE-0CC47A01CAEA.root',
            #'root://cmsxrootd.fnal.gov//store/himc/pPb816Summer16DR/ReggeGribovPartonMC_EposLHC_pPb_4080_4080_DataBS/AODSIM/MB_80X_mcRun2_pA_v4-v2/120000/06A49EE1-1A09-E711-B8FB-842B2B5C2299.root'
            'root://cmsxrootd.fnal.gov//store/himc/pPb816Summer16DR/ReggeGribovPartonMC_EposLHC_pPb_4080_4080_DataBS/AODSIM/MB_80X_mcRun2_pA_v4-v2/120000/0A000C53-F008-E711-A26F-001E67504D15.root'
#            ),
#        secondaryFileNames = cms.untracked.vstring(
#'root://cmsxrootd.fnal.gov//store/himc/pPb816Summer16DR/ReggeGribovPartonMC_EposLHC_pPb_4080_4080_DataBS/GEN-SIM-RAW/MB_80X_mcRun2_pA_v4-v2/120001/DC711B0C-ED09-E711-92C2-002590DE6E88.root',
#'root://cmsxrootd.fnal.gov//store/himc/pPb816Summer16DR/ReggeGribovPartonMC_EposLHC_pPb_4080_4080_DataBS/GEN-SIM-RAW/MB_80X_mcRun2_pA_v4-v2/120001/E0736135-FF09-E711-AAB0-1866DA87A7E7.root'
            )
        )

process.TFileService = cms.Service("TFileService",
        fileName = cms.string('PhiGenMatch_v5.root')
        )

process.test = cms.Sequence(process.PhiGenMatch)

process.p = cms.Path(process.test)
