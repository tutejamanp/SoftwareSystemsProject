

#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
char *messageactype = "PLEASE ENTER THE INPUT BETWEEN 1 AND 3\n";
char *message1 = "PLEASE ENTER INPUT OF 4 DIGITS\n";
long validate_and_take_input (int i1, int i2 , char * message1, int k) {
long value;
    printf("\n");
    printf("%s > ", message1);
    while (scanf("%ld", &value) != 1 || (!(value>=i1 && value<=i2) && k==0) )
    {
        int chr;
        printf("\tINVALID INPUT\n"); 
        do {
            chr = getchar();
        } while ((chr != '\0') && (chr != '\n'));
        printf("%s > ", message1);
    }
    return value;
}



long validate_and_take_password (int i1, int i2 , char * message1, int k) {
printf("ENTER THE PASSWORD FOR THE CUSTOMER\n");
long password1 =0;
long password2 =0;

do{
password1 = validate_and_take_input (i1, i2 , message1, k);
printf("ENTER AGAIN TO VALIDATE\n");
password2 = validate_and_take_input (i1, i2 , message1, k);
}
while(password1!=password2 );

return password1;   
}



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



void main()
{
	int ssfd;
	int len;
	int msgLen;
	struct sockaddr_in address;
	int result;
	FILE *fp;

	message *msg;
	message *reply;

	msg=(message*)malloc(sizeof(message));
	reply=(message*)malloc(sizeof(message));
	msgLen=sizeof(message);


	ssfd=socket(AF_INET,SOCK_STREAM,0);
	

	address.sin_family=AF_INET;
	address.sin_addr.s_addr=inet_addr("127.0.0.1");
	address.sin_port=9734;
	len=sizeof(address);

	result=connect(ssfd,(struct sockaddr*)&address,len);

	if(result==-1){
	perror("SERVER NOT FOUND");
	printf("\nERROR RESULT:%d",result);

}

printf("\nCONNECTION SUCCESSFUL");

do
{	
	printf("\n\n-----------------IIITB BANK--------------------\n");
	printf("\n1.\tCUSTOMER LOGIN");
	printf("\n2.\tADMIN USER");
	printf("\n3.\tEXIT\n");
	printf("\n\tPLEASE SELECT APPROPRIATE CHOICE : ");
	
	scanf("%d",&msg->ch);
	system("clear");
        write(ssfd,msg,msgLen);
	switch(msg->ch) //here ch refers to the choice that the user has made
	{int a;
	case 1://If the type of the user is Bank Customer
		a=0;
		msg->verify=0;
		do{ 
		printf("\nENTER USER ID : ");
		msg->uid = validate_and_take_input (1000,9999,message1,0);
		printf("\nENTER PASSWORD :");
		msg->pass=validate_and_take_password(1000,9999,message1,0);
		write(ssfd,msg,msgLen);
		read(ssfd,msg,msgLen);
                system("clear");
		a++;
		}while(msg->verify!=1 && a<=3);
		
		if(msg->verify==0)
		{
		break;
		}
		
		do
		{	
			printf("\n\n--------SELECT APPROPRIATE OPTION---------\n");
			printf("\n1.\tDEPOSIT");
			printf("\n2.\tWITHDRAWL");
			printf("\n3.\tBALANCE ENQUIRY");
			printf("\n4.\tLOGOUT\n");
			printf("\n\t ENTER CHOICE : ");
			scanf("%d",&msg->ch1);
			write(ssfd,msg,msgLen);
			switch(msg->ch1)
			{
			case 1:
				printf("\nENTER AMOUNT TO DEPOSIT : ");
				scanf("%ld",&msg->deposit);
				write(ssfd,msg,msgLen);
				read(ssfd,msg,msgLen);
                                system("clear");
				printf("\n\t DEPOSITED \n\t NEW AMOUNT : %ld ",msg->balance);
				break;
				
			case 2:
				printf("\nENTER AMOUNT TO WITHDRAW : ");
				scanf("%ld",&msg->Withdhraw);
				write(ssfd,msg,msgLen);
				read(ssfd,msg,msgLen);
                                int initialbalance = msg->balance;
				read(ssfd,msg,msgLen);
                                int finalbalance = msg->balance;
                                if(initialbalance!=finalbalance){
				printf("\n\t WITHDRAWED \n\t NEW AMOUNT : %ld ",msg->balance);
                                }
                                else{
                                printf("\n\t INSUFFICIENT FUNDS \n\t CURRENT BALANCE : %ld ",msg->balance);
                                }
				break;	
			
			case 3:
				read(ssfd,msg,msgLen);
				printf("\n\t CURRENT BALANCE : %ld ",msg->balance);
				break;	
				
			}
			
		}while(msg->ch1!=4);
	
	
		
	break;


	case 2://If the type of user is Administrator
			a=0;
			do{
			printf("\nENTER ADMIN ID : ");
			scanf("%d",&msg->aid);
			printf("\nENTER PASSWORD : ");
			scanf("%d",&msg->apass);
                        system("clear");
			write(ssfd,msg,msgLen);
			read(ssfd,msg,msgLen);
			a++;
			}while(msg->verify!=1 && a<=3);
			
			do
			{	
			printf("\n\n------ENTER YOUR CHOICE------\n");
			printf("\n1.\tCREATE USER");
			printf("\n2.\tLOGOUT\n");
			printf("\n\tYOUR CHOICE : ");
			scanf("%d",&msg->ch1);
			system("clear");
			write(ssfd,msg,msgLen);
			switch(msg->ch1)
			{
			case 1:
				printf("\nSELECT ACCOUNT TYPE : ");
				msg->atype = validate_and_take_input (1,3,messageactype,0); 
				 
				a=0;
				msg->verifyuid=0;
				do{
				printf("\nENTER CUSTOMER ID : ");
				msg->uid = validate_and_take_input (1000,9999,message1,0);
				write(ssfd,msg,msgLen);
				read(ssfd,msg,msgLen);
				a++;
				}while(msg->verifyuid!=1 && a<=3);
				
				if(msg->verifyuid==0)
				{
				break;
				}
				
				msg->pass=validate_and_take_password(1000,9999,message1,0);
				a=0;
				msg->verifyaccno=0;
				do{
				printf("\nENTER ACCOUNT NUMBER : ");
				msg->accno = validate_and_take_input (1000,9999,message1,0);
				write(ssfd,msg,msgLen);
				read(ssfd,msg,msgLen);
				a++;
				}while(msg->verifyaccno!=1 && a<=3);
				
				if(msg->verifyaccno==0)
				{
				break;
				}
				
				printf("\nENTER OPENING BALANCE : ");
				msg->balance = validate_and_take_input (1000,9999,"ENTER NUMERIC VALUES ONLY",1);
                                if(msg->atype!=3){
				system("clear");
				} 
				
				if(msg->atype==3)
				{
					a=0;
					msg->verifyuid1=0;
					do{
					printf("\nENTER SECOND USER ID : ");
					msg->uid1 = validate_and_take_input (1000,9999,message1,0);
					write(ssfd,msg,msgLen);
					read(ssfd,msg,msgLen);
					a++;
					}while(msg->verifyuid1!=1 && a<=3);
					
					if(msg->verifyuid1==0)
					{
					break;
					}
					
					msg->pass1=validate_and_take_password(1000,9999,message1,0);
					write(ssfd,msg,msgLen);
                                        system("clear");
				}
				write(ssfd,msg,msgLen);
				
			break;
				
			
			}
			
		}while(msg->ch1!=2);
			
			
			
	break;
	
	
		
	
	}

	}while(msg->ch!=3);// when the choice is to exit

	
}
	





