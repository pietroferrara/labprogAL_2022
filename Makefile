all:
	g++ -Wall exercise3.cpp -o main_executable.exe      
debug:
	g++ -g -Wall exercise3.cpp -o debug_executable.exe
clean:
	rm main_executable.exe debug_executable.exe
