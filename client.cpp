/*The templete of this program is provided by Dr. Rincon that I downloaded from blackboard*/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
#include<string>
using namespace std;
//This function gets the message and show the error.
void error(char *msg)
{
    perror(msg);
    exit(0);
}
//This is the structure for Client message.
struct node{
    char symbol;
    int frequency;
    int assignInt;
    int portno;
    char* hostname;
    string fibonacciCodeWord;
   
};
//This is the structure to keep the server message.(which is the fibonacci code word)
struct serverNode{
    char fibonacciCodeWord[10];
};
//this struct is used to in sort based on frequency.
struct greater_than_key
{
    inline bool operator() (const node& node1, const node& node2)
    {
        if(node1.frequency == node2.frequency)
            return((node1.symbol)>(node2.symbol));
        return (node1.frequency > node2.frequency);
    }
};
//This is the function that each thread is calling.
void *inc_x(void *x_void_ptr)
{
	node *x_ptr = (node *)x_void_ptr;
    int sockfd, portno, n;
    char*e;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    
    portno = (*x_ptr).portno; //gets the port number
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        strcpy(e,"ERROR opening socket");
        error(e);
    }
    server = gethostbyname((*x_ptr).hostname);//gets the host name
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
    {
        strcpy(e,"ERROR connecting");
        error(e);
    }
  
    //put the node in socket
    n = write(sockfd,&(*x_ptr).assignInt,sizeof(int));
    if (n < 0) 
    {
        strcpy(e,"ERROR writing to socket");
        error(e);
    }
    //read the node from sockets
    struct serverNode serverMessage;
    n = read(sockfd,&serverMessage,sizeof(serverNode));
    if (n < 0) 
    {
        strcpy(e,"ERROR reading from socket");
        error(e);
    }
    
    (*x_ptr).fibonacciCodeWord = string(serverMessage.fibonacciCodeWord);//put the fibonacci code word that server sends to each node 
    bzero(buffer,256);
    close(sockfd); //closing the socket
	return NULL;
}
//This function gets a vector, array of string(which is in the file) and the inputfile array
//It returns decomposed message of the input file.
string decompossedFunction(vector<node>clientMessage, string inputFile[], int sizeOfInput)
{
    bool flag = false;
    int counter =0;
    string result;
    for(int i=0; i<sizeOfInput; i++)
    {
        for(int j=0;j< clientMessage.size(); j++)
        {
            if(clientMessage[j].fibonacciCodeWord == inputFile[i])
            {
                result =result + clientMessage[j].symbol;
            }
        }
    }
    return result;
}
int main(int argc, char *argv[])
{
    string inputLine;//to hold line of input
    int numberOfSymbol;//the number of symbols
    getline(cin,inputLine);//read the first line of input
    numberOfSymbol =atoi(inputLine.c_str());//convert the string input to intger
    vector<node> clinetMessage; //this is the message that will be send to the server 
    node in;
    for(int i=0; i<numberOfSymbol;i++) //read the input and put it in clinetMessage vector which contins the info that needs to be send to server
    {
        getline(cin, inputLine);
        in.symbol = inputLine[0];
        in.frequency = (int)(inputLine[2])-48;
        clinetMessage.push_back(in);
    }
    string fileName;//to hold filename
    getline(cin,fileName);
    //sorting the clinetMessage based on frequency
    sort(clinetMessage.begin(),clinetMessage.end(), greater_than_key());
    int counter=1;
    //This is an error that if the input has less than 3 inputs 
    char* e;
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    
    //in this loop assign integer to the each symbol and also portnumber and hostname
    for(int i =0; i<=numberOfSymbol; i++)
    {
        clinetMessage[i].assignInt = counter++;
        clinetMessage[i].portno =atoi(argv[2]);
        clinetMessage[i].hostname = argv[1];
    }
    //Till this point reads the info from input and creating message
    //____________________________________________________


    //_________________________________________________________
    //Create threads to be sent to server 
    pthread_t *tid;
    tid = new pthread_t[numberOfSymbol];
    for(int i=0;i<numberOfSymbol;i++)
	{
		if(pthread_create(&tid[i], NULL, inc_x, &clinetMessage[i])) 
		{
			fprintf(stderr, "Error creating thread\n");
			return 1;
		}	
	}
    //wait for the child thread
    for(int i =0;i<numberOfSymbol;i++)
    {
        pthread_join(tid[i], NULL);	
    }
    delete[] tid;
   //After this we have all the information and have to decompose the input file.
    ifstream inputFile;
    string fileInput;
    inputFile.open(fileName.c_str());
    if (inputFile.is_open())
      inputFile >> fileInput;
    inputFile.close();
    int i =0;
    int c =0;
    string * fileInputArray = new string[100];
    while(fileInput.length()!=0) //This loop divide the text inside the input file and put it inside a string array.
    {
        if(fileInput[i] == '1'&& fileInput[i+1] == '1')
        {
            fileInputArray[c++]= fileInput.substr(0,i+2);
            fileInput.erase(0,i+2);
            i = 0;
        }
        else{
            i++;
        }
    }
    cout<<"Decompressed message = "<<decompossedFunction(clinetMessage,fileInputArray,c)<<endl;
    return 0;
}
