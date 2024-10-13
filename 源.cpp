#include<iostream>
#include<easyx.h>
#include<graphics.h>
#include<string>
#include<vector>
#include<cmath>
using namespace std;
//IMAGE player_left[6];
//IMAGE player_right[6];
int playerx=500,playery=500;
bool running = 1;
bool start = 0;
//const int player_width = 80;
//const int player_height = 80;
//const int shadow_width = 32;
#pragma comment(lib,"Winmm.LIB")
//同步图片透明度,调用Windows的API进行修改
#pragma comment(lib,"MSIMG32.LIB")
inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
//传统方法载入玩家动画
/*void load_player()
{
	for(int i=0;i<6;i++)
	{
		wstring path = L"img/player_left_" + std::to_wstring(i) + L".png";
		loadimage(&player_left[i], path.c_str());
	}
	for (int i = 0; i < 6; i++)
	{
		wstring path = L"img/player_right_" + std::to_wstring(i) + L".png";
		loadimage(&player_right[i], path.c_str());
	}
}*/
using namespace std;

bool gamerunning = 1;
IMAGE shadow;
int tot = 0;
class Animation {
public:
	Animation(LPCTSTR path, int num, int interval)
	{
		interval_ms = interval;
		TCHAR path_file[256];
		for (size_t i = 0; i < num; i++)
		{
			_stprintf_s(path_file, path, i);
			IMAGE* frame = new IMAGE();
			loadimage(frame, path_file);
			frame_list.push_back(frame);
		}
		timestart = 0;
		timeend = 0;
	}
	void play(int x, int y ,int delta)
		{
		    timeend= GetTickCount();
		    DWORD timer = timeend - timestart;
			if (timer >= interval_ms)
			{
				idx_frame = (idx_frame + 1) % (frame_list.size());
				timer = 0;
				timestart = timeend;
			}
			putimage_alpha(x, y, frame_list[idx_frame]);
			
	    }
	~Animation() {
		for (size_t i = 0; i < frame_list.size(); i++)
		{
			delete frame_list[i];
		}
	};
private:
	int timer = 0;
	int idx_frame = 0;
	int interval_ms = 0;
	vector<IMAGE*>frame_list;
	DWORD timestart;
	DWORD timeend;
};
class player {
public:
	player()
	{
		loadimage(&shadow, _T("img/shadow_player.png"));
		mplayer_left=new Animation(_T("img/player_left_%d.png"), 6, 45);
		mplayer_right=new Animation(_T("img/player_right_%d.png"), 6, 45);
	}
	~player()
	{
		delete mplayer_left;
		delete mplayer_right;
	}
	void move()
	{
		ExMessage msg;
			while (peekmessage(&msg))
			{
				if (msg.message == WM_KEYDOWN)
				{
					switch (msg.vkcode)
					{
					case VK_UP:
						ismoveup = 1;
						break;
					case 'w':
						ismoveup = 1;
						break;
					case 'W':
						ismoveup = 1;
						break;
					case VK_LEFT:
						ismoveleft = 1;
						break;
					case 'a':
						ismoveleft = 1;
						break;
					case 'A':
						ismoveleft = 1;
						break;
					case VK_DOWN:
						ismovedown = 1;
						break;
					case 's':
						ismovedown = 1;
						break;
					case 'S':
						ismovedown = 1;
						break;
					case VK_RIGHT:
						ismoveright = 1;
						break;
					case 'd':
						ismoveright = 1;
						break;
					case 'D':
						ismoveright = 1;
						break;
					case 'k':
						mciSendString(_T("play shift from 0"), NULL, 0, NULL);
						if (ismoveup == 1)		playery -= 100;
						if (ismovedown == 1)	playery += 100;
						if (ismoveleft == 1)	playerx -= 100;
						if (ismoveright == 1)	playerx += 100;
					case 'K':
						mciSendString(_T("play shift from 0"), NULL, 0, NULL);
						if (ismoveup == 1)		playery -= 100;
						if (ismovedown == 1)	playery += 100;
						if (ismoveleft == 1)	playerx -= 100;
						if (ismoveright == 1)	playerx += 100;
					}
				}
				if (msg.message == WM_KEYUP)
				{
					switch (msg.vkcode)
					{
					case VK_UP:
						ismoveup = 0;
						break;
					case 'w':
						ismoveup = 0;
						break;
					case 'W':
						ismoveup = 0;
						break;
					case VK_LEFT:
						ismoveleft = 0;
						break;
					case 'a':
						ismoveleft = 0;
						break;
					case 'A':
						ismoveleft = 0;
						break;
					case VK_DOWN:
						ismovedown = 0;
						break;
					case 's':
						ismovedown = 0;
						break;
					case 'S':
						ismovedown = 0;
						break;
					case VK_RIGHT:
						ismoveright = 0;
						break;
					case 'd':
						ismoveright = 0;
						break;
					case 'D':
						ismoveright = 0;
						break;
					}
				}
			}
			if (ismovedown == 1)	playery += playerspeed;
			if (ismoveup == 1)	playery -= playerspeed;
			if (ismoveleft == 1)	playerx -= playerspeed;
			if (ismoveright == 1)	playerx += playerspeed;
			//设置边界
			if (playerx <= 0)	playerx = 0;
			if (playery <= 0)	playery = 0;
			if (playerx >= 1280 - 80)	playerx = 1280 - 80;
			if (playery >= 720 - 80)	playery = 720 - 80;
			return;
		
	}
	void draw(int delta)
	{
		    int dir_x = ismoveright - ismoveleft;
			static bool faceing_left = 0;
			int shadow_x = playerx + (player_width - shadow_width) / 2;
			int shadow_y = playery + player_height - 8;
			putimage_alpha(shadow_x, shadow_y, &shadow);
			if (dir_x < 0)
			{
				faceing_left = 1;
			}
			else if (dir_x > 0) faceing_left = 0;
			if (faceing_left == 1)	mplayer_left->play(playerx, playery, delta);
			if (faceing_left == 0)	mplayer_right->play(playerx, playery, delta);
		}
	int playerx = 500, playery = 500;
	const int player_width = 80;
	const int player_height = 80;
private:
	
	
	const int shadow_width = 32;
	Animation* mplayer_left;
	Animation* mplayer_right;
	bool ismoveup = 0;
	bool ismovedown = 0;
	bool ismoveright = 0;
	bool ismoveleft = 0;
	int playerspeed = 5;
	IMAGE img_background;
};
/*Animation player_left(_T("img/player_left_%d.png"), 6, 45);
Animation player_right(_T("img/player_right_%d.png"), 6, 45);
Animation enemy_right(_T("img/enemy_left_%d.png"), 6, 45);
Animation enemy_left(_T("img/enemt_left_%d.png"), 6, 45);*/
/*void DrawPlayer(int delta, int dir_x)
{
	static bool faceing_left = 0;
	int shadow_x = playerx + (player_width - shadow_width) / 2;
	int shadow_y = playery+ player_height-8;
	putimage_alpha(shadow_x, shadow_y, &shadow);
	if (dir_x < 0)
	{
		faceing_left = 1;
	}
	else if(dir_x>0) faceing_left = 0;
	if (faceing_left == 1)	player_left.play(playerx, playery, delta);
	if (faceing_left == 0)	player_right.play(playerx, playery, delta);

}*/
class bullet {
public:
	POINT position = { 0,0 };
public:
	bullet() = default;
	~bullet() = default;
	void draw()
	{
		setlinecolor(RGB(255,155,50));
		setfillcolor(RGB(200, 75, 10));
		fillcircle(position.x, position.y, 10);
	}
};
void updatebullet(vector<bullet>& bullets, const player& nplayer)
{
	const double radial_speed = 0.004;//径向速度
	const double tangent_speed = 0.0055;//切向速度
	double radial_interval = 2 * 3.14159 / bullets.size();//子弹之间的间距
	double radiul = 90 + 20 * sin(GetTickCount() * radial_speed);//子弹半径
	for (size_t i = 0; i < bullets.size();i++)
	{
		double radial = GetTickCount() * tangent_speed + radial_interval * i;
		bullets[i].position.x = nplayer.playerx + nplayer.player_width / 2 + (int)(radiul * cos(radial));
		bullets[i].position.y = nplayer.playery + nplayer.player_height / 2 + (int)(radiul * sin(radial));
	}
}
class enemy {
public:
	enemy()
	{
		loadimage(&shadow, _T("img/shadow_enemy.png"));
		enemy_left = new Animation(_T("img/enemy_left_%d.png"), 6, 45);
		enemy_right = new Animation(_T("img/enemy_right_%d.png"), 6, 45);
		enum  bian{ up=0,down,left,right };
		bian edge = (bian)(rand() % 4);
		if (edge == up)
		{
			y = 0;
			x = (rand() % 1280);
		}
		if (edge == down)
		{
			y = 720;
			x = (rand() % 1280);
		}
		if (edge == left)
		{
			x = 0;
			y = (rand() % 720);
		}
		if (edge == right)
		{
			x = 1280;
			y = (rand() & 720);
		}
	}
	~enemy()
	{
		delete enemy_left;
		delete enemy_right;
	}
	bool checkplayer(player& pl)//检查与玩家的碰撞
	{
		int pointx = x + enemy_width / 2;
		int pointy = y + enemy_height / 2;
		if (pointx >= pl.playerx && pointy >= pl.playery && pointx <= pl.playerx + pl.player_width && pointy <= pl.playery + pl.player_height)
		{
			return 0;
		}
		else return 1;
	}
	void checkbullet(bullet& bu)
	{
		if (bu.position.x >= x && bu.position.y >= y+10 && bu.position.x <= x + enemy_width && bu.position.y <= y + enemy_height-10)
		{
			alive = 0;
		}
	}
	void draw(const player &pl)
	{
		int shadow_x = x + (enemy_width - shadow_width) / 2-10;
		int shadow_y = y + enemy_height-35;
		putimage_alpha(shadow_x, shadow_y, &shadow);
		//if (x == pl.playerx && y == pl.playery)
		//{
			//enemy_left->play(x, y, 1000 / 60);
			//running = 0;
			//return;
		//}
		if (x < pl.playerx)
		{
			if (y < pl.playery)
			{
				x += speed;
				y += speed;
			}
			if (y > pl.playery)
			{
				x += speed;
				y -= speed;
			}
			if (y == pl.playery)
			{
				x += speed;
			}
			enemy_right->play(x, y, 1000 / 60);
		}
		else if (x > pl.playerx)
		{
			if (y < pl.playery)
			{
				x -= speed;
				y += speed;
			}
			if (y > pl.playery)
			{
				x -= speed;
				y -= speed;
			}
			if (y == pl.playery)
			{
				x -= speed;
			}
			enemy_left->play(x, y, 1000 / 60);
		}
		else if (x == pl.playerx)
		{
			if (y < pl.playery)
			{
				y += speed;
			}
			if (y > pl.playery)
			{
				y -= speed;
			}
			enemy_left->play(x, y, 1000 / 60);
		}
		if (x == pl.playerx && y == pl.playery)
		{
			//running = 0;
			return;
		}
	}
	bool alive = 1;
    int speed=2;
private:
	int x, y;
	IMAGE shadow;
	int enemy_width = 80;
	int enemy_height = 80;
	int shadow_width = 32;
	Animation* enemy_left;
	Animation* enemy_right;
};
void produceenemy(vector<enemy*>& enemy_list,double shengcunn)
{
	static int jishu = 0;
	static int jianju = 100;
	static int tottime = 0;
	jishu++;
	if (int(shengcunn / 1000) % 10 == 0 && shengcunn >= 1000&& int(shengcunn / 1000)>tottime)
	{
		tottime = int(shengcunn / 1000);
		jianju -= 10;
		if (jianju <= 40)
		{
			jianju +=8;
		}
		if (jianju <= 0)
		{
			jianju = 1;
		}
	}
	if (jishu>=jianju)
	{
		enemy_list.push_back(new enemy);
		//tot++;
		jishu = 0;
	}
	
}
class anniu1
{
public:
	anniu1(int x, int y)
	{
		this->x = x;
		this->y = y;
		loadimage(&anniu11, _T("img/ui1.png"));
		loadimage(&anniu22, _T("img/ui2.png"));
		loadimage(&anniu33, _T("img/ui3.png"));
	}
	void put()
	{
		if (zhuangtai == 1)
		{
			putimage(x, y, &anniu11);
		}
		if (zhuangtai == 2)
		{
			putimage(x, y, &anniu22);
		}
		if (zhuangtai == 3)
		{
			putimage(x, y, &anniu33);
		}
	}
	int x;
	int y;
	int zhuangtai = 1;
	IMAGE anniu11;
	IMAGE anniu22;
	IMAGE anniu33;
};
class anniu2
{
public:
	anniu2(int x, int y)
	{
		this->x = x;
		this->y = y;
		loadimage(&anniu11, _T("img/ui4.png"));
		loadimage(&anniu22, _T("img/ui5.png"));
		loadimage(&anniu33, _T("img/ui6.png"));
	}
	void put()
	{
		if (zhuangtai == 1)
		{
			putimage(x, y, &anniu11);
		}
		if (zhuangtai == 2)
		{
			putimage(x, y, &anniu22);
		}
		if (zhuangtai == 3)
		{
			putimage(x, y, &anniu33);
		}
	}
	int x;
	int y;
	int zhuangtai = 1;
	IMAGE anniu11;
	IMAGE anniu22;
	IMAGE anniu33;
};
void shubiao(anniu1 &a1,anniu2 &a2)
{
	ExMessage a;
	bool flag =1;
	bool flag1 = 0;
	bool flag2 = 0;
	while (flag)
	{
		if (peekmessage(&a))
		{
		if (a.message== WM_MOUSEMOVE|| a.message == WM_LBUTTONDOWN)
		{
			if (a.x >= a1.x && a.x <= a1.x + 192 && a.y >= a1.y && a.y <= a1.y + 95)
			{
				a1.zhuangtai = 2;
				flag = 0;
				flag1 = 1;
			}
			else
			{
				a1.zhuangtai = 1;
				flag = 0;
			}
			if (a.message == WM_LBUTTONDOWN&&flag1==1)
			{
				a1.zhuangtai = 3;
				start = 1;
				flag = 0;
			}
			if (a.x >= a2.x && a.x <= a2.x + 192 && a.y >= a2.y && a.y <= a2.y + 95)
			{
				a2.zhuangtai = 2;
				flag = 0;
				flag2 = 1;
			}
			else
			{
				a2.zhuangtai = 1;
				flag = 0;
			}
			if (a.message == WM_LBUTTONDOWN && flag2 == 1)
			{
				a1.zhuangtai = 3;
				flag = 0;
				gamerunning = 0;
			}
			
		}
		}

	}
	
}
int main()
{
	double shengcun = 0;
	int biansu=10;
	srand(time(NULL));
	mciSendString(_T("open mus/shift.wav alias shift"), NULL, 0, NULL);
	mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("open mus/hit.wav alias hit"), NULL, 0, NULL);
	//loadimage(&shadow, _T("img/shadow_player.png"));
	//load_player();
	initgraph(1280, 720);
	setlinecolor(RGB(255,0,0));
	HWND hwnd = GetHWnd();
	IMAGE menu;
	loadimage(&menu, _T("img/menu.png",1280,720));
	// 设置窗口位置
	SetWindowPos(hwnd, HWND_TOPMOST, 100, 50,1280, 720, SWP_SHOWWINDOW);
	anniu1 an1(540,400);
	anniu2 an2(540,500);
	while (gamerunning)
	{	BeginBatchDraw();
		if (start == 0)
		{
			putimage(0, 0, &menu);
			an1.put();
			an2.put();
			FlushBatchDraw();
			shubiao(an1, an2);
			putimage(0, 0, &menu);
			an1.put();
			an2.put();
			FlushBatchDraw();
		}
		else
		{
			double shengcun = 0;
			int biansu = 10;

			
			mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
			vector<enemy*>newenemy;
			vector<bullet>bullets(3);
			//ExMessage msg;
			IMAGE img_background;
			loadimage(&img_background, _T("img/background.png"));
			/*bool ismoveup = 0;
			bool ismovedown = 0;
			bool ismoveright = 0;
			bool ismoveleft = 0;
			int playerspeed = 10;*/
			player paimeng;
			BeginBatchDraw();
			DWORD starttime = 0;
			setbkmode(TRANSPARENT);
			while (start)
			{
				/*while (peekmessage(&msg))
				{
					if (msg.message == WM_KEYDOWN)
					{
					 switch (msg.vkcode)
					 {
					 case VK_UP:
						ismoveup = 1;
						break;
					 case VK_LEFT:
						ismoveleft = 1;
						break;
					 case VK_DOWN:
						ismovedown = 1;
						break;
					 case VK_RIGHT:
						ismoveright = 1;
						break;
					 }
					}
					if (msg.message == WM_KEYUP)
					{
						switch (msg.vkcode)
						{
						case VK_UP:
							ismoveup = 0;
							break;
						case VK_LEFT:
							ismoveleft = 0;
							break;
						case VK_DOWN:
							ismovedown = 0;
							break;
						case VK_RIGHT:
							ismoveright = 0;
							break;
						}
					}
				}
				if (ismovedown == 1)	playery += playerspeed;
				if (ismoveup == 1)	playery -= playerspeed;
				if (ismoveleft == 1)	playerx -= playerspeed;
				if (ismoveright == 1)	playerx += playerspeed;
				//设置边界
				if (playerx <= 0)	playerx = 0;
				if (playery <= 0)	playery = 0;
				if (playerx >= 1280- 80)	playerx = 1280 - 80;
				if (playery >= 720 - 80)	playery = 720 - 80;*/
				cleardevice();
				putimage(0, 0, &img_background);
				TCHAR s[20];
				_stprintf_s(s, _T("你已生存%.2f秒"), shengcun / 1000);
				outtextxy(600, 30, s);
				//a.draw(paimeng);
				//b.draw(paimeng);
				produceenemy(newenemy, shengcun);
				paimeng.move();
				paimeng.draw(1000 / 60);
				updatebullet(bullets, paimeng);
				for (int i = 0; i < bullets.size(); i++)
				{
					bullets[i].draw();
				}
				for (int i = 0; i < newenemy.size(); i++)
				{
					for (int j = 0; j < bullets.size(); j++)
					{
						newenemy[i]->checkbullet(bullets[j]);
					}
					if (newenemy[i]->alive == 1)
					{
						newenemy[i]->draw(paimeng);
					}
					if (newenemy[i]->alive == 0)
					{
						mciSendString(_T("play hit from 0"), NULL, 0, NULL);
						enemy* New = newenemy[i];
						swap(newenemy[i], newenemy.back());
						newenemy.pop_back();
						//tot--;
						delete New;
					}
				}
				/*	for (int i= 0; i < newenemy.size(); i++)
					{
						if(newenemy[i]->alive==1)
						{
							newenemy[i]->draw(paimeng);
						}
						if (newenemy[i]->alive == 0)
						{
							mciSendString(_T("play hit from 0"), NULL, 0, NULL);
							enemy* New = newenemy[i];
							swap(newenemy[i], newenemy.back());
							newenemy.pop_back();
							tot--;
							delete New;
						}

					}*/
				for (int i = 0; i < newenemy.size(); i++)
				{
					if (newenemy[i]->checkplayer(paimeng) == 0)
					{
						MessageBox(GetHWnd(), s, _T("游戏结束"), MB_OK);
						//Sleep(10000000);
						start = 0;
						mciSendString(_T("stop bgm"), NULL, 0, NULL);
						newenemy.clear();
					}

				}
				//line(0, 0, 500, 600);
				//putimage(500, 500, &player_left[tot]);
				//putimage_alpha(playerx, playery, &player_left[tot]);
				//tot++;
				//tot %= 6;
				//DrawPlayer(1000 / 60,  ismoveright-ismoveleft);
				FlushBatchDraw();
				DWORD endtime = GetTickCount();
				if (endtime - starttime < 1000 / 60)
				{
					Sleep(1000 / 60 - (endtime - starttime));
				}
				starttime = endtime;
				shengcun += 1000 / 60;
				if (shengcun / 1000 > biansu)
				{
					for (int i = 0; i < newenemy.size(); i++)
					{
						newenemy[i]->speed += 1;
					}
					biansu += 10;
				}
			}
		}
	}
	return 0;
}