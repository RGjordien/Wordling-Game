#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

char currentUser[50];
int score;
int greenSolved[5] = {0,0,0,0,0};

void setColor(int c){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void waitms(int ms){
    Sleep(ms);
}

void showLogo(){
    system("cls");
    system("COLOR 0A");
    printf("                                                         ,--,                                      \n");
    printf("                   ,----..                           ,---.'|                    ,--.               \n");
    printf("           .---.   /   /   \\  ,-.----.       ,---,    |   | :      ,---,       ,--.'|  ,----..     \n");
    printf("          /. ./|  /   .     : \\    /  \\    .'  .' `\\  :   : |   ,`--.' |   ,--,:  : | /   /   \\    \n");
    printf("      .--'.  ' ; .   /   ;.  \\;   :    \\ ,---.'     \\ |   ' :   |   :  :,`--.'`|  ' :|   :     :   \n");
    printf("     /__./ \\ : |.   ;   /  ` ;|   | .\\ : |   |  .`\\  |;   ; '   :   |  '|   :  :  | |.   |  ;. /   \n");
    printf(" .--'.  '   \\' .;   |  ; \\ ; |.   : |: | :   : |  '  |'   | |__ |   :  |:   |   \\ | :.   ; /--`    \n");
    printf("/___/ \\ |    ' '|   :  | ; | '|   |  \\ : |   ' '  ;  :|   | :.'|'   '  ;|   : '  '; |;   | ;  __   \n");
    printf(";   \\  \\;      :.   |  ' ' ' :|   : .  / '   | ;  .  |'   :    ;|   |  |'   ' ;.    ;|   : |.' .'  \n");
    printf(" \\   ;  `      |'   ;  \\; /  |;   | |  \\ |   | :  |  '|   |  ./ '   :  ;|   | | \\   |.   | '_.' :  \n");
    printf("  .   \\    .\\  ; \\   \\  ',  / |   | ;\\  \\'   : | /  ; ;   : ;   |   |  ''   : |  ; .''   ; : \\  |  \n");
    printf("   \\   \\   ' \\ |  ;   :    /  :   ' | \\.'|   | '` ,/  |   ,/    '   :  ||   | '`--'  '   | '/  .'  \n");
    printf("    :   '  |--''    \\   \\ .'   :   : :-'  ;   :  .'    '---'     ;   |.' '   : |      |   :    /   \n");
    printf("     \\   \\ ;        `---`     |   |.'    |   ,.'                '---'   ;   |.'       \\   \\ .'     \n");
    printf("      '---'                   `---'      '---'                          '---'          `---`       \n");
    printf("\nPress ENTER to enter the game...");
    getchar();
}

void winAnim(){
    system("cls");
    setColor(10);
    printf(" \\O/\n  |\n / \\\nYOU WIN!\n");
    waitms(1200);
    setColor(7);
}

void loseAnim(){
    system("cls");
    setColor(12);
    printf("  O\n /|\\\n / \\\nGAME OVER\n");
    waitms(1200);
    setColor(7);
}

int login(){
    system("cls");
    char u[50],p[50],fu[50],fp[50];
    FILE *f=fopen("users.txt","r");
    if(!f) return 0;
    printf("Username: ");
    scanf("%s",u);
    printf("Password: ");
    scanf("%s",p);
    while(fscanf(f,"%s %s",fu,fp)!=EOF){
        if(!strcmp(u,fu)&&!strcmp(p,fp)){
            strcpy(currentUser,u);
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void registerUser(){
    system("cls");
    char u[50],p[50];
    FILE *f=fopen("users.txt","a");
    printf("New username: ");
    scanf("%s",u);
    printf("New password: ");
    scanf("%s",p);
    fprintf(f,"%s %s\n",u,p);
    fclose(f);
}

void saveScore(){
    FILE *f=fopen("scores.txt","r");
    FILE *t=fopen("temp.txt","w");
    char u[50];
    int s,found=0;
    if(f){
        while(fscanf(f,"%s %d",u,&s)!=EOF){
            if(!strcmp(u,currentUser)){
                fprintf(t,"%s %d\n",u,score>s?score:s);
                found=1;
            } else fprintf(t,"%s %d\n",u,s);
        }
        fclose(f);
    }
    if(!found) fprintf(t,"%s %d\n",currentUser,score);
    fclose(t);
    remove("scores.txt");
    rename("temp.txt","scores.txt");
}

int mathChallenge(){
    int a=rand()%10+1,b=rand()%10+1,op=rand()%4,in,ans;
    if(op==0){printf("%d + %d = ",a,b);ans=a+b;}
    else if(op==1){printf("%d - %d = ",a,b);ans=a-b;}
    else if(op==2){printf("%d * %d = ",a,b);ans=a*b;}
    else{printf("%d / %d = ",a*b,b);ans=a;}
    scanf("%d",&in);
    if(in==ans) return 1;
    return 0;
}

void rangeHint(char t[],int r[]){
    for(int i=0;i<5;i++){
        if(!r[i]){
            char lo=t[i]-2,hi=t[i]+2;
            if(lo<'a') lo='a';
            if(hi>'z') hi='z';
            printf("HINT position %d: [%c - %c]\n",i+1,lo,hi);
            return;
        }
    }
}

void processMath(char g[], char t[], int mathDone[]){
    for(int i=0;i<5;i++){
        if(g[i]==t[i] && !mathDone[i]){
            if(mathChallenge()){
                mathDone[i]=1;
            }else{
                score-=5;
            }
        }
    }
}

void printResult(char g[], char t[]){
    int used[5]={0}, mark[5]={0};

    for(int i=0;i<5;i++){
        if(g[i]==t[i]){
            mark[i]=2;
            used[i]=1;
        }
    }

    for(int i=0;i<5;i++){
        if(mark[i]==0){
            for(int j=0;j<5;j++){
                if(!used[j] && g[i]==t[j]){
                    mark[i]=1;
                    used[j]=1;
                    break;
                }
            }
        }
    }

    for(int i=0;i<5;i++){
        if(mark[i]==2) setColor(10);
        else if(mark[i]==1) setColor(14);
        else setColor(7);
        printf("%c ",g[i]);
    }
    setColor(7);
    printf("\n");
}


void playWordling(){
    system("cls");
    char *w[]={
"apple","angle","actor","arena","beach","brick","blaze","bloom","candy","cabin","camel","clock",
"dream","drift","doubt","eagle","earth","elite","faith","fancy","flame","glory","ghost","giant",
"habit","honey","house","input","irony","image","jolly","judge","karma","knife","knock","light",
"limit","lucky","magic","metal","noble","night","ocean","olive","orbit","piano","power","pride",
"queen","quick","quiet","radio","raven","river","scope","smile","solar","sword","table","tempo",
"unity","upper","urban","value","vivid","voice","water","witty","world","youth","zebra","crown",
"flock","grain","heart","index","jewel","kneel","latch","moral","novel","organ","plant","quake",
"round","stone","trace","usage","vigor","weird","yield","zesty"
};
    char target[6],guess[50];
    int revealed[5]={0};
    int mathDone[5]={0};
    score=100;
    strcpy(target,w[rand()%100]);
	for(int a=1;a<=6;a++){
    	printf("Attempt %d | Score %d\n",a,score);
    	scanf("%s",guess);
    	if(strlen(guess)!=5){a--;continue;}
    processMath(guess,target,mathDone);
    printResult(guess,target);
    rangeHint(target,mathDone);
    if(!strcmp(guess,target)){
        winAnim();
        saveScore();
        return;
    }
}
    loseAnim();
    saveScore();
}

void leaderboard(){
    system("cls");
    FILE *f=fopen("scores.txt","r");
    if(!f) return;
    char u[100][50];
    int s[100],n=0;
    while(fscanf(f,"%s %d",u[n],&s[n])!=EOF) n++;
    fclose(f);
    for(int i=0;i<n-1;i++)
        for(int j=0;j<n-i-1;j++)
            if(s[j]<s[j+1]){
                int ts=s[j];s[j]=s[j+1];s[j+1]=ts;
                char tu[50];strcpy(tu,u[j]);strcpy(u[j],u[j+1]);strcpy(u[j+1],tu);
            }
    for(int i=0;i<n;i++)
        printf("%d. %s - %d pts\n",i+1,u[i],s[i]);
    printf("\nPress ENTER...");
    getchar();
}

int main(){
    srand(time(NULL));
    int c;
    showLogo();
    while(1){
        system("cls");
        printf("1. Login\n2. Register\n3. Exit\n");
        scanf("%d",&c);
        getchar();
        if(c==1&&login()) break;
        if(c==2) registerUser();
        if(c==3) exit(0);
    }
    while(1){
        system("cls");
        printf("1. Play\n2. Leaderboard\n3. Logout\n");
        scanf("%d",&c);
        getchar();
        if(c==1) playWordling();
        if(c==2) leaderboard();
        if(c==3) break;
    }
    return 0;
}

