rm r1
rm r2
for ((n=1;n<=1000;n++))
do
	echo $n
	./exception $n >> r1
	./flags $n >> r2
done
diff r1 r2
