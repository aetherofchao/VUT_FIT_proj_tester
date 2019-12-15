gcc -std=c99 -Wall -Wextra -Werror proj3.c -o proj3


expected=""

echo ./proj3 --lpath 1 1 bludiste.txt
./proj3 --lpath 1 1 bludiste.txt > pr3/out.out
result=`timeout 5s diff pr3/out.out pr3/l11.out`
if [ "$result" = "$expected" ] ; then
  echo "OK"
fi

echo ./proj3 --lpath 3 7 bludiste.txt 
./proj3 --lpath 3 7 bludiste.txt  > pr3/out.out
result=`timeout 5s diff pr3/out.out pr3/l37.out`
if [ "$result" = "$expected" ] ; then
  echo "OK"
fi

echo ./proj3 --lpath 6 1 bludiste.txt 
./proj3 --lpath 6 1 bludiste.txt  > pr3/out.out
result=`timeout 5s diff pr3/out.out pr3/l61.out`
if [ "$result" = "$expected" ] ; then
  echo "OK"
fi

echo ./proj3 --rpath 1 1 bludiste.txt 
./proj3 --rpath 1 1 bludiste.txt  > pr3/out.out
result=`timeout 5s diff pr3/out.out pr3/r11.out`
if [ "$result" = "$expected" ] ; then
  echo "OK"
fi

echo ./proj3 --rpath 3 7 bludiste.txt 
./proj3 --rpath 3 7 bludiste.txt  > pr3/out.out
result=`timeout 5s diff pr3/out.out pr3/r37.out`
if [ "$result" = "$expected" ] ; then
  echo "OK"
fi

echo ./proj3 --rpath 6 1 bludiste.txt 
./proj3 --rpath 6 1 bludiste.txt  > pr3/out.out
result=`timeout 5s diff pr3/out.out pr3/r61.out`
if [ "$result" = "$expected" ] ; then
  echo "OK"
fi

echo ./proj3 --shortest 1 1 bludiste.txt 
./proj3 --shortest 1 1 bludiste.txt > pr3/out.out
result=`timeout 5s diff pr3/out.out pr3/s11.out`
if [ "$result" = "$expected" ] ; then
  echo "OK"
fi

echo ./proj3 --shortest 3 7 bludiste.txt 
./proj3 --shortest 3 7 bludiste.txt > pr3/out.out
result=`timeout 5s diff pr3/out.out pr3/s37.out`
if [ "$result" = "$expected" ] ; then
  echo "OK"
fi

echo ./proj3 --shortest 6 1 bludiste.txt 
./proj3 --shortest 6 1 bludiste.txt > pr3/out.out
result=`timeout 5s diff pr3/out.out pr3/s61.out`
if [ "$result" = "$expected" ] ; then
  echo "OK"
fi


