if test $# -eq 3
then
	if [[ $1 =~ [0-9]+ && $3 =~ [0-9]+ ]]
	then
		if [[ $2 =~ [+-/%]+ ]]
		then
			res=`expr $1 $2 $3`
			echo "$res"
		else
			echo "$2 nu e operator"
		fi

	else
		echo "$1 sau $3 nu sunt numere"
	fi

else
	if test $# -gt 3
	then
		echo "Prea multe argumente"
	else
		echo "Prea putine argumente"
	fi
fi