#!/bin/sh

svn_master='https://rathena.svn.sourceforge.net/svnroot/rathena/trunk/'
svn_local='/home/rathena/trunk/'

TMPFILE=`mktemp -t svn_head.XXXXXX`
svn log -q -r "HEAD" "$svn_master" > $TMPFILE
rev_master=`cat $TMPFILE | sed -ne '/^r[0-9]/{ s/^r//; s/ .*//; p; q; }'`
rm -f $TMPFILE
rev_local=`svnversion $svn_local`

if [ "$rev_local" -lt "$rev_master" ]
then
	echo "Updating mirror from $rev_local to $rev_master ..."
	svn up /home/rathena/trunk/
	
	perl -pi -e 's/PROJECT_NUMBER         = .*$/PROJECT_NUMBER         = '$rev_master'/g' /home/rathena/doxygen/rathena.ini
	
	doxygen /home/rathena/doxygen/rathena.ini
	
	rm -rf /var/www/rathena.org/htdocs/doxygen/
	mv /var/www/rathena.org/htdocs/html /var/www/rathena.org/htdocs/doxygen
	
	echo "Done generating Doxygen for rathena $rev_master"
fi
