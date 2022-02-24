all:
	g++ -Wall hello.cpp -o main_executable.exe      
debug:
	g++ -g -Wall hello.cpp -o debug_executable.exe
clean:
	rm main_executable.exe debug_executable.exe
