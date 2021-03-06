# distributed_version_of_the_parallel_Fibonacci_code_generator
These programs are:
The client program:
The user will execute this program using the following syntax:
./exec_filename hostname port_no < input_filename
where exec_filename is the name of your executable file, hostname is the address where the server program is located, port_no is the port number used by the server program, and input_filename is the name of the file with the information about the alphabet. 

This program reads the information about the alphabet (symbols and frequencies) from the input file, sorting the symbols in the alphabet in decreasing order based on the frequency. If two or more symbols have the same frequency, it will use the symbol's ASCII value to break the tie (the higher the value, the higher the priority).

After assigning a positive integer value (starting from 1) to the symbols in the sorted alphabet, the program must create a child thread per line in the input file.  Each child thread will send a request to the server with an integer value greater than 0 to determine the Fibonacci code using stream sockets. After the child threads receive the Fibonacci code from the server, they will write it on a memory location available to the main thread. Finally, the main thread will use the codes generated by the child threads to decompress a file.
Each child thread will execute the following tasks:
Create a socket to communicate with the server program.
Send an integer value greater than zero to the server program using sockets. 
Wait for the Fibonacci code from the server program.
Write the received information into a memory location accessible by the main thread.
Finish its execution.
Input Format: Your program should read its input from stdin (C++ cin) and use input redirection. 

The input file has the following information:
The number of symbols in the alphabet (integer value)
n lines (where n is the number of symbols in the alphabet) with the information about the symbols in the alphabet. Each line has one character and one integer representing a symbol and its frequency (separated by a single white space).
The name of the compressed file.
Example Input File:

7
C 2
O 1
S 1
  1
3 2
6 1
0 1
compfile1.txt
Finally, after receiving the Fibonacci codes from the child threads, the main thread decompresses the contents of a file (sequence of bits represented as a string) and prints the decompressed message.
Given the previous input file and the following compressed file:
111011001111010110110110001110011 
The expected output is:
Decompressed message = COSC 3360
The server program:
The user will execute this program using the following syntax:
./exec_filename port_no
where exec_filename is the name of your executable file, and port_no is the port number to create the socket.
This program receives requests from the child threads of the client program using sockets. It creates a child process per request. For this reason, the parent process needs to handle zombies processes implementing the fireman() call in the primer. Each child process will calculate the Fibonacci code based on the integer value it receives from a child thread from the client program, returning the Fibonacci code to the client program using sockets.
The server program will not print any information to STDOUT.
Notes:
It can safely assume that the input files will always be in the proper format.
The program uses the output statement format based on the example above.
The program implements my solution only using the requested files client.cpp and server.cpp.
