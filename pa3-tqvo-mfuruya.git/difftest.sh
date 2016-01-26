#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: ./difftest.sh <file to compress and uncompress>"
	exit 1;
fi

#compress the filename specified; write to compressed_outfile
comp=`./compress $1 compressed_outfile`

#uncompress compressed_outfile; write to uncompressed_outfile
uncomp=`./uncompress compressed_outfile uncompressed_outfile`

#diff the original file with uncompressed_outfile
diff=`diff $1 uncompressed_outfile`
echo $diff
