make

if [[ ! -f "./pcodeGen.exe" ]]; then
    exit 1
fi

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m'

declare -i count=0
printf "${YELLOW} Running HW1 Tests: ${NC}\n"
for i in {1..17}; do
    
    ./pcodeGen.exe tests/input$i.in 2> garbage.txt
    mv outputFile.txt outputs/output$i.out
    diff_res=$(diff -w outputs/output$i.out pcode_$i.txt)
    
    if [[ ! -z $diff_res ]]; then
        printf "Test $i: ${RED} DIFF ERROR ${NC}\n"
        printf "$diff_res\n"
    else
        printf "Test $i: ${GREEN} SUCCESS ${NC}\n"
        count+=1
    fi
done
printf "\n$count/17 Tests PASSED!\n\n"

declare -i count=0
printf "${YELLOW} Running HW2 Tests: ${NC}\n"
for i in {21..27}; do
    
    ./pcodeGen.exe tests/input$i.in 2> garbage.txt
    mv outputFile.txt outputs/output$i.out
    diff_res=$(diff -w outputs/output$i.out pcode_$i.txt)
    
    if [[ ! -z $diff_res ]]; then
        printf "Test $i: ${RED} DIFF ERROR ${NC}\n"
        printf "$diff_res\n"
    else
        printf "Test $i: ${GREEN} SUCCESS ${NC}\n"
        count+=1
    fi
done
printf "\n$count/7 Tests PASSED!\n\n"