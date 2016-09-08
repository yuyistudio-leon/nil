make 2> .err 1> /dev/null
if [ $? -ne 0 ]; then
	clear
	head .err -n 8
	echo 'compile error'
	exit
fi
count_warning=`cat .err | wc -l`
if [ $count_warning -gt 0 ]; then
	clear
	echo warnings found
	cat .err
	exit 1
fi
./nl.exe $1 $2
