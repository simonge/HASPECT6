{
  //optional Set input file directory, could be done with --hsin or setenv HSIN
  //HSin("INPUT_FILE_DIR");
  //optional set ouput directory or file.
  //Directory=>1 out file for every in file
  //file=> 1 total output file
  //HSout("FULL_PATH_OUTPUT_FILE_DIR_OR_FILE");
  
  //Set up the input chain to be analysed,could remove HSin() if you want
  TChain* tree=new TChain("TREENAME","datachain");
  tree->Add(HSin()+"FILENAMES");

  //if using proof
  //Give proof destination file (env variables can't be read directly
  if(gProof) {gProof->AddInput(new TNamed("HSOUT",HSout().Data()));tree->SetProof();}


  tree->Process("SELECTOR");
  
  //Not actually sure if this is needed or not...
  if(gProof)gProof->ClearData(TProof::kUnregistered | TProof::kForceClear);
}
