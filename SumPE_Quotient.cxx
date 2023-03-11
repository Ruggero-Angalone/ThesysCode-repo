#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
using namespace std;

#define Renorm true

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

TFile * OpenFile(std::string FileName, std::string FolderName){
  TFile *fp = new TFile((FileName).c_str(), "UPDATE");
  if ((fp->IsZombie()) || (fp == nullptr)) {
   std::cout << "\n Error opening file" << std::endl;
   exit(-1);
  }
  return fp;
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

void TreeQuotient(std::string VarName="SumPE", std::string FileName="../Output/500Events_1500MeV/OutAna_ChargeVertex_NEvents500_Energy100.root"){
  cout << "-----------------------------------------------------------------------------------------------------------" << endl;
  cout << "Energy = " << FindEnergyFromFileName(FileName) << endl;
  cout << "FileName = " << FileName << endl;
  cout << "-----------------------------------------------------------------------------------------------------------" << endl;
  Float_t pAr, p10, pX;
  TFile *FilePointer_Ar = OpenFile(FileName.c_str(), "flashmatchAnaArOnly");
  TFile *FilePointer_10 = OpenFile(FileName.c_str(), "flashmatchAna10ppm");
  TTree *T_Ar = OpenTree(FilePointer_Ar, "flashmatchAnaArOnly/FlashMatchTree");
  TTree *T_10 = OpenTree(FilePointer_10, "flashmatchAna10ppm/FlashMatchTree");

  //T_Ar->Print();
  //T_10->Print();

  string VarName_temp;
  string VarName_Norm = "EnergyDepositedTotal";
  string Pos = "TrueX";
  Float_t pNormAr, pNorm10, pPos;

  if(VarName == "SumPEDepositedTotalt")
    VarName_temp = "GammaIonAndScint";
  else
    VarName_temp = VarName;

  std::string Title = "("+VarName_temp+"_10ppm/"+VarName_Norm+")"+"/"+"("+VarName_temp+"_ArOnly/"+VarName_Norm+")";
  //T_Ar->Show();
  // T_Ar->Draw(VarName.c_str());
  // cout << '\n' << "Press a key to continue...";
  //T_10->Show();
  // T_10->Draw(VarName.c_str());
  // cout << '\n' << "Press a key to continue...";
  vector<Float_t> pQuot_V, pPos_V;
  T_Ar->SetBranchAddress(Pos.c_str(),&pPos);
  T_Ar->SetBranchAddress(VarName.c_str(),&pAr);
  T_10->SetBranchAddress(VarName.c_str(),&p10);
  T_Ar->SetBranchAddress(VarName_Norm.c_str(),&pNormAr);
  T_10->SetBranchAddress(VarName_Norm.c_str(),&pNorm10);
  string FitGraphSaveName = "/home/ruggero/Desktop/DUNE/Graphs/SumPE_Quotient_" + to_string(int(FindEnergyFromFileName(FileName))) + "MeV.jpg";

  for(Int_t i=0; i < T_Ar->GetEntries(); i++){
    pPos_V.push_back(fabs(pPos));
    T_Ar -> GetEntry(i);
    T_10 -> GetEntry(i);
    if(Renorm)
      pQuot_V.push_back((p10/pNorm10) / (pAr/pNormAr));
    else
      pQuot_V.push_back((p10) / (pAr));
    //std::cout << "i = " << i << "\t" << pAr << "\t" << p10 << " div " << pAr << "="<< pQuot_V[i-1] <<std::endl;
  }
  auto *c1 = new TCanvas("c1", "c1", 3000, 1000);
  c1->cd();
  TH1F *h1 = new TH1F("h1",Title.c_str(),100,-1,1);
  for(auto pQuot : pQuot_V)
    h1->Fill(pQuot);
  h1->Draw();
  h1->Fit("landau");
  gStyle->SetOptFit(1111);
  c1->SaveAs(FitGraphSaveName.c_str());
  cout << "\t\t Save again with:" << endl;
  cout << "Save again with: \n \t c1 -> SaveAs( \""<< FitGraphSaveName.c_str() <<"\" )"<< endl;
  // TGraph *graph = new TGraph(pQuot_V.size(), &pPos_V[0], &pQuot_V[0]);
  // graph -> Draw("AP");
}
