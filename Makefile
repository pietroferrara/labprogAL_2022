all:
	g++ -Wall lecture22_templates.cpp -o main_executable.exe      
debug:
	g++ -g -Wall lecture22_templates.cpp -o debug_executable.exe
clean:
	rm main_executable.exe debug_executable.exe
