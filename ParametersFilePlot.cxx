// Ruggero Angalone r.angalone@campus.unimib.it

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <filesystem>
#include "TGraph.h"

using namespace std;

void eraseSubStr(string & mainStr, const string & toErase){
    // Search for the substring in string
    size_t pos = mainStr.find(toErase);
    if (pos != string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }
}
//Loads a csv file inside of a vector x vector of type string
vector<vector<string>> ParseCSV(string FileName){
  ifstream  data(FileName.c_str());
  string line;
  vector<vector<string> > parsedCsv;
  while(getline(data,line)) {
    line.erase(remove(line.begin(), line.end(), ';'), line.end());
    stringstream lineStream(line);
    string cell;
    vector<string> parsedRow;
    while(getline(lineStream,cell,'\t')){
      cell.erase(remove(cell.begin(), cell.end(), ' '), cell.end());
      if(cell.size() > 0)
        parsedRow.push_back(cell);
    }
    parsedCsv.push_back(parsedRow);
  }
  return parsedCsv;
}

vector<vector<float>> ParseCSVToFloatMatrix(string FileName){
  ifstream  data(FileName.c_str());
  string line;
  vector<vector<float> > parsedCsv;
  while(getline(data,line)) {
    line.erase(remove(line.begin(), line.end(), ';'), line.end());
    stringstream lineStream(line);
    string cell;
    vector<float> parsedRow;
    while(getline(lineStream,cell,'\t')){
      cell.erase(remove(cell.begin(), cell.end(), ' '), cell.end());
      if(cell.size() > 0)
        parsedRow.push_back(stof(cell));
    }
    parsedCsv.push_back(parsedRow);
  }
  return parsedCsv;
}

vector<vector<float>> Matrix_StringToFloat(vector<vector<string>> Matrix_String){
  vector<vector<float>> Matrix_Float;
  for(auto& Row : Matrix_String){
    vector<float> Row_Float;
    for(auto& Cell : Row){
      float temp = stof(Cell);
      Row_Float.push_back(temp);
    }
    Matrix_Float.push_back(Row_Float);
  }
  return Matrix_Float;
}

vector<float> ParseMatrixColumn(vector<vector<float>> Matrix, int ColumnIndex){
  vector<float> Column;
  for(auto& Row : Matrix){
    if(ColumnIndex < Row.size()){
      Column.push_back(Row.at(ColumnIndex));
    }
    else{
      cout << endl;
      cout << "ERROR: Trying to take column # = " << ColumnIndex << " but the matrix has only " << Matrix.at(0).size() << " and the maximum index is " << Matrix.at(0).size()-1 << endl;
      cout << endl;
      exit(-1);
    }
  }
  return Column;
}

template <typename T> void PrintMatrix(vector<vector<T>> Matrix){
  cout << "----------------"<<typeid(Matrix.at(0).at(0)).name()<<"----------------" << endl;
  for(auto& row:Matrix){
   for(auto& col:row){
      cout << col << "\t";
    }
    cout << endl;
  }
}

template <typename T> void PrintVector(vector<T> row){
  for(auto& col:row){
      cout << col << "\n";
    }
}

class MyDataset{
public:
  vector<float> x,y,dx,dy;
  string Name;
  bool dx_check, dy_check;
  MyDataset(){
    Name = "";
  }
  MyDataset(string AssignName, string FileName, int IndexX, int IndexY){
    vector<vector<float>> Matrix = ParseCSVToFloatMatrix(FileName);
    Name = AssignName;
    if (Name.find("Pandora_Best_X_Shifted") != string::npos) Name = std::regex_replace(Name, std::regex("Pandora_Best_X_Shifted"), "ShiftedPandoraX");
    x = ParseMatrixColumn(Matrix, IndexX);
    y = ParseMatrixColumn(Matrix, IndexY);
    dx_check = false;
    dy_check = false;
  }
  MyDataset(string AssignName, string FileName, int IndexX, int IndexY, int IndexDY){
    vector<vector<float>> Matrix = ParseCSVToFloatMatrix(FileName);
    Name = AssignName;
    if (Name.find("Pandora_Best_X_Shifted") != string::npos) Name = std::regex_replace(Name, std::regex("Pandora_Best_X_Shifted"), "ShiftedPandoraX");
    x = ParseMatrixColumn(Matrix, IndexX);
    y = ParseMatrixColumn(Matrix, IndexY);
    dy = ParseMatrixColumn(Matrix, IndexDY);
    dx_check = false;
    dy_check = true;
  }
  MyDataset(string AssignName, string FileName, int IndexX,  int IndexDX, int IndexY, int IndexDY){
    vector<vector<float>> Matrix = ParseCSVToFloatMatrix(FileName);
    Name = AssignName;
    if (Name.find("Pandora_Best_X_Shifted") != string::npos) Name = std::regex_replace(Name, std::regex("Pandora_Best_X_Shifted"), "ShiftedPandoraX");
    x = ParseMatrixColumn(Matrix, IndexX);
    dx = ParseMatrixColumn(Matrix, IndexDX);
    y = ParseMatrixColumn(Matrix, IndexY);
    dy = ParseMatrixColumn(Matrix, IndexDY);
    dx_check = true;
    dy_check = true;
  }
  MyDataset(string AssignName, vector<float> AssignX, vector<float> AssignY){
    Name = AssignName;
    if (Name.find("Pandora_Best_X_Shifted") != string::npos) Name = std::regex_replace(Name, std::regex("Pandora_Best_X_Shifted"), "ShiftedPandoraX");
    x = AssignX;
    y = AssignY;
    dx_check = false;
    dy_check = false;
  }
  MyDataset(string AssignName, vector<float> AssignX, vector<float> AssignY, vector<float> AssignDY){
    Name = AssignName;
    if (Name.find("Pandora_Best_X_Shifted") != string::npos) Name = std::regex_replace(Name, std::regex("Pandora_Best_X_Shifted"), "ShiftedPandoraX");
    x = AssignX;
    y = AssignY;
    dy = AssignDY;
    dx_check = false;
    dy_check = true;
  }
  MyDataset(string AssignName, vector<float> AssignX, vector<float> AssignDX, vector<float> AssignY, vector<float> AssignDY ){
    Name = AssignName;
    x = AssignX;
    dx = AssignDX;
    y = AssignY;
    dy = AssignDY;
    dx_check = true;
    dy_check = true;
  }
  void Print(){
    std::cout << "---------------------------------------- \t " << Name << "\t ----------------------------------------" << std::endl;
    string OutLine;
    for(int cont = 0; cont < x.size(); cont++){
      OutLine = "x = " + to_string(x[cont]);
      if(dx_check == true) OutLine = OutLine + " +- " + to_string(dx[cont]);
      OutLine = OutLine + "\t y = " + to_string(y[cont]);
      if(dy_check == true) OutLine = OutLine + " +- " + to_string(dy[cont]);
      cout << OutLine << endl;
      OutLine.erase();
    }
    std::cout << "---------------------------------------- ++++++++++++++++++++ ----------------------------------------" << std::endl;
  }
};

vector<MyDataset> multipleFileLoadVData( string Path, vector<string> FileNameVector_All, vector<string> VCondition, vector<string> VConditionNot, std::string TypePlot ){
  bool ConditionCheck;
  vector<MyDataset> VData;
  int IndexX = 0;
  int IndexY;
  if(TypePlot == "constant")
    IndexY = 1;
  if(TypePlot == "mu")
    IndexY = 3;
  if(TypePlot == "sigma")
    IndexY = 5;
  for(string& FileName : FileNameVector_All){
    ConditionCheck = true;
    FileName = Path + FileName;
    for(string& Condition : VCondition){
      if( (Condition == "ShiftedPandoraX") || (Condition == "shiftedpandoraX"))  Condition = "Pandora_Best_X_Shifted";
      if( (Condition == "ArOnly") || (Condition == "aronly") || (Condition == "arOnly"))  Condition = "flashmatchAnaArOnly";
      if( (Condition == "10ppm") )  Condition = "flashmatchAna10ppm";
      if (FileName.find(Condition) == string::npos) ConditionCheck = false;
      }
    for(string& ConditionNot : VConditionNot){
      if( (ConditionNot == "ShiftedPandoraX") || (ConditionNot == "shiftedpandoraX"))  ConditionNot = "Pandora_Best_X_Shifted";
      if( (ConditionNot == "ArOnly") || (ConditionNot == "aronly") || (ConditionNot == "arOnly"))  ConditionNot = "flashmatchAnaArOnly";
      if( (ConditionNot == "10ppm") )  ConditionNot = "flashmatchAna10ppm";
      if (FileName.find(ConditionNot) != string::npos) ConditionCheck = false;
      }
    if(ConditionCheck == true){
      string tempString = FileName;
      eraseSubStr(tempString, "../Output/ParametersFile/GaussParametersFromDeltaE_flashmatchAna");
      eraseSubStr(tempString, ".txt");
      VData.push_back( MyDataset(tempString, FileName, IndexX, IndexY, IndexY+1) );
    }
  }
  return VData;
}

void PlotAndSave(vector<MyDataset> VData, string Title, string Path, string TypePlot, string XTitle = "Energy [MeV]"){
  auto *c1 = new TCanvas("c1", "c1", 3000, 1000);
  c1->cd();
  TMultiGraph  *mg  = new TMultiGraph();
  if (Title.find(".txt") != string::npos){
    Title = "All" + Title;
    eraseSubStr(Title, ".txt");
  }
  mg->SetTitle(("#" + TypePlot +" - "+ Title).c_str());
  TLegend *leg = new TLegend(0.7, 0.7, 0.3, 0.9);
  //leg->SetHeader("The Legend Title","C"); // option "C" allows to center the header
  leg->SetFillColor(0);
  leg->SetHeader("TextFile:");
  leg->Draw();
  string FitVarName;
  for(int i = 0; i < VData.size(); i++){
    MyDataset Data = VData[i];
    if(VData.size() > 2) FitVarName = "["+Data.Name+"_Mean"+"]";
    else{
      if(Data.Name.find("ArOnly") < 5){
        //cout << "-----------------------------------------------Found ArOnly-------------------------------------------------------" << endl;
        FitVarName = "[ArOnly_Mean]";
        //cout << "Data.Name = " << Data.Name << "\t" <<  "FitVarName = " << FitVarName << "\t" << endl;
        //cout << "------------------------------------------------------------------------------------------------------" << endl;
        }
      else if(Data.Name.find("10ppm") < 5) {
        //cout << "----------------------------------------------Found 10ppm-------------------------------------------------------" << endl;
        FitVarName = "[10ppm_Mean]";
        //cout << "Data.Name = " << Data.Name << "\t" <<  "FitVarName = " << FitVarName << "\t" << endl;
        //cout << "------------------------------------------------------------------------------------------------------" << endl;
      }
      else {
        //cout << "----------------------------------------------Found Something-------------------------------------------------------" << endl;
        FitVarName = "["+Data.Name+"_Mean"+"]";
        //cout << "Data.Name = " << Data.Name << "\t" <<  "FitVarName = " << FitVarName << "\t" << endl;
        //cout << "------------------------------------------------------------------------------------------------------" << endl;
      }
    }
    TF1  *f1 = new TF1(Data.Name.c_str(), FitVarName.c_str(), 0, 2500);
    TGraphErrors *graph = new TGraphErrors(Data.x.size(), Data.x.data(), Data.y.data(), nullptr, Data.dy.data());
    graph -> SetTitle(Title.c_str());
    graph -> SetLineWidth(2);
    //if((i == 0) || (i == 10) || (i == 2) ) graph -> SetLineColor(i+25);
    if( i == 4 ) {
      f1 -> SetLineColor(i+10);
      graph -> SetLineColor(i+10);
      graph -> SetMarkerColorAlpha(i+10 , 1);
      }
    else {
      f1 -> SetLineColor(i+1);
      graph -> SetLineColor(i+1);
      graph -> SetMarkerColorAlpha(i+1 , 1);
    }
    graph -> Fit(f1);
    FitVarName = FitVarName + " = " + to_string(f1->GetParameter(0)) + "#pm" + to_string(f1->GetParError(0));
    gStyle->SetOptFit(0000);
    graph -> SetMarkerStyle(21);
    mg -> Add(graph);
    eraseSubStr(Data.Name, "_FFinal");
    leg->AddEntry(graph, Data.Name.c_str(), "lpe");
    leg->AddEntry("f1", FitVarName.c_str(), "");
    delete f1;
  }
  mg -> GetXaxis()-> SetTitle(XTitle.c_str());
  mg -> GetYaxis()-> SetTitle(("#"+TypePlot).c_str());
  mg -> Draw("AP");
  leg -> DrawClone("Same");
  cout << "Save again with: \n \t c1 -> SaveAs( \""<< (Path+TypePlot+"_"+Title+".jpg") <<"\" )"<< endl;
  c1 -> SaveAs((Path+TypePlot+"_"+Title+".jpg").c_str());
  delete leg;
}

int Test(){
  string Path = "../Output/ParametersFile/";
  vector <string> FileNameVector_All = {
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedAllArOnlyParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedAllParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedAronlyP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_NothingFixed.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_FixedAllArOnlyParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_FixedAllParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_FixedAronlyP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_FixedP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_NothingFixed.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedAllArOnlyParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedAllParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedAronlyP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_NothingFixed.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_FixedAllArOnlyParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_FixedAllParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_FixedAronlyP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_FixedP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_NothingFixed.txt",
    };
  vector<vector<float>> Matrix = ParseCSVToFloatMatrix(Path + FileNameVector_All[0]);
  PrintMatrix(Matrix);
  MyDataset Data = MyDataset(FileNameVector_All[0], ParseMatrixColumn(Matrix, 1), ParseMatrixColumn(Matrix, 2));
  Data.Print();
  // vector<float> Column = ParseMatrixColumn(Matrix, 1);
  // PrintVector(Column);
  return 0;
}

int ParametersFilePlot(vector <string> VCondition = {}, vector <string> VConditionNot = {}, int EnergyInterpolation = 0, int TypePlotIndex = -1){
  string InputPath = "../Output/ParametersFile/";
  string SavePath = "../Graphs/ParametersFile/";
  vector <string> FileNameVector_All_500 = {
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedAllArOnlyParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedAllParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedAronlyP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_NothingFixed.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_FixedAllArOnlyParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_FixedAllParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_FixedAronlyP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_FixedP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_NothingFixed.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedAllArOnlyParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedAllParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedAronlyP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_NothingFixed.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_FixedAllArOnlyParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_FixedAllParametersFrom500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_FixedAronlyP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_FixedP2From500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_NothingFixed.txt",
    };
  vector <string> FileNameVector_All_1500 = {
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedAllParametersFrom1500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedAllArOnlyParametersFrom1500MeV_FFinal_AddedScaleFactor.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_Pandora_Best_X_Shifted_FixedAllArOnlyParametersFrom1500MeV_FFinal.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_FixedAllArOnlyParametersFrom1500MeV_FFinal_AddedScaleFactor.txt",
    "GaussParametersFromDeltaE_flashmatchAna10ppm_TrueX_FixedAllArOnlyParametersFrom1500MeV_FFinal.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedAllParametersFrom1500MeV.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedAllArOnlyParametersFrom1500MeV_FFinal_AddedScaleFactor.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_Pandora_Best_X_Shifted_FixedAllArOnlyParametersFrom1500MeV_FFinal.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_FixedAllArOnlyParametersFrom1500MeV_FFinal_AddedScaleFactor.txt",
    "GaussParametersFromDeltaE_flashmatchAnaArOnly_TrueX_FixedAllArOnlyParametersFrom1500MeV_FFinal.txt",
  };
  vector <string> FileNameVector_All;
  vector <string> VTypePlot = {"constant", "mu", "sigma"};
  vector <string> VFitType = {
    "NothingFixed",                                       // 0
    "FixedAllParametersFrom500MeV",                       // 1
    "FixedAllArOnlyParametersFrom500MeV",                 // 2
    "FixedP2From500MeV",                                  // 3
    "FixedAronlyP2From500MeV",                            // 4
    "FixedAllArOnlyParametersFrom500MeV_AddedScaleFactor",  // 5
    "FixedAllArOnlyParametersFrom1500MeV",                //6
    "FixedAllArOnlyParametersFrom1500MeV_FFinal_AddedScaleFactor" //7
    };
  vector <string> Vxname = { "TrueX",                     // 0
                             "ShiftedPandoraX"            // 1
                           };
  vector <string> VFolderName = {
                                  "flashmatchAnaArOnly",  // 0
                                  "flashmatchAna10ppm"    // 1
                                };
  if( VCondition.size() == 0 ){
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Call must be of the type: ParametersFilePlot( VCondition, VConditionNot, EnergyInterpolation, TypePlotIndex)" << endl;
    cout << "ERROR, VCONDITION MUST CONTAIN AT LEAST ONE ACCEPTED CONDITION; IF YOU WANT TO PLOT ALL THE AVAILABLE DATA CALL THIS FUNCTION WITH  ParametersFilePlot({\"All\"}) " << endl;
    cout << "available conditions are: " << endl;
    cout << "\t XVariable ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ " << endl;
    for(auto & tempX : Vxname) cout << "\t \"" << tempX << "\""<< endl;
    cout << "\t Type of fit ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ " << endl;
    for(auto & tempFit : VFitType) cout << "\t \"" << tempFit << "\""<< endl;
    cout << "\t Folder selection ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ " << endl;
    for(auto & tempFolder : VFolderName) cout << "\t \"" << tempFolder << "\""<< endl;
    cout << "\t ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ " << endl;
    cout << "those can also be passed as NEGATIVE CONDITION, VConditionNot CAN be empty" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    exit(-1);
    }
  else if( VCondition.at(VCondition.size()-1) == "All" ){
    VCondition.clear();
    VCondition.push_back(".txt");
    }
  if( (TypePlotIndex == -1) || (TypePlotIndex >= VTypePlot.size()) ){
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Call must be of the type: ParametersFilePlot( VCondition, VConditionNot, EnergyInterpolation, TypePlotIndex)" << endl;
    cout << "ERROR, TypePlotIndex MUST BE INDICATED" << endl;
    cout << "available values are: " << endl;
    for(int temp = 0; temp < VTypePlot.size(); temp ++) cout << "\t" << temp << "\t" << VTypePlot[temp] << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    exit(-1);
  }

  if(EnergyInterpolation == 500){
    FileNameVector_All = FileNameVector_All_500;
  }
  else if(EnergyInterpolation == 1500){
    FileNameVector_All = FileNameVector_All_1500;
  }
  else{
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Call must be of the type: ParametersFilePlot( VCondition, VConditionNot, Energy, TypePlotIndex)" << endl;
    cout << "ERROR, EnergyInterpolation MUST BE INDICATED" << endl;
    cout << "available values are: \n \t\t 500 \n\t\t 1500" << endl;
    exit(-1);
  }
  string TypePlot = VTypePlot[TypePlotIndex];
  // vector <string> VCondition = {
  //   VFitType[0],
  //   }; // Qui metti le condizioni per scegliere quali file plottare
  // vector <string> VConditionNot = {
  //   }; // Qui metti le condizioni per scegliere quali file plottare
  //
  string Title;
  for(string& Condition : VCondition) Title = Title + "_" + Condition;
  Title.erase(0,1); // Cancella la prima _ di Title
  if(VConditionNot.size() > 0) Title = Title + "__NOT_";
  for(string& Condition : VConditionNot) Title = Title + "_" + Condition;
  vector<MyDataset> VData = multipleFileLoadVData( InputPath, FileNameVector_All, VCondition, VConditionNot, TypePlot );
  eraseSubStr(Title, "_FFinal");
  PlotAndSave( VData, Title, SavePath, TypePlot);
  return 0;
}
