#!/bin/sh

svn up /home/rathena/trunk/

rev_master=`svnversion /home/rathena/trunk/`
perl -pi -e 's/PROJECT_NUMBER         = .*$/PROJECT_NUMBER         = '$rev_master'/g' /home/rathena/doxygen/rathena.ini

doxygen /home/rathena/doxygen/rathena.ini

rm -rf /home/project-web/rathena/htdocs/doxygen/
mv /home/project-web/rathena/htdocs/html /home/project-web/rathena/htdocs/doxygen

echo "Done generating Doxygen for rathena $rev_master"
