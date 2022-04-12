

myshell : 
	gcc -o myshell *.c ./module/*.c

debug :
	gcc -o debugshell -g *.c ./module/*.c

clean :
	rm myshell debugshell
tar :
	tar -cvzf ${PWD}/../20180546.tar.gz ${PWD}/../myshell
tar_del :
	rm ${PWD}/../20180546.tar.gz
scp :
	scp ${PWD}/../20180546.tar.gz cse20180546@cspro.sogang.ac.kr:/sogang/under/cse20180546