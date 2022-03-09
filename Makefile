all:
	g++ -Wall calculator.cpp -o main_executable.exe      
debug:
	g++ -g -Wall calculator.cpp -o debug_executable.exe
clean:
	rm main_executable.exe debug_executable.exe
