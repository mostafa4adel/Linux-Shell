#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>


int tokenizeString(char *untokenizedInput, char *tokenizedOutput[],char *lastArgument)
{
    
    int count = 0;
    char *token = strtok(untokenizedInput, " ");
    
    while (token != NULL)
    {
        // printf("token = %s and length = %ld\n",token,strlen(token));
        // printf ("Arg number = %d\n" ,count);
        // printf("value to change = %s|\n",tokenizedOutput[count]);
        free(tokenizedOutput[count]);
        tokenizedOutput[count] = malloc(strlen(token)+1);
        strcpy(tokenizedOutput[count], token);
        count = count + 1;
        // printf ("Arg number = %d\n" ,count);
        token = strtok(NULL, " ");
    }
    
    strcpy(lastArgument,tokenizedOutput[count-1]);
    if(strcmp(tokenizedOutput[count-1],"&")==0)
        tokenizedOutput[count-1] = NULL;
    else
        tokenizedOutput[count] = NULL;    
    return count;
}
pid_t processId;


void childTerminated()
{
    
   FILE* fptr =  fopen("logfile.txt", "a");
   if (fptr == NULL)
   {
    //    create  file if the file dosen't exist
       fptr = fopen("logfile.txt","w");
       if(fptr == NULL)
       {
        //    if systems dosen't allow file creatino exit
           printf("EXITING FILE ERROR");
           exit(EXIT_FAILURE);
       }
   }
   char terminationMessage[] ="Child Terminated Successfully\n";
    // printf("%s",terminationMessage);
    fputs(terminationMessage, fptr);
    fclose(fptr);
}

int main()
{
    signal(SIGCHLD,childTerminated);
    
    char *userInput = malloc(1024);
    char **tokenizedInput=malloc(64);
    char *lastArgument = malloc(1024); 
    int numOfArgs;

    
    while (1) 
    {
        
        fgets(userInput, 1024, stdin);
        userInput[strcspn(userInput, "\n")] = ' '; // to remove the newline in the input
        // printf("You Entered = %s|\n",userInput);
        numOfArgs = tokenizeString(userInput, tokenizedInput,lastArgument);
        
        if (strcmp(tokenizedInput[0],"exit")==0)
        {
            printf("Exiting\n");
            break;
        }
        processId = fork();
        
         if(strcmp(tokenizedInput[0],"cd")==0)
        {
            int i = chdir(tokenizedInput[1]);
            if(i==-1)
            {
                printf("No Such Directory");
            }
        }

        if(processId==0)
        {
            // process id should equal 0
            if(strcmp(tokenizedInput[0],"cd")!=0)
            {
                int i=execvp(tokenizedInput[0], tokenizedInput);
                if(i==-1)
                {
                    printf("Error in Command\n");
                }
            }
            
            
            exit(0);
            
        }
        else
        {
            if(strcmp(lastArgument , "&")!=0 )
            {
               // process id should equal child id
                // printf("Waiting for Child \n");   
                wait(NULL);
            }

            
            // printf("Child Executed\n");
        }
        
        
    }

    return 0;
}

