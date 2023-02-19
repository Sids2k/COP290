default:
	gcc -Wall -c ../src/hm.c      
	gcc -Wall -c ../src/list.c
	gcc -Wall -c ../src/mythread.c
	gcc -Wall -c ./main.c
	gcc -pthread -o prog hm.o list.o mythread.o main.o
	./prog ../data/in1.txt ../data/in2.txt ../data/in3.txt ../data/in4.txt
	
report:
	pdflatex texample.tex


