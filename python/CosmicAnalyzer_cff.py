import FWCore.ParameterSet.Config as cms


CosmicAnalyzer = cms.EDAnalyzer('CosmicAnalyzer',
    nameOfOutput = cms.string('tree.root'),
    MuonCollectionSplit = cms.InputTag('splitMuons'),
    MuonCollectionOneLeg = cms.InputTag('muons1Leg')
)

