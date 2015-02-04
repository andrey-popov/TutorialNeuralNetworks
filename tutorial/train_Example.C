{
    // Create a log file and a TMVA factory
    TString jobName("TutorialTrain");
    TFile infoFile(TString("info") + jobName + ".root", "recreate");
    TMVA::Factory factory(jobName, &infoFile, "Color=True");
    
    
    // Read signal and background trees
    TString srcDirectory("../data/");
    
    TFile sgnTrainFile(srcDirectory + "tH_10k.root");
    TFile sgnTestFile(srcDirectory + "tH_5k.root");
    TFile bkgTrainFile(srcDirectory + "ttbar_10k.root");
    TFile bkgTestFile(srcDirectory + "ttbar_5k.root");
    
    TTree *sgnTrainTree = (TTree *) sgnTrainFile.Get("Vars");
    TTree *sgnTestTree = (TTree *) sgnTestFile.Get("Vars");
    TTree *bkgTrainTree = (TTree *) bkgTrainFile.Get("Vars");
    TTree *bkgTestTree = (TTree *) bkgTestFile.Get("Vars");
    
    
    // Feed the trees to the factory. The second argument is an additional weight, which is applied
    //to all events in the tree
    factory.AddSignalTree(sgnTrainTree, 1., TMVA::Types::kTraining);
    factory.AddBackgroundTree(bkgTrainTree, 1., TMVA::Types::kTraining);
    factory.AddSignalTree(sgnTestTree, 1., TMVA::Types::kTesting);
    factory.AddBackgroundTree(bkgTestTree, 1., TMVA::Types::kTesting);
    
    
    // Specify the event weight
    factory.SetWeightExpression("Weight");
    
    
    // Specify input variables
    factory.AddVariable("glb_Charge_Lep");
    factory.AddVariable("glb_Sphericity");
    factory.AddVariable("abs(tHq_Eta_Recoil)");
    factory.AddVariable("tHq_NumBTag_Higgs");
    factory.AddVariable("log(tHq_Pt_Higgs)");
    factory.AddVariable("log(tHq_Pt_Recoil)");
    factory.AddVariable("tt_DeltaR_Light");
    factory.AddVariable("log(tt_Mass_TopHad)");
    factory.AddVariable("tt_NumPassBTag_Light");
    
    
    // Make the factory copy and preprocess the trees
    factory.PrepareTrainingAndTestTree("", "NormMode=EqualNumEvents");
    
    
    // Book some MVA methods
    factory.BookMethod(TMVA::Types::kLikelihood, jobName + "_Likelihood");
    factory.BookMethod(TMVA::Types::kMLP, jobName + "_BP",
     "VarTransform=Norm:TrainingMethod=BP:NeuronType=tanh:HiddenLayers=20:EstimatorType=CE:"
     "BPMode=sequential:NCycles=500:TestRate=5");
    factory.BookMethod(TMVA::Types::kMLP, jobName + "_BFGS_WeightDecay",
     "VarTransform=Norm:TrainingMethod=BFGS:NeuronType=tanh:HiddenLayers=20:EstimatorType=CE:"
     "NCycles=100:TestRate=1:UseRegulator=True");
    
    
    // Train and test all the MVA methods
    factory.TrainAllMethods();
    factory.TestAllMethods();
    factory.EvaluateAllMethods();
    
    
    return 0;
}
