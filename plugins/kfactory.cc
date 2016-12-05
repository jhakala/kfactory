// John Hakala
// October 28, 2016

#include "hcalcalib/kfactory/interface/kfactory.h"

//
// constants, enums and typedefs
//

//
// static data member definitions


//
// constructors and destructor
//
kfactory::kfactory(const edm::ParameterSet& iConfig) {
  //now do what ever initialization is needed
  usesResource("TFileService");

  debugFlag = iConfig.getUntrackedParameter<bool>("debugFlag");
  doTriggerResults = false;

  rechitTag = iConfig.getUntrackedParameter<edm::InputTag>("rechitTag", edm::InputTag("hbhereco"));
  rechitTok = consumes<HBHERecHitCollection>(rechitTag);

  if (doTriggerResults) {
    triggerResTag = iConfig.getUntrackedParameter<edm::InputTag>("triggerResTag", edm::InputTag("TriggerResults"));
    triggerResTok = consumes<edm::TriggerResults>(triggerResTag);
  }

  triggerEventTag = iConfig.getUntrackedParameter<edm::InputTag>("triggerEventTag", edm::InputTag("hltTriggerSummaryAOD"));
  triggerEventTok = consumes<trigger::TriggerEvent>(triggerEventTag);

  //triggerName = "hltL1sSingleMu22";
  ktree = fileservice->make<TTree>("ktree","ktree");  
}


//
// member functions
//

// ------------ method called for each event  ------------
void kfactory::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;

  //hasBigMuon = false;

  Handle<HBHERecHitCollection> rechitCollection;
  if (!(iEvent.getByToken(rechitTok, rechitCollection))) {
    std::cout << "Error: rechit collection not available." << std::endl;
    exit(1);
  }
  Handle<trigger::TriggerEvent> triggerEvent;
  if (!(iEvent.getByToken(triggerEventTok, triggerEvent)) ) {
    std::cout << "Error: trigger event collection not available or not valid." << std::endl;
    exit(1);
  }

  if ( triggerEvent.isValid() ) {
    if (doTriggerResults ) {
      Handle<edm::TriggerResults> triggerResults;
      const edm::TriggerNames& triggerNames = iEvent.triggerNames(*triggerResults);
      if (!(iEvent.getByToken(triggerResTok, triggerResults)) || !triggerResults.isValid()) {
        std::cout << "Error: trigger results collection not available." << std::endl;
        exit(1);
      }
      if (debugFlag) std::cout << "This event has the following trigger results available: " << std::endl;
      for (int iTriggerRes = 0; iTriggerRes < (int) triggerResults->size(); ++iTriggerRes) {
        if (debugFlag) {
          std::cout << "    " << triggerNames.triggerName(iTriggerRes) << std::endl;
        }
      }
    }

    if (debugFlag) std::cout << "This event has the following trigger event info available: " << std::endl;
    const trigger::TriggerObjectCollection & triggerObjects = triggerEvent -> getObjects();
    //size_t nFilters       = triggerEvent -> sizeFilters();
    //size_t nFiltersPassed = 0;
    //size_t iFilter        = 0;

    //for (; iFilter < nFilters; ++iFilter) {
    //  std::string          name = triggerEvent -> filterTag ( iFilter ).label();
    //  //if ( name != triggerName ) continue;
    //  const trigger::Keys& keys = triggerEvent -> filterKeys( iFilter );
    //  const trigger::Vids& vids = triggerEvent -> filterIds ( iFilter );
    //  int nKeys = (int) keys.size();
    //  int nVids = (int) vids.size();
    //  assert ( nKeys == nVids ) ;
    //  if (debugFlag) std::cout << "Checking filter with name: " << name << std::endl;
    //  for (int iTriggerObject = 0; iTriggerObject < nKeys; ++iTriggerObject ) { 
    //    int                id  = vids[iTriggerObject];
    //    trigger::size_type key = keys[iTriggerObject];
    //    const trigger::TriggerObject & triggerObject = triggerObjects [key];
    //    if (debugFlag) {
    //      std::cout << "  triggerObject " << iTriggerObject << " has id   = " << id                   << std::endl;
    //      std::cout << "  triggerObject " << iTriggerObject << " has pt   = " << triggerObject.pt  () << std::endl;
    //      std::cout << "  triggerObject " << iTriggerObject << " has eta  = " << triggerObject.eta () << std::endl;
    //      std::cout << "  triggerObject " << iTriggerObject << " has phi  = " << triggerObject.phi () << std::endl;
    //      std::cout << "  triggerObject " << iTriggerObject << " has mass = " << triggerObject.mass() << std::endl; 
    //    }
    //    if (triggerObject.pt() > 30) { hasBigMuon = true; }
    //  }
    //}

    size_t nObjects       = triggerEvent -> sizeObjects();
    size_t iObject        = 0;
    
    for (; iObject < nObjects; ++iObject) {
      const trigger::TriggerObject & triggerObject = triggerObjects [iObject];
        if (triggerObject.pt() < 1) continue;
        if (debugFlag) {
          std::cout << "  triggerObject " << iObject << " has id     = " << triggerObject.id      () << std::endl;
          std::cout << "  triggerObject " << iObject << " has energy = " << triggerObject.energy  () << std::endl;
          std::cout << "  triggerObject " << iObject << " has et     = " << triggerObject.et      () << std::endl;
          std::cout << "  triggerObject " << iObject << " has pt     = " << triggerObject.pt      () << std::endl;
          std::cout << "  triggerObject " << iObject << " has eta    = " << triggerObject.eta     () << std::endl;
          std::cout << "  triggerObject " << iObject << " has phi    = " << triggerObject.phi     () << std::endl;
          std::cout << "  triggerObject " << iObject << " has mass   = " << triggerObject.mass    () << std::endl; 
        }
      //if (triggerObject.pt() > 30) { hasBigMuon = true; }
    }
  }
    
  //if (hasBigMuon) { 

    if (debugFlag) { 
      iRechit     = 0;
    }
    nBigRechits = 0;
    kenergies .clear();
    ktimes    .clear();
    kietas    .clear();
    kiphis    .clear();
    kdepths   .clear();
    // Find all big rechits
    for (HBHERecHitCollection::const_iterator rechit=rechitCollection->begin(); rechit<rechitCollection->end(); rechit++) {
      if (debugFlag) { 
        ++iRechit;
      }
      if (rechit->energy() > 20) {
        ++nBigRechits;
        kenergies .push_back( rechit ->  energy()     ) ;
        ktimes    .push_back( rechit ->  time()       ) ;
        kietas    .push_back( rechit ->  id().ieta()  ) ;
        kiphis    .push_back( rechit ->  id().iphi()  ) ;
        kdepths   .push_back( rechit ->  id().depth() ) ;
        if (debugFlag) {
          std::cout << " Big Rechit number " << nBigRechits << " is rechit number " << iRechit              << " and has:" << std::endl;
          std::cout << "      energy = "  << rechit->energy()     << std::endl;
          std::cout << "      time   = "  << rechit->time()       << std::endl;
          std::cout << "      ieta   = "  << rechit->id().ieta()  << std::endl;
          std::cout << "      iphi   = "  << rechit->id().iphi()  << std::endl;
          std::cout << "      depth  = "  << rechit->id().depth() << std::endl;
        }
      }
    }
    if (debugFlag) { 
      iRechit     = 0;
    }
    // Find all neighboring rechits to big ones
    if (nBigRechits>0) {
      for(std::size_t iBigRechit = 0; iBigRechit != nBigRechits; ++iBigRechit) {
        /* std::cout << *it; ... */
        for (HBHERecHitCollection::const_iterator rechit=rechitCollection->begin(); rechit<rechitCollection->end(); rechit++) {
          if (debugFlag) { 
            ++iRechit;
          }
          if(rechit->energy() > 0.1 && std::abs(kietas[iBigRechit]-rechit->id().ieta()) < 3 && std::abs(kiphis[iBigRechit]-rechit->id().iphi()) < 3 ) {
            alreadyThere=false;
            for(size_t i = 0; i != kietas.size(); ++i) {
              if (rechit->id().ieta() == kietas[i]) {
                if (rechit->id().iphi() == kiphis[i]) {   
                  if(rechit->id().depth() == kdepths[i]) {
                    alreadyThere=true;
                  }
                }
              }
            }
            if (alreadyThere) continue;
            kenergies .push_back( rechit ->  energy()     ) ;
            ktimes    .push_back( rechit ->  time()       ) ;
            kietas    .push_back( rechit ->  id().ieta()  ) ;
            kiphis    .push_back( rechit ->  id().iphi()  ) ;
            kdepths   .push_back( rechit ->  id().depth() ) ;
            if (debugFlag) {
              std::cout << " Neighboring rechit is rechit number " << iRechit              << " and has:" << std::endl;
              std::cout << "      energy = "  << rechit->energy()     << std::endl;
              std::cout << "      time   = "  << rechit->time()       << std::endl;
              std::cout << "      ieta   = "  << rechit->id().ieta()  << std::endl;
              std::cout << "      iphi   = "  << rechit->id().iphi()  << std::endl;
              std::cout << "      depth  = "  << rechit->id().depth() << std::endl;
            }
          }
        }
      }
      ktree->Fill();
    }
  //}
}


// ------------ method called once each job just before starting event loop  ------------
void kfactory::beginJob() {
  ktree->Branch( "kenergies" , &kenergies );
  ktree->Branch( "ktimes"    , &ktimes    );
  ktree->Branch( "kietas"    , &kietas    );
  ktree->Branch( "kiphis"    , &kiphis    );
  ktree->Branch( "kdepths"   , &kdepths   );
}

// ------------ method called once each job just after ending the event loop  ------------
void 
kfactory::endJob() {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void kfactory::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(kfactory);
