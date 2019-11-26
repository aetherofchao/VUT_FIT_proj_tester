#
#	test.h 
#
# ██████╗ ██████╗  ██████╗      ██╗███████╗██╗  ██╗████████╗    ██████╗    
# ██╔══██╗██╔══██╗██╔═══██╗     ██║██╔════╝██║ ██╔╝╚══██╔══╝    ╚════██╗   
# ██████╔╝██████╔╝██║   ██║     ██║█████╗  █████╔╝    ██║        █████╔╝   
# ██╔═══╝ ██╔══██╗██║   ██║██   ██║██╔══╝  ██╔═██╗    ██║       ██╔═══╝    
# ██║     ██║  ██║╚██████╔╝╚█████╔╝███████╗██║  ██╗   ██║       ███████╗██╗
# ╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚════╝ ╚══════╝╚═╝  ╚═╝   ╚═╝       ╚══════╝╚═╝
#
#	25.11.2019
#	Alukard

# make proj2.c
gcc -std=c99 -Wall -Wextra -Werror proj2.c -lm -o proj2


#	||==================||
#	||  Input Examples  ||
#	||==================||

# new line
echo;
# what is being run
echo ./proj2 12 47 1e-2;
# runing it
./proj2 12 47 1e-2;
# the corect output
echo Up=0.673828 V;
echo Ip=0.207937 A

echo;
echo ./proj2 12 47 1e-7;
./proj2 12 47 1e-7;
echo Up=0.677633 V
echo Ip=0.240902 A

echo;
echo ./proj2 24 330 1e-7;
./proj2 24 330 1e-7;
echo Up=0.64596 V
echo Ip=0.0707696 A

echo;
echo ./proj2 24 330 1e-20;
./proj2 24 330 1e-20;
echo Up=0.64596 V
echo Ip=0.0707698 A

echo;
echo ./proj2 inf 5 .5;
./proj2 inf 5 .5;
echo Up=inf V
echo Ip=inf A

echo;
echo ./proj2 -33 5 .1;
./proj2 -33 5 .1;
echo error: invalid arguments;

echo ;
echo ./proj2 0 5 .1;
./proj2 0 5 .1;
echo Up=0 V
echo Ip=0 A

echo ;
echo ./proj2 5 inf .1;
./proj2 5 inf .1;
echo Up=0 V
echo Ip=0 A

echo ;
echo ./proj2 0 0 0;
./proj2 0 0 0;
echo error: invalid arguments;

echo ;
echo ./proj2 5 0 0.1;
./proj2 5 0 0.1;
echo error: invalid arguments;


echo ;
echo ./proj2 5 -5 5;
./proj2 5 -5 5;
echo error: invalid arguments;


echo ;
echo ./proj2 5 5 0;
./proj2 5 5 0;
echo error: invalid arguments;

echo ;
echo ./proj2 5 5 -5;
./proj2 5 5 -5;
echo error: invalid arguments;

