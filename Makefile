# -*- MakeFile -*-

#target: dependencies
#	action

#Este target va a compilar todos los archivos
all:
	gcc primos.c -o primos -l pthread
	gcc primosn.c -o primosn -l pthread

clean:
	rm -rf *o primos
	rm -rf *o primosn
	
rmfiles:
	rm -rf 1.txt
	rm -rf 2.txt
	rm -rf 3.txt
	rm -rf 4.txt
	rm -rf 5.txt
	rm -rf 6.txt
	rm -rf 7.txt
	rm -rf 8.txt
	rm -rf 9.txt
	rm -rf 10.txt





