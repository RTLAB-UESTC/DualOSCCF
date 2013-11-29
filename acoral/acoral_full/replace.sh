#!/bin/sh
curdir=`pwd`
echo "back current dir to name-b"
FILES=`find $curdir -name *.[hc]`
echo $FILES
for i in $FILES;do
	sed 's/acoral_u32 time/acoral_time time/g' $i>$curdir/tmp
	mv $curdir/tmp $i
done
