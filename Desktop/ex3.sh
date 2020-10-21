
var=$(date | cut -f 1 -d ':' | cut -f 5 -d ' ')
if [[ $var -gt 6 && $var -lt 12 ]]
then
	d="buna dimineata"
else
	if [[ $var -gt 12 && $var -lt 18 ]]
	then
		d="buna ziua"
	else
		if [[ $var -gt 18 && $var -lt 22 ]]
		then
			d="buna seara"
		else
			d="noapte buna"
	
		fi
	fi
fi
s=$(whoami)

echo "$d $s!"