// Ruggero Angalone r.angalone@campus.unimib.it

//  v 0) Opens file, take tree and checks for mistakes
//  1) Adds the function to the tree
//  2) Loads the fit function
//  3) Inverts for the energy
//  4) Computes the DE
//  5) Understands the energy

// Launch with:
//  .L TreeComplete.cxx
//  TreeComplete("/home/ruggero/Desktop/DUNE/Output/500Events_1800MeV/OutAna_ChargeVertex_NEvents500_Energy1800.root", "flashmatchAna10ppm", "FlashMatchTree_Test", "TrueX")
//TotalTreeComplete cycles through all the energy and files 


#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

//sign function
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void LoadVariable(std::vector <std::string> &VFitType, std::vector <std::string> &VFileName, std::vector <std::string> &Vxname){
  //VFitType = {"NothingFixed_Final", "FixedAllParametersFrom500MeV_Final", "FixedAllArOnlyParametersFrom500MeV_Final", "FixedP2From500MeV_Final", "FixedAronlyP2From500MeV_Final", "FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor_Final"};

  VFileName = {"/home/ruggero/Desktop/DUNE/Output/500Events_100MeV/OutAna_ChargeVertex_NEvents500_Energy100.root",
                "/home/ruggero/Desktop/DUNE/Output/500Events_300MeV/OutAna_ChargeVertex_NEvents500_Energy300.root",
                "/home/ruggero/Desktop/DUNE/Output/500Events_500MeV/OutAna_ChargeVertex_NEvents500_Energy500.root",
                "/home/ruggero/Desktop/DUNE/Output/500Events_1500MeV/OutAna_ChargeVertex_NEvents500_Energy1500.root",
                "/home/ruggero/Desktop/DUNE/Output/500Events_1800MeV/OutAna_ChargeVertex_NEvents500_Energy1800.root",
                "/home/ruggero/Desktop/DUNE/Output/500Events_2000MeV/OutAna_ChargeVertex_NEvents500_Energy2000.root",
                "/home/ruggero/Desktop/DUNE/Output/500Events_2200MeV/OutAna_ChargeVertex_NEvents500_Energy2200.root",
                "/home/ruggero/Desktop/DUNE/Output/500Events_2500MeV/OutAna_ChargeVertex_NEvents500_Energy2500.root"
              };

  Vxname = {"TrueX", "ShiftedPandoraX"};
}

void eraseSubStr(std::string & mainStr, const std::string & toErase){
    // Search for the substring in string
    size_t pos = mainStr.find(toErase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }
}

TFile * OpenFile(std::string FileName, std::string FolderName){
  TFile *fp = new TFile((FileName).c_str(), "UPDATE");
  if ((fp->IsZombie()) || (fp == nullptr)) {
   std::cout << "\n Error opening file" << std::endl;
   exit(-1);
  }
  return fp;
}

float FindEnergyFromFileName(std::string FileName){
  float EnergyValue;
  //This is implemented for filename of the type ...Energy*******{Limiter}...
  //Where:
  //  ******* are the energy value to be extracted
  //  {Limiter} is something contained in the limiter vector
  //  ... is everything else
  std::string EnergyString = "";
  std::string StringToFind = "Energy";
  std::vector<std::string> LimiterVector = {"_", ".", " ", "/"};
  std::size_t foundEnergy = FileName.find(StringToFind);
  std::size_t foundLimiter;
  std::size_t temp = FileName.size()+1;
  for(auto Limiter : LimiterVector){
    foundLimiter = FileName.find(Limiter,foundEnergy+1);
    //std::cout << foundEnergy << "\t" << foundLimiter << "\t" << temp << "\t" << StringToFind.size() << std::endl;
    if( (foundLimiter < temp) && (foundLimiter < FileName.size()) ){
      temp = foundLimiter;
      }
    }
  foundLimiter = temp;
  //std::cout << FileName << std::endl;
  EnergyString = FileName.substr(foundEnergy+StringToFind.size(), foundLimiter-foundEnergy-StringToFind.size());
  //std::cout << "EnergyString = " << EnergyString << std::endl;
  //std::cout << "EnergyInt = " << std::stof(EnergyString) << std::endl;
  EnergyValue = std::stof(EnergyString);
  return EnergyValue;
}

TTree * OpenTree(TFile * FilePointer, std::string TreePath){
  TTree *T = new TTree();
  gDirectory->GetObject(TreePath.c_str(), T);
  if(T == nullptr){
    std::cout << "\n Tree = " << TreePath <<  " not found" << std::endl;
    FilePointer->ls();
    exit(-1);
  }
  // T->Show();
  // T->Print();
  return T;
}

void SetNames(std::string FileName, std::string FolderName, std::string XName, std::string FitType, float EnergyValue, std::string &FitGraphSaveName, std::string &InterpolVarSaveName, std::string &DEVariationSaveName, std::string &DEGraphSaveName, std::string &DEGraphTitle){
  std::string EnergyString = std::to_string(int(EnergyValue)) + "MeV";
  std::string FolderNameInGraph = FolderName;
  eraseSubStr(FolderNameInGraph, "flashmatchAna");
  std::string SavePath = "../Graphs/" + EnergyString + "/" + FolderNameInGraph + "/";
  std::string XType;
  //XType identifies the x varialbe used to invert the fitting function, the one passed in TreeComplete under XName
  if(XName == "Pandora_Best_X") XType = "PandoraX";
  else if(XName == "Pandora_Best_X_Shifted")  XType = "ShiftedPandoraX";
  else  XType = XName;
  //Name of PE/DepE vs TrueX
  FitGraphSaveName = SavePath + "PE_over_E_vs_AbsTrueX_fitted_" + FitType + "_500Events_" + FolderNameInGraph + ".jpg";
  //Name of the interpolated variable
  InterpolVarSaveName = "EnergyDepositedTotal_" + XType + "_" + FitType + "_500Events_Interpolated";
  DEVariationSaveName = "Variation_" + InterpolVarSaveName;
  DEGraphSaveName = SavePath + "DE_fitted_"+ XType + "_" + FitType + "_500Events_" + EnergyString + ".jpg";
  DEGraphTitle = "#delta E = #frac{E_{Extrapolated_{" + XType + "_" + FitType + "}}-E_{Deposited}}{E_{Deposited}}";
  eraseSubStr(DEGraphTitle, "_Final");
  eraseSubStr(DEGraphSaveName, "_Final");
  eraseSubStr(FitGraphSaveName, "_Final");
}

void FunctionForSettingParameters(TF1 *f1, std::string FolderName){
  if( FolderName == "flashmatchAnaArOnly"){
    f1->SetParameter(0,-2.89);
    f1->SetParameter(1,7522);
    f1->SetParameter(2,0.006034);
    f1->SetParameter(3,-4.656);
  }
  if( FolderName == "flashmatchAna10ppm" ){
    f1->SetParameter(0,2.433);
    f1->SetParameter(1,5433);
    f1->SetParameter(2,0.006321);
    f1->SetParameter(3,-5.047);
    }
}

TF1 * AddFunction(TFile * FilePointer, TTree * T, std::string FitTitle, std::string FitGraphSaveName, std::string FitType, std::string FolderName, std::string FileName){
  double XMinFit = 20;
  double XMax = 400.0;
  std::string FuncName;
  if ( (FitType == "FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor_Final") || (FitType == "FixedAllArOnlyParametersFrom1500MeV_FFinal_AddedScaleFactor_Final") )  FuncName = "[4] * ( [0] + [1]*exp(-x*[2]+[3]) ) ";
  else FuncName = "[0] + [1]*exp(-x*[2]+[3]) ";
  TF1 *f1 = new TF1(FitTitle.c_str(), FuncName.c_str(), XMinFit, XMax);
  eraseSubStr(FitTitle, "flashmatchAna");
  if(FitType == "NothingFixed_Final"){ // Servono per aiutare il fit
    f1->SetParameter(0,-30);
    f1->SetParameter(1,2000);
    f1->SetParameter(2,0);
    f1->SetParameter(3,-3);
  }
  else if (FitType == "FixedAllParametersFrom500MeV_Final"){
    if( FolderName == "flashmatchAnaArOnly") {
      f1->SetParameter(0, -2.89);
      f1->SetParLimits(0, -2.89, -2.89);
      f1->SetParameter(1, 7522);
      f1->SetParLimits(1, 7522, 7522);
      f1->SetParameter(2, 0.006034);
      f1->SetParLimits(2, 0.006034, 0.006034);
      f1->SetParameter(3, -4.656);
      f1->SetParLimits(3, -4.656, -4.656);
    }
    else if( FolderName == "flashmatchAna10ppm" ){
      f1->SetParameter(0, 2.433);
      f1->SetParLimits(0, 2.433, 2.433);
      f1->SetParameter(1, 5433);
      f1->SetParLimits(1, 5433, 5433);
      f1->SetParameter(2, 0.006321);
      f1->SetParLimits(2, 0.006321, 0.006321);
      f1->SetParameter(3, -5.047);
      f1->SetParLimits(3, -5.047, -5.047);
    }
    else{
      std::cout << "Folder unknown" << std::endl;
      exit(-1);
    }

  }
  else if (FitType == "FixedAllParametersFrom1500MeV_Final"){
    if( FolderName == "flashmatchAnaArOnly") {
      f1->SetParameter(0, -64.01);
      f1->SetParLimits(0, -64.01, -64.01);
      f1->SetParameter(1, 2257);
      f1->SetParLimits(1, 2257, 2257);
      f1->SetParameter(2, 0.00179);
      f1->SetParLimits(2, 0.00179, 0.00179);
      f1->SetParameter(3, -2.928);
      f1->SetParLimits(3, -2.928, -2.928);
    }
    else if( FolderName == "flashmatchAna10ppm" ){
      f1->SetParameter(0, -7.388);
      f1->SetParLimits(0, -7.388, -7.388);
      f1->SetParameter(1, 1086);
      f1->SetParLimits(1, 1086, 1086);
      f1->SetParameter(2, 0.003373);
      f1->SetParLimits(2, 0.003373, 0.003373);
      f1->SetParameter(3, -3.273);
      f1->SetParLimits(3, -3.273, -3.273);
    }
    else{
      std::cout << "Folder unknown" << std::endl;
      exit(-1);
    }

  }
  else if (FitType == "FixedAllArOnlyParametersFrom500MeV_Final"){
    f1->SetParameter(0, -2.89);
    f1->SetParLimits(0, -2.89, -2.89);
    f1->SetParameter(1, 7522);
    f1->SetParLimits(1, 7522, 7522);
    f1->SetParameter(2, 0.006034);
    f1->SetParLimits(2, 0.006034, 0.006034);
    f1->SetParameter(3, -4.656);
    f1->SetParLimits(3, -4.656, -4.656);
  }
  else if (FitType == "FixedAllArOnlyParametersFrom1500MeV_FFinal_Final"){
    f1->SetParameter(0, -64.01);
    f1->SetParLimits(0, -64.01, -64.01);
    f1->SetParameter(1, 2257);
    f1->SetParLimits(1, 2257, 2257);
    f1->SetParameter(2, 0.00179);
    f1->SetParLimits(2, 0.00179, 0.00179);
    f1->SetParameter(3, -2.928);
    f1->SetParLimits(3, -2.928, -2.928);
  }
  else if (FitType == "FixedP2From500MeV_Final"){
    if( FolderName == "flashmatchAnaArOnly") {
      f1->SetParameter(2, 0.006034);
      f1->SetParLimits(2, 0.006034, 0.006034);

      f1->SetParameter(0, -5);
      f1->SetParameter(1, 7000);
      f1->SetParameter(3, -4);
    }
    else if( FolderName == "flashmatchAna10ppm" ){
      f1->SetParameter(2, 0.006321);
      f1->SetParLimits(2,0.006321,0.006321);

      f1->SetParameter(0, -5);
      f1->SetParameter(1, 7000);
      f1->SetParameter(3, -4);
    }
    else{
      std::cout << "Folder unknown" << std::endl;
      exit(-1);
    }
  }
  else if (FitType == "FixedAronlyP2From500MeV_Final"){
    f1->SetParameter(2, 0.006034);
    f1->SetParLimits(2, 0.006034, 0.006034);

    f1->SetParameter(0, -5);
    f1->SetParameter(1, 7000);
    f1->SetParameter(3, -4);
  }
  else if (FitType == "FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor_Final"){
    f1->SetParameter(0, -2.89);
    f1->SetParLimits(0, -2.89, -2.89);
    f1->SetParameter(1, 7522);
    f1->SetParLimits(1, 7522, 7522);
    f1->SetParameter(2, 0.006034);
    f1->SetParLimits(2, 0.006034, 0.006034);
    f1->SetParameter(3, -4.656);
    f1->SetParLimits(3, -4.656, -4.656);

    f1->SetParameter(4, 1);
  }
  else if (FitType == "FixedAllArOnlyParametersFrom1500MeV_FFinal_AddedScaleFactor_Final"){
    f1->SetParameter(0, -64.01);
    f1->SetParLimits(0, -64.01, -64.01);
    f1->SetParameter(1, 2257);
    f1->SetParLimits(1, 2257, 2257);
    f1->SetParameter(2, 0.00179);
    f1->SetParLimits(2, 0.00179, 0.00179);
    f1->SetParameter(3, -2.928);
    f1->SetParLimits(3, -2.928, -2.928);
    f1->SetParameter(4, 1);
  }
  TTree *TFit  = OpenTree(FilePointer, FolderName+"/FlashMatchTree_Fit");
  std::string XName = "TrueX";
  std::string SumPeName = "SumPE";
  std::string EnDepName = "EnergyDepositedTotal";
  Float_t pX, pSumPe, pEnDep;
  TFit->SetBranchAddress(XName.c_str(),&pX);
  TFit->SetBranchAddress(SumPeName.c_str(),&pSumPe);
  TFit->SetBranchAddress(EnDepName.c_str(),&pEnDep);
  std::vector<Float_t> v1,v2;
  Long64_t nentries = TFit -> GetEntries();
  for (Int_t i = 0; i<nentries; i++) {
    TFit -> GetEntry(i);
    v1.push_back(fabs(pX));
    v2.push_back((pSumPe/pEnDep));
    }
  std::string TitleDef = "PE over Energy as a function of x, fitted in range Abs(TrueX)>20 " + FolderName;
  std::string nameYaxis = "#frac{PE}{DepEn} #frac{[PE]}{[MeV]}";
  std::string nameXaxis = "|TrueX| [cm]";
  auto c1 = new TCanvas("c1","c1");
  TLatex lat;
  TGraph *g = new TGraph(v1.size(), &v1[0], &v2[0]);
  g -> Fit(f1,"REM");
  g -> SetMarkerStyle(7);
  g -> SetTitle(TitleDef.c_str());
  gStyle->SetOptFit(1111);
  g -> Draw("AP");
  lat.DrawLatex(5,5,FuncName.c_str());
  g -> GetXaxis()->SetTitle(nameXaxis.c_str());
  g -> GetYaxis()->SetTitle(nameYaxis.c_str());
  c1->SaveAs(FitGraphSaveName.c_str());
  std::cout << "Graph saved: " << FitGraphSaveName << std::endl;
  T -> ResetBranchAddresses();
  FilePointer->cd(FolderName.c_str());
  f1 -> Write("", TObject::kOverwrite);
  FilePointer->cd("..");
  return f1;
}

TF1 * LoadsFunction(TFile * FilePointer, TTree * T, std::string FitTitle, std::string FitGraphSaveName, std::string FitType, std::string FolderName, std::string FileName){
  TF1 *f1;
  gDirectory->GetObject((FolderName + std::string("/") + FitTitle).c_str(), f1);
  if( f1 == nullptr ){
  //if( f1 == nullptr ){
    std::cout <<"\n Function not found -- Adding function" << std::endl;
    f1 = AddFunction(FilePointer, T, FitTitle, FitGraphSaveName, FitType, FolderName, FileName);
    if( f1 == nullptr){
      std::cout << "Something went wrong with adding the function" << std::endl;
      exit(-1);
    }
    else
      std::cout << "Function added successfully" << std::endl;
  }

  return f1;
}

void FitInversion(TFile *FilePointer, TTree * T, std::string FolderName, TF1 *f1, std::string xname, std::string yname, std::string var1Name, std::string BranchNameInterpolated ){
  //Load the xname variable inside px, access value i with T->GetEntry(i);
    Float_t px = 0;
    T->SetBranchAddress(xname.c_str(),&px);
  //Load the yname variable inside py, acces value i with T->GetEntry(i);
    Float_t py = 0;
    T->SetBranchAddress(yname.c_str(),&py);
  //Load the deposited energy, used to check
    Float_t pz = 0;
    T->SetBranchAddress(var1Name.c_str(),&pz);
  //Usefull variables
    Int_t Dimension = T->GetEntries();
  //Create interpolated branch
    Float_t pInterpolated, pInterpolated_v[Dimension];
    std::string temp = BranchNameInterpolated + std::string("/F");
    TBranch *BranchInterpolated = T -> Branch(BranchNameInterpolated.c_str(), &pInterpolated, temp.c_str());
  //Compute the interpolated variable
    for(Int_t i=0; i<Dimension; i++){
      T -> GetEntry(i);
      pInterpolated_v[i] = py/(f1->Eval(fabs(px)));
      //std::cout << "x = " << px << "\t f("<< px <<") = " << (f1->Eval(px)) << "\t f(|"<< px <<"|) = " << (f1->Eval(fabs(px))) <<"\t SumPE = " << py << " \t EnDep = " << pz << "\t EnInterp = " << pInterpolated_v[i] <<"\n";
      }
  //Fill the interpolated branch
    for(Int_t i=0; i<Dimension; i++){
      pInterpolated = pInterpolated_v[i];
      BranchInterpolated -> Fill();
    }
  //Write the file
    FilePointer->cd(FolderName.c_str());
    T->Write("", TObject::kOverwrite);
    FilePointer->cd("..");
    T -> ResetBranchAddresses();
}

void EnergyVariation(TFile *FilePointer, TTree * T, std::string FolderName, std::string nameData, std::string nameInterpolation, std::string BranchNameVariation){
  Float_t p0, p1;
  T->SetBranchAddress(nameData.c_str(), &p0); //Dati
  T->SetBranchAddress(nameInterpolation.c_str(), &p1); //Stimata
  std::vector<Float_t> v0,v1;
  Float_t pVariation;
  std::string temp = BranchNameVariation + std::string("/F");
  TBranch *BranchVariation = T -> Branch(BranchNameVariation.c_str(), &pVariation, temp.c_str());
  Int_t nentries = T->GetEntries();
  for (Int_t i = 0; i<nentries; i++) {
    T -> GetEntry(i);
    pVariation = (p1-p0)/p0;
    BranchVariation->Fill();
  }
  FilePointer->cd(FolderName.c_str());
  T->Write("", TObject::kOverwrite);
  FilePointer->cd("..");
  T -> ResetBranchAddresses();
}

void SaveTheFitParameters(TH1F *h1, std::string FileName, std::string FolderName, std::string FitType, std::string xname){
  //Saves the parameters of the gaussian fit of DE in an apposite file
  //Takes the gaussian parameters
    TF1 *fg = (TF1*)h1->GetListOfFunctions()->FindObject("gaus");
    float EnergyValue = FindEnergyFromFileName(FileName);
    std::string EnergyName = std::to_string(int(EnergyValue));
    double Constant = fg -> GetParameter(0);
    double ConstantError = fg -> GetParError(0);
    double Mu = fg -> GetParameter(1);
    double MuError = fg -> GetParError(1);
    double Sigma = fg -> GetParameter(2);
    double SigmaError =  fg -> GetParError(2);
  //Opens the file to check if the energy value is already present
    std::string PathParametersFile = "../Output/ParametersFile/";
    std::string FileParameterName =  PathParametersFile + "GaussParametersFromDeltaE_" + FolderName + "_" + xname + "_" + FitType + ".txt";
    eraseSubStr(FileParameterName, "_Final");
    fstream TXTfileRead;
    TXTfileRead.open (FileParameterName, ios::app | ios::out | ios::in);
    //TXTfile << "\t" << EnergyValue << "\t" << Constant << "\t" << ConstantError << "\t" << Mu << "\t" << MuError << "\t" << Sigma << "\t" << SigmaError << "\t" << std::endl;
  //Checks wheter this file already contain the line referring to this specific energy
    bool PresenceCheck = false;
    std::string line;
    //std::cout << "Energy = " << EnergyName << std::endl;
    while ( std::getline (TXTfileRead,line) )
    {
      if(line.find( EnergyName) < 3) PresenceCheck = true;
    }
    TXTfileRead.close();
  //   if ( ! TXTfile.is_open()) {
  //     std::cout << "FileParameterName = " << FileParameterName << std::endl;
  //     std::cout << "!!!!!!!!!!!!! Can't open TXT file !!!!!!!!!!!!!!" << std::endl;
  //   }
  // //Add the parameters
    if(PresenceCheck == false){
      ofstream TXTfileWrite;
      TXTfileWrite.open (FileParameterName, ios::app);
      TXTfileWrite << EnergyValue << "; \t\t" << Constant << "; \t\t" << ConstantError << "; \t\t" << Mu << "; \t\t" << MuError << "; \t\t" << Sigma << "; \t\t" << SigmaError << "; \t\t" << std::endl;
      TXTfileWrite.close();
      std::cout << "Parameters written to: " << FileParameterName << std::endl;
    }
}

void DrawBeautifulHistogram(TTree * T, std::string BranchTitle, std::string FolderName, std::string FitType, std::string xname, std::string SaveName, float EnergyValue, std::string FileName, float XMin, float XMax, std::string TitleX, std::string TitleY, std::string TitleHisto){
  Float_t px;
  auto c2 = new TCanvas("c2","c2");
  c2->SetTitle(TitleHisto.c_str());
  c2->cd();
  T->SetBranchAddress(BranchTitle.c_str(),&px);
  TH1F *h1 = new TH1F("h1","h1 title",100,XMin, XMax);
  for (Int_t cont = 0; cont < T->GetEntries(); cont++) {
    T -> GetEntry(cont);
    h1 -> Fill(px);
  }
  gStyle->SetOptFit(1111);
  gStyle->SetTitleSize(0.03f,"t");
  h1->Fit("gaus","","",XMin,XMax);
  SaveTheFitParameters(h1, FileName, FolderName, FitType, xname);
  h1->GetXaxis()->SetTitle(TitleX.c_str());
  h1->GetYaxis()->SetTitle(TitleY.c_str());
  h1->SetTitle(TitleHisto.c_str());
  h1->Draw();
  std::string FolderNameInGraph = FolderName;
  eraseSubStr(FolderNameInGraph, "flashmatchAna");
  std::string EnergyString = std::to_string(int(FindEnergyFromFileName(FileName))) + "MeV";
  c2->SaveAs(SaveName.c_str());
  T -> ResetBranchAddresses();
}

int TreeComplete(std::string FileName = "/home/ruggero/Desktop/DUNE/Output/500Events_1800MeV/OutAna_ChargeVertex_NEvents500_Energy1800.root", std::string FolderName = "flashmatchAna10ppm", std::string TreeName = "FlashMatchTree_Test", std::string xname = "Pandora_Best_X_Shifted", std::string FitType = "FixedP2From500MeV_Final"){
  float EnergyValue = FindEnergyFromFileName(FileName);
  //File and tree used for the analysis
    std::string TreePath = FolderName + std::string("/") + TreeName;
  //Type of the fit function
  //std::string FitType;
    //FitType = "NothingFixed_Final";
    // FitType = "FixedAllParametersFrom500MeV_Final";
    // FitType = "FixedAllArOnlyParametersFrom500MeV_Final";
    // FitType = "FixedP2From500MeV_Final";
    // FitType = "FixedAronlyP2From500MeV_Final";
    // FitType = "FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor_Final";

    std::string FitTitle = "Fit at " + std::to_string(int(EnergyValue)) + " | " + FitType + FolderName;



  //Variables for the fit function inversion
    //std::string xname = "TrueX";
    //std::string xname = "Pandora_Best_X_Shifted";
    // std::string xname = "HitToX";
    if(xname == "ShiftedPandoraX") xname = "Pandora_Best_X_Shifted";
    std::string yname = "SumPE";
    //std::string var1Name = "EnergyDepositedTotal";
    std::string var1Name = "EnergyDepositedTotal";

    std::string FitGraphSaveName, InterpolVarSaveName, DEVariationSaveName, DEGraphSaveName, DEGraphTitle;
    SetNames(FileName, FolderName, xname, FitType,  EnergyValue, FitGraphSaveName, InterpolVarSaveName, DEVariationSaveName, DEGraphSaveName, DEGraphTitle);


    TFile *FilePointer = OpenFile(FileName, FolderName);
    // FilePointer->cd("flashmatchAnaArOnly");
    // FilePointer->ls();
    TTree *T = OpenTree(FilePointer, TreePath);
    TF1 *f1 = LoadsFunction(FilePointer, T, FitTitle, FitGraphSaveName, FitType, FolderName, FileName);

    std::string BranchNameInterpolated = InterpolVarSaveName;
    FitInversion(FilePointer, T, FolderName, f1, xname, yname, var1Name, InterpolVarSaveName);

    std::string BranchNameVariation = DEVariationSaveName;
    EnergyVariation(FilePointer, T, FolderName, var1Name, InterpolVarSaveName, BranchNameVariation);

    std::string TitleX = "#delta E";
    std::string TitleY = "";
    std::string TitleHisto = DEGraphTitle;
    float XMin = -1;
    float XMax = 1;
    std::string SaveName = DEGraphSaveName;

    DrawBeautifulHistogram(T, BranchNameVariation, FolderName, FitType, xname, SaveName, EnergyValue, FileName, XMin, XMax, TitleX, TitleY, TitleHisto);
    // std::cout << "Press enter to continue " << std::endl;
    // std::getchar(); //Waits for input
    FilePointer -> Close();
  return 0;
}

void TotalTreeComplete(){
  std::vector <std::string> VFitType = {
  "NothingFixed_Final",
  "FixedAllParametersFrom500MeV_Final",
  "FixedAllArOnlyParametersFrom500MeV_Final",
  "FixedP2From500MeV_Final",
  "FixedAronlyP2From500MeV_Final",
  "FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor_Final",
  "FixedAllArOnlyParametersFrom1500MeV_FFinal_Final",
  "FixedAllArOnlyParametersFrom1500MeV_FFinal_AddedScaleFactor_Final"
};
  //std::vector <std::string> VFileName = {"/home/ruggero/Desktop/DUNE/Output/500Events_100MeV/OutAna_ChargeVertex_NEvents500_Energy100.root", "/home/ruggero/Desktop/DUNE/Output/500Events_300MeV/OutAna_ChargeVertex_NEvents500_Energy300.root", "/home/ruggero/Desktop/DUNE/Output/500Events_500MeV/OutAna_ChargeVertex_NEvents500_Energy500.root", "/home/ruggero/Desktop/DUNE/Output/500Events_1500MeV/OutAna_ChargeVertex_NEvents500_Energy1500.root", "/home/ruggero/Desktop/DUNE/Output/500Events_1800MeV/OutAna_ChargeVertex_NEvents500_Energy1800.root", "/home/ruggero/Desktop/DUNE/Output/500Events_2000MeV/OutAna_ChargeVertex_NEvents500_Energy2000.root", "/home/ruggero/Desktop/DUNE/Output/500Events_2200MeV/OutAna_ChargeVertex_NEvents500_Energy2200.root", "/home/ruggero/Desktop/DUNE/Output/500Events_2500MeV/OutAna_ChargeVertex_NEvents500_Energy2500.root", "/home/ruggero/Desktop/DUNE/Output/500Events_2800MeV/OutAna_ChargeVertex_NEvents500_Energy2800.root", "/home/ruggero/Desktop/DUNE/Output/500Events_3000MeV/OutAna_ChargeVertex_NEvents500_Energy3000.root", "/home/ruggero/Desktop/DUNE/Output/500Events_3200MeV/OutAna_ChargeVertex_NEvents500_Energy3200.root"};
  std::vector <std::string> VFileName = {
    "/home/ruggero/Desktop/DUNE/Output/500Events_100MeV/OutAna_ChargeVertex_NEvents500_Energy100.root",
    "/home/ruggero/Desktop/DUNE/Output/500Events_300MeV/OutAna_ChargeVertex_NEvents500_Energy300.root",
    "/home/ruggero/Desktop/DUNE/Output/500Events_500MeV/OutAna_ChargeVertex_NEvents500_Energy500.root",
    "/home/ruggero/Desktop/DUNE/Output/500Events_1500MeV/OutAna_ChargeVertex_NEvents500_Energy1500.root",
    "/home/ruggero/Desktop/DUNE/Output/500Events_1800MeV/OutAna_ChargeVertex_NEvents500_Energy1800.root",
    "/home/ruggero/Desktop/DUNE/Output/500Events_2000MeV/OutAna_ChargeVertex_NEvents500_Energy2000.root",
    "/home/ruggero/Desktop/DUNE/Output/500Events_2200MeV/OutAna_ChargeVertex_NEvents500_Energy2200.root",
    "/home/ruggero/Desktop/DUNE/Output/500Events_2500MeV/OutAna_ChargeVertex_NEvents500_Energy2500.root"
  };
  std::vector <std::string> Vxname = {"TrueX", "ShiftedPandoraX"};
  std::vector <std::string> VFolderName = {"flashmatchAnaArOnly","flashmatchAna10ppm"};
  std::string TreeName = "FlashMatchTree_Test";

  //(FileName, FolderName, TreeName, xname, FitType)

  //std::string FileName, FolderName, TreeName, xname, FitType;

  for(std::string FitType : VFitType){
    for(std::string FileName : VFileName){
      for(std::string  xname : Vxname){
        for(std::string FolderName : VFolderName){
          // std::cout << "\n" << FileName << "\n" << FolderName << "\n" << TreeName << "\n" << xname << "\n" << FitType << std::endl;
          TreeComplete(FileName, FolderName, TreeName, xname, FitType);
        }
      }
    }
  }

}
