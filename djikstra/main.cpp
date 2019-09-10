#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include<time.h>

// kruskal
#define finishx 80
#define finishy 24
#define wall 219
#define way 32
#define visited_node 176
// djikstra
int startx,starty,targetx,targety;
char map[25][80];

//Maze Generation

void gotoxy(int x, int y, int index)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	_COORD pos;
	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(hConsole, pos);
	printf("%c",index);
}

void init_map()
{
	for(int i = 0 ; i < finishy ; i ++)
	{
		for(int j = 0 ; j < finishx ; j ++)
		{
			map[i][j] = wall;
		}
	}
}

void view_map()
{
	for(int i = 0 ; i < finishy ; i ++)
	{
		for(int j = 0 ; j < finishx ; j ++)
		{
			printf("%c",map[i][j]);
		}
	}
}

void reload_map()
{
	//system("cls");
	//view_map();
	//gotoxy()
}

void init_visited(bool temp[])
{
	for(int i = 0 ; i < 4 ; i++)
		temp[i] = false;
}

bool is_clear(bool temp[])
{
	int counter=0;
	for(int i = 0 ; i < 4 ; i++)
		if(temp[i])
			counter++;
	if(counter==4)
		return true;
	else
		return false;
}

void dfs(int y, int x)
{
	Sleep(50);
	map[y][x] = way;
	gotoxy(x,y,way);

	srand(time(0));
	bool visited[4];
	init_visited(visited);
	

	while(!is_clear(visited))
	{
		int path = rand()%4;
		if(visited[path])
			continue;
		switch(path)
		{
		case 0:
			if((y-2)>0 && map[y-2][x]!=way)
			{
				map[y-1][x] = map[y-2][x] = way;
				gotoxy(x,y-1,way);
				gotoxy(x,y-2,way);
				dfs(y-2,x);
			}
			break;
		case 1:
			if((x+2)<finishx && map[y][x+2]!=way)
			{
				map[y][x+1] = map[y][x+2] = way;
				gotoxy(x+1,y,way);
				gotoxy(x+2,y,way);
				dfs(y,x+2);
			}
			break;
		case 2:
			if((y+2)<finishy && map[y+2][x]!=way)
			{
				map[y+1][x] = map[y+2][x] = way;
				gotoxy(x,y+1,way);
				gotoxy(x,y+2,way);
				dfs(y+2,x);
			}
			break;
		case 3:
			if((x-2)>0 && map[y][x-2]!=way)
			{
				map[y][x-1] = map[y][x-2] = way;
				gotoxy(x-1,y,way);
				gotoxy(x-2,y,way);
				dfs(y,x-2);
			}
			break;
		}
		visited[path] = true;
	}
}

void generate_map()
{
	init_map();
	view_map();
	dfs(0,0);
}

void ascii()
{
	for(int i = 0 ;i < 255; i++)
	{
		printf("%d : %c\n",i,i);
	}
}

// djikstra
#define start 1
#define finish 2

#define expanded 176;

bool is_finish;

struct node2
{
	int x,y;
	int cost;
	struct node2 *parent;
	int stat;//0=un-visited 1=visited
}*source;

struct box
{
	struct node2 *expanded_node;
	struct box *next,*prev;
}*head, *curr, *tail;

struct optimal_path
{
	int x,y;
	struct optimal_path *next,*prev;
}*begin,*c,*end;

void init_source()
{
	source = (struct node2*) malloc (sizeof(struct node2));
	source->cost = 0;
	source->parent = NULL;
	source->stat = 0;
	source->x = startx;
	source->y = starty;

	curr = (struct box*) malloc(sizeof(struct box));
	curr->expanded_node = source;
	curr->next = curr->prev = NULL;
	head = tail = curr;
}

void backtract(struct box *out)
{
	struct node2 *op = out->expanded_node;

	while(op->parent!=NULL)
	{
		c = (struct optimal_path *) malloc(sizeof(struct optimal_path));
		c->x = op->x;
		c->y = op->y;
		c->next = c->prev =NULL;

		if(begin == NULL)
		{
			begin = end = c;
		}
		else
		{
			end->next = c;
			c->prev = end;
			end = c;
		}
		op = op->parent;
	}
}

void pop(struct box *out)
{
	if( out == tail )
	{
		tail = tail->prev;
		free(out);
		tail->next = NULL;
	}
	else if( out == head )
	{
		head = head->next;
		free(out);
		head->prev = NULL;
	}
	else
	{
		out->prev->next = out->next;
		out->next->prev = out->prev;
		free(out);
	}
}

void push(int x, int y, int post_cost)
{
	source = (struct node2*) malloc (sizeof(struct node2));
	source->cost = 10+post_cost;
	source->stat = 0;
	source->x = x;
	source->y = y;

	curr = (struct box*) malloc(sizeof(struct box));
	curr->expanded_node = source;
	curr->next = curr->prev = NULL;

	tail->next = curr;
	curr->prev = tail;
	tail = curr;

	map[y][x] = expanded;
	gotoxy(x,y,176);
}

void expand_node(struct box *curr_expanded_node)
{
	if(curr_expanded_node->expanded_node->x == targetx && curr_expanded_node->expanded_node->y == targety)
	{
		is_finish = true;
		backtract(curr_expanded_node);
		return;
	}
	if( curr_expanded_node->expanded_node->x > 0 && map[curr_expanded_node->expanded_node->y][curr_expanded_node->expanded_node->x-1] == 32 )
	{
		push( curr_expanded_node->expanded_node->x-1, curr_expanded_node->expanded_node->y, curr_expanded_node->expanded_node->cost );
		tail->expanded_node->parent = curr_expanded_node->expanded_node;
	}
	if( curr_expanded_node->expanded_node->y > 0 && map[curr_expanded_node->expanded_node->y-1][curr_expanded_node->expanded_node->x] == 32 )
	{
		push( curr_expanded_node->expanded_node->x, curr_expanded_node->expanded_node->y-1, curr_expanded_node->expanded_node->cost );
		tail->expanded_node->parent = curr_expanded_node->expanded_node;
	}
	if( curr_expanded_node->expanded_node->x < finishx && map[curr_expanded_node->expanded_node->y][curr_expanded_node->expanded_node->x+1] == 32 )
	{
		push( curr_expanded_node->expanded_node->x+1, curr_expanded_node->expanded_node->y, curr_expanded_node->expanded_node->cost );
		tail->expanded_node->parent = curr_expanded_node->expanded_node;
	}
	if( curr_expanded_node->expanded_node->y < finishy && map[curr_expanded_node->expanded_node->y+1][curr_expanded_node->expanded_node->x] == 32 )
	{
		push( curr_expanded_node->expanded_node->x, curr_expanded_node->expanded_node->y+1, curr_expanded_node->expanded_node->cost );
		tail->expanded_node->parent = curr_expanded_node->expanded_node;
	}
	//map[curr_expanded_node->expanded_node->y][curr_expanded_node->expanded_node->x] = expanded;
	pop(curr_expanded_node);
}

void smallest()
{
	struct box *temp = head;
	curr = head;
	while(curr->next!=NULL)
	{
		curr = curr->next;
		if(curr->expanded_node->cost < temp->expanded_node->cost )
			temp = curr;
	}
	curr = temp;
}

void move()
{
	while(end!=NULL)
	{
		gotoxy(end->x,end->y,42);
		end=end->prev;
		Sleep(100);
	}
}

void random_point()
{
	srand(time(0));
	startx = targetx = rand()%finishx;
	starty = targety = rand()%finishy;
	while( startx == targetx || starty == targety || abs(startx-targetx) < 30 || abs(starty-targety)<10 || map[starty][startx] != way || map[targety][targetx] != way)
	{
		startx = rand()%finishx;
		starty = rand()%finishy;
		targetx = rand()%finishx;
		targety = rand()%finishy;
	}
}

void djikstra()
{
	random_point();
	is_finish = false;
	init_source();
	gotoxy(startx,starty,start);
	gotoxy(targetx,targety,finish);
	Sleep(1000);
	while(head!=NULL && !is_finish)
	{
		smallest();
		expand_node(curr);
		Sleep(5);
	}
	gotoxy(startx,starty,start);
	gotoxy(targetx,targety,finish);
	move();
}

int main()
{
	generate_map();
	djikstra();
	//ascii();

	getchar();
	return 0;
}