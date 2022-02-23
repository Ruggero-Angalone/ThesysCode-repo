#! /bin/bash
#change UName and PWD to change the user
UName=YourUname                                                                                                #CHANGEME!
PWD=YourPassword                                                                                               #CHANGEME!
unset autologout
TicketStatus=$(klist 2>&1) #klist's output goes in stderr and not in stdout
printf "Select server type followed by enter: \n \t 1: gpvm \n \t 2: build \n"
read ServerType
if [ $ServerType -eq "1" ];then
     ServerType=""                                                                                             #CHANGEME! Write the run name here CHANGEME!
elif [ $ServerType -eq "2" ];then
     ServerType=""                                                                                             #CHANGEME! Write the build name here CHANGEME!
else
     echo "Unacceptable input"
     ServerType="WrongInput"
     exit 1
fi
echo "Write Server number followed by enter"
read ServerNumber
if [ ${#ServerNumber} -eq 1 ];then
     ServerNumber="0${ServerNumber}"
fi

ConnectionID="${UName}@${ServerType}${ServerNumber}."                                                          #CHANGEME! Add the domain address here CHANGEME!
export ConnectionCheck="${UName}"
if [[ $TicketStatus == *"No credentials cache found"* ]]; then
     expect <<'END_EXPECT'
          spawn kinit -f $env(ConnectionCheck)
          expect "Password for $env(ConnectionCheck): "
          send "$env(PWD)\r"
          expect "Ticket cache"
END_EXPECT
fi
ssh -X $ConnectionID
