{
    // STEP 1: Create a TMVA factory
    
    
    // Read signal and background trees from source files
    TString srcDirectory("../data/");
    
    TFile sgnTrainFile(srcDirectory + "tH_10k.root");
    TFile sgnTestFile(srcDirectory + "tH_5k.root");
    TFile bkgTrainFile(srcDirectory + "ttbar_10k.root");
    TFile bkgTestFile(srcDirectory + "ttbar_5k.root");
    
    TTree *sgnTrainTree = (TTree *) sgnTrainFile.Get("Vars");
    TTree *sgnTestTree = (TTree *) sgnTestFile.Get("Vars");
    TTree *bkgTrainTree = (TTree *) bkgTrainFile.Get("Vars");
    TTree *bkgTestTree = (TTree *) bkgTestFile.Get("Vars");
    
    
    // STEP 2: Feed trees with signal and background events to the TMVA factory. Specify per-event
    //weights
    
    
    // STEP 3: Specify input variables
    
    
    // STEP 4: Command the TMVA factory to copy and process the trees
    
    
    // STEP 5: Book an NN trained with backpropagation
    
    // STEP 7: Book an NN trained with BFGS algorithm and weight decay. Also book a projective
    //likelihood discriminator
    
    
    // STEP 6: Train the NN and run testing for it
    
    
    return 0;
}
