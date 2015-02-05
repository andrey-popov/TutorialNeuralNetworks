{
    // Create a TMVA reader
    TMVA::Reader reader("Color=True");
    
    
    // Register input variables. Their names and order must be same in training
    Float_t glb_Charge_Lep, glb_Sphericity, abs_tHq_Eta_Recoil, tHq_NumBTag_Higgs, log_tHq_Pt_Higgs,
     log_tHq_Pt_Recoil, tt_DeltaR_Light, log_tt_Mass_TopHad, tt_NumPassBTag_Light;
    
    reader.AddVariable("glb_Charge_Lep", &glb_Charge_Lep);
    reader.AddVariable("glb_Sphericity", &glb_Sphericity);
    reader.AddVariable("abs(tHq_Eta_Recoil)", &abs_tHq_Eta_Recoil);
    reader.AddVariable("tHq_NumBTag_Higgs", &tHq_NumBTag_Higgs);
    reader.AddVariable("log(tHq_Pt_Higgs)", &log_tHq_Pt_Higgs);
    reader.AddVariable("log(tHq_Pt_Recoil)", &log_tHq_Pt_Recoil);
    reader.AddVariable("tt_DeltaR_Light", &tt_DeltaR_Light);
    reader.AddVariable("log(tt_Mass_TopHad)", &log_tt_Mass_TopHad);
    reader.AddVariable("tt_NumPassBTag_Light", &tt_NumPassBTag_Light);
    
    
    // Book the neural network with backpropagation
    reader.BookMVA("TutorialTrain_BP", "weights/TutorialTrain_TutorialTrain_BP.weights.xml");
    
    
    // A histogram to be filled
    TH1D sgnResponseHist("sgnResponseHist", "NN response in signal events", 20, 0., 1.);
    sgnResponseHist.Sumw2();
    
    
    // Open the source file and get the tree
    TFile srcFile("../data/tH_5k.root");
    TTree *srcTree = (TTree *) srcFile.Get("Vars");
    unsigned long const nEntries = srcTree->GetEntries();
    
    
    // Set branch addresses to read the tree
    Float_t tHq_Eta_Recoil, tHq_Pt_Higgs, tHq_Pt_Recoil, tt_Mass_TopHad;
    Float_t Weight;
    
    srcTree->SetBranchAddress("glb_Charge_Lep", &glb_Charge_Lep);
    srcTree->SetBranchAddress("glb_Sphericity", &glb_Sphericity);
    srcTree->SetBranchAddress("tHq_Eta_Recoil", &tHq_Eta_Recoil);
    srcTree->SetBranchAddress("tHq_NumBTag_Higgs", &tHq_NumBTag_Higgs);
    srcTree->SetBranchAddress("tHq_Pt_Higgs", &tHq_Pt_Higgs);
    srcTree->SetBranchAddress("tHq_Pt_Recoil", &tHq_Pt_Recoil);
    srcTree->SetBranchAddress("tt_DeltaR_Light", &tt_DeltaR_Light);
    srcTree->SetBranchAddress("tt_Mass_TopHad", &tt_Mass_TopHad);
    srcTree->SetBranchAddress("tt_NumPassBTag_Light", &tt_NumPassBTag_Light);
    srcTree->SetBranchAddress("Weight", &Weight);
    
    
    // Loop over the tree
    for (unsigned long ev = 0; ev < nEntries; ++ev)
    {
        srcTree->GetEntry(ev);
        
        
        // Calculate the few input variables that require some transformation
        abs_tHq_Eta_Recoil = fabs(tHq_Eta_Recoil);
        log_tHq_Pt_Higgs = log(tHq_Pt_Higgs);
        log_tHq_Pt_Recoil = log(tHq_Pt_Recoil);
        log_tt_Mass_TopHad = log(tt_Mass_TopHad);
        
        
        // Calculate response of the neural network for the current event
        double const response = reader.EvaluateMVA("TutorialTrain_BP");
        
        
        // Fill the histogram
        sgnResponseHist.Fill(response, Weight);
    }
    
    
    // Create a canvas and adjust decoration
    TCanvas canvas;
    gStyle->SetOptStat(0);
    sgnResponseHist.SetMarkerStyle(20);
    sgnResponseHist.SetLineColor(kBlack);
    
    
    // Draw the histogram and save the figure
    sgnResponseHist.Draw("p0 e1");
    canvas.Print("sgnResponse.png");
}
