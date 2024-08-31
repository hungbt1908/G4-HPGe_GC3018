void plot()
{
    gROOT->Reset();
	gROOT->SetStyle("Plain");

    double eDep;
    int entries;
    
    // create the root file
    TFile* f = new TFile("output_gauss.root", "READ");
    f->ls(); // list all Tree and Historgram

    // create the tree
    TTree* tree = (TTree*)f->Get("HPGe;1");
    tree->Print(); // print all branches
    tree->SetBranchAddress("fEdep", &eDep);
    entries = tree->GetEntries();
    cout << "Number of entries: " << entries << endl;

    // create new histogram
    TH1D* eDepHist = new TH1D("HPGe Histogram", "Deposition Energies", 2000, 0., 2.);

    // read ntuples from the root file to above histogram
	for(int i=0; i<entries; ++i)
    {
        tree->GetEntry(i);

        //cout << eSum << endl;
		eDepHist->Fill(eDep);
    }

    // draw
    TCanvas* c1 = new TCanvas("c1", " ", 20, 20, 800, 1000);
    c1->cd();
    eDepHist->SetTitle("Gamma Spectrum");
    eDepHist->GetXaxis()->CenterTitle(true);
    eDepHist->GetYaxis()->CenterTitle(true);
	eDepHist->GetXaxis()->SetTitle("MeV");
	eDepHist->GetYaxis()->SetTitle("Counts");
	eDepHist->SetStats(kFALSE);
	eDepHist->SetMarkerStyle(7);
	eDepHist->SetMarkerSize(3);
	eDepHist->SetMarkerColor(4);
	eDepHist->Draw("LP");
}