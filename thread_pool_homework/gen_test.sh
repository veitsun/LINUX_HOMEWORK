#!/bin/bash
numberfile=test/num1
i=$1
echo $i > $numberfile
while [ $i -ge -$1 ]; do
  ((i=i-1))
  echo $i >> $numberfile 
done
i=$1
numberfile=test/num2
echo $i > $numberfile
while [ $i -ge -$1 ]; do
  ((i=i-1))
  echo $i >> $numberfile
done
i=$1
numberfile=test/num3
echo $i > $numberfile
while [ $i -ge -$1 ]; do
  ((i=i-1))
  echo $i >> $numberfile
done