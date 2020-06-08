#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char idNum[10];
int idCheck(char idNum[10]);
void newTest(),checkScores(),questionGenerate(struct QUESTION *pt);
struct QUESTION{
	int no;
	int num1;
	int num2;
	int operat; // 0:+ 1:- 2:* 3:/
	char readableOperat;
	int ans;
	int inputAns;
};

int main(){
	int opt;
	do{
		printf("Please input your ID no: ");
		gets(idNum);
	}while (idCheck(idNum)); // repeat inputing when invalid.
	
	do{
		printf("\n\nMain Menu -- Math Practicing Program\n");
		printf("\n(1) Start a test");
		printf("\n(2) Check scores");
		printf("\n(3) Exit");
		printf("\nPlease select your option: ");
		opt=0;
		scanf("%d",&opt);
		getchar(); // to accept '\n'.
		switch(opt){
			case 1:
			newTest();
			break;
			
			case 2:
			checkScores();
			break;
			
			case 3:
			exit(0);
			break;
		}
	}while (1); // repeat to the main menu for the better user experience.
	
	return 0;
}

int idCheck(char idNum[10]){
	int i;
	int isInvalid=0; // invalid if 'isInvalid' was changed to 1.
	for (i=0;i<2;i++){
		if(!(idNum[i]>='A' && idNum[i]<='Z')) isInvalid=1; // change 'isInvalid' when the first and second char aren't capital letters.
	}
	for (i=2;i<6;i++){
		if(!(idNum[i]>='0' && idNum[i]<='9')) isInvalid=1; // change 'isInvalid' when the next 4 chars aren't digits.
	}
	if (idNum[6]!='\0') isInvalid=1; // change 'isInvalid' when the idNum longer then 6 chars.
	if (isInvalid == 1)printf("ID Number invalid.\n");
	return isInvalid;
}

void newTest(){
	FILE *recordFile;
	time_t startTime,endTime;
	char readableOperator;
	struct QUESTION quests[10];
	int score = 0;
	int timeUsed;
	
	printf("\n\nStart A Test -- Math Practicing Program\n");
	if ((recordFile=fopen("record.txt", "a+"))==NULL)
	{
		printf("\nFailed to open the file \"record.txt\".");
		return ;
	}
	questionGenerate(quests);
	printf("\nBe ready then press ENTER to start testing and timing...");
	getchar();
	startTime=time(0);
	
	
	for (int i=0;i<10;i++){
		do{
			printf("(%d) %d %c %d = ",quests[i].no,quests[i].num1,quests[i].readableOperat,quests[i].num2);
			scanf("%d",&quests[i].inputAns);
		}while(getchar()!='\n'); // to accept '\n' and prevent other char being input resulting bug.
	}
	endTime=time(0);
	
	
	timeUsed = endTime - startTime;
	printf("\nQues.      | Correct Answ. | Ur Answ.");
	for (int i=0;i<10;i++){
		printf("\n(%d) %d%c%d=\t%d\t\t%d",quests[i].no,quests[i].num1,quests[i].readableOperat,quests[i].num2,quests[i].ans,quests[i].inputAns);
		if (quests[i].ans == quests[i].inputAns) score+=10;
	}
	printf("\n============================\nYour score: %d\nTime consumption: %d seconds.",score,timeUsed);
	fprintf(recordFile,"%s  %d  %d seconds\n",idNum,score,timeUsed);
	fclose(recordFile);
	printf("\n\nYour score has been saved successfully.\nPress ENTER to back to main menu...");
	getchar();
}

void questionGenerate(struct QUESTION *quests){
	int isExist[4]={0,0,0,0};
	for(int i=0;i<10;i++){
		do{
			quests[i].no = i + 1;
			quests[i].num1 = rand()%99+1; // generate number between 1 and 100 .
			quests[i].num2 = rand()%99+1;
			if (i>0){
				do{ 
					quests[i].operat = rand()%4;
				}while(quests[i-1].operat == quests[i].operat);// ensure operator is diffenent from the previous one.
			}else{
				quests[i].operat = rand()%4;
			}
			
			switch(quests[i].operat){
				case 0:
				quests[i].ans = quests[i].num1 + quests[i].num2;
				quests[i].readableOperat = '+';
				break;
				
				case 1:
				quests[i].ans = quests[i].num1 - quests[i].num2;
				quests[i].readableOperat = '-';
				break;
				
				case 2:
				quests[i].ans = quests[i].num1 * quests[i].num2;
				quests[i].readableOperat = '*';
				break;
				
				case 3:
				while( (quests[i].num1 % quests[i].num2) != 0){ // ensure num1 be divided by num2 with no remainder
					quests[i].num1 = rand()%99+1;
					quests[i].num2 = rand()%99+1;
				}
				quests[i].ans = quests[i].num1 / quests[i].num2;
				quests[i].readableOperat = '/';
				break;
			}
		}while(!(quests[i].ans > 0 && quests[i].ans < 100));// ensure this question answer <100.
	}
	
	for (int i=0;i<10;i++){
		switch(quests[i].operat){
			case 0:
			isExist[0]=1;
			break;
			
			case 1:
			isExist[1]=1;
			break;
			
			case 2:
			isExist[2]=1;
			break;
			
			case 3:
			isExist[3]=1;
			break;
		}
	}
	if(isExist[0]+isExist[1]+isExist[2]+isExist[3] < 4)questionGenerate(quests); // ensure four operators exist or regenerate.
	
	
}

void checkScores(){
	FILE *inRecordFile;
	char records[30]={0};
	int isThisId=1;
	printf("\n\nCheck scores -- Math Practicing Program\n");
	if ((inRecordFile=fopen("record.txt", "r"))==NULL)
	{
		printf("\nFailed to open the file \"record.txt\". Try to start a new test first.\nPress ENTER to back to main menu...");
		getchar();
		return ;
	}
	printf("\nYour previous records are:\n");
	
	while(!feof(inRecordFile)){
		memset(records,0,sizeof(records));
		fgets(records,sizeof(records)-1,inRecordFile); // read file line by line.
		
		isThisId=1; 
		for(int i=0;i<6;i++){
			if(records[i]!=idNum[i])isThisId=0;
		}
		if(isThisId)printf("%s",records); // ensure this piece of record belongs to this id.
	}
	fclose(inRecordFile);
	printf("\n\nPress ENTER to back to main menu...");
	getchar();
}
