#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define WALL_X 48/*!<MAX Position of X>*/
#define WALL_Y 32/*!<MAX Position of Y>*/

#define REFRASH 7
#define MAX_FOOD_COUNT 20/*!<MAX of food count>*/

#define BODY_MAP "body_map.txt"/*!<config file>*/

typedef struct pos
{
	struct pos*next;/*!<next node of pointer>*/
	int pos_x;/*!<Cursor Position of X>*/
	int pos_y;/*!<Cursor Position of Y>*/
	char color;/*!<not used>*/
}pos_t,*ppos;

struct 
{
	int pos_x;/*!<Food Cursor Position of X>*/
	int pos_y;/*!<Food Cursor Position of Y>*/
	char color;/*!<not used>*/
}food[MAX_FOOD_COUNT];

ppos header;/*!<pointer to first node of body>*/

int grade = 0;/*!<to count body lenth as player record>*/

char x_dir = 1;/*!<header move dir in X,0 for decrease;1 for standby;2 for increase>*/
char y_dir = 1;/*!<header move dir in Y,0 for decrease;1 for standby;2 for increase>*/

int speed_count = REFRASH;/*!<body move speed count>*/
int ms50_count = 0;/*!<timer step count as 50ms is 1 step>*/

/*************************************************/
/*function:clear_Console clear the console       */
/*param:none                                     */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void clear_Console(void)
{
	system("cls");
}

/*************************************************/
/*function:clear_Cursor clear the cursor         */
/*param:none                                     */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void clear_Cursor(void)
{
	CONSOLE_CURSOR_INFO cursor_info = {1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}

/*************************************************/
/*function:set_Title set the console of title    */
/*param:[IN]output string of title               */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void set_Title(char*string)
{
	SetConsoleTitle(string);
}

/*************************************************/
/*function:set_Color set the cursor color        */
/*param:[IN]back for backup color                */
/*param:[IN]front for front color                */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void set_Color(int back,int front)
{
	//0 黑 1深蓝 2绿 3浅蓝 4红 5紫 6黄 7白 8灰 9中蓝  
	//10亮绿 11亮蓝 12亮红 13亮紫 14亮黄 15亮灰   (7,0)-->白字黑底
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),(front%16)|(back%16*16));
}

/*************************************************/
/*function:set_Position set cursor position      */
/*param:[IN]place_x is x count                   */
/*param:[IN]place_y is y count                   */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void set_Position(int place_x,int place_y)
{
	COORD pos;
	pos.X = place_x;
	pos.Y = place_y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

/*************************************************/
/*function:set_Char set place and set char       */
/*param:[IN]place_x is x count                   */
/*param:[IN]place_y is y count                   */
/*param:[IN]param is display of char             */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void set_Char(int place_x,int place_y,char param)
{
	set_Position(place_x,place_y);
	printf("%c",param);
}

/*************************************************/
/*function:set_String set place and set string   */
/*param:[IN]place_x is x count                   */
/*param:[IN]place_y is y count                   */
/*param:[IN]string is display of string pointer  */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void set_String(int place_x,int place_y,char*string)
{
	set_Position(place_x,place_y);
	printf("%s",string);
}

/*************************************************/
/*function:init_Header init the first node       */
/*param:none                                     */
/*return:pointer of first node                   */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
ppos init_Header(void)
{
	ppos pheader = (ppos)malloc(sizeof(pos_t));
	if(pheader == NULL)
		return NULL;
	pheader->pos_x = (int)rand()%(WALL_X - 2) + 1;/*!<rand the header place of x count>*/
	pheader->pos_y = (int)rand()%(WALL_Y - 2) + 1;/*!<rand the header place of y count>*/
	pheader->color = 0;
	pheader->next = NULL;
	return pheader;
}

/*************************************************/
/*function:add_Node add new node for snacker body*/
/*param:[IN]place_x is body of X                 */
/*param:[IN]place_y is body of Y                 */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void add_Node(int place_x,int place_y)
{
	ppos tail = header;
	ppos node = (ppos)malloc(sizeof(pos_t));
	if(node == NULL)
		return;
	node->pos_x = place_x;
	node->pos_y = place_y;
	node->color = 0;
	node->next = NULL;
	while(tail->next != NULL)
	{
		tail = tail->next;
	}
	tail->next = node;/*!<insert new node into tail>*/
}

/*************************************************/
/*function:print_Node output the body of snacker */
/*param:[IN]color body node of color             */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void print_Node(char color)
{
	ppos tail = header;
	grade = 0;
	while(tail != NULL)
	{
		set_Color(0,4);
		set_Char(tail->pos_x,tail->pos_y,'O');/*!<print body of all node>*/
		set_Color(0,7);
		tail = tail->next;
		grade++;/*!<count the body node>*/
	}
	grade -= 1;
	if(speed_count > 1)/*!<snacker speed logic>*/
	{
		speed_count = REFRASH;
		speed_count -= grade/MAX_FOOD_COUNT;
	}
}

/*************************************************/
/*function:clear_Node clear the body of all node */
/*param:none                                     */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void clear_Node(void)
{
	ppos tail = header;
	while(tail != NULL)
	{
		set_Char(tail->pos_x,tail->pos_y,' ');/*!<clear body of all node>*/
		tail = tail->next;
	}
}

/*************************************************/
/*function:exchange_Node move body node to next  */
/*param:[IN]last pointer of the last node param  */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void exchange_Node(ppos last)
{
	ppos tail = header;
	pos_t swap1,swap2;
	memcpy(&swap1,tail,sizeof(pos_t));
	while((tail != NULL) && (tail->next != NULL))/*!<exchange the body bettwen two node of place count>*/
	{
		swap2.pos_x = tail->next->pos_x;
		swap2.pos_y = tail->next->pos_y;
		tail->next->pos_x = swap1.pos_x;
		tail->next->pos_y = swap1.pos_y;
		swap1.pos_x = swap2.pos_x;
		swap1.pos_y = swap2.pos_y;
		tail = tail->next;
	}
	memcpy(last,&swap1,sizeof(pos_t));/*!<get the last node of tail for other use>*/
}

/*************************************************/
/*function:rand_Node rand the food node place    */
/*param:none                                     */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void rand_Node(int index)
{
	int food_count = 0;
	ppos tail = header;
	while(1)
	{
		char same_flag = 0;
		food[index].pos_x = (int)rand()%(WALL_X - 2) + 1;
		food[index].pos_y = (int)rand()%(WALL_Y - 2) + 1;
		while(tail != NULL)
		{
			if((food[index].pos_x == tail->pos_x) && (food[index].pos_y == tail->pos_y))/*!<new food is not in body>*/
			{
				same_flag = 1;
				break;
			}
			tail = tail->next;
		}
		for(food_count = 0;food_count < MAX_FOOD_COUNT;food_count++)
		{
			if((food_count != index) && (food[index].pos_x == food[food_count].pos_x) && (food[index].pos_y == food[food_count].pos_y))/*!<new food is not in foods>*/
			{
				same_flag = 1;
				break;
			}
		}
		if(same_flag == 0)
			return;
	}
}

/*************************************************/
/*function:print_Wall print the edge of play area*/
/*param:none                                     */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void print_Wall(void)
{
	int i;
	int j;
	for(i=0;i<(WALL_X+2);i++)
	{
		for(j=0;j<(WALL_Y+2);j++)
		{
			if((i == 0) || (i == (WALL_X+1)) || (j == 0) || (j == (WALL_Y+1)))
			{
				set_Color(0,10);
				set_Char(i,j,'#');
				set_Color(0,7);
			}
		}
	}
}


int load_body(void)
{
	char str_table[200];
	char*status = NULL;
	ppos tail = header;
	FILE*fp = NULL;
	fp = fopen(BODY_MAP,"r+");/*file is readable*/
	if(fp == NULL)
		return -1;
	do
	{
		memset(str_table,0,200);
		status = fgets(str_table,200,fp);
		if(NULL != status)
		{
			ppos temp = (ppos)malloc(sizeof(pos_t));
			if(NULL == temp)
				return -1;
			sscanf(str_table,"x_axis:%d y_axis:%d color:%d",&(temp->pos_x),&(temp->pos_y),&(temp->color));
			temp->next = NULL;
			while(NULL != tail)
			{
				tail = tail->next;
			}
			tail = temp;
#if 0
			do
			{
				char string[200];
				memset(string,0,200);
				sprintf(string,"echo x:%d_y:%d_color:%d >> logfile.txt",temp->pos_x,temp->pos_y,temp->color);
				//sprintf(string,"echo %s >> logfile.txt",str_table);
				system(string);
			}while(0);
#endif
		}
	}while(NULL != status);
	fclose(fp);
	return 0;
}

/*************************************************/
/*function:save_body snacke body save function   */
/*param:none                                     */
/*return:save status                             */
/*auth:cxzxiao                                   */
/*    date:20190304                              */
/*    release note:first release                 */
/*************************************************/
int save_body(void)
{
	ppos tail = header;
	FILE*fp = NULL;
	fp = fopen(BODY_MAP,"w+");/*file is writeable*/
	if(fp == NULL)
		return -1;
	while(tail != NULL)
	{
#if 1
		char str_table[100];
		memset(str_table,0,100);
		sprintf(str_table,"x_axis:%d y_axis:%d color:%d\r\n",tail->pos_x,tail->pos_y,tail->color);
		fwrite(str_table,strlen(str_table),1,fp);
#else
		fprintf(fp,"x_axis:%d y_axis:%d color:%d\r\n",tail->pos_x,tail->pos_y,tail->color);
#endif
		tail = tail->next;
	}
	fflush(fp);
	fclose(fp);
	return 0;
}

/*************************************************/
/*function:dir_check header move dir check       */
/*param:[IN]speed_type pointer of speed type     */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void dir_check(char*speed_type)
{
	static char old_key[2] = {0,0};
	char this_key = 0;
	if(_kbhit())
	{
		this_key = _getch();
		/*!<header move dir in X,0 for decrease;1 for standby;2 for increase>*/
		/*!<header move dir in Y,0 for decrease;1 for standby;2 for increase>*/
		switch(this_key)
		{
			case 'w':
			case 'W':
					if(y_dir != 2)
					{
						x_dir = 1;
						y_dir = 0;
						old_key[0] = 'w';
						old_key[1] = 'W';
					}
				break;
					
			case 's':
			case 'S':
					if(y_dir != 0)
					{
						x_dir = 1;
						y_dir = 2;
						old_key[0] = 's';
						old_key[1] = 'S';
					}
				break;
					
			case 'a':
			case 'A':
					if(x_dir != 2)
					{
						x_dir = 0;
						y_dir = 1;
						old_key[0] = 'a';
						old_key[1] = 'A';
					}
				break;
				
			case 'd':
			case 'D':
					if(x_dir != 0)
					{
						x_dir = 2;
						y_dir = 1;
						old_key[0] = 'd';
						old_key[1] = 'D';
					}
				break;
					
			case 'q':
			case 'Q':
					set_String(5,WALL_Y+2,"enter any key to continue!                  ");
					_getch();
					set_String(5,WALL_Y+2,"direction:w s a d pause:Q save:t load:r     ");
				break;
				
			case 'r'://load config
			case 'R':
					load_body();
					print_Node(0);
				break;
				
			case 't'://save config
			case 'T':
					save_body();
				break;
		
			default:
				break;
		}
		if((this_key == old_key[0]) || (this_key == old_key[1]))/*!<the same key cause sancker move fast logic>*/
			*speed_type = 3;
	}
}

/*************************************************/
/*function:eat_check logic of header and food    */
/*param:[IN]place_x tail Position of X           */
/*param:[IN]place_x tail Position of Y           */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void eat_check(int place_x,int place_y)
{
	int count = 0;
	for(count = 0;count<MAX_FOOD_COUNT;count++)
	{
		if((header->pos_x == food[count].pos_x) && (header->pos_y == food[count].pos_y))
		{
			add_Node(place_x,place_y);
			rand_Node(count);
		}
	}
}

/*************************************************/
/*function:print_Grade output the player grade   */
/*param:[IN]place_x cursor Position of X         */
/*param:[IN]place_x cursor Position of Y         */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void print_Grade(int place_x,int place_y)
{
	char string[200];
	sprintf(string,"Max grade is %d",grade);
	set_String(place_x,place_y,string);
}

/*************************************************/
/*function:wall_Check check header is not in wall*/
/*param:none                                     */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void wall_Check(void)
{
	if((header->pos_x < 1) || (header->pos_x > WALL_X) || (header->pos_y < 1) || (header->pos_y > WALL_Y))
	{
		system("cls");
		print_Grade(3,3);
		printf("\r\n\r\n\r\n\r\n\r\n");
		system("pause");
		exit(0);
	}
}

/*************************************************/
/*function:body_Check check header is not in body*/
/*param:none                                     */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void body_Check(void)
{
	ppos tail = header->next;
	while(tail != NULL)
	{
		if((tail->pos_x == header->pos_x) && (tail->pos_y == header->pos_y))
		{
			system("cls");
			print_Grade(3,3);
			printf("\r\n\r\n\r\n\r\n\r\n");
			system("pause");
			exit(0);
		}
		tail = tail->next;
	}
}

/*************************************************/
/*function:main main function                    */
/*param:[IN]argc arg count                       */
/*param:[IN]argv args pointer                    */
/*return:none                                    */
/*auth:cxzxiao                                   */
/*    date:20190110                              */
/*    release note:first release                 */
/*************************************************/
void main(int argc,int**argv)
{
	int cycle_count = 0;
	char double_speed = 1;
	int food_count = 0;
	pos_t last_node;
	system("cls");
	set_Title("Snacker");
	system("mode con cols=50 lines=36");
	clear_Cursor();
	print_Wall();
	srand(time(NULL));
	header = init_Header();
	print_Node(0);
	set_String(5,WALL_Y+2,"direction:w s a d pause:Q save:t load:r     ");
	for(food_count = 0;food_count<MAX_FOOD_COUNT;food_count++)
	{
		rand_Node(food_count);
	}
	while(1)
	{
		double_speed = 1;
		dir_check(&double_speed);
		if(speed_count/double_speed == 0)
			speed_count = double_speed;
		if(ms50_count%(speed_count/double_speed) == 0)
		{
			clear_Node();
			if(x_dir == 0)header->pos_x--;
			if(x_dir == 2)header->pos_x++;
			if(y_dir == 0)header->pos_y--;
			if(y_dir == 2)header->pos_y++;
			wall_Check();
			body_Check();
			exchange_Node(&last_node);
			eat_check(last_node.pos_x,last_node.pos_y);
			for(food_count = 0;food_count<MAX_FOOD_COUNT;food_count++)
			{
				set_Color(0,13);
				set_Char(food[food_count].pos_x,food[food_count].pos_y,'*');
				set_Color(0,7);
			}
			print_Node(0);
			print_Grade(5,WALL_Y+3);
		}
		Sleep(50);
		ms50_count++;
	}
}
