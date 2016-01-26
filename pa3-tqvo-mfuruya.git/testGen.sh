#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Usage: $0 <num_chars_in_outfile> <outfile>"
    exit 1;
fi

symbols="a b c d e f g h i j k l m n o p q r s t u v w x y z 1 2 3 4 5 6 7 8 9 0 ! @ # $ % ^ & ( ) - _ = + [ ] { } \ | ; : ' \" , . < > / ? ~ "

for i in $( seq 1 $1 )
do
    c=`shuf -e $symbols -n 1`
    printf "%s" $c >> $2
done
