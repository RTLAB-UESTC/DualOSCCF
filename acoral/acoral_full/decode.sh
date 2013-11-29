#!/bin/sh
FILES=`find $1 -name '*.[sShc]' -exec file {} \;|grep "ISO-8859"|awk -F ":" '{print $1}'`
echo $FILES
for i in $FILES;do
	iconv -f GBK -t UTF-8 $i>tmp
	mv tmp $i
done
