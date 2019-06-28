all:
	g++ -o start.exe start.cpp -lpthread
	g++ -o pipe.exe pipe.cpp -lpthread
	g++ -o sh.exe sh.cpp -lpthread
	g++ -o sh_client.exe sh_client.cpp -lpthread
	rm -rf *.txt
clean:
	rm -rf *.txt *.exe

