import FWCore.ParameterSet.Config as cms


process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load('Configuration.Geometry.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = '94X_mc2017_realistic_v12'  # or some other global tag depending on your CMSSW release and sample. 


process.load("MyAnalysis.CosmicAnalyzer.CosmicAnalyzer_cff")


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       [
            'file:/eos/cms/store/user/pablom/Cosmics/FE6AC2DD-3BE2-E511-9E12-0025904C6224.root'
       ]
    )
)

process.p = cms.Path(process.CosmicAnalyzer)


