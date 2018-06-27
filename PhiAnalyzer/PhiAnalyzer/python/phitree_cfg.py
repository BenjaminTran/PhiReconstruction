from PhiAnalyzer.PhiAnalyzer.phianalysis_cfg import process, cms

# process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1) )

#process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10000))

process.TFileService = cms.Service("TFileService",
        fileName = cms.string('Phi_Tree_v3.root')
        )

process.test = cms.Sequence(process.PhiTree)

process.p = cms.Path(process.test)
