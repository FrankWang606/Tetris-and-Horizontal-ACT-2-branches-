#include <SDL.h>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <SDL_image.h>

#include <vector>

void animateMain();
void handlechar();
int get_charP(int *board,int capicity);
void attack(int* board1,int* board2, int capicity);


SDL_Rect charpos;
SDL_Rect attpos;
SDL_Rect r;
SDL_Rect rAtt;
const int fps = 12;
SDL_Window* gWindow;
SDL_Surface* gSurface;  //surface to show picture in window
SDL_Surface* bgSurface;  //background picture
SDL_Renderer* render;

struct sprite {
	SDL_Texture* texture;
	std::vector<SDL_Rect> spriteRects;
	Uint32 lastUpdate;            
	Uint32 lastframe;   

};
struct sprite hero; struct sprite mons;



int main(int argc, char* argv[]);
void loadsource();
void init();

SDL_Texture* loadImage(const char* file);
void showImage(int x,int y,const char* file);
void close();

void attack(int* board1, int* board2, int capicity) {
	int i;
	for (i = 0; i < capicity; i++) {
		if (board2[i] == 1) {
			board1[i + 1] = 2;
		}
	}

}


void init(){
	int ret=SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);   //initialize(will add functions)  | :use together
	if (ret < 0) {
		printf("fail to initialize: %s/n", SDL_GetError());
		exit(1);
	}
	// window init create
	gWindow= SDL_CreateWindow("WITCH", SDL_WINDOWPOS_UNDEFINED,//init position(undefine) both x and y
		SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);//status use to shown
	gSurface = SDL_GetWindowSurface(gWindow);
	
	render = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);                     
}

void loadsource() {
	bgSurface = SDL_LoadBMP("image/witch.bmp");
	
}
SDL_Texture* loadImage(const char* file) {
	SDL_Texture* texture= IMG_LoadTexture(render, file);                               
	return texture;
}
void showImage(int x,int y,const char* file) {
	SDL_Texture* image= loadImage(file);
	SDL_Rect pos;   
	pos.x = x;
	pos.y = y;
	SDL_QueryTexture(image, NULL, NULL, &pos.w, &pos.h);   

	SDL_RenderCopy(render, image, NULL, &pos);              

}

void close() {

}
void handlechar() {
	
	int width[6]{ 0,0,72,0,83,0 };
	int diff[6]{ 0,0,11,0,-7,0 };
	for (int i = 0; i < 6; i++) {
		r.x = 1 + width[i];
		r.y = 1;
		r.w = 72 + diff[i];
		r.h = 136;

		hero.spriteRects.push_back(r);
	}
	r.x = 232; r.y = 1;            //run=7
	r.w = 84; r.h = 136;
	hero.spriteRects.push_back(r);
	r.x = 315; r.y = 275;          // attack=8
	r.w = 90; r.h = 136;
	hero.spriteRects.push_back(r);
	SDL_Surface* csurface = IMG_Load("image/renwu1.gif");
	hero.texture = SDL_CreateTextureFromSurface(render, csurface);

}



int get_charP(int *board,int capicity) {
	for (int i = 0; i < capicity; i++) {
		if (board[i] == 1) {
			return i;
		}
	}
}
void get_charA(int* board1, int capicity) {
	SDL_Texture* attack1 = IMG_LoadTexture(render, "image/attack.gif");
	int k{ 100 };
	for (int i = 0; i < capicity; i++) {
		if (board1[i] == 2) {
			if (i < 49) {
				attpos.x = 150 + i * 20;
				attpos.y = 450;
				attpos.w = r.w;
				attpos.h = r.h;
				rAtt.x = 27; rAtt.y = 184; rAtt.w = 250; rAtt.h = 163;
				SDL_RenderCopy(render, attack1, &rAtt, &attpos);
				k = i;
			}
			else if (i == 49) {
				board1[i] = 0;
				return;
			}
		}
	}
	if (board1[k + 1] == 0 && k >= 0 && k < 49) {
		board1[k + 1] = 2;
		board1[k] = 0;
	}
}

void animateMain() {
	int board[50]{ 0 };
	int boarda[50]{ 0 };
	int boardM[40]{ 0 };
	board[1] = 1;
	boardM[39] = 3;
	int curP, curA;
	
	handlechar();
	SDL_Event e1;
	int stay = 0;
	SDL_Texture* bg1 = IMG_LoadTexture(render, "image/bg1.bmp");
	bool animateRun = true;
	while (animateRun == true) {
		Uint32 current = SDL_GetTicks();    
		float interval = (current - hero.lastUpdate) / 1000.0f;
		if (interval >= (1.0 / fps)) { 
			hero.lastframe += 1;
			hero.lastframe = hero.lastframe % 6;  
			hero.lastUpdate = current;
			stay = hero.lastframe;


			bool move1 = true;
			get_charA(boarda, 50);
			SDL_RenderPresent(render);

			while (SDL_PollEvent(&e1) != 0) {
				if (e1.type == SDL_KEYDOWN) {
					if (e1.key.keysym.sym == SDLK_RIGHT) {
						int i = get_charP(board, 50);
						bool move1 = false;
						if (i < 49) {
							board[i] = 0; board[i + 1] = 1;
						}
						curP = get_charP(board, 50);
						charpos.x = 150 + curP * 20;
						charpos.y = 450;
						charpos.w = r.w;
						charpos.h = r.h;
						SDL_RenderCopy(render, bg1, NULL, NULL);
						SDL_RenderCopy(render, hero.texture, &hero.spriteRects[6], &charpos);
						get_charA(boarda, 50);
						SDL_RenderPresent(render);
						continue;
					}
					else if (e1.key.keysym.sym == SDLK_LEFT) {
						int i = get_charP(board, 50);
						if (i > 0) {
							board[i] = 0; board[i - 1] = 1;
						}
						curP = get_charP(board, 50);
						charpos.x = 150 + curP * 20;
						charpos.y = 450;
						charpos.w = r.w;
						charpos.h = r.h;
						SDL_RenderCopy(render, bg1, NULL, NULL);
						SDL_RenderCopy(render, hero.texture, &hero.spriteRects[stay], &charpos);
						get_charA(boarda, 50);
						SDL_RenderPresent(render);
					}
					else if (e1.key.keysym.sym == SDLK_f) {
						attack(boarda, board, 50);
						move1 = false;
						curP = get_charP(board, 50);
						charpos.x = 150 + curP * 20;
						charpos.y = 450;
						charpos.w = r.w;
						charpos.h = r.h;
						SDL_RenderCopy(render, bg1, NULL, NULL);
						SDL_RenderCopy(render, hero.texture, &hero.spriteRects[7], &charpos);
						SDL_RenderPresent(render);
					}
					if (e1.key.keysym.sym == SDLK_ESCAPE) {
						animateRun = false;
					}
				}
			}

			SDL_RenderCopy(render, bg1, NULL, NULL);
			curP = get_charP(board, 50);
			charpos.x = 150 + curP * 20;
			charpos.y = 450;
			charpos.w = r.w;
			charpos.h = r.h;
			if (move1) {
				SDL_RenderCopy(render, hero.texture, &hero.spriteRects[stay], &charpos);
			}
		}
	}
}
		
			

			
			



int main(int argc, char* argv[]) {
	init();    
	
	showImage(0, 0, "image/witch.bmp");
	showImage(120, 52, "image/biaoti.gif");
	showImage(800, 400, "image/zhishi.gif");
	
	SDL_RenderPresent(render);
	//go into loop
	bool stop = false;
	handlechar();
	SDL_Event e;   //define event
	while (stop == false) {
		while (SDL_PollEvent(&e) != 0) {        
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE) {         
					stop = true;
					break;
				}
				else if (e.key.keysym.sym == SDLK_s) {
					animateMain();
				}
				else {
					break;
				}
			}
		}
	}
	close();
	return 0;

}

















