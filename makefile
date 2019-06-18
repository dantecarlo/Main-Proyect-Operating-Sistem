all:
	gcc -o start.exe start.cpp
	gcc -o pipe_pairs.exe pipe_pairs.c
	gcc -o pipe_impairs.exe pipe_impairs.c
	gcc -o sh_par.exe sh_par.c 
	gcc -o sh_impar.exe sh_impar.c 
	gcc -o sh_impar_client.exe sh_impar_client.c 
	gcc -o sh_par_client.exe sh_par_client.c 

clean:
	rm -rf *.txt *.exe

