all:
	g++ -o start.exe start.cpp -lpthread
	g++ -o pipe.exe pipe.cpp -lpthread
	gcc -o sh_par.exe sh_par.c 
	gcc -o sh_par_client.exe sh_par_client.c 

clean:
	rm -rf *.txt *.exe

