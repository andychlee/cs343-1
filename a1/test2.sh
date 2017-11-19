rm r1
rm r2
for ((n=1;n<=7;n++))
do
	for ((j=1;j<=7;j++))
	do
		for ((k=1;k<=7;k++))
		do
			for((m=1;m<=7;m++))
			do
				echo $n $j $k $m 
				./longjmp $n $j $k $m >> r1
				./throwcatch $n $j $k $m>> r2
			done
		done
	done
done
diff r1 r2
