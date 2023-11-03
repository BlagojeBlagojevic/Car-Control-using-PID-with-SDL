#include<SDL2/SDL.h>
#include<SDL2/SDL_hints.h>
#include<SDL2/SDL_error.h>
#include<SDL2/SDL_log.h>
#include<SDL2/SDL_video.h>
#include<SDL2/SDL_render.h>
#include<SDL2/SDL_pixels.h>
#include<SDL2/SDL_rect.h>
#include<SDL2/SDL_surface.h>
#include<SDL2/SDL_clipboard.h>
#include<SDL2/SDL_events.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDl_timer.h>
#include<assert.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<omp.h>
#undef main 
#define width  1200
#define height 800


//#define Ki 0.001
//#define Kp 0.6
//#define Kd 0.001
#define SEED 100
float Ki=0.0001, Kd = 0.001 , Kp = 0.6; 

typedef struct CAR{
	
	SDL_Point desired;
	SDL_Rect car;
	float speed[2];    //0 x , 1 y
	//float acceleration[2];	
	float e_sum[2];
	float e_pr[2];
	
	
}CAR;
typedef struct GRAPH{
	int speed[2][width];
	int position[2][width];
	int error[2][width];
}GRAPH;

void Init_Car(CAR *car)
{
	car->e_sum[0] = 0.0;
	car->e_sum[1] = 0.0;
	car->e_pr[0] = 0.0f;
	car->e_pr[1] = 0.0f;
	car->speed[0] = 0.0f;
	car->speed[1] = 0.0f;
	car->desired.x = 800;
	car->desired.y = 600;
	car->car.h = 30;
	car->car.w = 30;
	car->car.x = 10;
	car->car.y = 10;
}

void Check_Boundary(CAR *car)
{
	if(car->desired.x >= width)
		car->desired.x=width-30;
	if(car->car.x <= 0)
		car->car.x = 30;
	if(car->car.y >= height) 
		car->car.y= height-30;
	if(car->car.y <= 0) 
		car->car.y = 30;
	
}
void MainRenderrer(SDL_Renderer  *renderer,CAR *car)
{
	
	SDL_Rect rect;
	rect.h=car->car.h;
	rect.w=car->car.w;
	rect.x=car->desired.x;
	rect.y=car->desired.y;
	//Check_Boundary(car);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer,0,255,0,255);
	SDL_RenderFillRect(renderer,&car->car);
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_RenderFillRect(renderer,&rect);
	

	
	SDL_SetRenderDrawColor(renderer,0,0,0,0);
	SDL_RenderPresent(renderer);
	SDL_Delay(100);
	
}
void GraphRenderer(GRAPH *graph,SDL_Renderer *renderer)
{
	
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer,0,255,0,255);
	for(size_t x = 0; x < 200;x++ )
	{
		int error =  (int)sqrt(graph->error[0][x]*graph->error[0][x]+graph->error[1][x]*graph->error[1][x]);//graph->error[0][x];
		//int error =  graph->position[0][x]	;
		if(error > 0)
		SDL_RenderDrawPoint(renderer,x*6,width/2-error);
	}
		
	SDL_SetRenderDrawColor(renderer,0,0,0,0);
	SDL_RenderPresent(renderer);
	SDL_Delay(10000);
}

void Limit_Speed_Acceleration(CAR *car)

{
	//ACCELERATION LIMIT
	
	//SPEED LIMIT x
	if(car->speed[0] > 15)
		car->speed[0] = 10;
	if(car->speed[0] > 15)
		car->speed[0] = 10;
		
	if(car->speed[0] < -15)
		car->speed[0] = -10;
	if(car->speed[0] < -15)
		car->speed[0] = -10;
		
		
	if(car->speed[1] > 10)
		car->speed[1] = 5;
	if(car->speed[1] > 10)
		car->speed[1] = 5;
		
	if(car->speed[1] < -10)
		car->speed[1] = -10;
	if(car->speed[1] < -10)
		car->speed[1] = -10;		

}
float RandError() //This Function is used to model error during measurement process
{
	//return 0;
	if(rand()%2==0)
		return rand()%10;
	return -1*rand()%10;
}

void UpdateCar(CAR *car,GRAPH *graph,size_t i)
{
	//X(n)=Vx(n)+X(n-1)
	//Y(n)=Vy(n)+Y(n-1)
	//Assumed that we can disretised the speed so acceleration is same as a speed
	
	//Calculate the speed PID control
	float ex = (car->desired.x-car->car.x);  
	float ey = (car->desired.y-car->car.y);
	graph->error[0][i] = (int)ex;
	graph->error[1][i] = (int)ey;
	
	car->speed[0]+=Kp*ex+RandError()+Ki*car->e_sum[0]+Kd*(ex-car->e_pr[0]);
	car->speed[1]+=Kp*ey+RandError()+Ki*car->e_sum[1]+Kd*(ey-car->e_pr[1]);
	graph->speed[0][i] = car->speed[0];
	graph->speed[1][i] = car->speed[1];
	car->e_sum[0]+=ex;
	car->e_sum[1]+=ey;
	car->e_pr[0] = ex;
	car->e_pr[1] = ey;
	Limit_Speed_Acceleration(car);
	
	//Update posiions
	//car->car.x+=(int)car->speed[0];car->car.y+=(int)car->speed[1]*1.4;
	car->car.x+=(10*tanh(car->speed[0]));
	car->car.y+=10*tanh(car->speed[1]);
	//car->car.y+=exp(car->speed[1]*0.3)-10;
	graph->position[0][i]=car->car.x;
	graph->position[1][i]=car->car.y;
	
	printf("ex: %.2f ey: %.2f\n",ex,ey);
	//
	
	// 
}

void RandDestination(CAR *car)
{
	car->desired.x = rand()%(width-30);
	car->desired.y = rand()%(height-30);
}
void Input()
{
	printf("Do you want to eneter Kp,Ki,Kd (y-YES ,n-NO)\n");
	char a;
	scanf("%c",&a);
	if(a=='y')
	{
		printf("Kp:  ");
		scanf("%f",&Kp);
		printf("Ki:  ");
		scanf("%f",&Ki);
		printf("Kd:  ");
		scanf("%f",&Kd);
	}
	else;
	
}
int main()
{
	//
	Input();
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer(width,height,SDL_RENDERER_ACCELERATED,&window,&renderer);
	CAR car[10];
	GRAPH graph;
	Init_Car(&car);
	srand(time(0));
	size_t i = 0;

	//MainRenderrer(renderer,&car);
	while(1){
		MainRenderrer(renderer,&car);
		//GrapRenderrer(renderer,&graph,i);
		UpdateCar(&car,&graph,i);
	
		i++;
		if(i==200)
		{
			RandDestination(&car);
			i=0;
			GraphRenderer(&graph,renderer);
			
		}
		//Limit_Speed_Acceleration(&car);
		
	} 
	return 0;
}
