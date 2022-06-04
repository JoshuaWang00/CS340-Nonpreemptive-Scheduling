//JOSHUA WANG
#include <unistd.h>         
#include <sys/types.h>  
#include <stdio.h>   
#include <string.h>         
#include <stdlib.h>   
#include <pthread.h>   
#include <fcntl.h>


//gcc WANG_23764990.c -o WANG_23764990.exe


//initialize our threads and mutex
  pthread_t tid[2];
pthread_mutex_t lock;
//create functions that we will use in our pointer array
int power(int a, int b){
        int finalProduct = a;
        //power of 0 always gives one
        if (b == 0){return 1;}
    
        //return fraction if negative power
        if (b < 0){
         for (int i = 1; i < b; i++){
        finalProduct = finalProduct * a;
        }
            return (1/finalProduct);           
        }
        //multiply our first parameter b times
        for (int i = 1; i < b; i++){
        finalProduct = finalProduct * a;
        }
        return finalProduct;  
}


int sum(int a, int b){
        int summation = a;
        //adds each value cumulatively from a to b. ++i makes sure i is a value ahead
for (int i = a+1; i <= b; ++i){
        summation = summation + i;
        }
 return summation;              
}


int fibonacci(int a, int b){
//in order to calculate fibonacci, we need to have the first three in the sequence. n is fib, n-1 is recentfib, n-2 is pastfib
        int fib = 1;
        int pastFib = 0;
        int recentFib = 1;
        //every fibonacci value is the sum of its past two sequences. all three must be updated to their next sequence to properly work
for (int i = 0; i < b-1; i++){
        fib = pastFib + recentFib;
        pastFib = recentFib;
        recentFib = fib;
        }
return fib;
}
//this function is factorial, multiplies the sequence of a up to b
int product(int a, int b){
  int prod = a;
for (int i = a+1; i <= b; ++i){
        prod = prod * i;
        }
 return prod;  
}
//create miniPCB which will represent our processes. each line read in our input file, then dispatched in our scheduler will be stored using a miniPCB
struct miniPCB{
        int a;
        int b;
        int bTime;
        int prio;
        int process;
        int (*myFuncs[4])(int a, int b);
        char op[20];
        int result;
};
//since pthreads can only pass in 1 parameter in its 4th argument, a struct must be used to pass in multiple.
struct processArgs{
char stringTxt[400];
 char arg1[40];
        char arg2[40];
        char arg3[40];
};


//need 2 different comparison sort functions for sjf and priority quicksorts
int prioCompare(const void *a, const void *b){
const struct miniPCB *procA = a;
const struct miniPCB *procB = b;
//values with high priority aka lower prio number will end up at bottom(first) of our queue and vice versa
        if(procA->prio < procB->prio){return -1;}
        else if(procA->prio > procB->prio){return 1;}
        else return 0;
}
int burstCompare(const void *a, const void *b){
const struct miniPCB *procA = a;
const struct miniPCB *procB = b;
//works similarly to priority comparison except it uses lower burst time as a priority
        if(procA->bTime < procB->bTime){return -1;}
        else if(procA->bTime > procB->bTime){return 1;}
        else return 0;
}


void FCFS(struct miniPCB rQ[]){
//since fcfs already comes sorted in order no scheduling needs to be done.
           return;   
}
//restructures our readyQueue to prioritize low burst time
void SJF(struct miniPCB rQ[]){
//SJF starts with shortest burst time
        qsort(rQ, 15, sizeof(struct miniPCB), burstCompare);
        return;
}
//restructures readyQueue to prioritize the best priority
void PRIORITY(struct miniPCB rQ[]){
qsort(rQ, 15, sizeof(struct miniPCB), prioCompare);
        return;
}


        //our fifo will be a queue of processes aka miniPCBs. size 15 since each input file only has 15 lines/processes
struct FIFO{
struct miniPCB procInfo[15];
        int rear;
        int front;
        //it is important to track the rear, so we can increment it on send and decrement on receive
} myFIFO = {
.rear = 0,
        .front = 0
};
//send to fifo rear and increments to new rear
void send(struct miniPCB process){
myFIFO.procInfo[myFIFO.rear] = process;
        myFIFO.rear++;
        return;
}
//save the fifo front to be returned, remove front in the queue by decrementing all processes in positions by one. since last value has no higher value to decrement, set its operation value to null instead
struct miniPCB receive(){
        if (myFIFO.procInfo[myFIFO.front].op[0]=='\0'){
        printf("Nothing in stack to receive");
        }
    
        struct miniPCB frontProcess = myFIFO.procInfo[myFIFO.front];
        //all values decremented
        for (int i = 0; i < 14; i++){
        myFIFO.procInfo[i] = myFIFO.procInfo[i+1];
        }
        //declare our last struct operation as "null"
        myFIFO.procInfo[14].op[0]= '\0';
        myFIFO.rear--;
        return frontProcess;
    
}
//schedule and dispatcher function to be run in first pthread
void* schedDispatcher(void* arg){
printf("Test: Begin sched/dispatch pthread\n");
//lock our mutex so logger won't receive from empty fifo and concurrently access same critical section as this thread
pthread_mutex_lock(&lock);
struct processArgs *prArgs = (struct processArgs*)arg;
printf("Test: Mutex has just been locked. \n");


//initialize our function pointers
        void (*sched[3]) (struct miniPCB rQ[]) = {FCFS, SJF, PRIORITY};
        int (*funcs[4]) (int a, int b) = {power, sum, fibonacci, product};
    
    
        printf("Test: Function pointer arrays created \n");
        //here we set up our ready queue aka array of miniPCBS
        struct miniPCB readyQueue[15];
//in order to figure out how to schedule, we will need to parse the input files. We check what our first system argument is so we know what kind of parse we track


        //parse string depending on FCFS
            if (strcmp(prArgs->arg1, "FCFS") == 0){
        printf("Test: Begin FCFS parse\n");
            int process[15];
            //15 of length 20 string
            char ops[15][20];
            int ar1[15];
            int ar2[15];


           int i = 0;
           int x = 0;
           //get each string between a comma or newline. we use these tokens to split our lines into attributes. we tokenize until reaching the end of our file string.
           char* token = strtok(prArgs->stringTxt,",\n");
            while (token != NULL) {


            //each line can be tokenized to 4 attributes, so we can use %4 to know which attribute fits which
                if (i % 4 == 0){x = i/4;process[x] = atoi(token);                
                }
                else if (i % 4 == 1){x = (i-1)/4;strcpy(ops[x],token);}
                else if (i % 4 == 2){x = (i-2)/4;ar1[x] = atoi(token);}
                else if (i % 4 == 3){x = (i-3)/4;ar2[x] = atoi(token);                
 }
                i++;
            token = strtok(NULL, ",\n");               
        }
        //fill each process in ready queue with the 4 attributes of each line
         for (int j = 0; j < 15; j++){
            readyQueue[j].a = ar1[j];
            readyQueue[j].b = ar2[j];
            readyQueue[j].process = process[j];
            strcpy(readyQueue[j].op, ops[j]);
}   
        }
        //parse string depending on SJF, order is process#, burst time, op name, input param 1, 2
        if (strcmp(prArgs->arg1, "SJF") == 0){
        printf("Test: Begin SJF parse\n");
            int process[15];
            int bTime[15];
            //15 of length 20 string
            char ops[15][20];
            int ar1[15];
            int ar2[15];


           int i = 0;
           int x = 0;
           char* token = strtok(prArgs->stringTxt,",\n");
           //parsing process is nearly identical to FCFS, except sfj processes need to track an extra attribute, burst time. therefore we use %5 increment a process every 5 attributes
            while (token != NULL) {


                if (i % 5 == 0){x = i/5;process[x] = atoi(token);                
                }
                else if (i % 5 == 1){x = (i-1)/5;bTime[x] = atoi(token);}
                else if (i % 5 == 2){x = (i-2)/5;strcpy(ops[x],token);}
                else if (i % 5 == 3){x = (i-3)/5;ar1[x] = atoi(token);}
                else if (i % 5 == 4){x = (i-4)/5;ar2[x] = atoi(token);                
 }
                i++;
            token = strtok(NULL, ",\n");               
        }
        //fills each process with 5 attributes
         for (int j = 0; j < 15; j++){
            readyQueue[j].a = ar1[j];
            readyQueue[j].bTime = bTime[j];
            readyQueue[j].b = ar2[j];
            readyQueue[j].process = process[j];
            strcpy(readyQueue[j].op, ops[j]);
   }
    
        }


        //parse string depending on priority, order is process, priority, operation, param 1, 2
        if (strcmp(prArgs->arg1, "PRIORITY") == 0){
        printf("Test: Begin PRIORITY parse\n");
            int process[15];
            int prio[15];
            //15 of length 20 string
            char ops[15][20];
            int ar1[15];
            int ar2[15];


           int i = 0;
           int x = 0;
           //parsing process more similar to sjf, except now we track priority instead of burst time
           char* token = strtok(prArgs->stringTxt,",\n");
            while (token != NULL) {


                if (i % 5 == 0){x = i/5;process[x] = atoi(token);                
                }
                else if (i % 5 == 1){x = (i-1)/5;prio[x] = atoi(token);}
                else if (i % 5 == 2){x = (i-2)/5;strcpy(ops[x],token);}
                else if (i % 5 == 3){x = (i-3)/5;ar1[x] = atoi(token);}
                else if (i % 5 == 4){x = (i-4)/5;ar2[x] = atoi(token);                
 }
                i++;
            token = strtok(NULL, ",\n");               
        }
        //fills each process with 5 attributes
         for (int j = 0; j < 15; j++){
            readyQueue[j].a = ar1[j];
            readyQueue[j].prio = prio[j];
            readyQueue[j].b = ar2[j];
            readyQueue[j].process = process[j];
            strcpy(readyQueue[j].op, ops[j]);
 }  
        }
        //now that our input file string is parsed into rqueue, we run our scheduler function array to reorder our queue depending on burst or priorty. fcfs scheduler does not need to reorder since it is already parsed in fcfs order.
        //0 is fcfs, 1 is sjf, 2 is priority
    
        if (strcmp(prArgs->arg1, "FCFS") == 0){
(*sched[0])(readyQueue);
        }
        else if (strcmp(prArgs->arg1, "SJF") == 0){
           (*sched[1])(readyQueue);
        }
        else if (strcmp(prArgs->arg1, "PRIORITY") == 0){
            (*sched[2])(readyQueue);
        }
    
        //now that our rQ is properly scheduled, "dispatch" them by writing result into each miniPCB, order is power sum fib product
        for (int i = 0; i < 15; i++){
            if (strcmp(readyQueue[i].op, "power")==0){readyQueue[i].result = (*funcs[0])(readyQueue[i].a, readyQueue[i].b);}
            else if (strcmp(readyQueue[i].op, "sum")==0){readyQueue[i].result = (*funcs[1])(readyQueue[i].a, readyQueue[i].b);}
             else if (strcmp(readyQueue[i].op, "fibonacci")==0){readyQueue[i].result = (*funcs[2])(readyQueue[i].a, readyQueue[i].b);}
             else if (strcmp(readyQueue[i].op, "product")==0){readyQueue[i].result = (*funcs[3])(readyQueue[i].a, readyQueue[i].b);}
            else printf("could not properly compare miniPCB operation to operations list");
        }
        //miniPCBs have their results, now send them to FIFO
        for (int i = 0; i<15; i++){
        send(readyQueue[i]);
        }
        //fifo no longer being utilized, now unlock mutex for logger use
pthread_mutex_unlock(&lock);
        return NULL;
}


//logger function will write from our fifo to output file
void* logger(void* arg){
 
struct processArgs *prArgs = (struct processArgs*)arg;
         // FILE *outfptr;
        //outfptr = fopen(prArgs->arg3, "w+");
        int fD = open(prArgs->arg3, O_WRONLY|O_CREAT|O_TRUNC|O_APPEND,0644);
    
        struct miniPCB frontProcess;
    
        while(1){
            //wait until the last fifo value gets filled
            if (myFIFO.procInfo[14].op[0]!='\0'){
            //when fifo is filled, we receive from fifo so we lock it
            pthread_mutex_lock(&lock);   
                for(int i =0; i< 15;i++){
                    char lineToWrite[60];
                    lineToWrite[0] = '\0';
                frontProcess = receive();
                //convert ints to strings to be concatenated
                char pA[20];
                 sprintf(pA, "%d", frontProcess.a);
                 char pB[20];
                 sprintf(pB, "%d", frontProcess.b);
                 char pRes[20];
                 sprintf(pRes, "%d", frontProcess.result);
                    //order by operation input param1 param2 result
                    strcpy(lineToWrite, frontProcess.op);
                    //concatenate a string we can write to our output file
                strcat(lineToWrite, ", ");    
                    strcat(lineToWrite, pA);
                    strcat(lineToWrite, ", ");
                    strcat(lineToWrite, pB);
                    strcat(lineToWrite, ", ");
                    strcat(lineToWrite, pRes);
                    strcat(lineToWrite, "\n");
                    write(fD, lineToWrite, strlen(lineToWrite));
                }
                
          //leave while loop after finishing writing to our file          
                break;
            }
            
            
        }
    
close(fD);
pthread_mutex_unlock(&lock);
return NULL;
}
//first argument is scheduling type, 2nd is file we read from, 3rd is the name of our output file
int main(int argc,char* argv[]){
        if (argc != 4){
            printf("There must be 3 arguments. Try again.");
            exit(0);
        }
 //make sure our arguments have a valid scheduling type
        if (strcmp(argv[1],"FCFS" ) != 0 && strcmp(argv[1], "PRIORITY") != 0 && strcmp(argv[1], "SJF") != 0){
        printf("Give a valid scheduling type of FCFS or PRIORITY or SJF."); exit(0);
        }
    
 
    
        if (pthread_mutex_init(&lock, NULL) != 0) {
            printf("Could not make mutex\n");
            return 1;
        }
        //open our input file to be read
 printf("Test: Now opening file.\n");
        FILE *srcfptr;
        srcfptr = fopen(argv[2], "r");


        printf("Test: Just opened file.Now seeking.\n");
        //seek to the end so we get its size and then return to the start of the file
        fseek(srcfptr, 0, SEEK_END); // go to last position
int srcfsize = ftell(srcfptr); //get the last position
fseek(srcfptr, 0, SEEK_SET); // go back to the first position
 printf("Test: Just seeked. Now allocating memory.\n");
        //allocate memory to the length of file
         char *str;
            str = (char *) malloc(srcfsize+1);
            int i = 0;
            //read and copy each char from our file into a string until end of file. we end up with a string identical to the whole input file, which is now able to be parsed
                while (feof(srcfptr)==0){
            str[i] = fgetc(srcfptr);
                    i++;
        } ;
    
        fclose(srcfptr);
    
        //null terminate our string;
              str[i-1] = '\0';  
           printf("Test: Allocation complete.\n");


        //need struct to pass in multiple args, fill that struct with our input string and command line arguments
        struct processArgs pArg;
        strcpy(pArg.stringTxt,str);
        strcpy(pArg.arg1,argv[1]);
         strcpy(pArg.arg2,argv[2]);
         strcpy(pArg.arg3,argv[3]);
         printf("Test: Successfully copied arguments to argument struct.\n");
//create our pthreads for logger and dispatcher, passing in our argument struct to pass in multiple values
        pthread_create(&(tid[0]), NULL, &schedDispatcher,(void*)&pArg);
 pthread_create(&(tid[1]), NULL, &logger, (void*)&pArg);
 
           pthread_join(tid[0], NULL);
   pthread_join(tid[1], NULL);
            
        pthread_mutex_destroy(&lock);
        exit(0);
}