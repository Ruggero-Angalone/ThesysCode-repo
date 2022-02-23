## ThesysCode-repo
# A collection of some of the code I wrote for my MsC thesys, the things that I think might be usefull for future students
- Activation.sh launches the environments for Larsoft
- DuneLogin.sh is used to login to the Dune nodes, takes as input the type and the number of the node you want to login to. The following are to be set by the user due to security concerns
  - Set UName and PWD to change the user
  - Add the run and build name
  - Add the domain name to ConnectionID (e.g.: ${UName}@${ServerType}${ServerNumber}.SomeDomain.SomeOtherDomain)
- MultipleEnergyLaunch.sh launches the gen->ana cycle for multiple values of the energy and moves the generated files in pnfs, modify:
  - EventNumber=500 <- How many events are generated
  - EnergyValues=(0.1 0.3 1.5)                                                                      <- Array of the energy in GeV
  - UserName=MyUserName                                                                             <-Set this to set the path in the subfolders
  - FclPath="fcl/"                                                                                  <- Set the path for the fcl files
  - OutPath="${UserName}/Output/"                                                                   <- Set the path where the output file are generated
  - OutTxtPath="${UserName}/larsoft_mydev/MyCode/Output/"                                           <- Set the path for the output of the lar commands
  - OutName=("OutGen" "OutG4" "OutDetsim" "OutReco" "OutAna")                                       <- Set the name of the output files #OutName and FclFilesName must both be ordered
  - TextLineOriginal="#All values writtten as @<name>@ are made to be changed with a sed command"   <- Comment line that gets changed in the gen file by sed
  - FilePrototype="${FclPath}GEN_prototype.fcl"                                                     <- Name of the prototype of the gen.fcl file missing the energy; the energy is inserted with "sed "s|@NEvents@|$EventNumber|g;s|@Energy@|$E|g;s|$TextLineOriginal|$TextLineSubstitute|g;" $FilePrototype > "fcl/${GenName}"
  - PnfsPath="pnfs/users/${UserName}/Output/"                                                        <- Path in which the .root files are moved after the process; the ana.root file is copied
