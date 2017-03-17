for (( i = 0; i < 50; i++ )) 
do
    for nt in 600 700 800 900 1100 #500 1000 1250 1500 1750 2000 2250 2500 2750 3000
	do
		./myprog.out --numtraces $nt --startsample 400 --numsamples 200 --random --savek
	done
done
#./myprog.out --numtraces 5000 --startsample 400 --numsamples 600 --unknown --random