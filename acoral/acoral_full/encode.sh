#!/bin/sh
curdir=`pwd`
echo $1
cp $1 $curdir/acoral_gbk -rf
cd $curdir/acoral_gbk
rm tools -rf
rm test -rf
rm tags -rf
rm rule.mk -rf
find ./ -name '.svn' -exec rm -rf {} \;
find ./ -name 'Makefile' -exec rm -rf {} \;
find ./ -name 'cfg.in' -exec rm -rf {} \;
find ./ -name 'cfg.mk' -exec rm -rf {} \;
find ./ -name '.config' -exec rm -rf {} \;
find ./ -name '.config.old' -exec rm -rf {} \;
find ./ -name 'cscope*' -exec rm -rf {} \;

FILES=`find . -name *.[hc]`
echo $FILES
for i in $FILES;do
	iconv -f UTF-8 -t GBK $i>tmp
	mv tmp $i
done
