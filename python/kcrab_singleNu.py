from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'kfactoryTuples_singleNuDec1'
config.General.workArea = 'crab_jobs_singleNu_kfactory_Dec1'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'ConfFile_cfg.py'

config.Data.inputDataset = '/SingleNeutrino/RunIISpring16DR80-PUSpring16_RECO_NZS_80X_mcRun2_asymptotic_2016_v3-v1/GEN-SIM-RECO'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 30
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False
config.Data.outputDatasetTag = 'kfactoryTuples_singleNu_Dec1'

config.Site.storageSite = 'T3_US_Brown'
