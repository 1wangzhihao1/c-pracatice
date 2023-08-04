//简单的飞机大战小游戏
//游戏界面使用Easyx制作


/*游戏规则：
* 1.如果英雄和敌机相碰则失败
* 2.如果英雄被敌机子弹击中则失败
*/

#include<iostream>
#include<graphics.h>//easyx的头文件
#include<vector>
#include<conio.h>

using namespace std;


constexpr auto swidth=600;
constexpr auto sheight=1100;

//判断鼠标按下区域函数
bool PointInRect(int x, int y, RECT& r)
{
	return (x >= r.left && x <= r.right && y >= r.top && y <= r.bottom);
}

//如果敌机重叠相撞
bool RectHit(RECT &r1,RECT &r2)
{
	RECT r;
	r.left = r1.left - (r2.right - r2.left);
	r.right = r1.right;
	r.top = r1.top - (r2.bottom-r2.top);
	r.bottom = r1.bottom;

	return(r2.left > r.left && r2.left <= r.right && r2.top >= r.top && r2.top <= r.bottom);
}
//游戏欢迎界面
void Welcome()
{
	//定义三个字符串
	LPCTSTR title = _T("飞机大战");
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR texit = _T("退出游戏");
	
	//定义开始游戏和退出游戏位置结构体
	RECT tplayr, texitr;

	BeginBatchDraw(); 

	setbkcolor(WHITE);//设置游戏背景颜色
	cleardevice();
	settextstyle(60,0,_T("黑体"));//设置游戏字体格式大小
	settextcolor(BLUE);//设置游戏字体颜色
	outtextxy(swidth/2 - textwidth(title) / 2, sheight/8,title);//显示“飞机大战”
	settextstyle(40, 0, _T("黑体"));//更改游戏字体颜色

	//设置“开始游戏”位置
	tplayr.left = swidth / 2 - textwidth(tplay)/2;
	tplayr.right = swidth / 2 + textwidth(tplay) / 2;
	tplayr.top = sheight / 5*1.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	//设置“退出游戏”位置
	texitr.left = swidth / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = sheight / 5*2;
	texitr.bottom = texitr.top + textheight(texit);

	//显示
	outtextxy(tplayr.left, tplayr.top, tplay);
	outtextxy(texitr.left, texitr.top, texit);

	EndBatchDraw();

	while (1)
	{
		ExMessage mess;//定义鼠标结构体
		getmessage(&mess, EM_MOUSE);//获取鼠标信息
		if (mess.lbutton)//判断鼠标左键按下
		{
			if (PointInRect(mess.x, mess.y, tplayr))//判断鼠标按下区域为开始游戏区域
			{
				return;
			}
			else if (PointInRect(mess.x,mess.y,texitr))
			{
				exit(0);
			}
		}
	}
}

//游戏结束界面
void Finish(unsigned long long &killcount)
{
	TCHAR* str = new TCHAR[128];
	_stprintf_s(str, 128, _T("击杀数：%llu"), killcount);

	settextcolor(RED);
	outtextxy(swidth/2-textwidth(str) / 2, sheight / 5, str);
	//键盘事件，按enter返回
	LPCTSTR info = _T("按Enter返回");
	settextstyle(30,0,_T("黑体"));
	outtextxy(swidth - textwidth(info), sheight - textheight(info)*9, info);

	while (true)
	{
		ExMessage mess;
		getmessage(&mess,EM_KEY);
		if (mess.vkcode == 0x0D)//判断是否按下Enter键
		{
			return;
		}
	}
} 

//面向敌机，战机，子弹，背景编程
class BK//背景
{
public:
	BK(IMAGE &img)
		:img(img),y(-sheight)
	{

	}
	void show()
	{
		if (y == 0) { y = -sheight; }
		y += 2;
		putimage(0, y, &img);
	}
private:
	IMAGE& img;
	int  y;
};


class Hero//战机
{
public:
	Hero(IMAGE& img)
		:img(img)
	{
		rect.left = swidth / 2 - img.getwidth() / 2;
		rect.top = sheight - img.getheight();
		rect.right = rect.left + img.getwidth();
		rect.bottom = sheight;
	}
	void show()
	{
		putimage(rect.left, rect.top, &img);
	}
	void control()
	{
		ExMessage mess;//定义鼠标结构体
		if (peekmessage(&mess, EM_MOUSE))
		{
			rect.left = mess.x-img.getwidth()/2;
			rect.top = mess.y - img.getheight() / 2;
			rect.right= rect.left + img.getwidth();
			rect.bottom = rect.top+img.getheight();
		}
	}
	RECT& GetRect() { return rect; }

private:
	IMAGE& img;
	RECT rect;

};

class Enemy
{

public:
	Enemy(IMAGE& img,int x)
		:img(img)
	{
		rect.left = x;
		rect.top = -img.getheight();
		rect.right =rect.left+img.getwidth();
		rect.bottom = rect.top+img.getheight();
	}
	bool show()
	{
		if (rect.top >= sheight)
		{
			return false;
		}
		rect.top += 6;
		rect.bottom += 6;
		putimage(rect.left, rect.top, &img);

		return true;
	}
	RECT& GetRect() { return rect; }

private:
	IMAGE& img;
	RECT rect; 
};

class Bullet
{
public:
	Bullet(IMAGE& img,RECT pr)
		:img(img)
	{
		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth()/2;
		rect.right = rect.left + img.getwidth();
		rect.top = pr.top - img.getheight();
		rect.bottom = rect.top+img.getheight();
	}
	bool show()
	{
		if (rect.bottom <= 0)
		{
			return false;
		}

		rect.top -= 20;
		rect.bottom -= 20;
		putimage(rect.left, rect.top, &img);

		return true;
	}
	RECT& GetRect() { return rect; }
private:
	IMAGE& img;
	RECT rect;
};

//释放敌机，并且防止敌机重合
bool AddEnemy(vector<Enemy*> &es,IMAGE &enemyimg)
{
	Enemy* e = new Enemy(enemyimg, abs(rand()) % (swidth - enemyimg.getwidth()));
	for (auto& i: es)
	{
		if (RectHit(i->GetRect(), e->GetRect()))//如果敌机重合不添加
		{
			delete e;
			return false;
		}
	}
	es.push_back(e);//添加敌机
	return true;
}

//游戏开始程序
bool Play()
{
	setbkcolor(WHITE);
	cleardevice();
	bool is_play=true;

	IMAGE heroimg, enemyimg, bkimg, bulletimg;

	//加载图片
	loadimage(&heroimg,_T("D:\\picture_temp\\herop.jpg"),80,90);
	loadimage(&enemyimg, _T("D:\\picture_temp\\enemyp.jpg"),50,60);
	loadimage(&bkimg, _T("D:\\picture_temp\\bk.png"),swidth,sheight*2);
	loadimage(&bulletimg, _T("D:\\picture_temp\\butt.png"),8,10);

	BK bk = BK(bkimg);
	Hero hp = Hero(heroimg);

	vector<Enemy*> es;
	vector<Bullet*> bs;
	int bsing=0;
	unsigned long long killcount = 0;

	for (int i = 0; i < 5; i++)//释放敌机
	{
		AddEnemy(es, enemyimg);
	}

	while (is_play)
	{
		bsing++;
		if (bsing == 10)//调节子弹密度
		{
			bsing = 0;
			bs.push_back(new Bullet(bulletimg,hp.GetRect()));
		}

		BeginBatchDraw();

		bk.show();
		Sleep(6);
		flushmessage();
		Sleep(2);
		hp.control();
		if (_kbhit())
		{
			char v = _getch();
			if (v == 0x20)
			{
				Sleep(500);
				while (true)
				{
					if (_kbhit())
					{
						v = _getch();
						if (v == 0x20)
						{
							break;
						}
					}
					Sleep(16);
				}
			}
		}
		hp.show();

		for (auto i : bs)//显示子弹
		{
			i->show();
		}

		auto bsit = bs.begin();
		while (bsit != bs.end())
		{
			if (!(*bsit)->show())
			{
				bsit = bs.erase(bsit);
			}
			else
			{
				bsit++; 
			}
		}

		auto it = es.begin();
		while (it!=es.end())//遍历敌机图片
		{
			if (RectHit((*it)->GetRect(),hp.GetRect()))//如果英雄和敌机相碰，则失败
			{
				is_play = false;
			}

			auto bit = bs.begin();
			while (bit!=bs.end())//遍历子弹图片
			{
				if (RectHit((*bit)->GetRect(), (*it)->GetRect()))//如果子弹和敌机相碰，则子弹和敌机都消失
				{
					delete(*it);
					es.erase(it);
					it = es.begin();
					delete(*bit);
					bs.erase(bit);
					killcount++;
					break;
				}
				else
				{
					bit++;
				}
			}
			if (!(*it)->show())
			{
				delete(*it);
				it=es.erase(it); 
			}
			else
			{
				it++;
			}
			
		}

		for (int i = 0; i < 5 - es.size(); i++)//填充被击败的敌机
		{
			AddEnemy(es, enemyimg);
		}

		EndBatchDraw();
	}
	Finish(killcount);
	return true;
}

int main()
{
	initgraph(swidth,sheight,EX_NOMINIMIZE| EX_SHOWCONSOLE);
	bool is_live = true;

	while (is_live)
	{
		Welcome();//游戏初始化界面
		is_live=Play();//游戏开始运行程序

	}

	return 0;

}