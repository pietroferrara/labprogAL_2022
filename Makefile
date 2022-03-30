all:
	g++ -Wall lecture14_csvreader.cpp -o main_executable.exe      
debug:
	g++ -g -Wall lecture14_csvreader.cpp -o debug_executable.exe
clean:
	rm main_executable.exe debug_executable.exe
