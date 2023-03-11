// Ruggero Angalone r.angalone@campus.unimib.it

// Aggiunge ad un tree un oggetto funzione


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Riostream.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>

#define NTrees 5
#define NFolders 4
#define NPlots 2
#define XMinFit 20
#define XMin 0
#define XMax 400.0
#define YMin -5.0
#define YMax 30000.0

//Change this to change the used folder
#define NPath 2

std::string FolderNames[NFolders] = {"flashmatchAnaArOnly", "flashmatchAnaXeOnly", "flashmatchAna10ppm"};

//Change this to change the file from which the tree is loaded
//std::string FileName = "../Output/500Events_100MeV/OutAna_Pandora_NEvents500_Energy100.root";
//std::string FileName = "../Output/500Events_100MeV/OutAna_Pandora_NEvents500_Energy100.root";
//std::string FileName = "../download/OutAna_NEvents500_Energy100.root";
//std::string FileName = "../Output/500Events_2500MeV/OutAna_TrueOnly_NEvents500_Energy2500.root";
std::string FileName = "../Output/500Events_2200MeV/OutAna_ChargeVertex_NEvents500_Energy2200.root";

std::string TreeNames[NTrees] = {"FlashMatchTree_Clean" ,"FlashMatchTree" ,"LargestFlashTree", "SelectedFlashTree", "CountWaveforms"};

std::string FuncName = "[0] + [1]*exp(-x*[2]+[3]) ";
//std::string FitName = "[0]+[1]/(x^2)";
void FunctionForSettingParameters(TF1 *f1){
  if( NPath == 0){
    f1->SetParameter(0,-2.89);
    f1->SetParameter(1,7522);
    f1->SetParameter(2,0.006034);
    f1->SetParameter(3,-4.656);
  }
  if( NPath == 2 ){
    f1->SetParameter(0,2.433);
    f1->SetParameter(1,5433);
    f1->SetParameter(2,0.006321);
    f1->SetParameter(3,-5.047);
    }
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

std::string TitleDef = "PE over Energy as a function of x, fitted in range Abs(TrueX)>20                     " + FolderNames[NPath];
std::string nameYaxis = "#frac{PE}{DepEn} #frac{[PE]}{[MeV]}";
std::string nameXaxis = "|TrueX| [cm]";
//std::string FitTitle = "Fit " + FolderNames[NPath];
std::string FitTitle = "Fit at 500MeV | " + FolderNames[NPath];

TF1 *f1 = new TF1(FitTitle.c_str() ,FuncName.c_str(), XMinFit, XMax);

void TreeAddFunction(){

  TFile *fp = new TFile(FileName.c_str(), "UPDATE");
  //Check if file is opened
  if (fp->IsZombie()) {
     std::cout << "Error opening file" << std::endl;
     exit(-1);
  }
  TTree *T;
  fp->cd(FolderNames[NPath].c_str());
  gDirectory->GetObject(TreeNames[0].c_str(), T);


  eraseSubStr(TitleDef, "flashmatchAna");
  FunctionForSettingParameters(f1);

  f1 -> Write("", TObject::kOverwrite);
  fp -> Close();

}
