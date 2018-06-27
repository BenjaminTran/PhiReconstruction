from PhiAnalyzer.PhiAnalyzer.phianalysis_cfg import process, cms

process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1) )

#process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(500))

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
            'root://cmsxrootd.fnal.gov//store/user/davidlw/PAHighMultiplicity1/RecoSkim2016_pPb_V0Cascade_FullSkim_v4/170803_222621/0000/pPb_HM_117.root'
#            ),
#        secondaryFileNames = cms.untracked.vstring(
            )
        )

process.TFileService = cms.Service("TFileService",
        fileName = cms.string('PhiBDT_v1.root')
        )

process.test = cms.Sequence(process.BDTApp)

process.p = cms.Path(process.test)
