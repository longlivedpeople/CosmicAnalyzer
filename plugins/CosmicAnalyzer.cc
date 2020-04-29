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
#include "DataFormats/TrackReco/interface/Track.h"

#include "TFile.h"
#include "TTree.h"


//
// class declaration
//






TFile *file_out;
TTree *tree_out;


//
Int_t nSplit;
Float_t Split_pt[20];
Float_t Split_eta[20];
Float_t Split_phi[20];
Float_t Split_dxy[20];
Float_t Split_dz[20];
Float_t Split_q[20];


Int_t nOneLeg;
Float_t OneLeg_pt[20];
Float_t OneLeg_eta[20];
Float_t OneLeg_phi[20];
Float_t OneLeg_dxy[20];
Float_t OneLeg_dz[20];
Float_t OneLeg_q[20];



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
      edm::EDGetTokenT<edm::View<reco::Muon> > theMuonCollectionSplit;
      edm::EDGetTokenT<edm::View<reco::Muon> > theMuonCollectionOneLeg;
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
   theMuonCollectionSplit = consumes<edm::View<reco::Muon> >  (iConfig.getParameter<edm::InputTag>("MuonCollectionSplit"));
   theMuonCollectionOneLeg = consumes<edm::View<reco::Muon> >  (iConfig.getParameter<edm::InputTag>("MuonCollectionOneLeg"));
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

   edm::Handle<edm::View<reco::Muon> > muonsSplit;
   iEvent.getByToken(theMuonCollectionSplit, muonsSplit);
   
   edm::Handle<edm::View<reco::Muon> > muonsOneLeg;
   iEvent.getByToken(theMuonCollectionOneLeg, muonsOneLeg);

   nSplit = 0;   
   for(auto itmuon = muonsSplit->begin(); itmuon != muonsSplit->end(); itmuon++) {

       if(itmuon->pt() < 20) continue;
       Split_pt[nSplit] = itmuon->pt();
       Split_eta[nSplit] = itmuon->eta();
       Split_phi[nSplit] = itmuon->phi();
       Split_dxy[nSplit] = sqrt(itmuon->vx() *  itmuon->vx()  + itmuon->vy() *  itmuon->vy());
       Split_dz[nSplit] = itmuon->vz();
       Split_q[nSplit] = itmuon->charge();
       nSplit++;
   }



   nOneLeg = 0;   
   for(auto itmuon = muonsSplit->begin(); itmuon != muonsSplit->end(); itmuon++) {

       if(itmuon->pt() < 20) continue;
       OneLeg_pt[nOneLeg] = itmuon->pt();
       OneLeg_eta[nOneLeg] = itmuon->eta();
       OneLeg_phi[nOneLeg] = itmuon->phi();
       OneLeg_dxy[nOneLeg] = sqrt(itmuon->vx() *  itmuon->vx()  + itmuon->vy() *  itmuon->vy());
       OneLeg_dz[nOneLeg] = itmuon->vz();
       OneLeg_q[nOneLeg] = itmuon->charge();
       nOneLeg++;
   }


   tree_out->Fill();

}

// ------------ method called once each job just before starting event loop  ------------
void 
CosmicAnalyzer::beginJob()
{

    output_filename = parameters.getParameter<std::string>("nameOfOutput");
    file_out = new TFile(output_filename.c_str(), "RECREATE");
    tree_out = new TTree("Events", "Events");

    tree_out->Branch("nSplit", &nSplit, "nSplit/I");
    tree_out->Branch("Split_pt", Split_pt, "Split_pt[nSplit]/F");
    tree_out->Branch("Split_eta", Split_eta, "Split_eta[nSplit]/F");
    tree_out->Branch("Split_phi", Split_phi, "Split_phi[nSplit]/F");
    tree_out->Branch("Split_dxy", Split_dxy, "Split_dxy[nSplit]/F");
    tree_out->Branch("Split_dz", Split_dz, "Split_dz[nSplit]/F");
    tree_out->Branch("Split_q", Split_q, "Split_q[nSplit]/F");

    tree_out->Branch("nOneLeg", &nOneLeg, "nOneLeg/I");
    tree_out->Branch("OneLeg_pt", OneLeg_pt, "OneLeg_pt[nOneLeg]/F");
    tree_out->Branch("OneLeg_eta", OneLeg_eta, "OneLeg_eta[nOneLeg]/F");
    tree_out->Branch("OneLeg_phi", OneLeg_phi, "OneLeg_phi[nOneLeg]/F");
    tree_out->Branch("OneLeg_dxy", OneLeg_dxy, "OneLeg_dxy[nOneLeg]/F");
    tree_out->Branch("OneLeg_dz", OneLeg_dz, "OneLeg_dz[nOneLeg]/F");
    tree_out->Branch("OneLeg_q", OneLeg_q, "OneLeg_q[nOneLeg]/F");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
CosmicAnalyzer::endJob() 
{

    file_out->cd();
    tree_out->Write();
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
