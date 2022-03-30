all:
	g++ -Wall lecture15_exercise5.cpp -o main_executable.exe      
debug:
	g++ -g -Wall lecture15_exercise5.cpp -o debug_executable.exe
clean:
	rm main_executable.exe debug_executable.exe
