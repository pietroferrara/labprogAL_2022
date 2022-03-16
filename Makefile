all:
	g++ -Wall lecture11_BPS.cpp -o main_executable.exe      
debug:
	g++ -g -Wall lecture11_BPS.cpp -o debug_executable.exe
clean:
	rm main_executable.exe debug_executable.exe
