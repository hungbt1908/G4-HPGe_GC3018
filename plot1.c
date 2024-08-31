Double_t AddReso(Double_t eDep)
{
	double_t newEdep, deltaedep;
	Double_t r1, r2, x1, x2, edepstdgaus;
	r1 = r2 = 0.;
	x1 = x2 = 0.;
	edepstdgaus = 0.;
	TRandom2* rand = new TRandom2(0);
	r1 = rand->Rndm();
	r2 = rand->Rndm();
	x1 = TMath::Sqrt(-2.*TMath::Log(r1));
	x2 = TMath::Sin(2.*TMath::Pi()*r2);
	edepstdgaus = x1 * x2;
	

	Double_t a, b, c;
	a = 0.00157; 
	b = 4.86907E-4;
	c = 0.3372;
	Double_t fwhm = a + b*TMath::Sqrt(eDep + c*eDep*eDep);
	Double_t sigma = fwhm/2.355;
	
	deltaedep = sigma*eDep*edepstdgaus;

	newEdep = eDep + deltaedep;
	return newEdep;
}

void plot2()
{
    gROOT->Reset();
	gROOT->SetStyle("Plain");

    double eDep;
    int entries;
    
    // create the root file
    TFile* f = new TFile("output.root", "READ");
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
		eDepHist->Fill(AddReso(eDep));
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