
if test $# -lt 3
then

	echo Usage: bash $0 n file n1 n2 ...
	exit 1
fi
c=0
numebash=$0
var1=$1
numefisier=$2
shift 2
p=1
for ni
do
	p=`expr $p \* $ni`
	rest=`expr $ni % $var1`
	if test $rest -eq 0
	then c=`expr $c + 1`
	fi
done
v=`echo $p | wc -L`
	
if [[ -f $numefisier && ! -h $numefisier ]]
then
	echo "$numefisier este obisnuit"
	echo $v > $numefisier
	echo $c >> $numefisier
else
	echo "nu este obisnuit"

fi


