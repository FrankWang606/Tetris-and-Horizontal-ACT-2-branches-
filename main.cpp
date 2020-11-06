
#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>
#include <iostream>
using namespace sf;







int blocks[7][4] = {
	{1,3,5,7},                                                   //block type
	{2,4,5,7},
	{3,5,4,6},
	{3,5,4,7},
	{2,3,5,7},
	{3,5,7,6},
	{2,3,4,5}
};

int BlockIndex;  //current block type
//game area
const int ROW = 20;
const int COL = 10;

int table[ROW][COL]{ 0 };


struct Point {
	int x;
	int y;
};
Point curBlock[4];
Point bakBlock[4];

Sound sou;   //sound effect

//drop speed
const float speed_normal = 0.4;
const float speed_quick = 0.1;
float delay=speed_normal;

void newBlock() {
	//get random type block
	BlockIndex = 1 + rand() % 7; 
	int n = BlockIndex - 1;
	for (int i = 0; i < 4; i++) {
		curBlock[i].x = blocks[n][i] % 2;        
		curBlock[i].y = blocks[n][i] / 2;
	}
}

bool check() {
	for (int i = 0; i < 4; i++) {
		if (curBlock[i].x < 0 || curBlock[i].x >= COL || curBlock[i].y >= ROW || table[curBlock[i].y][curBlock[i].x] != 0)
			return false;
	}
	return true;
}
void drop() {
	for (int i = 0; i < 4; i++) {
		bakBlock[i] = curBlock[i];
		curBlock[i].y += 1;
	}
	if (check() == false) {
		
		for (int i = 0; i < 4; i++) {
			table[bakBlock[i].y][bakBlock[i].x] = BlockIndex;
		}
		newBlock();
		}
	}
	
void moveleftright(int dx) {
	for (int i = 0; i < 4; i++) {
		bakBlock[i] = curBlock[i];
		curBlock[i].x += dx;
	}
	if (!check()) {
		for (int i = 0; i < 4; i++) {
			curBlock[i] = bakBlock[i];
		}
	}
}
void dorotate() {
	if (BlockIndex == 7) {
		return;
	}
	for (int i = 0; i < 4; i++) {
		bakBlock[i] = curBlock[i];

	}
	Point p = curBlock[1];
	for (int i = 0; i < 4; i++) {
		struct Point tmp = curBlock[i];
		curBlock[i].x = p.x - tmp.y + p.y;
		curBlock[i].y = tmp.x - p.x + p.y;
	}
	
	if (!check()) {
		for (int i = 0; i < 4; i++) {
			curBlock[i] = bakBlock[i];
		}
	}
}

void keyEvent(RenderWindow *window) {                                        //handle key press
	bool rotate = false; 
	int dx = 0;   
	Event e;    
	while (window->pollEvent(e)) {                              
		if (e.type == Event::Closed) {
			window->close();
		}
		if (e.type == Event::KeyPressed) {
			switch (e.key.code) {
			case Keyboard::Up:
				rotate = true;
				break;
			case Keyboard::Left:
				dx = -1;
				break;
			case Keyboard::Right:
				dx = 1;
				break;
			default:
				break;
			}
		}
	
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			delay = speed_quick;
		}
		else {
			delay = speed_normal;
		}
		if (dx != 0) {
			moveleftright(dx);
		}
		if (rotate) {
			dorotate();
		}
	}
}

void clearLine(int &point) {
	int k = ROW - 1;
	for (int i = ROW-1; i > 0; i--) {
		int count = 0;
		for (int j = 0; j < COL; j++) {
			if (table[i][j]) {
				count++;
			}
			table[k][j] = table[i][j];
		}
		if (count < COL) {
			k--;    
		}
		else {
			point += 10;
			sou.play();
		}
	}
}
void drawtext(Text *score, RenderWindow *window,std::string output) {
	score->setPosition(0, 0);
	score->move(435, 143);
	score->setString(output);
	window->draw(*score);
}

void drawflame(Sprite* spriteframe, RenderWindow* window) {
	spriteframe->setTextureRect(IntRect(8, 1, 192, 368));
	spriteframe->setPosition(0, 0);
	spriteframe->move(130, 33);
	window->draw(*spriteframe);
}

void drawBlocks(Sprite *spriteBlock,RenderWindow *window) {
	
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (table[i][j] == 1 || table[i][j] == 2) {
				
				spriteBlock->setTextureRect(IntRect(42, 4, 18, 18));
				spriteBlock->setPosition(j * 18, i * 18);
			
				spriteBlock->move(133, 31);
				window->draw(*spriteBlock);
			}
			else if (table[i][j] == 3 || table[i][j] == 4 || table[i][j] == 5) {
				spriteBlock->setTextureRect(IntRect(4, 43, 18, 18));
				spriteBlock->setPosition(j * 18, i * 18);
				spriteBlock->move(133, 31);
				window->draw(*spriteBlock);
			}
			else if (table[i][j] == 6 || table[i][j] == 7) {
				spriteBlock->setTextureRect(IntRect(62, 82, 18, 18));
				spriteBlock->setPosition(j * 18, i * 18);
				spriteBlock->move(133, 31);
				window->draw(*spriteBlock);
			}

		}
	}
	
	for (int i = 0; i < 4; i++) {
		if (BlockIndex == 1 || BlockIndex == 2) {
			spriteBlock->setTextureRect(IntRect(42, 4, 18, 18));
			spriteBlock->setPosition(curBlock[i].x * 18, curBlock[i].y * 18);
			spriteBlock->move(133, 31);
			window->draw(*spriteBlock);
		}
		else if (BlockIndex == 3 || BlockIndex == 4 || BlockIndex == 5) {
			spriteBlock->setTextureRect(IntRect(4, 43, 18, 18));
			spriteBlock->setPosition(curBlock[i].x * 18, curBlock[i].y * 18);
			spriteBlock->move(133, 31);
			window->draw(*spriteBlock);
		}
		else if (BlockIndex == 6 || BlockIndex == 7) {
			spriteBlock->setTextureRect(IntRect(62, 82, 18, 18));
			spriteBlock->setPosition(curBlock[i].x * 18, curBlock[i].y * 18);
			spriteBlock->move(133, 31);
			window->draw(*spriteBlock);
		}
	}
}

int main(void) {
	srand(time(0));          
	//bgm
	Music bgm;
	bgm.openFromFile("bgm.wav");
	bgm.setLoop(true);   
	bgm.play();
	int points = 0;
	std::string output;
	

	SoundBuffer xiaochu;         
	xiaochu.loadFromFile("peng.wav");
	sou.setBuffer(xiaochu);

	
	Font font;   
	font.loadFromFile("font1.ttf");
	

	
	RenderWindow window(
		VideoMode(611, 416),   
		"Rock-2020");   

	Texture t1;
	t1.loadFromFile("image/background.jpg");
	Sprite spritebg(t1);        

	Texture t3;
	t3.loadFromFile("image/flame.png");
	Sprite spriteflame(t3);

	Texture t2;
	t2.loadFromFile("image/block.jpg");
	Sprite spriteBlock(t2);
	
	
	
	newBlock();
	
	Clock clock;
	float timer = 0;   //total time


	while (window.isOpen()) {               
		float time=clock.getElapsedTime().asSeconds();     
		clock.restart();
		timer += time;
		if (timer > delay) {
			drop();   
			timer = 0;
		}
		
		clearLine(points);
		output = std::to_string(points);
		
		keyEvent(&window);
	
		window.draw(spritebg);
		
		
		Text score(output, font, 75);
		drawtext(&score,&window,output);
		
		drawBlocks(&spriteBlock,&window);
		drawflame(&spriteflame, &window);

		window.display();

	}

	system("pause");
	return 0;
}
