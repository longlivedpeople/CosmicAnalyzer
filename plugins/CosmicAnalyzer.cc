// -*- C++ -*-
//
// Package:    MyAnalysis/CosmicAnalyzer
// Class:      CosmicAnalyzer
// 
/**\class CosmicAnalyzer CosmicAnalyzer.cc MyAnalysis/CosmicAnalyzer/plugins/CosmicAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Pablo Martinez Ruiz Del Arbol
//         Created:  Thu, 23 Apr 2020 07:22:30 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"


#include "TFile.h"


//
// class declaration
//






TFile *file_out;




// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class CosmicAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit CosmicAnalyzer(const edm::ParameterSet&);
      ~CosmicAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      std::string output_filename;
      edm::EDGetTokenT<edm::View<reco::Muon> > theMuonCollection;
      edm::ParameterSet parameters;


      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
CosmicAnalyzer::CosmicAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   usesResource("TFileService");
   theMuonCollection = consumes<edm::View<reco::Muon> >  (iConfig.getParameter<edm::InputTag>("MuonCollection"));
   parameters = iConfig;

}


CosmicAnalyzer::~CosmicAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
CosmicAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   edm::Handle<edm::View<reco::Muon> > muons;
   iEvent.getByToken(theMuonCollection, muons);

   for (auto itmuon=muons->begin(); itmuon != muons->end(); itmuon++){
       std::cout << " Pt: " << itmuon->pt() << " Phi: " << itmuon->phi() << " Eta: " << itmuon->eta() << " vx: " << itmuon->vx() << " vy: " << itmuon->vy() <<" vz: " << itmuon->vz() <<std::endl;
   }


}

// ------------ method called once each job just before starting event loop  ------------
void 
CosmicAnalyzer::beginJob()
{

    output_filename = parameters.getParameter<std::string>("nameOfOutput");
    file_out = new TFile(output_filename.c_str(), "RECREATE");
    file_out->cd();

}

// ------------ method called once each job just after ending the event loop  ------------
void 
CosmicAnalyzer::endJob() 
{

    file_out->cd();
    file_out->Close();

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CosmicAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CosmicAnalyzer);
