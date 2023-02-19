default:
	gcc -Wall -c ../src/hm.c      
	gcc -Wall -c ../src/list.c
	gcc -Wall -c ../src/mythread.c
	gcc -Wall -c ./main.c
	gcc -pthread -o prog hm.o list.o mythread.o main.o
	time ./prog .../data/notrepeat.txt ../data/notrepeat2.txt
	
report:
	pdflatex texample.tex


