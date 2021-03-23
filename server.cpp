/*The templete of this program is provided by Dr. Rincon that I downloaded from blackboard*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include<iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
/*Firman() is copied from Dr. Rincon notes*/
void fireman(int)
{
   while (waitpid(-1, NULL, WNOHANG) > 0)
      {   
      }  
}
//This function gets the message and show the error.
void error(char *msg)
{
    perror(msg);
    exit(1);
}
//This is the structure for Client message.
struct node{
    char symbol;
    int frequency;
    int assignInt;
    int portno;
    char* hostname;
    string fibonnaciCodeWord;

};
//This is the structure to keep the server message.(which is the fibonacci code word)
struct serverNode{
    char fibonacciCodeWord[10];
};
//This function gets any number and pointer to the fibonacci table.
//It returns the closes fibonacci number to that number.
int FindClosesFibNum(int n,int*fibTable)
{
    int i = 0;
    while (n >= fibTable[i])
        i++;
    return fibTable[i - 1];
}
//This function gets fibonacci number and pointer to the fibonacci table.
//It returns fibonacci number index
int FindFibIndex(int n,int*fibTable)
{
    int i = 1;
    while (n != fibTable[i])
    {
        i++;
    }
    return i;
}
//This function gets a number and returns fibonacci numbers
int RecursiveFibFunction(int n)
{
    if (n <= 1)
        return n;
    else
        return RecursiveFibFunction(n - 2) + RecursiveFibFunction(n - 1);
}
/*this function fill out the fibonacci table*/
void FillFibTable(int*fibTable)
{
    for (int i = 1; i < 10; i++)
    {
        fibTable[i] = RecursiveFibFunction(i);
    }
}
//This function gets symbol assign integer and pointer to the fibTable and return array of index which contains all fibnocci number that we need for that interger.
int* GenerateFibonacciIndexArray(int assignInt, int* fibTable)
{  
   int reminder = assignInt;
   int x = 0;
   int j = 0;
   int n = assignInt;
   int *fibIndex = new int[10];
   int fibnumber;
    while (reminder != 0)
    {
        fibnumber =FindClosesFibNum(n,fibTable);
        reminder = n - fibnumber;
        fibIndex[j++] = FindFibIndex(fibnumber,fibTable);
        n = reminder;
    }
    return fibIndex;
}
//This function gets pointer to the fibonacci Index array of each number and returns string which is fibonacci Code word
string GenerateFibbonacciCodeWord(int *fibIndex)
{
    string result;
    if(fibIndex[0]==1)
    {
        result ="11";
        return result; 
    }
    int x = fibIndex[0] - 2;
    for (int j = 0; j <= x; j++)
    {
        result += '0';
    }
    result[x] = '1';
    int z = 1;
    while (fibIndex[z]!=0)
    {
        x = fibIndex[z] - 2;
        if (x < -1)
            break;
        if (x == -1)
        {
            result[0] = '1';
            break;
        }
        if (result[x + 1] != '1' && result[x - 1] != '1')
        {
            result[x] = '1';
        }
        z++;
    }
    result = result +'1';
    return result;
}
int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[256];
     
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     char* e;
     if (sockfd < 0) 
     {
        
        strcpy(e,"ERROR opening socket");
        error(e);       
     }
        
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
     {
        
        strcpy(e,"ERROR on binding");
        error(e);
     }
    
    //Creating the fibonacci table 
    int* fibTable = new int[20];
    FillFibTable(fibTable);
    pid_t pid;
    int *fibIndex;
    int assignInt;
    struct serverNode serverMessage;
    listen(sockfd,20);
    clilen = sizeof(cli_addr);
    struct node clientMessage;
    signal(SIGCHLD, fireman);           
    while(true)
    {
      
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
      if (newsockfd < 0) 
      {
        
        strcpy(e,"ERROR on accept");
        error(e);
      }
      pid = fork();
      if (pid == 0) //child process
      {
        n = read(newsockfd,&assignInt,sizeof(int)); //read the socket
        if (n < 0) error(strcpy(e,"ERROR reading from socket"));
        fibIndex = GenerateFibonacciIndexArray(assignInt,fibTable);//create fibonacci index array
        strcpy(serverMessage.fibonacciCodeWord, GenerateFibbonacciCodeWord(fibIndex).c_str());//converting string to array of characters
        n = write(newsockfd,&serverMessage,sizeof(serverMessage));//write to the sockets
        if (n < 0) error(strcpy(e,"ERROR writing to socket"));
        close(newsockfd);
        _exit(0);
      }
    
   }
    delete[] fibTable;
    delete[] fibIndex;
     return 0; 
}
