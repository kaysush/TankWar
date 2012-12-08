#include<allegro.h>
#include<pthread.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<sys/dir.h>
//#include<dos.h>
//#include<io.h>

#define PI 3.1428571428571428571428571428571
#define BROWN makecol(174,123,0)
#define WHITE makecol(255,255,255)
#define TOMATO makecol(255,99,71)
//*******************************STRUCTURE FOR TANK VARIABLES***********************************************
struct tank_war
{
double x,y;
int dir;
int score;
int health;
int explode;
}t[2];
//******************************STRUCTURE FOR BULLET VARIABLES************************************************
struct bullet
{
int alive;
double x,y;
int dir;
}b[2];
//********************************STRUCTURE FOR BULLET COUNTER**************************************************
struct bullet_counter
{
int fired;
int missed;
}bc[2];
//**********************************STRUCTURE FOR SAVING REPLAY***************************************************
struct REPLAY
{
double x1,y1;
double x2,y2;
int h1,h2;
int explode1,explode2;
int dir1,dir2;
double bx1,by1,bx2,by2;
int alive1,alive2;
struct REPLAY *previous;
struct REPLAY *next;
}*start1,*rear1;
//*********************************STRUTURE FOR LOADING REPLAY*************************************************
struct LOAD_REPLAY
{
double x1;
double y1;
int h1;
int dir1;
int explode1;
double x2;
double y2;
int h2;
int dir2;
int explode2;
double bx1;
double by1;
double bx2;
double by2;
int alive1;
int alive2;
}obj;
//*************************************STRUCTURE FOR BACKGROUND SONGS**************************************
struct SONG
{
int id;
char song_name[100];
long int duration;
struct SONG *previous;
struct SONG *next;
}*start,*rear,*currsong;
//*************************************STRUCT FOR LOADING SONGS*****************************************
struct LOAD_SONGS_FROM_FILE
{
int id;
char song_name[100];
long int duration;
}load_songs;
//*************************************STRUCTURE FOR HIGHSCORE**********************************************
struct HSCORE
{
int score;
struct HSCORE *next;
}*start2,*rear2;
//***********************************STRUCTURE FOR LOADING HSCORE******************************************
struct LOAD_HSCORE
{
int score;
}load_hscore;
//**************************************STRUCTURE FOR REPLAY LIST*******************************************
struct REPLAY_LIST
{
char rep_name[10];
struct REPLAY_LIST *prev;
struct REPLAY_LIST *next;
}*start3,*rear3;
//**************************************STRUCTURE FOR SAVE/LOAD GAME*******************************************
struct SAVE__LOAD_GAME
{
double x1,y1;
double x2,y2;
int dir1,dir2;
int h1,h2;
int s1,s2;
BITMAP *m;
}save_load_game;
//*************************************STRUCTURE FOR ANIMATIONS*********************************************
struct ANIM
{
char name[10];
struct ANIM *next;
}*start4,*rear4;
//***********************************STRUCTURE FOR LOADING ANIMATIONS**************************************
struct LOAD_ANIM
{
char name[10];
}load_anim;
//*************************************STRUCTURE FOR LOGIN TREE**********************************************
struct LOGIN_TREE
{
char user_id[15];
char password[15];
struct LOGIN_TREE *left;
struct LOGIN_TREE *right;
}*root,*current;
//**************************************STRUCTURE FOR LOADING TREE********************************************
struct LOAD_TREE
{
char user_id[15];
char password[15];
}load_login_tree;
//*************************************THREAD VARIABLES*******************************************************
int id;
pthread_t pthread0;
int threadid0=0;
//**********************************SOME HANDY VARIABLES******************************************************
DATAFILE *images;
BITMAP *map;
BITMAP *tank[2][64],*bullet,*explode,*progress;
BITMAP *error_sound,*error;
BITMAP *mouse_bmp;
BITMAP *back;
BITMAP *new_game,*high,*help,*settings,*exit_bmp;
BITMAP *bitmap_null;
BITMAP *tank_image,*tank_font,*war,*press;
BITMAP *welcome,*present;
BITMAP *replayimage;
BITMAP *anim[180];
SAMPLE *song;
SAMPLE *sample,*track;
int gameover=0,a,n;
int volume=128,panning=128,frequency=1000;
int pause_music=0,pos_value=0;
//******************************************FUCTIONS PROTOTYPE*****************************************************************
void savegame();
void loadgame();
int find_file(char);
void savereplay(struct REPLAY *);
struct SONG*load_song(char);
struct REPLAY *create_node(double,double,int,int,int,int,double,double,int,int,int,int,double,double,double,double,int,int);
void insert_node(struct REPLAY*);
void openreplay(char);
void replay(struct REPLAY*);
void insert_song(struct SONG*);
int load_song_from_file(char);
void printhealth();
int check_health(int);
int welcome_screen();
int check_path(int);
int check_bullet(int);
int firebullet(int);
int updatebullet(int);
int explosion(int);
int menu();
int check_box(int);
int load_tank();
int setupscreen();
int tank_war();
int check_tank(int);
int turnright(int);
int turnleft(int);
int move_up(int);
int move_down(int);
void showtank();
int input();
int input();
void *thread(void *);
int destroy();
int sound_error();
void replay_screen();
void resumemenu();
void setparameteres();
void play_game();
void load_images();
void savescore(int);
int count_highscore(struct HSCORE *);
struct HSCORE *create_hscore_node(int);
void insert_hscore_node(struct HSCORE*);
void sort_hscore_list_by_bubble(struct HSCORE*);
void sort_hscore_list_by_selection(struct HSCORE*);
void show_highscore();
void help_menu();
void controls();
void rules();
void joy_input(int);
void setting();
void view_replay();
void game_trax();
struct REPLAY_LIST *create_replay_list_node(char );
void insert_replay_list_node(struct REPLAY_LIST *);
BITMAP * load_map();
void signup();
void login();
struct LOGIN_TREE *create_login_tree_node(char ,char);
void create_tree();
void insert_login_tree_node(struct LOGIN_TREE *);
int search_login_tree(char *);
int check_id_pwd(char *,char *);
//********************************************LOAD MAP*****************************************************************
BITMAP *load_map()
{
BITMAP *b;
b=load_bitmap("map1.BMP",NULL);
return(b);
}
//******************************************VOID SAVEGAME***************************************************************
void savegame()
{
FILE *fp;
fp=fopen("save","wb");
rewind(fp);
save_load_game.x1=t[0].x;
save_load_game.y1=t[0].y;
save_load_game.s1=t[0].score;
save_load_game.h1=t[0].health;
save_load_game.dir1=t[0].dir;
save_load_game.x2=t[1].x;
save_load_game.y2=t[1].y;
save_load_game.s2=t[1].score;
save_load_game.h2=t[1].health;
save_load_game.dir2=t[1].dir;
save_load_game.m=map;
fwrite(&save_load_game,sizeof(save_load_game),1,fp);
fclose(fp);
}
//******************************************VOID LOADGAME*****************************************************************
void loadgame()
{
FILE *fp;
fp=fopen("save","rb");
if(fp==NULL)
{
pause_music=1;
rest(800);
allegro_message("No game saved");
clear_keybuf();
pause_music=0;
pos_value=-1;
setting();
}
fread(&save_load_game,sizeof(save_load_game),1,fp);
t[0].x=save_load_game.x1;
t[0].y=save_load_game.y1;
t[0].dir=save_load_game.dir1;
t[0].score=save_load_game.s1;
t[0].health=save_load_game.h1;

t[1].x=save_load_game.x2;
t[1].y=save_load_game.y2;
t[1].dir=save_load_game.dir2;
t[1].score=save_load_game.s2;
t[1].health=save_load_game.h2;

gameover=0;

fclose(fp);
clear_keybuf();
play_game();
replay_screen();
}
//*****************************************CREATE REPLAY LIST NODE*****************************************************************
struct REPLAY_LIST *create_replay_list_node(char replay_name[])
{
struct REPLAY_LIST *newptr;
newptr=(struct REPLAY_LIST*)malloc(sizeof(struct REPLAY_LIST));
strcpy(newptr->rep_name,replay_name);
newptr->prev=NULL;
newptr->next=NULL;
return(newptr);
}
//******************************************INSERT REPLAY LIST NODE************************************************************
void insert_replay_list_node(struct REPLAY_LIST *p)
{
if(start3==NULL)
start3=rear3=p;
else
{
p->prev=rear3;
rear3->next=p;
rear3=p;
}
}
//*****************************************VOID FINDFILE**********************************************************
/*int find_file(char string[])
{
int find=0;
struct REPLAY_LIST *newptr,*p;
char name[10];
int count=0;
int i=10;
long done;
//struct _finddata_t ffblk;
struct ffblk ff;
//done=findfirst(string,&ffblk);
done=_findfirst(&ff);

//while(!find)
while(done)
{
count++;
if(strcmp(string,"*.rep")==0)
{
newptr=create_replay_list_node(ffblk.name);
insert_replay_list_node(newptr);
}
//find=findnext(done,&ffblk);
done=_findnext(&ff);
}
return(count);

}*/
//**********************************VOID SAVEREPLAY******************************************************************
void savereplay(struct REPLAY *p)
{
FILE *fp;
char name[10];
int count,len;
//count=find_file("*.rep");
if(count==10)
{
count=0;
}
//itoa(count+1,name,10);
sprintf(name,"%d",count+1);
len=strlen(name);
name[len]='.';
name[len+1]='r';
name[len+2]='e';
name[len+3]='p';
name[len+4]='\0';
fp=fopen(name,"wb");
while(p)
{
obj.x1=p->x1;
obj.y1=p->y1;
obj.h1=p->h1;
obj.dir1=p->dir1;
obj.explode1=p->explode1;
obj.x2=p->x2;
obj.y2=p->y2;
obj.h2=p->h2;
obj.dir2=p->dir2;
obj.explode2=p->explode2;
obj.bx1=p->bx1;
obj.by1=p->by1;
obj.bx2=p->bx2;
obj.by2=p->by2;
obj.alive1=p->alive1;
obj.alive2=p->alive2;
fwrite(&obj,sizeof(obj),1,fp);
p=p->next;
}
fclose(fp);
rest(1000);
allegro_message(name);
}
//******************************************LOAD SONG(CREATE SONG NODE)*********************************************************************
struct SONG * load_song(int id,char name[],long int duration)
{
struct SONG *newsong;
newsong=(struct SONG*)malloc(sizeof(struct SONG));
newsong->id=id;
strcpy(newsong->song_name,name);
newsong->duration=duration;
newsong->previous=NULL;
newsong->next=NULL;
return(newsong);
}
//*************************************REPLAY NODE************************************************************************
struct REPLAY* create_node(double x1,double y1,int h1,int dir1,int explode1,double x2,double y2,int h2,int dir2,int explode2,double bx1,double by1,double bx2,double by2,int alive1,int alive2)
{
struct REPLAY *newptr;
newptr=(struct REPLAY*)malloc(sizeof(struct REPLAY));
newptr->x1=x1;
newptr->y1=y1;
newptr->h1=h1;
newptr->dir1=dir1;
newptr->explode1=explode1;
newptr->x2=x2;
newptr->y2=y2;
newptr->h2=h2;
newptr->dir2=dir2;
newptr->explode2=explode2;
newptr->bx1=bx1;
newptr->by1=by1;
newptr->bx2=bx2;
newptr->by2=by2;
newptr->alive1=alive1;
newptr->alive2=alive2;
newptr->previous=NULL;
newptr->next=NULL;
return(newptr);
}
//*****************************INSERT REPLAY NODE**********************************************************************
void insert_node(struct REPLAY *p)
{
if(start1==NULL)
start1=rear1=p;
else
{
p->previous=rear1;
rear1->next=p;
rear1=p;
}
}
//*******************************OPENREPLAY*****************************************************************************
void openreplay(char name[])
{
FILE *fp;
double x1,y1,x2,y2;
double bx1,by1,bx2,by2;
int dir1,dir2;
int h1,h2;
int alive1,alive2;
struct REPLAY *newptr;
fp=fopen(name,"rb");
if(fp==NULL)
{
allegro_message("Error loading file");
}
while(fread(&obj,sizeof(obj),1,fp)==1)
{
newptr=create_node(obj.x1,obj.y1,obj.h1,obj.dir1,obj.explode1,obj.x2,obj.y2,obj.h2,obj.dir2,obj.explode2,obj.bx1,obj.by1,obj.bx2,obj.by2,obj.alive1,obj.alive2);
insert_node(newptr);
}
fclose(fp);
}
//*****************************INSERT SONG NODE**********************************************************************
void insert_song(struct SONG *newsong)
{
if(start==NULL)
{
start=rear=newsong;
}
else
{
newsong->previous=rear;
rear->next=newsong;
rear=newsong;
}
}
//***************************************CREATE ANIMATION NODE*****************************************************
struct ANIM *create_anim_node(char name[])
{
struct ANIM *newptr;
newptr=(struct ANIM*)malloc(sizeof(struct ANIM));
strcpy(newptr->name,name);
newptr->next=NULL;
return(newptr);
}
//***************************************INSERT ANIMATION NODE********************************************************
void insert_anim_node(struct ANIM *p)
{
if(start4==NULL)
start4=rear4=p;
else
{
rear4->next=p;
rear4=p;
}
}
//****************************************PLAY REPLAY******************************************************************
void replay(struct REPLAY *p)
{
while(p && !key[KEY_ESC] && !joy[0].button[3].b)
{
poll_joystick();
stretch_blit(map,screen,0,0,992,1056,0,0,800,600);
masked_blit(tank[0][p->dir1],screen,0,0,p->x1,p->y1,32,32);
masked_blit(tank[1][p->dir2],screen,0,0,p->x2,p->y2,32,32);
if(p->alive1==1)
{
masked_blit(bullet,screen,0,0,p->bx1,p->by1,9,9);
}
if(p->alive2==1)
{
masked_blit(bullet,screen,0,0,p->bx2,p->by2,9,9);
}
if(p->explode1==1)
{
stop_sample(sample);
masked_blit(explode,screen,0,0,p->x2,p->y2,32,32);
play_sample(sample,128,128,1000,0);
rest(100);
}
if(p->explode2==1)
{
stop_sample(sample);
masked_blit(explode,screen,0,0,p->x1,p->y1,32,32);
play_sample(sample,128,128,1000,0);
rest(100);
}
rest(60);
p=p->next;
if(keypressed())
{
if(key[KEY_RIGHT] || joy[0].stick[0].axis[0].d2 || joy[1].stick[0].axis[0].d2)
p=p->next;
}
if(key[KEY_LEFT] || joy[0].stick[0].axis[0].d1  || joy[1].stick[0].axis[0].d2)
p=p->previous->previous;
}
}
//******************************LOAD SONGS FROM FILE*******************************************************
int load_song_from_file(char fname[])
{
struct SONG *newsong;
FILE *fp;
fp=fopen(fname,"rb");
if(fp==NULL)
{
allegro_message("Error loading file");
}
while(fread(&load_songs,sizeof(load_songs),1,fp)==1)
{
newsong=load_song(load_songs.id,load_songs.song_name,load_songs.duration);
insert_song(newsong);
}
fclose(fp);
}
//***********************************PRINT HEALTH********************************************************************
void print_health()
{
textout_ex(screen,font,"Blue Tank",650,49,0,-1);
textout_ex(screen,font,"Health",595,63,0,-1);
masked_blit(progress,screen,0,0,650,60,t[1].health,10);
rect(screen,650,60,750,70,0);
textprintf_ex(screen,font,595,80,0,-1,"SCORE %i",t[1].score);
textout_ex(screen,font,"Green Tank",120,500,0,-1);
textout_ex(screen,font,"Health",65,514,0,-1);
masked_blit(progress,screen,0,0,120,511,t[0].health,10);
rect(screen,120,511,220,521,0);
textprintf_ex(screen,font,70,531,0,-1,"SCORE %i",t[0].score);
}
//***********************************CHECK HEALTH**************************************************************
int check_health(int num)
{
if(t[num].health==0)
{
gameover=1;
}
}
//*************************************WELCOME SCREEN********************************************************
int welcome_screen()
{
MIDI * sample;
sample=load_midi("5.mid");
if(!sample)
{
allegro_message("Error playing sound");
allegro_exit();
}
struct ANIM *newptr,*p;
int i=0;
FILE *fp;
fp=fopen("animations.dat","rb");
if(fp==NULL)
{
allegro_message("Error loading file");
}
while(fread(&load_anim,sizeof(load_anim),1,fp)==1)
{
newptr=create_anim_node(load_anim.name);
insert_anim_node(newptr);
}
fclose(fp);
p=start4;
clear_bitmap(screen);
textout_ex(screen,font,"loading.....",350,300,BROWN,-1);
while(p)
{
anim[i]=load_bitmap(p->name,NULL);
i++;
p=p->next;
}
i=0;
play_midi(sample,0);
while(i!=180)
{
stretch_blit(anim[i],screen,0,0,853,480,0,0,800,600);
rest(58);
i++;
}
for(i=0;i<=180;i++)
{
destroy_bitmap(anim[i]);
}
}
//********************************************CHECK PATH OF TANK****************************************************************
int check_path(int num)
{
double x1,y1;
x1=(t[num].x+16)+16*cos(((t[num].dir*5.625)*PI)/180)-4.5;
y1=(t[num].y+16)+16*sin(((t[num].dir*5.625)*PI)/180)-4.5;
if(num==0)
{
if(x1>t[num+1].x && x1<t[num+1].x+25 && y1>t[num+1].y && y1<t[num+1].y+25)
{
return 0;
}
else 
return 1;
}
else if(num==1)
{
if(x1>t[num-1].x && x1<t[num-1].x+25 && y1>t[num-1].y && y1<t[num-1].y+25)
{
return 0;
}
else
return 1;
}
}
//********************************CHECK BULLET FOR OUT OF SCREEN**********************************************************
int check_bullet(int num)
{
if(b[num].x<0||b[num].x>SCREEN_W||b[num].y<0||b[num].y>SCREEN_H)
{
b[num].alive=0;
bc[num].missed=bc[num].fired-t[num].score;
}
}
//********************************SET BULLET POSITION FOR FIRE***************************************
int firebullet(int num)
{
b[num].alive=1;
b[num].dir=t[num].dir;
b[num].x=(t[num].x+16)+16*cos(((t[num].dir*5.625)*PI)/180)-4.5;
b[num].y=(t[num].y+16)+16*sin(((t[num].dir*5.625)*PI)/180)-4.5;
}
//*********************************UPDATE BULLET********************************************************
int updatebullet(int num)
{
if(b[num].alive==1)
{
b[num].x+=25*cos(((b[num].dir*5.625)*PI)/180);
b[num].y+=25*sin(((b[num].dir*5.625)*PI)/180);
}
}
//************************************CHECK FOR EXPLOSION **************************************
int explosion(int num)
{
struct REPLAY *newptr2;
int x,y;
if(num==0)
{
x=b[num].x+4;
y=b[num].y+4;
if(x>t[num+1].x&&x<t[num+1].x+(tank[num+1][t[num+1].dir]->w)&&y>t[num+1].y&&y<t[num+1].y+(tank[num+1][t[num+1].dir]->h))
{
b[num].alive=0;
t[num].explode=1;
newptr2=create_node(t[0].x,t[0].y,t[0].health,t[0].dir,t[0].explode,t[1].x,t[1].y,t[1].health,t[1].dir,t[1].explode,b[0].x,b[0].y,b[1].x,b[1].y,b[0].alive,b[1].alive);
insert_node(newptr2);
showtank();
t[num].explode=0;
masked_blit(explode,screen,0,0,t[num+1].x,t[num+1].y,32,32);
play_sample(sample,128,128,1000,0);
t[num].score+=2;
t[num+1].score--;
t[num+1].health-=4;
b[num].x=1000;
b[num].y=1000;
rest(100);
}
}
if(num==1)
{
x=b[num].x+4;
y=b[num].y+4;
if(x>t[num-1].x&&x<t[num-1].x+(tank[num-1][t[num-1].dir]->w)&&y>t[num-1].y&&y<t[num-1].y+(tank[num-1][t[num-1].dir]->h))
{
b[num].alive=0;
t[num].explode=1;
newptr2=create_node(t[0].x,t[0].y,t[0].health,t[0].dir,t[0].explode,t[1].x,t[1].y,t[1].health,t[1].dir,t[1].explode,b[0].x,b[0].y,b[1].x,b[1].y,b[0].alive,b[1].alive);
insert_node(newptr2);
showtank();
t[num].explode=0;
masked_blit(explode,screen,0,0,t[num-1].x,t[num-1].y,32,32);
play_sample(sample,128,128,1000,0);
t[num].score+=2;
t[num-1].score--;
t[num-1].health-=4;
b[num].x=1000;
b[num].y=1000;
rest(100);
}
}
}

//************************************MENU******************************************************************
int menu()
{
int select=0,choice;
clear_keybuf();

position_mouse(SCREEN_W/2,SCREEN_H/2);
set_mouse_range(0,0,SCREEN_W,SCREEN_H);
set_mouse_sprite(mouse_bmp);
scare_mouse();
stretch_blit(back,screen,0,0,1023,571,0,0,800,600);
masked_blit(new_game,screen,0,0,500,150,250,47);
masked_blit(high,screen,0,0,500,220,250,48);
masked_blit(settings,screen,0,0,500,290,250,45);
masked_blit(help,screen,0,0,500,360,250,46);
masked_blit(exit_bmp,screen,0,0,500,430,250,48);
unscare_mouse();
show_mouse(screen);
while(select==0)
{
if(mouse_b&1)
{
if(mouse_x>500&&mouse_x<750&&mouse_y>150&&mouse_y<197)
{
select=1;
choice=1;
}
else if(mouse_x>500&&mouse_x<750&&mouse_y>220&&mouse_y<268)
{
select=1;
choice=2;
}
else if(mouse_x>500&&mouse_x<750&&mouse_y>290&&mouse_y<335)
{
select=1;
choice=3;
}
else if(mouse_x>500&&mouse_x<750&&mouse_y>360&&mouse_y<406)
{
select=1;
choice=4;
}
else if(mouse_x>500&&mouse_x<750&&mouse_y>430&&mouse_y<478)
{
select=1;
choice=5;
}
}
else if(keypressed())
{
if(key[KEY_1])
{
select=1;
choice=1;
}
else if(key[KEY_2])
{
select=1;
choice=2;
}
else if(key[KEY_3])
{
select=1;
choice=3;
}
else if(key[KEY_4])
{
select=1;
choice=4;
}
else if(key[KEY_5])
{
select=1;
choice=5;
}
}
}
switch(choice)
{
case 1:
scare_mouse();
rest(200);
start1=rear1=NULL;
setparameteres();
setupscreen();
play_game();
clear_keybuf();
replay_screen();
menu();
break;
case 2:
show_highscore();
break;
case 3:
setting();
break;
case 4:
help_menu();
break;
case 5:
exit(0);
}
}
//********************************* CHECK BOX****************************************************************************
int check_box(int num)
{
double x,y;
x=t[num].x;
y=t[num].y;
if(x>562 && x<750 &&y>20 &&y<85)
{
return 0;
}
else if(x>33 && x<221 && y>471 && y<536)
{
return 0;
}
else
return 1;
}
//***********************************LOAD TANK************************************************************************
int load_tank()
{
int i;
struct SONG * newsong;
sample=load_sample("rat_01.wav");
map=load_map();
for(i=1;i<64;i++)
{
tank[0][i]=create_bitmap(32,32);
clear_to_color(tank[0][i], makecol(255,0,255));
rotate_sprite(tank[0][i],tank[0][0],0,0,itofix(i*4));
}
for(i=1;i<64;i++)
{
tank[1][i]=create_bitmap(32,32);
clear_to_color(tank[1][i], makecol(255,0,255));
rotate_sprite(tank[1][i],tank[1][0],0,0,itofix(i*4));
}
}
//***************************************SETUPSCREEN*************************************************************************
int setupscreen()
{
t[0].x=100;
t[0].y=450;
t[0].dir=3;

t[1].x=645;
t[1].y=135;
t[1].dir=21;

stretch_blit(map,screen,0,0,992,1056,0,0,800,600);
masked_blit(tank[0][t[0].dir],screen,0,0,t[0].x,t[0].y,32,32);
masked_blit(tank[1][t[1].dir],screen,0,0,t[1].x,t[1].y,32,32);
}


//************************************TANK WAR PAGE ********************************************************************
int tank_war()
{
int i,found=0,a;
clear_keybuf();
stretch_blit(tank_image,screen,0,0,449,318,0,0,800,600);
for(i=0;i<=400;i+=5)
{
clear_keybuf();
stretch_blit(tank_image,screen,0,0,449,318,0,0,800,600);
 masked_blit(tank_font,screen,0,0,10,480-i,227,68);
 masked_blit(war,screen,0,0,650-i,80,196,68);
rest(10);
}
masked_blit(press,screen,0,0,280,10,318,31);
while(!keypressed()&&found==0)
{
poll_joystick();
for(a=0;a<joy[0].num_buttons;a++)
{
if(joy[0].button[a].b)
{
found=1;
break;
}
}
}
}
//***********************************CHECK TAMK FOR SCREEN****************************************************



int check_tank(int num)
{
if(t[num].x<0||t[num].x>SCREEN_W-32||t[num].y<0||t[num].y>SCREEN_H-32)
{
return 0;
}
else
{
return 1;
}
}
//**************************************TURNRIGHT****************************************************************

int turnright(int num)
{
t[num].dir++;
if(t[num].dir>63)
{
t[num].dir=0;
}
}
//**********************************TURNLEFT************************************************************************
int turnleft(int num)
{
t[num].dir--;
if(t[num].dir<0)
{
t[num].dir=63;
}
}
//************************************MOVE UP***************************************************************************
int move_up(int num)
{
int check;
t[num].x+=2*cos(((t[num].dir*5.625)*PI)/180);
t[num].y+=2*sin(((t[num].dir*5.625)*PI)/180);
check=check_box(num);
if(check==0)
{
t[num].x-=2*cos(((t[num].dir*5.625)*PI)/180);
t[num].y-=2*sin(((t[num].dir*5.625)*PI)/180);
}
check=check_path(num);
if(check==0)
{
t[num].x-=2*cos(((t[num].dir*5.625)*PI)/180);
t[num].y-=2*sin(((t[num].dir*5.625)*PI)/180);
}
check=check_tank(num);
if(check==0)
{
t[num].x-=2*cos(((t[num].dir*5.625)*PI)/180);
t[num].y-=2*sin(((t[num].dir*5.625)*PI)/180);
}

}
//**********************************MOVE DOWN*************************************************************************
int move_down(int num)
{
int check;
t[num].x-=2*cos(((t[num].dir*5.625)*PI)/180);
t[num].y-=2*sin(((t[num].dir*5.625)*PI)/180);
check=check_box(num);
if(check==0)
{
t[num].x+=2*cos(((t[num].dir*5.625)*PI)/180);
t[num].y+=2*sin(((t[num].dir*5.625)*PI)/180);
}
check=check_tank(num);
if(check==0)
{
t[num].x+=2*cos(((t[num].dir*5.625)*PI)/180);
t[num].y+=2*sin(((t[num].dir*5.625)*PI)/180);
}
}

//******************************************SHOWTANK*******************************************************************
void showtank()
{
stretch_blit(map,screen,0,0,992,1056,0,0,800,600);
masked_blit(tank[0][t[0].dir],screen,0,0,t[0].x,t[0].y,32,32);
masked_blit(tank[1][t[1].dir],screen,0,0,t[1].x,t[1].y,32,32);
if(b[0].alive==1)
{
masked_blit(bullet,screen,0,0,b[0].x,b[0].y,9,9);
}
if(b[1].alive==1)
{
masked_blit(bullet,screen,0,0,b[1].x,b[1].y,9,9);
}
print_health();
}
//***********************************INPUT*************************************************************
int input()
{
poll_joystick();
if(key[KEY_A])
{
turnleft(0);
showtank();
}
if(key[KEY_D])
{
turnright(0);
showtank();
}
if(key[KEY_W])
{
move_up(0);
showtank();
}
if(key[KEY_S])
{
move_down(0);
showtank();
}
if(key[KEY_SPACE])
{
if(b[0].alive==0)
{
stop_sample(sample);
bc[0].fired++;
firebullet(0);
}
}

if(key[KEY_LEFT])
{
turnleft(1);
showtank();
}
if(key[KEY_RIGHT])
{
turnright(1);
showtank();
}

if(key[KEY_UP])
{
move_up(1);
showtank();
}
if(key[KEY_DOWN])
{
move_down(1);
showtank();
}
if(key[KEY_ENTER])
{
if(b[1].alive==0)
{
stop_sample(sample);
bc[1].fired++;
firebullet(1);
}
}
if(key[KEY_F5])
{
savegame();
}
if(key[KEY_ESC])
{
rest(100);
unscare_mouse();
resumemenu();
}
if(key[KEY_N])
{
stop_sample(song);
rest(50);
pos_value=-1;
}
}
//******************************************SONG THREAD******************************************
void *thread0(void *data)
{
currsong=start;
while(currsong)
{
if(pos_value>171 || pos_value==0)
{
pos_value=0;
song=load_sample(currsong->song_name);
play_sample(song,128,128,1000,0);
}
if(pause_music==1)
{
stop_sample(song);
while(pause_music)
{
rest(100);
}
}

rest(1000);
++pos_value;



if(pos_value>171 || pos_value==0)
{
destroy_sample(song);
currsong=currsong->next;
}
}
}
//******************************************DESTROY IMAGES *******************************************
int destroy()
{
int i;
for(i=0;i<32;i++)
{
destroy_bitmap(tank[0][i]);
destroy_bitmap(tank[1][i]);
}
destroy_sample(sample);
destroy_sample(track);
}
//******************************************SHOW SOUND ERROR SCREEN******************************************
int sound_error()
{
stretch_blit(bitmap_null,screen,0,0,640,360,0,0,800,600);
masked_blit(error,screen,0,0,230,10,352,101);
masked_blit(error_sound,screen,0,0,180,200,489,92);
while(1)
{
if(key[KEY_Y])
{
break;
}
if(key[KEY_N])
{
exit(0);
}
}
}
//******************************************REPLAY SCREEN*******************************************************************
void replay_screen()
{
int finish=1;
setupscreen();
masked_blit(replayimage,screen,0,0,170,100,392,110);
rect(screen,150,520,170,540,0);
textout_ex(screen,font,"P",157,527,0,-1);
textout_ex(screen,font,"Preview",180,527,0,-1);
rect(screen,350,520,370,540,0);
textout_ex(screen,font,"S",357,527,0,-1);
textout_ex(screen,font,"Save",380,527,0,-1);
rect(screen,550,520,570,540,0);
vline(screen,565,525,535,0);
hline(screen,555,535,565,0);
line(screen,555,535,558,532,0);
line(screen,555,535,558,538,0);
textout_ex(screen,font,"Exit",580,527,0,-1);
while(finish)
{
if(keypressed())
{
if(key[KEY_P])
{
finish=0;
replay(start1);
}
if(key[KEY_S])
{
pause_music=1;
finish=0;
savereplay(start1);
}
pause_music=0;
pos_value=-1;
if(key[KEY_ENTER])
{
clear_keybuf();
menu();
}
}
}
}
//*******************************************RESUME MENU********************************************
void resumemenu()
{
FONT *f,*neuro;
int x=70,y=295;
int choice=1;
int select=0;
BITMAP * snapitc,*neuropol;
snapitc=load_bitmap("snap.itc.bmp",NULL);
neuropol=load_bitmap("neuro.bmp",NULL);
f=grab_font_from_bitmap(snapitc);
neuro=grab_font_from_bitmap(neuropol);
scare_mouse();
clear_bitmap(screen);
rectfill(screen,100,100,150,150,BROWN);
rectfill(screen,103,103,147,147,0);
rectfill(screen,110,150,140,155,BROWN);
rectfill(screen,113,106,121,142,BROWN);
rectfill(screen,130,106,138,142,BROWN);
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
rect(screen,550,520,570,540,WHITE);
vline(screen,565,525,535,WHITE);
hline(screen,555,535,565,WHITE);
line(screen,555,535,558,532,WHITE);
line(screen,555,535,558,538,WHITE);
textout_ex(screen,font,"Select",580,527,WHITE,-1);
textout_ex(screen,f,"Pause Menu",175,100,0,-1);
textout_ex(screen,neuro,"Resume Game",75,300,0,-1);
textout_ex(screen,neuro,"End Game",500,300,0,-1);
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
do
{
poll_joystick();
if(key[KEY_RIGHT] || joy[0].stick[0].axis[0].d2 || joy[1].stick[0].axis[0].d2)
{
scare_mouse();
rect(screen,x,y,x+270,y+50,0);
unscare_mouse();
x+=425;
if(x==70)
{
choice=1;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
}
else if(x==495)
{
choice=2;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
}
if(x>495)
{
x=70;
choice=1;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
}
rest(200);
}
else if(key[KEY_LEFT] || joy[0].stick[0].axis[0].d1 || joy[1].stick[0].axis[0].d1)
{
scare_mouse();
rect(screen,x,y,x+270,y+50,0);
unscare_mouse();
x-=425;
if(x==70)
{
choice=1;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
}
else if(x==495)
{
choice=2;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
}
else if(x<70)
{
x=495;
choice=2;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
}
rest(200);
}
else if(key[KEY_ESC] || joy[0].button[3].b || joy[1].button[3].b)
{
rest(200);
scare_mouse();
return;
}
else if(mouse_x>70 && mouse_x<340 && mouse_y>295 && mouse_y<345)
{
scare_mouse();
rect(screen,x,y,x+270,y+50,0);
unscare_mouse();
x=70;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
if(mouse_b&1)
{
choice=1;
select=1;
}
rest(60);
}
else if (mouse_x>495 && mouse_x<765 && mouse_y>295 && mouse_y<345)
{
scare_mouse();
rect(screen,x,y,x+270,y+50,0);
unscare_mouse();
x=495;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
if(mouse_b&1)
{
choice=2;
select=1;
}
rest(60);
}
}
while(!key[KEY_ENTER] && !joy[0].button[1].b && !joy[1].button[1].b && !select);

switch(choice)
{
case 1:
rest(200);
scare_mouse();
return;
break;

case 2:
clear_keybuf();
menu();
break;
}
}
//*****************************************PARAMETERES*********************************
void setparameteres()
{
t[0].score=0;
t[1].score=0;
t[0].health=100;
t[1].health=100;
t[0].explode=0;
t[1].explode=0;
b[0].alive=0;
b[1].alive=0;
gameover=0;
}
//*****************************************PLAY GAME****************************************
void play_game()
{

struct REPLAY *newptr2;
BITMAP *fb,*fg;
FONT *forteb,*forteg;
while(!gameover)
{
updatebullet(0);
updatebullet(1);
check_bullet(0);
check_bullet(1);
explosion(0);
explosion(1);
check_health(0);
check_health(1);
rest(60);
newptr2=create_node(t[0].x,t[0].y,t[0].health,t[0].dir,t[0].explode,t[1].x,t[1].y,t[1].health,t[1].dir,t[1].explode,b[0].x,b[0].y,b[1].x,b[1].y,b[0].alive,b[1].alive);
insert_node(newptr2);
if(keypressed())
{
input();
}
if(num_joysticks)
{
poll_joystick();
joy_input(0);
joy_input(1);
}
showtank();
}

if(t[0].score>t[1].score)
{
savescore(0);
clear_bitmap(screen);
clear_keybuf();
fg=load_bitmap("forteg.bmp",NULL);
forteg=grab_font_from_bitmap(fg);
textout_ex(screen,forteg,"GREEN Tank Wins",100,250,0,-1);
readkey();
rest(200);
clear_keybuf();
}
else if(t[1].score>t[0].score)
{
savescore(1);
clear_bitmap(screen);
clear_keybuf();
fb=load_bitmap("forteb.bmp",NULL);
forteb=grab_font_from_bitmap(fb);
textout_ex(screen,forteb,"BLUE Tank Wins",120,250,0,-1);
readkey();
rest(200);
clear_keybuf();
}
}
//**********************************LOAD IMAGES *************************************************
void load_images() 
{
tank[0][0]=(BITMAP*)images[17].dat;
tank[1][0]=(BITMAP*)images[18].dat;
bullet=(BITMAP*)images[1].dat;
explode=(BITMAP*)images[4].dat;
progress=(BITMAP*)images[5].dat;
error_sound=(BITMAP*)images[16].dat;
error=(BITMAP*)images[2].dat;
mouse_bmp=load_bitmap("mouse.bmp",NULL);
new_game=(BITMAP*)images[9].dat;
high=(BITMAP*)images[7].dat;
help=(BITMAP*)images[6].dat;
settings=(BITMAP*)images[15].dat;
exit_bmp=(BITMAP*)images[3].dat;
bitmap_null=(BITMAP*)images[10].dat;
tank_image=(BITMAP*)images[20].dat;
tank_font=(BITMAP*)images[19].dat;
war=(BITMAP*)images[21].dat;
press=(BITMAP*)images[13].dat;
welcome=(BITMAP*)images[11].dat;
present=(BITMAP*)images[12].dat;
replayimage=(BITMAP*)images[14].dat;
back=(BITMAP*)images[0].dat;
}
//********************************************CREATE HSCORE NODE****************************
struct HSCORE *create_hscore_node(int num)
{
struct HSCORE *newptr;
newptr=(struct HSCORE*)malloc(sizeof(struct HSCORE));
newptr->score=num;
newptr->next=NULL;
return(newptr);
}
//******************************************INSERT HSCORE NODE****************************
void insert_hscore_node(struct HSCORE *p)
{
if(start2==NULL)
{
start2=rear2=p;
}
else
{
rear2->next=p;
rear2=p;
}
}//***************************************COUNT HSCORE**********************************
int count_hscore(struct HSCORE *p)
{
int i=0;
while(p)
{
++i;
p=p->next;
}
return(i);
}
//*******************************************SORT HSCORE LIST BUBBLE********************************
void sort_hscore_list_by_bubble(struct HSCORE *p)
{
int count,temp;
int i,j;
count=count_hscore(start2);
for(i=0;i<=count-2;i++)
{
p=start2;
for(j=0;j<=count-i-2;j++)
{
if(p->score < p->next->score)
{
temp=p->score;
p->score=p->next->score;
p->next->score=temp;
}
p=p->next;
}
}
}
//******************************************SORT HSCORE LIST SELECTION********************************
void sort_hscore_list_by_selection(struct HSCORE *p)
{
int count,temp;
int i,j;
count=count_hscore(start2);
for(i=0;i<=count-1;i++)
{
p=start2;
for(j=i+1;j<=count-1;j++)
{
if(p->score < p->next->score)
{
temp=p->score;
p->score=p->next->score;
p->next->score=temp;
}
p=p->next;
}
}
}
//******************************************SORT HSCORE LIST INSERTION****************************************
void sort_hscore_list_by_insertion(struct HSCORE *p)
{
int i,j,k,l;
int temp;
int count;
count=count_hscore(start2);
for(i=1;i<=count-1;i++)
{
p=start2;
for(j=0;j<i;j++)
{
if(p->score < p->next->score)
{
temp=p->score;
p->score=p->next->score;
for(k=i;k>j;k--)
{
p->next->score=p->score;
}
p->next->score=temp;
}
p=p->next;
}
}
}
//*******************************************SAVESCORE***************************************
void savescore(int num)
{
start2=rear2=NULL;
struct HSCORE *newptr,*minadd,*p;
FILE *fp;
int min;
int count;
int found=0;
fp=fopen("score.sav","rb");
if(fp==NULL)
{
allegro_message("Error");
}
while(fread(&load_hscore,sizeof(load_hscore),1,fp)==1)
{
newptr=create_hscore_node(load_hscore.score);
insert_hscore_node(newptr);
}
fclose(fp);
count=count_hscore(start2);
if(count<10) 
{
p=start2;
while(p)
{
if(p->score==t[num].score)
{
found=1;
break;
}
p=p->next;
}
if(found==0)
{
newptr=create_hscore_node(t[num].score);
insert_hscore_node(newptr);
}
}


else
{
found=0;
p=start2;
while(p)
{
if(p->score==t[num].score)
{
found=1;
break;
}
p=p->next;
}
if(found==0)
{
p=start2;
min=t[num].score;
while(p)
{
if(p->score<min)
{
min=p->score;
minadd=p;
}
p=p->next;
}
if(min<t[num].score)
{
minadd->score=t[num].score;
}
}
}
fp=fopen("score.sav","wb");
p=start2;
while(p)
{
load_hscore.score=p->score;
fwrite(&load_hscore,sizeof(load_hscore),1,fp);
p=p->next;
}
fclose(fp);
}
//******************************************SHOW HIGHSCORE*************************************
void show_highscore()
{
clear_keybuf();
scare_mouse();
start2=rear2=NULL;
clear_bitmap(screen);
struct HSCORE *p,*newptr;
FILE *fp;
FONT *chiller,*bold;
BITMAP *c,*back,*b;
char score[4];
int x=380,y=150;
int choice;
int i=0;
c=load_bitmap("matura.bmp",NULL);
b=load_bitmap("bold.bmp",NULL);
bold=grab_font_from_bitmap(b);
back=load_bitmap("blue.bmp",NULL);
chiller=grab_font_from_bitmap(c);
stretch_blit(back,screen,0,0,1274,576,0,0,800,600);
fp=fopen("score.sav","rb");
if(fp==NULL)
{
pause_music=1;
rest(800);
allegro_message("No Record");
menu();
}
textout_ex(screen,bold,"High Scores",280,100,0,-1);
while(fread(&load_hscore,sizeof(load_hscore),1,fp)==1)
{
newptr=create_hscore_node(load_hscore.score);
insert_hscore_node(newptr);
}
fclose(fp);
choice=rand()%3+1;
switch(choice)
{
case 1:
textout_ex(screen,chiller,"Sorted By : Bubble Sort",20,20,0,-1);
sort_hscore_list_by_bubble(start2);
break;
case 2:
textout_ex(screen,chiller,"Sorted By : Selection Sort",20,20,0,-1);
sort_hscore_list_by_selection(start2);
break;
case 3:
textout_ex(screen,chiller,"Sorted By : Insertion Sort",20,20,0,-1);
sort_hscore_list_by_insertion(start2);
break;
}
p=start2;
for(i=1;i<=10;i++)
{
y+=38;
if(p!=NULL)
{
//itoa(i,score,10);
sprintf(score,"%d",i);
textout_ex(screen,chiller,score,x-45,y,0,-1);
//itoa(p->score,score,10);
sprintf(score,"%d",p->score);
textout_ex(screen,chiller,score,x,y,0,-1);
p=p->next;
}
else
{
//itoa(i,score,10);
sprintf(score,"%d",i);
textout_ex(screen,chiller,score,x-45,y,0,-1);
textout_ex(screen,chiller,"-----",x,y,0,-1);
}
}
readkey();
menu();
}

//****************************************HELP*****************************************************
void help_menu()
{
scare_mouse();
clear_bitmap(screen);
int x=70,y=295;
int choice=1;
int select=0;
BITMAP *n,*s;
FONT *snap,*neuro;
s=load_bitmap("snap.itc.bmp",NULL);
n=load_bitmap("neuro.bmp",NULL);
snap=grab_font_from_bitmap(s);
neuro=grab_font_from_bitmap(n);
textout_ex(screen,snap,"Help Me Out ???",220,100,0,-1);
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
rect(screen,550,520,570,540,WHITE);
vline(screen,565,525,535,WHITE);
hline(screen,555,535,565,WHITE);
line(screen,555,535,558,532,WHITE);
line(screen,555,535,558,538,WHITE);
textout_ex(screen,font,"Select",580,527,WHITE,-1);
textout_ex(screen,neuro,"Controls",75,300,0,-1);
textout_ex(screen,neuro,"Rules",500,300,0,-1);
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
do
{
poll_joystick();
if(key[KEY_RIGHT]|| joy[0].stick[0].axis[0].d2 || joy[1].stick[0].axis[0].d2)
{
scare_mouse();
rect(screen,x,y,x+270,y+50,0);
unscare_mouse();
x+=425;
if(x==70)
{
choice=1;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
}
else if(x==495)
{
choice=2;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
}
if(x>495)
{
x=70;
choice=1;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
}
rest(200);
}
else if(key[KEY_LEFT] || joy[0].stick[0].axis[0].d1 || joy[1].stick[0].axis[0].d1)
{
scare_mouse();
rect(screen,x,y,x+270,y+50,0);
unscare_mouse();
x-=425;
if(x==70)
{
choice=1;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
}
else if(x==495)
{
choice=2;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
}
else if(x<70)
{
x=495;
choice=2;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
}
rest(200);
}
else if(key[KEY_ESC] || joy[0].button[3].b|| joy[1].button[3].b)
{
rest(200);
menu();
}
else if(mouse_x>70 && mouse_x<340 && mouse_y>295 && mouse_y<345)
{
scare_mouse();
rect(screen,x,y,x+270,y+50,0);
unscare_mouse();
x=70;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
if(mouse_b&1)
{
choice=1;
select=1;
}
rest(60);
}
else if (mouse_x>495 && mouse_x<765 && mouse_y>295 && mouse_y<345)
{
scare_mouse();
rect(screen,x,y,x+270,y+50,0);
unscare_mouse();
x=495;
scare_mouse();
rect(screen,x,y,x+270,y+50,BROWN);
unscare_mouse();
if(mouse_b&1)
{
choice=2;
select=1;
}
rest(60);
}
}
while(!key[KEY_ENTER] && !joy[0].button[1].b && !joy[1].button[1].b && !select);
switch(choice)
{
case 1:
rest(200);
controls();
break;
case 2:
rest(200);
rules();
break;
}
}
//*****************************************CONTROLS**************************************************
void controls()
{
scare_mouse();
clear_bitmap(screen);
unscare_mouse();
clear_keybuf();
BITMAP*c,*ch;
int choice=1;
c=load_bitmap("bold.bmp",NULL);
ch=load_bitmap("chiller.bmp",NULL);
FONT *century,*chiller;
century=grab_font_from_bitmap(c);
chiller=grab_font_from_bitmap(ch);
clear_bitmap(screen);
clear_keybuf();
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
rect(screen,550,520,570,540,WHITE);
vline(screen,565,525,535,WHITE);
hline(screen,555,535,565,WHITE);
line(screen,555,535,558,532,WHITE);
line(screen,555,535,558,538,WHITE);
textout_ex(screen,font,"Next",580,527,WHITE,-1);
textout_ex(screen,century,"Green Tank",280,50,0,-1);
textout_ex(screen,chiller,"KEYBOARD",110,170,0,-1);
textout_ex(screen,chiller,"JOYSICK",610,170,0,-1);
textout_ex(screen,chiller,"Move Up : W",90,220,0,-1);
textout_ex(screen,chiller,"Move Up : Y-AXIS",590,220,0,-1);
textout_ex(screen,chiller,"Move Down : S",90,260,0,-1);
textout_ex(screen,chiller,"Move Down : Y-AXIS",590,260,0,-1);
textout_ex(screen,chiller,"Turn Left : A",90,300,0,-1);
textout_ex(screen,chiller,"Turn Left : X-AXIS",590,300,0,-1);
textout_ex(screen,chiller,"Turn Right : D",90,340,0,-1);
textout_ex(screen,chiller,"Turn Right : X-AXIS",590,340,0,-1);
textout_ex(screen,chiller,"Fire : SpaceBar",90,380,0,-1);
textout_ex(screen,chiller,"Fire : Any Button",590,380,0,-1);
while(!key[KEY_ESC])
{
if(keypressed() && !key[KEY_ENTER])
{
choice++;
if(choice>2)
{
choice=1;
}
if(choice==1)
{
clear_bitmap(screen);
clear_keybuf();
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
rect(screen,550,520,570,540,WHITE);
vline(screen,565,525,535,WHITE);
hline(screen,555,535,565,WHITE);
line(screen,555,535,558,532,WHITE);
line(screen,555,535,558,538,WHITE);
textout_ex(screen,font,"Next",580,527,WHITE,-1);
textout_ex(screen,century,"Green Tank",280,50,0,-1);
textout_ex(screen,chiller,"KEYBOARD",110,170,0,-1);
textout_ex(screen,chiller,"JOYSICK",610,170,0,-1);
textout_ex(screen,chiller,"Move Up : W",90,220,0,-1);
textout_ex(screen,chiller,"Move Up : Y-AXIS",590,220,0,-1);
textout_ex(screen,chiller,"Move Down : S",90,260,0,-1);
textout_ex(screen,chiller,"Move Down : Y-AXIS",590,260,0,-1);
textout_ex(screen,chiller,"Turn Left : A",90,300,0,-1);
textout_ex(screen,chiller,"Turn Left : X-AXIS",590,300,0,-1);
textout_ex(screen,chiller,"Turn Right : D",90,340,0,-1);
textout_ex(screen,chiller,"Turn Right : X-AXIS",590,340,0,-1);
textout_ex(screen,chiller,"Fire : SpaceBar",90,380,0,-1);
textout_ex(screen,chiller,"Fire : Any Button",590,380,0,-1);
}
else if(choice==2)
{
clear_keybuf();
clear_bitmap(screen);
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
rect(screen,550,520,570,540,WHITE);
vline(screen,565,525,535,WHITE);
hline(screen,555,535,565,WHITE);
line(screen,555,535,558,532,WHITE);
line(screen,555,535,558,538,WHITE);
textout_ex(screen,font,"Previous",580,527,WHITE,-1);
textout_ex(screen,century,"Blue Tank",280,50,0,-1);
textout_ex(screen,chiller,"KEYBOARD",110,170,0,-1);
textout_ex(screen,chiller,"JOYSICK",610,170,0,-1);
textout_ex(screen,chiller,"Move Up : Up ",90,220,0,-1);
textout_ex(screen,chiller,"Move Up : Y-AXIS",590,220,0,-1);
textout_ex(screen,chiller,"Move Down : Down",90,260,0,-1);
textout_ex(screen,chiller,"Move Down : Y-AXIS",590,260,0,-1);
textout_ex(screen,chiller,"Turn Left : Left",90,300,0,-1);
textout_ex(screen,chiller,"Turn Left : X-AXIS",590,300,0,-1);
textout_ex(screen,chiller,"Turn Right : Right",90,340,0,-1);
textout_ex(screen,chiller,"Turn Right : X-AXIS",590,340,0,-1);
textout_ex(screen,chiller,"Fire : Return",90,380,0,-1);
textout_ex(screen,chiller,"Fire : Any Button",590,380,0,-1);
}
}
}
clear_keybuf();
rest(200);
help_menu();
}
//************************************** RULES*****************************************************
void rules()
{
scare_mouse();
clear_bitmap(screen);
unscare_mouse();
BITMAP *c,*b;
FONT *century,*bold;
c=load_bitmap("century.bmp",NULL);
b=load_bitmap("bold.bmp",NULL);
century=grab_font_from_bitmap(c);
bold=grab_font_from_bitmap(b);
textout_ex(screen,bold,"Game Rules",310,0,0,-1);
textout_ex(screen,bold,"1.)",0,110,0,-1);
textout_ex(screen,century,"This is a simple two player game.",60,120,0,-1);
textout_ex(screen,bold,"2.)",0,150,0,-1);
textout_ex(screen,century,"The main aim of the game is to defeat your opponent.",60,160,0,-1);
textout_ex(screen,bold,"3.)",0,190,0,-1);
textout_ex(screen,century,"The game will end when the health of any one player is completly vanished.",60,200,0,-1);
textout_ex(screen,bold,"4.)",0,230,0,-1);
textout_ex(screen,century,"For every bullet you hit you get 2 points and for every bullet you are hitted by you are",60,240,0,-1);
textout_ex(screen,century,"awarded by -1 points.",60,280,0,-1);
textout_ex(screen,bold,"ALL THE BEST . . .",260,400,0,-1);
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
clear_keybuf();
readkey();
clear_keybuf();clear_keybuf();
rest(200);
help_menu();
}
//**************************************JOY INPUT*****************************************
void joy_input(int num)
{
int a;
if(joy[num].stick[0].axis[0].d1)
turnleft(num);
if(joy[num].stick[0].axis[0].d2)
turnright(num);
if(joy[num].button[0].b)
move_up(num);
if(joy[num].button[2].b)
move_down(num);
if(joy[0].button[3].b)
{
rest(200);
unscare_mouse();
resumemenu();
}
for(a=0;a<joy[num].num_buttons;a++)
{

if(joy[num].button[a].b && a!=0 && a!=1 && a!=2 && a!=3)
{
if(b[num].alive==0)
{
stop_sample(sample);
bc[num].fired++;
firebullet(num);
}
break;
}
}
}
//*************************************************SETTING MENU********************************************************
void setting()
{
int select=0;
scare_mouse();
clear_bitmap(screen);
FONT *neuro,*snap;
BITMAP *n,*s;
int x=260,y=195;
int choice=1;
n=load_bitmap("neuro.bmp",NULL);
s=load_bitmap("snap.itc.bmp",NULL);
neuro=grab_font_from_bitmap(n);
snap=grab_font_from_bitmap(s);
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
rect(screen,550,520,570,540,WHITE);
vline(screen,565,525,535,WHITE);
hline(screen,555,535,565,WHITE);
line(screen,555,535,558,532,WHITE);
line(screen,555,535,558,538,WHITE);
textout_ex(screen,font,"Select",580,527,WHITE,-1);
textout_ex(screen,snap,"Configure It ! ! !",200,50,0,-1);
textout_ex(screen,neuro,"Load Game",270,200,0,-1);
textout_ex(screen,neuro,"View Replay",270,300,0,-1);
textout_ex(screen,neuro,"Game Trax",270,400,0,-1);
rect(screen,x,y,x+260,y+50,BROWN);
unscare_mouse();
do
{
poll_joystick();
if(key[KEY_DOWN] || joy[0].stick[0].axis[1].d2 || joy[1].stick[0].axis[1].d2)
{
scare_mouse();
rect(screen,x,y,x+260,y+50,0);
unscare_mouse();
y+=100;
if(y==195)
{
scare_mouse();
rect(screen,x,y,x+260,y+50,BROWN);
unscare_mouse();
choice=1;
}
else if(y==295)
{
scare_mouse();
rect(screen,x,y,x+260,y+50,BROWN);
unscare_mouse();
choice=2;
}
else if(y==395)
{
scare_mouse();
rect(screen,x,y,x+260,y+50,BROWN);
unscare_mouse();
choice=3;
}
else if(y>395)
{
y=195;
scare_mouse();
rect(screen,x,y,x+260,y+50,BROWN);
unscare_mouse();
choice=1;
}
rest(200);
}
else if(key[KEY_UP] || joy[0].stick[0].axis[1].d1 || joy[1].stick[0].axis[1].d1)
{
scare_mouse();
rect(screen,x,y,x+260,y+50,0);
unscare_mouse();
y-=100;
if(y==195)
{
scare_mouse();
rect(screen,x,y,x+260,y+50,BROWN);
unscare_mouse();
choice=1;
}
else if(y==295)
{
scare_mouse();
rect(screen,x,y,x+260,y+50,BROWN);
unscare_mouse();
choice=2;
}
else if(y==395)
{
scare_mouse();
rect(screen,x,y,x+260,y+50,BROWN);
unscare_mouse();
choice=3;
}
else if(y<195)
{
y=395;
scare_mouse();
rect(screen,x,y,x+260,y+50,BROWN);
unscare_mouse();
choice=3;
}
rest(200);
}
else if(key[KEY_ESC] || joy[0].button[3].b)
{
rest(200);
menu();
}
else if(mouse_x>260 && mouse_x<520 && mouse_y>195 &&mouse_y<245)
{
scare_mouse();
rect(screen,x,y,x+260,y+50,0);
unscare_mouse();
y=195;
scare_mouse();
rect(screen,x,y,x+260,y+50,BROWN);
unscare_mouse();
if(mouse_b&1)
{
choice=1;
select=1;
}
rest(60);
}
else if(mouse_x>260 && mouse_x<520 && mouse_y>295 &&mouse_y<345)
{
scare_mouse();
rect(screen,x,y,x+260,y+50,0);
unscare_mouse();
y=295;
scare_mouse();
rect(screen,x,y,x+260,y+50,BROWN);
unscare_mouse();
if(mouse_b&1)
{
choice=2;
select=1;
}
rest(60);
}
else if(mouse_x>260 && mouse_x<520 && mouse_y>395 &&mouse_y<445)
{
scare_mouse();
rect(screen,x,y,x+260,y+50,0);
unscare_mouse();
y=395;
scare_mouse();
rect(screen,x,y,x+260,y+50,BROWN);
unscare_mouse();
if(mouse_b&1)
{
choice=3;
select=1;
}
rest(60);
}

}
while(!key[KEY_ENTER] && !joy[0].button[1].b && !joy[0].button[1].b && !select);

switch(choice)
{
case 1:
start1=rear1=NULL;
loadgame();
break;
case 2:
view_replay();
break;
/*case 3:
pause_music=1;
game_trax();
break;*/
}
}
//**********************************************VIEW REPLAY**************************************************
void view_replay()
{
start3=rear3=NULL;
scare_mouse();
clear_keybuf();
clear_bitmap(screen);
BITMAP*c;
FONT *chiller;
struct REPLAY_LIST *p;
int count,select=0;
c=load_bitmap("chiller.bmp",NULL);
chiller=grab_font_from_bitmap(c);
//count=find_file("*.rep");
int x=350 ,y=100;
p=start3;
while(p)
{
textprintf_ex(screen,chiller,x,y,0,-1,"%s",p->rep_name);
y+=50;
p=p->next;
}
x=300;
y=95;
clear_keybuf();
clear_keybuf();
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
rect(screen,500,500,530,520,WHITE);
textout_ex(screen,font,"1",505,507,WHITE,-1);
textout_ex(screen,font,"Preview",535,507,WHITE,-1);
rect(screen,x,y,x+150,y+35,BROWN);
p=start3;
poll_joystick();
do
{
poll_joystick();
if(key[KEY_DOWN] || joy[0].stick[0].axis[1].d2  || joy[1].stick[0].axis[1].d2)
{
rect(screen,x,y,x+150,y+35,0);
y+=50;
if(y>50*(count-1)+95)
{
p=start3;
y=95;
rect(screen,x,y,x+150,y+35,BROWN);
rest(200);
}
else
{
rect(screen,x,y,x+150,y+35,BROWN);
rest(200);
p=p->next;
}
}
else if(key[KEY_UP]  || joy[0].stick[0].axis[1].d1  || joy[1].stick[0].axis[1].d1)
{
rect(screen,x,y,x+150,y+35,0);
y-=50;
if(y<95)
{
y=50*(count-1)+95;
p=rear3;
rect(screen,x,y,x+150,y+35,BROWN);
rest(200);
}
else
{
p=p->prev;
rect(screen,x,y,x+150,y+35,BROWN);
rest(200);
}
}
else if (key[KEY_ESC] || joy[0].button[3].b || joy[1].button[3].b)
{
rest(200);
clear_keybuf();
unscare_mouse();
setting();
}
}
while(!key[KEY_1] && !joy[0].button[1].b);
start1=rear1=NULL;
openreplay(p->rep_name);
replay(start1);
rest(200);
view_replay();
}
//***************************************GAME TRAX**********************************************************************
void game_trax()
{
clear_keybuf();
scare_mouse();
clear_bitmap(screen);
BITMAP * c;
FONT *chiller;
struct SONG *p,*temp;
SAMPLE *trax;
int i;
int x=270,y=50;
c=load_bitmap("chiller.bmp",NULL);
chiller=grab_font_from_bitmap(c);
int scount=34;
p=start;
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
rect(screen,500,500,530,520,WHITE);
textout_ex(screen,font,"1",505,507,WHITE,-1);
textout_ex(screen,font,"Preview",535,507,WHITE,-1);
for(i=1;i<=5;i++)
{
textout_ex(screen,chiller,p->song_name,x,y,0,-1);
y+=100;
p=p->next;
}
p=start;
x=270;
y=50;
rect(screen,x-70,y-10,x+330,y+40,BROWN);
poll_joystick();
do
{
poll_joystick();
if(key[KEY_DOWN]  || joy[0].stick[0].axis[1].d2  || joy[1].stick[0].axis[1].d2)
{
rect(screen,x-70,y-10,x+330,y+40,0);
y+=100;

if(y>50+(4*100) && p==rear)
{
p=start;
y=50;
clear_bitmap(screen);
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
rect(screen,500,500,530,520,WHITE);
textout_ex(screen,font,"1",505,507,WHITE,-1);
textout_ex(screen,font,"Preview",535,507,WHITE,-1);
temp=p;

for(i=1;i<=5;i++)
{
textout_ex(screen,chiller,p->song_name,x,y,0,-1);
y+=100;
p=p->next;
}
y=50;
p=temp;
rect(screen,x-70,y-10,x+330,y+40,BROWN);
rest(200);
}
else if(y>50+(4*100))
{
y=50;
clear_bitmap(screen);
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
rect(screen,500,500,530,520,WHITE);
textout_ex(screen,font,"1",505,507,WHITE,-1);
textout_ex(screen,font,"Preview",535,507,WHITE,-1);
rect(screen,x-70,y-10,x+330,y+40,BROWN);
p=p->next;
temp=p;
for(i=1;i<=5;i++)
{
textout_ex(screen,chiller,p->song_name,x,y,0,-1);
y+=100;
p=p->next;
}
p=temp;
y=50;
rest(200);
}

else
{
rect(screen,x-70,y-10,x+330,y+40,BROWN);
p=p->next;
rest(200);
}
}
if(key[KEY_UP] || joy[0].stick[0].axis[1].d1  || joy[1].stick[0].axis[1].d1)
{
rect(screen,x-70,y-10,x+330,y+40,0);
y-=100;

if(y<50 && p==start)
{
p=rear;
y=450;
clear_bitmap(screen);
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
rect(screen,500,500,530,520,WHITE);
textout_ex(screen,font,"1",505,507,WHITE,-1);
textout_ex(screen,font,"Preview",535,507,WHITE,-1);
temp=p;
for(i=1;i<=5;i++)
{
textout_ex(screen,chiller,p->song_name,x,y,0,-1);
y-=100;
p=p->previous;
}
y=450;
p=temp;
rect(screen,x-70,y-10,x+330,y+40,BROWN);
rest(200);
}


else if(y<50)
{
y=450;
clear_bitmap(screen);
rect(screen,100,500,130,520,WHITE);
textout_ex(screen,font,"ESC",105,507,WHITE,-1);
textout_ex(screen,font,"Back",135,507,WHITE,-1);
rect(screen,500,500,530,520,WHITE);
textout_ex(screen,font,"1",505,507,WHITE,-1);
textout_ex(screen,font,"Preview",535,507,WHITE,-1);
rect(screen,x-70,y-10,x+330,y+40,BROWN);
p=p->previous;
temp=p;
for(i=1;i<=5;i++)
{
textout_ex(screen,chiller,p->song_name,x,y,0,-1);
y-=100;
p=p->previous;
}

p=temp;
y=450;
rect(screen,x-70,y-10,x+330,y+40,BROWN);
rest(200);
}
else
{
rect(screen,x-70,y-10,x+330,y+40,BROWN);
p=p->previous;
rest(200);
}

}
if(key[KEY_1] || joy[0].button[1].b)
{
stop_sample(trax);
trax=load_sample(p->song_name);
play_sample(trax,128,128,1000,0);
}
}
while(!key[KEY_ESC] && !joy[0].button[3].b);
stop_sample(trax);
clear_bitmap(screen);
pause_music=0;
pos_value=-1;
rest(200);
unscare_mouse();
setting();

}
//***************************************LOGIN*************************************************************
void login()
{
clear_bitmap(screen);
start:
char id[15];
char pwd[15];
int i;
int index=0;
int select=0;
int x=325,y=308;
int x1=260,y1=580;
BITMAP *n;
FILE *fp;
int search=0;
FONT *neuro;
n=load_bitmap("neuro.bmp",NULL);
neuro=grab_font_from_bitmap(n);
textout_ex(screen,neuro,"Player's Sign UP / Sign IN",200,10,0,-1);
textout_ex(screen,font,"User ID",230,308,WHITE,-1);
rectfill(screen,320,300,480,320,0);
rect(screen,320,300,480,320,BROWN);
textout_ex(screen,font,"Password",230,348,WHITE,-1);
rectfill(screen,320,340,480,360,0);
rect(screen,320,340,480,360,BROWN);

rectfill(screen,0,y1,800,y1+20,0);
while(!key[KEY_ENTER]&&!key[KEY_TAB])
{
textout_ex(screen,font,"Press ENTER or TAB to change the field",x1,y1,WHITE,-1);
if(key[KEY_BACKSPACE])
{
clear_keybuf();
rest(200);
rectfill(screen,320,300,480,320,0);
rect(screen,320,300,480,320,BROWN);
index-=2;
if(index<0)
{
index=0;
}
x=325;
for(i=0;i<index;i++)
{
textprintf_ex(screen,font,x,y,TOMATO,-1,"%c",id[i]);
x+=10;
}
}
else if(key[KEY_ESC])
{
exit(0);
}
else
{
if(index<14)
{
id[index]=readkey();
textprintf_ex(screen,font,x,y,TOMATO,-1,"%c",id[index]);
index++;
x+=10;
}
clear_keybuf();
}
}
id[index-1]='\0';
x-=10;
rectfill(screen,x1-10,y1-10,x1+500,y1+20,0);
textprintf_ex(screen,font,x,y,0,-1,"%c",'^');
index=0;
x=325;
y=348;
clear_keybuf();
rest(200);

while(!key[KEY_ENTER])
{
textout_ex(screen,font,"Press ENTER to login    Press Tab to change field",x1-50,y1,WHITE,-1);
if(key[KEY_BACKSPACE])
{
clear_keybuf();
rest(200);
rectfill(screen,320,340,480,360,0);
rect(screen,320,340,480,360,BROWN);
index-=2;
if(index<0)
{
index=0;
}
x=325;

for(i=0;i<index;i++)
{
textprintf_ex(screen,font,x,y,TOMATO,-1,"%c",'*');
x+=10;
}
}
else if(key[KEY_TAB])
{
clear_keybuf();
rest(200);
goto start;
}
else if(key[KEY_ESC])
{
exit(0);
}
else
{
if(index<14)
{
pwd[index]=readkey();
textprintf_ex(screen,font,x,y,TOMATO,-1,"%c",'*');
index++;
x+=10;
}
clear_keybuf();
}
}

pwd[index-1]='\0';

search=search_login_tree(id);
if(search==1)
{
search=0;
search=check_id_pwd(id,pwd);
if(search==1)
{
menu();

}
else
{
clear_bitmap(screen);
textout_ex(screen,neuro,"Wrong Password",270,300,BROWN,-1);
readkey();
exit(0);
}
             
}
else
{
    
fp=fopen("user.dat","rb+");
strcpy(load_login_tree.user_id,id);
strcpy(load_login_tree.password,pwd);
fseek(fp,0,SEEK_END);
fwrite(&load_login_tree,sizeof(load_login_tree),1,fp);
fclose(fp);
menu();
}
}
//******************************CREATE LOGIN TREE NODE***************************************
struct LOGIN_TREE *create_login_tree_node(char id[],char pwd[])
{
struct LOGIN_TREE *newptr;
newptr=(struct LOGIN_TREE*)malloc(sizeof(struct LOGIN_TREE));
strcpy(newptr->user_id,id);
strcpy(newptr->password,pwd);
newptr->left=NULL;
newptr->right=NULL;
return(newptr);
}
//********************************INSERT LOGIN TREE NODE*****************************************
void insert_login_tree_node(struct LOGIN_TREE *p)
{
if(root==NULL)
{
root=p;
}
else
{
current=root;
while(current->left && current->right)
{
if(strcmp(current->user_id,p->user_id)>0)
{
current=current->left;
}
else if(strcmp(current->user_id,p->user_id)<0)
{
current=current->right;
}
}
if(strcmp(current->user_id,p->user_id)>0)
{
current->left=p;
}
else
{
current->right=p;
}
}
}
//******************************************SEARCH LOGIN TREE****************************************
int search_login_tree(char id[])
{
struct LOGIN_TREE*p;
p=root;
while(p)
{
if(strcmp(p->user_id,id)>0)
{
p=p->left;
}
else if(strcmp(p->user_id,id)<0)
{
p=p->right;
}
else if(strcmp(p->user_id,id)==0)
{
return 1;
}
else
return 0;
}
}
//************************************** CHECK ID AND PWD********************************************
int check_id_pwd(char id[],char pwd[])
{
struct LOGIN_TREE*p;
p=root;
while(p)
{
if(strcmp(p->user_id,id)>0)
{
p=p->left;
}
else if(strcmp(p->user_id,id)<0)
{
p=p->right;
}
else if(strcmp(p->user_id,id)==0 && strcmp(p->password,pwd)==0)
{
return 1;
}
else
{
return 0;
}
}
}
//******************************************CREATE TREE****************************************
void create_tree()
{
FILE *fp;
struct LOGIN_TREE *newptr;
fp=fopen("user.dat","rb");
if(fp!=NULL)
{
while(fread(&load_login_tree,sizeof(load_login_tree),1,fp)==1)
{
newptr=create_login_tree_node(load_login_tree.user_id,load_login_tree.password);
insert_login_tree_node(newptr);
}
}
fclose(fp);
}


//****************************************** MAIN ******************************************
int main(void)
{
allegro_init();
install_keyboard();
install_mouse();
install_joystick(JOY_TYPE_AUTODETECT);
install_timer();
poll_joystick();
set_color_depth(32);
set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0);
images=load_datafile("images.dat");
if(!images)
{
allegro_message("Error loading file");
exit(0);
}
load_images();
if(install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL)!=0)
{
sound_error();
}
else
{
start=rear=NULL;
//load_song_from_file("songs.dat");
}
start2=rear2=NULL;
start3=rear3=NULL;
start4=rear4=NULL;
root=current=NULL;
welcome_screen();
load_tank();
tank_war();
rest(200);
clear_keybuf();
create_tree();
id=pthread_create(&pthread0,NULL,thread0,(void*)&threadid0);
login();
remove_sound();
destroy();
return 0;
}END_OF_MAIN()





