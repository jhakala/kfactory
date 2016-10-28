// John Hakala
// October 28, 2016

#include <memory>
#include <iostream>
#include <stdlib.h>
#include <vector>

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
    edm::Service<TFileService> fileservice;
    edm::InputTag rechitTag;
    edm::EDGetTokenT<HBHERecHitCollection> rechitTok;
    int iRechit;
    std::vector<float> kenergies;
    std::vector<float> ktimes;
    std::vector<short>   kietas;
    std::vector<short>   kiphis;
    std::vector<short>   kdepths;
    TTree* ktree;
};