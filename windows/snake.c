#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>

#define SNAKE_PATTERN "*"
#define FOOD_PATTEN "#"
#define SNAKE_WALLABLE 1
#define MAP_X 60
#define MAP_Y 20 //��ͼ�߽綨��
#define U 1
#define D 2
#define L 3
#define R 4
#define SNAKE_WALL 1
#define SNAKE_BODY 2
#define SNAKE_PAUSE 3 


typedef struct SNAKE {
	int x;
	int y;
	struct SNAKE *next;
}snake; 

snake *p;
snake *head;
snake *food;
snake *nexthead;
int delaytime = 200;
int score = 0;
int food_once = 10;
int snake_status = 0;
int key_scan = 0;

void init_snake();
void print_snake();
void Pos(int x,int y);//���ù��λ��
void print_gui();
void snake_move();

void init_snake(){
	int init_loop;
	snake *tail; //β�巨������ 
	tail = (snake*)malloc(sizeof(snake));
	tail->x=10;
	tail->y=10;
	tail->next=NULL;
	for(init_loop=0;init_loop<5;init_loop++){
		head = (snake*)malloc(sizeof(snake));
		head->next=tail;
		 head->x=10+2*init_loop;
		 head->y=10;
		 tail = head;
	}
	head = tail;
	p = head;
		while(p!=NULL){
		Pos(p->x,p->y);
		printf(SNAKE_PATTERN);
		p=p->next;
	}
} // ��ʼ������ 

void print_snake(){
	p = head;
	while(p!=NULL){
		Pos(p->x,p->y);
		printf(SNAKE_PATTERN);
		p=p->next;
	}
}

void Pos(int x,int y)//���ù��λ��
{
    COORD pos;
	HANDLE hOutput;
    pos.X=x;
    pos.Y=y;
    hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOutput,pos);
}//�Լ���װ�����ù��λ�ú��� 

void print_gui(){
	int loop;
	for(loop=0;loop<=MAP_X;loop+=2){
		Pos(loop,0);
		printf("#");
		Pos(loop,MAP_Y);
		printf("#"); 
	} //�� 
	loop = 0;
	for(loop=0;loop<MAP_Y;loop++){
		Pos(0,loop);
		printf("#");
		Pos(MAP_X,loop);
		printf("#");
	} //�� 
	Pos(0,21); 
	printf("̰���ߣ���\n");
	printf("���ߣ�Yewei  mail��yewei_andy@163.com\n");
}//GUI��ӡ���� 

void snake_body(){
	snake *snake_self;
	snake_self = head->next;
	while(snake_self!=NULL){
		if(snake_self->x==head->x&&snake_self->y==head->y){
			snake_status = SNAKE_BODY;
			break;
		}
		snake_self=snake_self->next;
	}
}

void snake_wall(){
if(head->x==0||head->x==MAP_X||head->y==0||head->y==MAP_Y){
	if(head->x==0){
		head->x=MAP_X-2;}
	else if(head->x==MAP_X){
		head->x=0+2;}		
	else if(head->y==0){
		head->y=MAP_Y-1;}
	else if(head->y==MAP_Y){
		head->y=0+1;
	}
	snake_status = SNAKE_WALL;
	}
}

int snake_food_body(){
	p=head;
	while(1){
		if(p->x==food->x&&p->y==food->y){
			return 1;
		}
		if(p->next==NULL){
			return 0;	
		}
		p=p->next;
	}
	return 0;
}//�������0����ʳ�ﴴ��û�к������ͻ 

void food_place(){
	food = (snake*)malloc(sizeof(snake));
	if(food==NULL){
		free(food);
		food_place();
	}
	food->x=rand()%(MAP_X-2)+2;
	while((food->x%2)!=0){ //��֤���ɵ��λ������ż�� 
		food->x=rand()%48+2;
	}
	food->y=rand()%(MAP_Y-2)+2;
	if(snake_food_body()==1){
		free(food);
		food_place();
	}
	Pos(food->x,food->y); //Ϊʳ��λ����ʾ 
	printf(FOOD_PATTEN);
}


int eatting(){
	if(head->x==food->x&&head->y==food->y){
		return 1;
	}
	return 0;
}

void snake_move(){
	nexthead = (snake*)malloc(sizeof(snake));
	if(key_scan == U){
		nexthead->x=head->x;
		nexthead->y=head->y-1;
	}else if(key_scan == D){
		nexthead->x=head->x;
		nexthead->y=head->y+1;
	}else if(key_scan == L){
		nexthead->x=head->x-2;
		nexthead->y=head->y;
	}else if(key_scan == R){
		nexthead->x=head->x+2;
		nexthead->y=head->y;
	}

		if(nexthead->x==food->x&&nexthead->y==food->y){
			nexthead->next=head;
			head=nexthead;
			p=head;
			while(p!=NULL)
            {
                Pos(p->x,p->y);
                printf(SNAKE_PATTERN);
                p=p->next;
            }
             score+=food_once;
             Pos(MAP_X+4,7);
             printf("����:%d",score);
             Sleep(10);
           	 food_place();
        }else{
            nexthead->next=head;
            head=nexthead;
            p=head;
            while(p->next->next!=NULL)
            {
                Pos(p->x,p->y);
                printf(SNAKE_PATTERN);
                p=p->next;        
            }
            Pos(p->next->x,p->next->y);
            printf(" ");
            //free(p->next);
            p->next=NULL;
        }		
	} //�ߵ��ƶ� 

void game_control(){
	food_place();
	getch();
	key_scan=R; 
	while(1){
		if(snake_status==SNAKE_WALL){
			if(SNAKE_WALLABLE){
				print_gui();
				snake_status=0;
			}else{
				printf("��ײǽ��!!\n");
				break;
			}
		}else if(snake_status==SNAKE_BODY){
			printf("���Լ�ҧ���Լ���\n");
			break;
		}
		if(GetAsyncKeyState(VK_UP)&&key_scan!=D){
			key_scan = U;
		}else if(GetAsyncKeyState(VK_DOWN)&&key_scan!=U){
			key_scan = D;
		}else if(GetAsyncKeyState(VK_LEFT)&&key_scan!=R){
			key_scan = L;
		}else if(GetAsyncKeyState(VK_RIGHT)&&key_scan!=L){
			key_scan = R;
		}else if(GetAsyncKeyState(VK_F5)){
				Sleep(100); 
				while(1){
				if(GetAsyncKeyState(VK_F5)){
					Sleep(1000);
					break;
				}
			}
		} //����ɨ�� 
		if(key_scan!=0){
			//Sleep(10);
			snake_move();
			snake_wall(); //�ж����Ƿ�ײǽ�ĺ��� 
			snake_body();
		}
			Sleep(80);
	}
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	print_gui();
	init_snake();
	game_control(); 
	Pos(0,23);
	printf("��Ϸ����!!\n");
	getchar(); 
	return 0;
}


