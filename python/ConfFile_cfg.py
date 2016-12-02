import FWCore.ParameterSet.Config as cms

process = cms.Process("kfactorize")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']

debug=False;

if debug:
  process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5) )
  process.MessageLogger.cerr.FwkReport.reportEvery = 1
else: 
  process.MessageLogger.cerr.FwkReport.reportEvery = 50000
  process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


# input
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    #'file:/home/hakala/calibSamples/calibMC/singleNuTest.root'
    'file:/home/hakala/calibSamples/calibData/82C43DFD-413B-E611-A736-02163E01225C.root'
  )
)

# output
process.TFileService = cms.Service("TFileService", 
  fileName = cms.string("kfactorytree.root"),
  closeFileFast = cms.untracked.bool(True)
)

process.kfactorize = cms.EDAnalyzer('kfactory',
  debugFlag = cms.untracked.bool(debug),
  rechitTag = cms.untracked.InputTag('hbhereco')  # standard rechit collection
)

process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
process.HBHENoiseFilterResultProducer.minZeros = cms.int32(99999)

process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
  inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
  reverseDecision = cms.bool(False)
)


process.ApplyHBHEIsoNoiseFilter = cms.EDFilter('BooleanFlagFilter',
  inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHEIsoNoiseFilterResult'),
  reverseDecision = cms.bool(False)
)

process.p = cms.Path()
process.p += process.HBHENoiseFilterResultProducer 
process.p += process.kfactorize
