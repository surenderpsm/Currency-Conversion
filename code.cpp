#include<iostream.h>
#include<stdio.h>
#include<conio.h>
#include<fstream.h>
#include<string.h>
#include<stdlib.h>
#include<iomanip.h>
//*********************************CONSTANTS AND GLOBAL VARIABLE DECLARATIONs***************************************
const char *PwdFile="PWD.dat";
const char *CurFile="C_DATA.dat";
const char *TempFile="TEMP.dat";
const char *DefData[7]={"USD:1","GBP:0.79","EUR:0.88","INR:69.9","MYR:4.14","SGD:1.36","AED:3.67"};
char pwd[10];
int i,j; //Counters
int n;
//******************************************FUNCTION PROTOTYPES*****************************************************
//Main Functions

int load(void);
void print(int);
float Conversion(int,int,float);
void Reset(void);
void Modify(int);

//Utility Functions

void FileCreate(void);
int StrValidate(int);
int Validate(int,int);
int PwdInp(void);
void PwdMod(void);
void width(void);

//Menus

void opt1(void);
void opt2(void);
int MainMenu(void);

struct Currency{       //Currency holds the name, rate
	  char name[3];
	  unsigned int code;
	  float rate;
}list[100];

//********************END of DECLARATION of GLOBAL VARIABLES, CONSTANTS and FUNCTIONS PROTOTYPES******************


int load(){
	char line[10], DAT[2][10], *token;
	ifstream C_File;
	C_File.open(CurFile, ios::in);
	i=0;
	while(!C_File.eof()){
		C_File.getline(line,25,10);
		if (strcmp(line,"")==0) continue;
		token=strtok(line,":");
		j=0;
		while(token!=NULL){
			strcpy(DAT[j],token);
			j++;
			token=strtok(NULL,":");
		}
		strcpy(list[i].name,DAT[0]);                //name loaded
		list[i].rate=atof(DAT[1]);          //rate loaded as FLOAT
		i++;
	}
	C_File.close();
	C_File.open(PwdFile ,ios::in);
	C_File.getline(pwd,9,'\0');							//Password loaded
	C_File.close();
	return i-1;
}      //FUNCTION ENDS HERE***

void print(int n){
	for(i=0;i<=n;i++){
		cout<<"\n"<<i+1<<"->\t";
		cout<<list[i].name;
		cout<<"\t"<<list[i].rate;
	}
}

float Conversion(int Inp_Index,int Opt_Index,float Inp_Val){
	int a=Inp_Index-1 , b=Opt_Index-1; //to be used as index to the structure 'list'
	float Inp_R=list[a].rate;
	float Opt_R=list[b].rate;
	float Opt_Val=Opt_R*(Inp_Val/Inp_R); //Formula to calculate the output amount
	return Opt_Val;
}

void Reset(){
	ofstream File;
	File.open(CurFile, ios::trunc);
	for(i=0;i<=6;i++)
		File<<DefData[i]<<"\n";
	File.close();
	File.open(PwdFile, ios::trunc);
	File<<"Test1234";
	File.close();
	cout<<"The program has been RESET to FACTORY SETTINGS. Password is : Test1234";
}

void Modify(int Ch){
	char line[10], token[3], name[3], temp[10];
	int ind;
	float rate;
	print(n);
	switch(Ch){
			case 1:cout<<"\t\t\tUpdate a currency: \n";
					 cout<<"Enter the currency index you want to update: ";
					 cin>>ind;
					 cout<<"Enter the new currency rate with 1 USD: ";
					 cin>>rate;
					 strcpy(name,list[ind-1].name);
					 cout<<"Currency with code "<<name<<" has been UPDATED from "<<list[ind-1].rate<<" to "<<rate;
					 break;

			case 2:cout<<"\t\t\tADD a currency: \n";
					 cout<<"Enter the 3 character currency code [in capitals] : ";
					 gets(name);
					 cout<<"Enter the currency rate with 1 USD: ";
					 cin>>rate;
					 cout<<"A new currency with code "<<name<<" and rate "<<rate<<"is ADDED.";
					 break;

			case 3:cout<<"\t\t\tDELETE a currency: \n";
					 cout<<"Enter the currency index you want to delete: ";
					 cin>>ind;
					 strcpy(name,list[ind-1].name);
					 cout<<"The currency with code "<<name<<" is DELETED";
					 break;
			case 5:char Inp;
					 clrscr();
					 cout<<"\n\n\t\t\t CONFIRM TO RESET? \n";
					 cout<<"Do you want to reset this program to its FACTORY SETTINGS?(Y/N): ";
					 cin>>Inp;
					 if((Inp!='Y')&&(Inp!='y')){
						 Ch=6;
						 cout<<"\t\t\tRESET CANCELLED >>>";
					 }
					 break;
	}

	ifstream C_File;
	ofstream Temp;
	C_File.open(CurFile, ios::in);
	Temp.open(TempFile, ios::out);
	while(!C_File.eof()){
		C_File.getline(line,25,10);
		if (strcmp(line,"")==0) continue;
		strcpy(temp,line);
		strcpy(token,strtok(temp,":"));
		switch(Ch){
			case 1:if (strcmp(token,name)!=0)  //Modify
						 Temp<<"\n"<<line;
					 else
						 Temp<<"\n"<<name<<":"<<rate;
					 break;
			case 2:Temp<<"\n"<<line;   //Add
					 break;
			case 3:if(strcmp(token,name)!=0)  //Delete
						 Temp<<"\n"<<line;
					 break;
		}
	}
	if (Ch==2) Temp<<"\n"<<name<<":"<<rate;
	C_File.close();
	Temp.close();
	remove(CurFile);
	rename(TempFile,CurFile);
	if (Ch==5) Reset();                 //CALL Reset();
	n=load();
}

//*****************************************UTILITY FUNCTIONS****************************************************

int StrValidate(int inp){
  while(!cin){
	  cin.clear();
	  cin.ignore(1000,'\n');
	  cout<<"ERROR: Input Integers only: ";
	  cin>>inp;
	}
	return inp;
}

int Validate(int val,int p){
	StrValidate(val);
	while ((val<=0)||(val>p+1)){
	cout<<"\n\tERROR: INVALID CODE. Please re-enter : ";
	cin>>val;
	StrValidate(val);
	}
	return val;
}

int PwdInp(){
	char Inp[10];
	cout<<"\t\tEnter the CURRENT password: ";
	gets(Inp);
	if (strcmp(Inp,pwd)==0)
		return 1;
	else return 0;

}

void PwdMod(){
	if(PwdInp()==1){
		char Inp1[10],Inp2[10];
		clrscr();
		cout<<"\n\n\t\tEnter a NEW password(8 characters): ";
		gets(Inp1);
		while(strlen(Inp1)!=8){
			cout<<"ERROR: Enter a password with exactly 8 characters: ";
			gets(Inp1);
		}
		cout<<"\n\n\n\t\tPlease re-enter password to CONFIRM: ";
		gets(Inp2);
		if(strcmp(Inp1,Inp2)==0){
			ofstream File;
			File.open(PwdFile,ios::trunc);
			File<<Inp1;
			File.close();
			strcpy(pwd,Inp1);
			cout<<"\n\n\t\t\tPassword RESET successfully!";
		}
		else
			cout<<"\n\n\t\t\tPASSWORDS do not match. EXITING WINDOW>>>";
	}
	else
		cout<<"\n\n\t\t\tINCORRECT PASSWORD. EXITING WINDOW";

}


void FileCreate(){
	clrscr();
	ofstream C_File;
	ofstream Pwd;
	C_File.open(CurFile,ios::noreplace);
	Pwd.open(PwdFile,ios::noreplace);
	C_File.close();
	Pwd.close();
	if((C_File!=0)&&(Pwd!=0)){
		Reset();
		cout<<"\n\t\t\tFILES CREATED : SETUP COMPLETE";
		cout<<"\n\n\t\t\tPRESS ENTER TO PROCEED";
		getch();
	}
}






//________________________________MENUS and DISPLAYS___________________________
void opt1() //GENERAL USER MENU
{
	char inp[3];
	int in1,in2;
	float in3;
	clrscr();
	cout<<"\n\n"<<setw(47)<<"CONVERT CURRENCIES";
	cout<<"\n\tList Of Currencies Available: ";
	print(n);
	cout<<"\n\t Enter the Currency ID: ";
	cout<<"\n\n\t\tFROM: ";
	cin>>in1;
	in1=Validate(in1,n);
	cout<<"\n\t\tTO: ";
	cin>>in2;
	in2=Validate(in2,n);
	cout<<"\n\n\tEnter the Input Value of currency in "<<list[in1-1].name<<" : ";
	cin>>in3;

	in3=StrValidate(in3);
	cout<<"\n\n\tThe Output value in "<<list[in2-1].name<<" : "<<Conversion(in1,in2,in3);
}

void opt2() //SETTINGs menu
{
	int Ch;
	char inp[10];
	clrscr();
	cout<<"\n\n"<<setw(45)<<"SETTINGS";
	cout<<"\n\n";
	int p;
	p=PwdInp();
	if(p==1) {
		clrscr();
		cout<<"\n\n\n"<<setw(45)<<"SETTINGS\n";
		for(int i=1;i<=80;i++)
			cout<<"*";
		cout<<"\n\n\n"<<"Choose an operation to proceed: ";
		cout<<"\n\n\t\t\t\t\t\t\t6->Return to Main Menu";
		cout<<"\n\n\n"<<"1->Update Currencies"<<"\n\n"<<"2->Add a Currency"<<"\n\n"<<"3->Delete a Currency\n\n"<<"4->Change the PASSWORD\n\n"<<"5->RESET to Factory Settings  \t";
		cin>>Ch;
		Validate(Ch,6);
		clrscr();
		if ((Ch!=4)&&(Ch!=6)){
			Modify(Ch);
		}
		else if(Ch==4)
			PwdMod();
		else cout<<"\n\n\n\n\t\tPRESS ENTER to return to MAIN MENU>>";
	}
	else cout<<"\t\t\tINCORRECT PASSWORD. EXITING WINDOW";
}

int MainMenu(){           //Main menu
	clrscr();
	int Ch;
	cout<<"\n\n\n\n\n"<<setw(47)<<"CURRENCY CONVERTER\n";
	for(int i=1;i<=80;i++)
		cout<<"*";
	cout<<"\n\n\n\n"<<"Choose an operation to proceed: ";
	cout<<"\n\n\n"<<"1->Convert Currencies"<<"\n\n"<<"2->Settings"<<"\n\n"<<"3->Exit\n\n";
	cin>>Ch;
	Validate(Ch,3);
	return Ch;
}  //END OF MAIN MENU FUNC. *********
void width(){
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout<<setprecision(2);
}

void main() //Main Function
{
	FileCreate();
	width();          //sets decimal points like money
	int Ch;
	clrscr();
	n=load();
	cout<<"ALL LOADED";
	do{
		Ch=MainMenu();
		switch(Ch){
			case 1: opt1();
					  getch();
					  break;
			case 2: opt2();
					  getch();
					  break;
			case 3: cout<<"\t\t\t>>>EXITING PROGRAM>>>";
					  getch();
					  break;
		}
	} while(Ch!=3);
}
