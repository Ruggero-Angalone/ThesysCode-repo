// Codice per caricare velocemente i Tree dei file presenti in Output


TTree *T
gDirectory->GetObject("flashmatchAnaArOnly/FlashMatchTree_Test",T)




T->Draw("TrueX-Pandora_Best_X","Pandora_Best_X<500")
htemp->GetXaxis()->SetTitle("TrueX-PandoraX  [cm]")
htemp->SetTitle("TrueX-PandoraX")
c1->SaveAs("../Graph/1800MeV/TrueX_minus_PandoraX_Test.jpg")


T->Draw("TrueX-fHitToX","fHitToX<500")
htemp->GetXaxis()->SetTitle("TrueX-HitToX  [cm]")
htemp->SetTitle("TrueX-HitToX")
c1->SaveAs("../Graph/100MeV/TrueX_minus_HitToX.jpg")


DE_fitted_ShiftedPandoraX_FixedParameters500MeV_500Events_1500MeV.jpg
c1->SaveAs("../Graph/2000MeV/ArOnly/DE_fitted_ShiftedPandoraX_FixedParameters500MeV_500Events_1500MeV.jpg")



htemp->GetXaxis()->SetTitle("#frac{SumPE_{10ppm}}{SumPE_{ArOnly}}")
