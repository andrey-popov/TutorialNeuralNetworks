{
    // Create a log file and a TMVA factory
    TString jobName("CompleteExample");
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
    
    
    // Give the trees to the factory. The second argument is an additional weight applied to all
    //events in the tree
    factory.AddSignalTree(sgnTrainTree, 1., TMVA::Types::kTraining);
    factory.AddBackgroundTree(bkgTrainTree, 1., TMVA::Types::kTraining);
    factory.AddSignalTree(sgnTestTree, 10., TMVA::Types::kTesting);
    factory.AddBackgroundTree(bkgTestTree, 10., TMVA::Types::kTesting);
    
    
    // Specify the event weight
    factory.SetWeightExpression("Weight");
    
    
    // Specify input variables
    factory.AddVariable("glb_Charge_Lep", 'F');
    factory.AddVariable("glb_Sphericity", 'F');
    factory.AddVariable("abs(tHq_Eta_Recoil)", 'F');
    factory.AddVariable("tHq_NumBTag_Higgs", 'F');
    factory.AddVariable("log(tHq_Pt_Higgs)", 'F');
    factory.AddVariable("log(tHq_Pt_Recoil)", 'F');
    factory.AddVariable("tt_DeltaR_Light", 'F');
    factory.AddVariable("log(tt_Mass_TopHad)", 'F');
    factory.AddVariable("tt_NumPassBTag_Light", 'F');
    
    
    // Make the factory copy and preprocess the trees
    factory.PrepareTrainingAndTestTree("", "NormMode=EqualNumEvents");
    
    
    // Book MVA methods
    factory.BookMethod(TMVA::Types::kLikelihood, jobName + "_Likelihood");
    factory.BookMethod(TMVA::Types::kMLP, jobName + "_BP",
     "TrainingMethod=BP:NeuronType=tanh:HiddenLayers=20:EstimatorType=CE:"
     "BPMode=batch:NCycles=500:TestRate=5");
    factory.BookMethod(TMVA::Types::kMLP, jobName + "_BFGS",
     "TrainingMethod=BFGS:NeuronType=tanh:HiddenLayers=20:EstimatorType=CE:"
     "NCycles=100:TestRate=1");
    factory.BookMethod(TMVA::Types::kMLP, jobName + "_BFGS_WeightDecay",
     "TrainingMethod=BFGS:NeuronType=tanh:HiddenLayers=20:EstimatorType=CE:"
     "NCycles=100:TestRate=1:UseRegulator=True");
    
    
    // Train and test the MVA
    factory.TrainAllMethods();
    factory.TestAllMethods();
    factory.EvaluateAllMethods();
    
    
    return 0;
}
