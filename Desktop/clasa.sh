
while read line
do
	 echo $line | grep "[a-zA-Z0-9()+-]*" | grep "^[a-zA-Z0-9(]" | grep "[a-zA-Z0-9)]$" | grep -v "[+(-] [+)-]" | grep -v "[a-zA-Z0-9] [a-zA-Z0-9]" 

	 var=`echo $line | grep "[a-zA-Z0-9()+-]*" | grep "^[a-zA-Z0-9(]" | grep "[a-zA-Z0-9)]$" | grep -v "[+(-] [+)-]" | grep -v "[a-zA-Z0-9] [a-zA-Z0-9]"  | wc -m`
	 
	 if test $var -eq 0
	 then
	 	echo "NU E BINEEE"
	 else
	 	echo "E BINEEES"
	 fi

done
