import FWCore.ParameterSet.Config as cms

process = cms.Process("kfactorize")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(25) )

# input
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/home/hakala/calibMC/02746755-1331-E611-B845-0025905D1D7A.root'
    )
)

# output
process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("kfactorytree.root"),
    closeFileFast = cms.untracked.bool(True)
)

process.kfactorize = cms.EDAnalyzer('kfactory',
  debugFlag = cms.untracked.bool(True),  # standard rechit collection
  rechitTag = cms.untracked.InputTag('hbhereco')  # standard rechit collection
)


process.p = cms.Path(process.kfactorize)
