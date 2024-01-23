if [[ $1 == "0" ]]
then
    ./BMP_converter --help
elif [[ $1 == "1" ]]
then
    ./BMP_converter --encode ./txtfiles/t8.shakespeare.txt ./outfile/shake.bmp
    code ./outfile/shake.bmp
elif [[ $1 == "2" ]]
then
    ./exec_demo --decode ./outfile/shake.bmp ./txtfiles/shake.txt
    diff -uN ./txtfiles/t8.shakespeare.txt ./txtfiles/shake.txt > diffShake.txt
    code diffShake.txt
elif [[ $1 == "3" ]]
then
    ./exec_demo --encode ./txtfiles/little.txt ./outfile/little.bmp
    code ./outfile/little.bmp
elif [[ $1 == "4" ]]
then
    ./exec_demo --decode ./outfile/little.bmp ./txtfiles/little_output.txt
    code ./txtfiles/little_output.txt
    diff -uN ./txtfiles/little.txt ./txtfiles/little_output.txt
elif [[ $1 == "5" ]]
then
    ./exec_demo --encode BMP_converter ./outfile/executable.bmp
    code ./outfile/executable.bmp
    ./exec_demo --decode ./outfile/executable.bmp executable_test_random
    diff -uN executable_test_random BMP_converter > diffExec.txt
    code diffExec.txt
elif [[ $1 == "6" ]]
then
    make re
    ./BMP_converter --encode BMP_converter ./outfile/executable.bmp
    ./BMP_converter --decode ./outfile/executable.bmp exec_demo
    chmod +x exec_demo
elif [[ $1 == "clean" ]]
then
    rm -f ./outfile/executable.bmp
    rm -f ./exec_demo
    rm -f ./executable_test_random
    rm -f ./diffExec.txt
    rm -f ./txtfiles/little_output.txt
    rm -f ./outfile/little.bmp
    rm -f ./txtfiles/shake.txt
    rm -f ./outfile/shake.bmp
    make fclean
fi
