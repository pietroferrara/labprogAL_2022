all:
	g++ -Wall lecture23_list_template.cpp -o main_executable.exe      
debug:
	g++ -g -Wall lecture23_list_template.cpp -o debug_executable.exe
clean:
	rm main_executable.exe debug_executable.exe
