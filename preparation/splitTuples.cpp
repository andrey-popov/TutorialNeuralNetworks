/**
 * The programs prunes tuples with input variables leaving only those that are needed and splits
 * them to get desired numbers of events.
 */

#include <TFile.h>
#include <TTree.h>

#include <string>
#include <sstream>
#include <list>
#include <memory>
#include <numeric>
#include <iostream>


using namespace std;


void SplitTuples(TString const &srcFileName, TString const &outFileBaseName,
 list<TString> const &branchesToKeep, list<unsigned> const &nEventsInGroup)
{
    // Name of trees in the tuples (it is always the same)
    TString const treeName("Vars");
    
    
    // Open the source file and get the source tree
    TFile srcFile(srcFileName);
    unique_ptr<TTree> srcTree(dynamic_cast<TTree *>(srcFile.Get(treeName)));
    unsigned long const nTotal = srcTree->GetEntries();
    unsigned long curEntry = 0;
    
    
    // Make sure that the number of events available is not smaller than what has been requested
    if (nTotal < accumulate(nEventsInGroup.begin(), nEventsInGroup.end(), 0ul))
        throw runtime_error("SplitTuples: Cannot yield the desired number of events.");
    
    
    // Switch off all branches in the source tree but the requested ones
    srcTree->SetBranchStatus("*", false);
    
    for (auto const &branchName: branchesToKeep)
        srcTree->SetBranchStatus(branchName, true);
    
    
    // Loop over the requested blocks of events
    for (auto const &nEventsRequested: nEventsInGroup)
    {
        // Create the output file
        ostringstream outFileName;
        outFileName << outFileBaseName << "_" << nEventsRequested / 1000. << "k.root";
        TFile outFile(outFileName.str().c_str(), "recreate");
        
        
        // Create the new tree using the structure of the source one (enabled branches only)
        unique_ptr<TTree> outTree(srcTree->CloneTree(0));
        outTree->SetDirectory(&outFile);
        
        
        // Copy the desired number of events from the tree, one by one
        for (unsigned long ev = 0; ev < nEventsRequested; ++ev)
        {
            srcTree->GetEntry(curEntry);
            outTree->Fill();
            ++curEntry;
        }
        
        
        // Write the tree
        outTree->Write();
        
        cout << "File \"" << outFileName.str() << "\" written.\n";
    }
}


int main()
{
    // Names of branches to keep
    list<TString> const branchesToKeep{"Weight", "glb_Charge_Lep", "glb_Sphericity",
     "tHq_Pt_Recoil", "tHq_Eta_Recoil", "tHq_Pt_Higgs", "tHq_NumBTag_Higgs", "tt_DeltaR_Light",
     "tt_Mass_TopHad", "tt_NumPassBTag_Light"};
    
    
    // Numbers of events to have in different tuples
    list<unsigned> const nEvents{10000, 5000, 1000};
    
    
    // Process the samples
    SplitTuples("../data/thq-nc-mg_53X.02.01_Wyg.root", "../data/tH", branchesToKeep, nEvents);
    SplitTuples("../data/ttbar-semilep-mg_53X.02.01_merged.root", "../data/ttbar", branchesToKeep,
     nEvents);
    
    
    return EXIT_SUCCESS;
}
