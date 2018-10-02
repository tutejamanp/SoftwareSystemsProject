#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<malloc.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/wait.h>

	struct flock fp;
	int fd;

	
FILE* getlineptr2(long customerid, FILE *fptr, FILE *prevptr )
{
	long offset = 16;
	char *message = "PLEASE ENTER 4 DIGITS\n";
        char *oneLine = malloc(60 * sizeof(char));
        char *word2 = (char *)malloc(5*sizeof(char));
        sprintf(word2, "%ld", customerid);
        while((fgets(oneLine,60 , fptr)) != NULL) {
		char *word3 = (char *)malloc(5*sizeof(char));
		for(int i=0;i<=3;i++){
			word3[i] = (char)oneLine[11+i]; 
			}
                word3[4]='\0';	
		if(strcmp(word3,word2) == 0){			 	
                        return prevptr;
	       		}
		fgets(oneLine,50 , prevptr);
	}
return '\0';
}

char * getPassword(long customerid)
{
	FILE *fptr2,*fptr1;
	fptr2 = fopen("banking.txt","r+");
        fptr1 = fopen("banking.txt","r+");
        FILE *fptr = getlineptr2(customerid,fptr1,fptr2);
        if(fptr){
        char *oneLine = malloc(60 * sizeof(char));
	fgets(oneLine,60 , fptr);
	fclose(fptr2);
        fclose(fptr1);
        return oneLine+24;
     }
return '\0';
}


long getaccforcorrectpassword(long customerid, long passwordentered){
        char *oneLine = malloc(60 * sizeof(char));
	char * password = malloc(5 * sizeof(char));
        char * accnumber = malloc(5 * sizeof(char)); 
	oneLine = getPassword(customerid);
        if(oneLine){
        for (int i=0; i<4;i++){
	    password[i] = oneLine[i];	
        }
        password[4]='\0';
	long checkwith = atoi(password);
        if(checkwith==passwordentered){
        for (int i=0; i<4;i++){
	    accnumber[i] = oneLine[i+18];	
        }
	accnumber[4]='\0';
        return atoi(accnumber);
        }
}        
return -1;
}


FILE* getlineptr(long accountnumber, FILE *fptr, FILE *prevptr ,int *cnt)
{
	long offset = 16;
	char *message = "PLEASE ENTER 4 DIGITS\n";
        char *oneLine = malloc(50 * sizeof(char));
        char *word2 = (char *)malloc(5*sizeof(char));
        sprintf(word2, "%ld", accountnumber);
        while((fgets(oneLine,50 , fptr)) != NULL) {
		char *word3 = (char *)malloc(5*sizeof(char));
		for(int i=0;i<=3;i++){
			word3[i] = (char)oneLine[14+i]; 
	        }
                word3[4]='\0';
		if(strcmp(word3,word2) == 0){			 	
                        return prevptr;
	       		}
		fgets(oneLine,50 , prevptr);
                  *cnt+=strlen(oneLine)+1;
	}
}

long checkbalance(long accountnumber)
{
	FILE *fptr2,*fptr1;
	int a;
	fptr2 = fopen("acbalance.txt","r+");
        fptr1 = fopen("acbalance.txt","r+");
        FILE *fptr = getlineptr(accountnumber,fptr1,fptr2,&a);

	
        char *oneLine = malloc(50 * sizeof(char));
	fgets(oneLine,50 , fptr);
	
	fclose(fptr2);
        fclose(fptr1);
        return atoi(oneLine+26);

}

long withdraw(int accountnumber, int amt)
{

        char ch;
        int cnt=0,cnt1;
	int a1,a2,diff;
	FILE *fptr2,*fptr1,*fptr3,*fptr4;
        fptr2 = fopen("acbalance.txt","r+");
        fptr1 = fopen("acbalance.txt","r+");
        FILE* lineptr = getlineptr(accountnumber,fptr1,fptr2,&cnt);
	fseek(lineptr, 26, SEEK_CUR);
	fp.l_type=F_WRLCK;
	fp.l_whence=0;
	fp.l_start=cnt;
	char *oneLine = malloc(50 * sizeof(char));
	fgets(oneLine,50 , lineptr);
	cnt1=strlen(oneLine);
	printf("\n cnt1 = %d\n",cnt1);
	fseek(lineptr, -cnt1, SEEK_CUR);
	fp.l_len=cnt1+26;
	fp.l_pid=getpid();
	fcntl(fd,F_SETLKW,&fp);
        printf("\n\n\ninside lock %d \n\n\n\n",getpid());
	printf("%d %d %d %ld %ld %d",fd,fp.l_type,fp.l_whence,fp.l_start,fp.l_len,fp.l_pid);
	scanf("%c",&ch);
        long Bal = checkbalance(accountnumber);
        char* a=(char*)calloc(10,sizeof(char));
	sprintf(a,"%ld",Bal);
	a1=strlen(a);
	free(a);
        Bal-= amt;
        if(Bal<0){
        Bal+=amt;
        }
	fprintf(lineptr, "%ld", Bal);
	a=(char*)calloc(10,sizeof(char));
	sprintf(a,"%ld",Bal);
	a2=strlen(a);
	diff=a1-a2;
	free(a);
	while(diff)
	{
	fputs(" ",lineptr);
	diff--;
	}
        fp.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&fp);
        fclose(fptr2);
        fclose(fptr1); 
       	close(fd);
   return Bal;
	
}

long deposit(long accountnumber, int amt)
{
	int cnt=0,cnt1;
	char ch;
        int a1,a2,diff;
        FILE *fptr2,*fptr1;
        fptr2 = fopen("acbalance.txt","r+");
        fptr1 = fopen("acbalance.txt","r+");
        FILE* lineptr = getlineptr(accountnumber,fptr1,fptr2,&cnt);
	fseek(lineptr, 26, SEEK_CUR);
	fp.l_type=F_WRLCK;
	fp.l_whence=SEEK_SET;
	fp.l_start=cnt;
	char *oneLine = malloc(50 * sizeof(char));
	fgets(oneLine,50 , lineptr);
	cnt1=strlen(oneLine);
	printf("\n cnt1 = %d\n",cnt1);
	fseek(lineptr, -cnt1, SEEK_CUR);
	fp.l_len=cnt1+26;
	fp.l_pid=getpid();
	fcntl(fd,F_SETLKW,&fp);
	long Bal = checkbalance(accountnumber);
	Bal+= amt;
	fprintf(lineptr, "%ld", Bal);
	fp.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&fp);
        fclose(fptr2);
        fclose(fptr1); 
        close(fd);
        return Bal;
}

int FindWord(char *word , char *filename , int offset , char *check){
int ispresent = 1;
char *ptr;
FILE *file = fopen(filename,"r");
char oneLine[offset];
int found =0;
while((fgets(oneLine, offset, file)) != NULL) {
   char cust [offset];
   strcpy(cust,check);
   strcat(cust,word);
   ptr = strstr(oneLine, cust);
   if(ptr) {
      found++;
      fclose(file); 
      return ispresent;
   }
}
if(!found) {
ispresent = 0;
fclose(file);
return ispresent;
}
}


void * accept_connection(void *);


int ssfd;
int ccfd,res;
int serverlen,clientlen;
struct sockaddr_in serveraddr;
struct sockaddr_in clientaddr;


typedef struct message
{
long uid;
long pass;
long uid1;
long pass1;
long deposit;
long balance;
long Withdhraw;
int aid;
int apass;
long atype;
int verify;
int ch;
int ch1;
int output;
int verifyuid;
int verifyuid1;
long accno;
int verifyaccno;
int v;
}message;


	int msgLen;
	
int ccfd;
	message *msg;
	message *reply;



void * accept_connection( void *arg)
{




	printf("\nCONNECTION ACCEPTED SUCCESSFULLY %d \n",getpid());
	while(1)
	{
		int a=0;
		read(ccfd,reply,msgLen);
		switch(reply->ch)
		{	
			case 1:a=0;//user
				reply->verify=0;				
				do{
				read(ccfd,reply,msgLen);
				reply->accno=getaccforcorrectpassword(reply->uid,reply->pass);
				
				if(reply->accno!=-1)
				{
				reply->verify=1;
				}
				write(ccfd,reply,msgLen);
				a++;
				}while(reply->verify!=1 && a<=3);
				
				if(reply->verify==0)
				{
				break;
				}	
				do
				{	read(ccfd,reply,msgLen);
					switch(reply->ch1)
					{
					case 1://deposit
						read(ccfd,reply,msgLen);
						reply->balance=deposit(reply->accno,reply->deposit);
						write(ccfd,reply,msgLen);
						break;
						
					case 2://Withdhraw
						read(ccfd,reply,msgLen);
                                                reply->balance=checkbalance(reply->accno);
                                                write(ccfd,reply,msgLen);
						reply->balance=withdraw(reply->accno,reply->Withdhraw);
						write(ccfd,reply,msgLen);
						break;	
						
					case 3://bal enquiry;
						reply->balance=checkbalance(reply->accno);
						write(ccfd,reply,msgLen);
						break;	
					
						
					}
					
				
				}while(reply->ch1!=4);
			
			
			break;
			
			case 2:a=0;//admin
				
				   char *messageactype = "PLEASE ENTER VALUES BETWEEN 1 AND 3";
				   char *message = "PLEASE ENTER 4 DIGITS";
				   char *word = (char *)malloc(5*sizeof(char));
				   char *word2 = (char *)malloc(5*sizeof(char));
				   char *checkcust = "CustomerID:";
				   char *checkacc = "AccountNumber:";
				   FILE *fptr;
				   FILE *fptr2;

				
					do{
					read(ccfd,reply,msgLen);
					reply->verify=1;
					write(ccfd,reply,msgLen);
					a++;
					}while(reply->verify!=1 && a<=3);
					
					do
					{	
					read(ccfd,reply,msgLen);
					switch(reply->ch1)
					{
					case 1://Create user
							a=0;
							reply->verifyuid=0;
							do{							//verify uid
							read(ccfd,reply,msgLen);
							sprintf(word, "%ld", reply->uid); 
							 if(FindWord(word,"banking.txt",16,checkcust)==0)
							 {
							 reply->verifyuid=1;
							 }
							write(ccfd,reply,msgLen);
							a++;
							}while(reply->verifyuid!=1 && a<=3);
							
							if(reply->verifyuid==0)
							{
							break;
							}
							
							a=0;
							reply->verifyaccno=0;
							do{							//verify accno
							read(ccfd,reply,msgLen);
							char *word2 = (char *)malloc(5*sizeof(char));
   							sprintf(word2, "%ld", reply->accno);
							if(FindWord(word2,"acbalance.txt",19,checkacc)==0)
							{
							reply->verifyaccno=1;
							}
							write(ccfd,reply,msgLen);
							a++;
							}while(reply->verifyaccno!=1 && a<=3);
						
							if(reply->verifyaccno==0)
							{
							break;
							}
							
							
							if(reply->atype==3)					//Account Type 3
							{
								a=0;
								reply->verifyuid1=0;
								do{							//verify uid1
								read(ccfd,reply,msgLen);
								sprintf(word2, "%ld", reply->uid1); 
								 if(FindWord(word2,"banking.txt",16,checkcust)==0)
								 {
								 reply->verifyuid1=1;
								 }
								write(ccfd,reply,msgLen);
								a++;
								
								}while(reply->verifyuid1!=1 && a<=3);
								
								if(reply->verifyuid1==0)
								{
								break;
								}
								read(ccfd,reply,msgLen);
							}
							read(ccfd,reply,msgLen);
							 fptr = fopen("banking.txt","a");
				   			fptr2 = fopen("acbalance.txt","a");
				   			
				   			if(reply->atype!=3 && reply->verifyuid==1)
				   			{
				   			
							fprintf(fptr,"CustomerID:%ldPassword:%ldAccountNumber:%ldAccountType:%ld\r\n",reply->uid,reply->pass,reply->accno,reply->atype);
  							 fprintf(fptr2,"AccountNumber:%ldBalance:%ld\r\n",reply->accno,reply->balance);	
  							 
  							 }
  							 else if(reply->atype==3 && reply->verifyuid1==1)
  							 {
  							 fprintf(fptr,"CustomerID:%ldPassword:%ldAccountNumber:%ldAccountType:%ld\r\n",reply->uid1,reply->pass1,reply->accno,reply->atype);
							  fprintf(fptr,"CustomerID:%ldPassword:%ldAccountNumber:%ldAccountType:%ld\r\n",reply->uid,reply->pass,reply->accno,reply->atype);
  							 fprintf(fptr2,"AccountNumber:%ldBalance:%ld\r\n",reply->accno,reply->balance);	
  							}
  							 
  							 
  							  fclose(fptr);
  							fclose(fptr2);
						break;
						
					}
					
				
				}while(reply->ch1!=2);
			
			
			break;
			

		}

	}
}


void main()
{
	
	int i =0,j=0;
        fd=open("acbalance.txt",O_RDWR);
        printf("\n fd= %d\n",fd);
	ssfd=socket(AF_INET,SOCK_STREAM,0);	
	msg=(message*)malloc(sizeof(message));
	reply=(message*)malloc(sizeof(message));
        //printing server side socket descriptor value  
	printf("\nSSFD VALUE:%d\n",ssfd);

	msgLen=sizeof(message);

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	
	serveraddr.sin_port=9734;
	serverlen=sizeof(serveraddr);
	if(bind(ssfd,(struct sockaddr*)&serveraddr,serverlen))
	{
	printf("\n BIND ERROR\n");
	}
	printf("\nCONNECTED TO SOCKET");
	listen(ssfd,5);
	while(1)
	{
                //client side socket descriptor 
		ccfd=accept(ssfd,(struct sockaddr*)&clientaddr,&clientlen);
		if(!fork())
		{	
		
			j=getpid();
			accept_connection(NULL);
			break;
		}
		else{
		
		}
	
	
	}
	wait(0);
	

close(fd);
}


