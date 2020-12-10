
if test $# -lt 1
then
	echo "Nr de argumente prea mic $0 arg1 arg2..."
else
	 
	gcc -Wall -o exec test2ex4.c
val=0
n=0
	while read line
	do
		val=$( ./exec $1 $line | wc -w ) 
		
		n=`expr $n + $val`

	done


echo $n

fi
