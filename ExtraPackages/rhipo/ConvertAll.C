{
  THipo Hipo;
  Hipo.AddCommandLineFiles();
  Hipo.ToRoot(); //process files given on command line

  //finished exit here so it doesn't try to process hipo files!
  gROOT->ProcessLine(".q");
}
