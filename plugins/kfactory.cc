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

  rechitTag = iConfig.getUntrackedParameter<edm::InputTag>("rechitTag", edm::InputTag("hbhereco"));
  rechitTok = consumes<HBHERecHitCollection>(rechitTag);

  ktree = fileservice->make<TTree>("ktree","ktree");  
}


//
// member functions
//

// ------------ method called for each event  ------------
void kfactory::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;

  Handle<HBHERecHitCollection> rechitCollection;
  if (!(iEvent.getByToken(rechitTok, rechitCollection))) {
    std::cout << "Error: rechit collection not available." << std::endl;
    exit(1);
  }
  kenergies .clear();
  ktimes    .clear();
  kietas    .clear();
  kiphis    .clear();
  kdepths   .clear();
  std::cout << "There are " << rechitCollection->size() << " rechits in this event " << std::endl; 
  for (HBHERecHitCollection::const_iterator rechit=rechitCollection->begin(); rechit<rechitCollection->end(); rechit++) {
    kenergies .push_back( rechit ->  energy()     ) ;
    ktimes    .push_back( rechit ->  time()       ) ;
    kietas    .push_back( rechit ->  id().ieta()  ) ;
    kiphis    .push_back( rechit ->  id().iphi()  ) ;
    kdepths   .push_back( rechit ->  id().depth() ) ;
    if (debugFlag) {
      std::cout << "  Rechit number " << iRechit              << " has:" << std::endl;
      std::cout << "      energy = "  << rechit->energy()     << std::endl;
      std::cout << "      time   = "  << rechit->time()       << std::endl;
      std::cout << "      ieta   = "  << rechit->id().ieta()  << std::endl;
      std::cout << "      iphi   = "  << rechit->id().iphi()  << std::endl;
      std::cout << "      depth  = "  << rechit->id().depth() << std::endl;
    }
    ++iRechit;
  }
  ktree->Fill();
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
