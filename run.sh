#==================================================================================================
#Author Name:          Zach Hofmeister
#Author Email:         zachhof@me.com
#==================================================================================================

#!/bin/bash

#Change parameters when using for new program
compilerName="g++-10"
filesToCompile="*.cpp"
exeName="lex"
exeArgs="AssignmentInfo/Samples/SampleInputFile.txt"

#other parameters
echoColor="\e[32m" #Green: \e[32m
secondColor="\e[96m" #Light cyan: \e[32m

#main compilation
echo -e "${echoColor}Compiling files: $filesToCompile as executable: \"$exeName\" with $compilerName" && tput init
$compilerName $filesToCompile -g -o $exeName

echo -e "${echoColor}Running: ./$exeName $exeArgs" && tput init
./$exeName $exeArgs

echo -e "${echoColor}\nRemoving executable..." && tput init
rm $exeName

# # optional extra debug
# echo -e "${echoColor}\nExpected output:" && tput init
# echo -e ${secondColor} && cat AssignmentInfo/Samples/SampleOutputFile.txt && tput init