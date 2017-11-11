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





