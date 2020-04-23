import FWCore.ParameterSet.Config as cms


CosmicAnalyzer = cms.EDAnalyzer('CosmicAnalyzer',
    nameOfOutput = cms.string('tree.root'),
    MuonCollection = cms.InputTag('splitMuons')
)

