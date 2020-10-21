dir=$1
fis=$2
var=`ls $dir | egrep ".txt" ` 
nr=`echo $var | wc -w`
numar=1
cont=0
while test $numar -le $nr
do
	file=`echo $var | cut -f $numar -d ' '`
	numar=`expr $numar + 1`

	indir=`cat $dir/$file | wc -m`
	echo $indir
	echo $file
	echo "$file $indir" >> $fis
	cont=`expr $cont  + $indir`
	


done	
echo "$cont" >> $fis