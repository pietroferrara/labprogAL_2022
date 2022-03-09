all:
	g++ -Wall lecture9_calculator.cpp -o main_executable.exe      
debug:
	g++ -g -Wall lecture9_calculator.cpp -o debug_executable.exe
clean:
	rm main_executable.exe debug_executable.exe
