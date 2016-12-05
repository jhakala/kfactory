// John Hakala
// October 28, 2016

#include <memory>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "TTree.h"
#include "TH1D.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"


class kfactory : public edm::one::EDAnalyzer<edm::one::SharedResources>  
{
   public:
      kfactory(const edm::ParameterSet&);
      virtual ~kfactory() {}

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

  protected:
    bool debugFlag;
    bool doTriggerResults;
    //bool hasBigMuon;
    edm::Service<TFileService> fileservice;
    edm::InputTag rechitTag       ;
    edm::InputTag triggerResTag   ;
    edm::InputTag triggerEventTag ;
    std::string   triggerName     ;
    edm::EDGetTokenT<HBHERecHitCollection>  rechitTok       ;
    edm::EDGetTokenT<edm::TriggerResults>   triggerResTok   ;
    edm::EDGetTokenT<trigger::TriggerEvent> triggerEventTok ;
    unsigned short iRechit     ;
    unsigned short nBigRechits ;
    std::vector<float>   kenergies ;
    std::vector<float>   ktimes    ;
    std::vector<short>   kietas    ;
    std::vector<short>   kiphis    ;
    std::vector<short>   kdepths   ;
    TTree* ktree;
    bool alreadyThere;
};
