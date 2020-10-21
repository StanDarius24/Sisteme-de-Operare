fis1=$1
fis2=$2

newline='
'
nr=1
listaemail=`cut -f 1 -d ";" $fis1`
listaip=`cut -f 2 -d ";" $fis1`

iteratii=`cat $fis1| wc -l `
iteratii=`expr $iteratii + 1`
while test $nr -le $iteratii
do
primu=`echo $listaemail | cut -f 1 -d "$newline" | cut -f $nr -d " " `

ipnr1=`echo $listaip | cut -f 1 -d "$newline" | cut -f $nr -d " " `

nr=`expr $nr + 1`

primcopie=`echo $primu | wc -m` 

echo $primu

primu=`echo $primu | egrep -v [+] | egrep "[.com|.ro|.eu]$"  | egrep  "^[^0-9]"  `
echo "$primu"

primu=`echo $primu | wc -m `

if test $primu -eq $primcopie
then
	echo "da"
else
	echo "nu"
fi



done