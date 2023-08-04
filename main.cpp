//�򵥵ķɻ���սС��Ϸ
//��Ϸ����ʹ��Easyx����


/*��Ϸ����
* 1.���Ӣ�ۺ͵л�������ʧ��
* 2.���Ӣ�۱��л��ӵ�������ʧ��
*/

#include<iostream>
#include<graphics.h>//easyx��ͷ�ļ�
#include<vector>
#include<conio.h>

 using namespace std;

constexpr auto swidth=600;
constexpr auto sheight=1100;
//constexpr unsigned int CHP = 10;//����ֵ����

constexpr auto hurttime = 1000;//Ϊ�˼������˴�������ȴʱ�䣬��λΪms

//�ж���갴��������
bool PointInRect(int x, int y, RECT& r)
{
	return (x >= r.left && x <= r.right && y >= r.top && y <= r.bottom);
}

//����л��ص���ײ
bool RectHit(RECT &r1,RECT &r2)
{
	RECT r;
	r.left = r1.left - (r2.right - r2.left);
	r.right = r1.right;
	r.top = r1.top - (r2.bottom-r2.top);
	r.bottom = r1.bottom;

	return(r2.left > r.left && r2.left <= r.right && r2.top >= r.top && r2.top <= r.bottom);
}
//��Ϸ��ӭ����
void Welcome()
{
	//���������ַ���
	LPCTSTR title = _T("�ɻ���ս");
	LPCTSTR tplay = _T("��ʼ��Ϸ");
	LPCTSTR texit = _T("�˳���Ϸ");
	
	//���忪ʼ��Ϸ���˳���Ϸλ�ýṹ��
	RECT tplayr, texitr;

	BeginBatchDraw(); 

	setbkcolor(WHITE);//������Ϸ������ɫ
	cleardevice();
	settextstyle(60,0,_T("����"));//������Ϸ�����ʽ��С
	settextcolor(BLUE);//������Ϸ������ɫ
	outtextxy(swidth/2 - textwidth(title) / 2, sheight/8,title);//��ʾ���ɻ���ս��
	settextstyle(40, 0, _T("����"));//������Ϸ������ɫ

	//���á���ʼ��Ϸ��λ��
	tplayr.left = swidth / 2 - textwidth(tplay)/2;
	tplayr.right = swidth / 2 + textwidth(tplay) / 2;
	tplayr.top = sheight / 5*1.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	//���á��˳���Ϸ��λ��
	texitr.left = swidth / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = sheight / 5*2;
	texitr.bottom = texitr.top + textheight(texit);

	//��ʾ
	outtextxy(tplayr.left, tplayr.top, tplay);
	outtextxy(texitr.left, texitr.top, texit);

	EndBatchDraw();

	while (1)
	{
		ExMessage mess;//�������ṹ��
		getmessage(&mess, EM_MOUSE);//��ȡ�����Ϣ
		if (mess.lbutton)//�ж�����������
		{
			if (PointInRect(mess.x, mess.y, tplayr))//�ж���갴������Ϊ��ʼ��Ϸ����
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

//��Ϸ��������
void Finish(unsigned long long &killcount)
{
	TCHAR* str = new TCHAR[128];
	_stprintf_s(str, 128, _T("��ɱ����%llu"), killcount);

	settextcolor(RED);
	outtextxy(swidth/2-textwidth(str) / 2, sheight / 5, str);
	//�����¼�����enter����
	LPCTSTR info = _T("��Enter����");
	settextstyle(30,0,_T("����"));
	outtextxy(swidth - textwidth(info), sheight - textheight(info)*9, info);

	while (true)
	{
		ExMessage mess;
		getmessage(&mess,EM_KEY);
		if (mess.vkcode == 0x0D)//�ж��Ƿ���Enter��
		{
			return;
		}
	}
} 

//����л���ս�����ӵ����������
class BK//����
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


class Hero//ս��
{
public:
	Hero(IMAGE& img)
		:img(img)/*,HP(CHP)*/
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
		ExMessage mess;//�������ṹ��
		if (peekmessage(&mess, EM_MOUSE))
		{
			rect.left = mess.x-img.getwidth()/2;
			rect.top = mess.y - img.getheight() / 2;
			rect.right= rect.left + img.getwidth();
			rect.bottom = rect.top+img.getheight();
		}
	}

	/*bool hurt()
	{
		HP--;
		return(HP == 0) ? false : true;
	}*/

	RECT& GetRect() { return rect; }

private:
	IMAGE& img;
	RECT rect;

	//unsigned int HP;

};

class Enemy
{

public:
	Enemy(IMAGE& img,int x,IMAGE *& boom)
		:img(img),isdie(false),boomnum(0)
	{
		selfboom[0] = boom[0];
		selfboom[1] = boom[1];
		selfboom[2] = boom[2];

		rect.left = x;
		rect.top = -img.getheight();
		rect.right =rect.left+img.getwidth();
		rect.bottom = rect.top+img.getheight();
	}
	bool show()
	{

		if (isdie)
		{
			if (boomnum == 3)
			{
				return false;
			}
			putimage(rect.left, rect.top, selfboom+boomnum);
			boomnum++;
			
			return true;
		}

		if (rect.top >= sheight)
		{
			return false;
		}
		rect.top += 4;
		rect.bottom += 4;
		putimage(rect.left, rect.top, &img);

		return true;
	}

	void Isdie()
	{
		isdie = true; 
	}

	RECT& GetRect() { return rect; }

private:
	IMAGE& img;
	RECT rect; 
	IMAGE selfboom[3];

	bool isdie;
	int boomnum;
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

		rect.top -= 30;
		rect.bottom -= 30;
		putimage(rect.left, rect.top, &img);

		return true;
	}
	RECT& GetRect() { return rect; }
protected:
	IMAGE& img;
	RECT rect;
};

class EBullet:public Bullet
{
public:
	EBullet(IMAGE& img, RECT pr)
		:Bullet(img, pr)
	{
		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth() / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = pr.bottom;
		rect.bottom = rect.top + img.getheight();
	}
	bool show()
	{
		if (rect.top >= sheight)
		{
			return false;
		}

		rect.top += 8;
		rect.bottom += 8;
		putimage(rect.left, rect.top, &img);

		return true;
	}
};

//�ͷŵл������ҷ�ֹ�л��غ�
bool AddEnemy(vector<Enemy*> &es,IMAGE &enemyimg,IMAGE* boom)
{
	Enemy* e = new Enemy(enemyimg, abs(rand()) % (swidth - enemyimg.getwidth()),boom);
	for (auto& i: es)
	{
		if (RectHit(i->GetRect(), e->GetRect()))//����л��غϲ����
		{
			delete e;
			return false;
		}
	}
	es.push_back(e);//��ӵл�
	return true;
}

//��Ϸ��ʼ����
bool Play()
{
	setbkcolor(WHITE);
	cleardevice();
	bool is_play=true;

	IMAGE heroimg, enemyimg, bkimg, bulletimg;
	IMAGE eboom[3];

	//����ͼƬ
	loadimage(&heroimg,_T("D:\\picture_temp\\herop.jpg"),80,90);
	loadimage(&enemyimg, _T("D:\\picture_temp\\enemyp.jpg"),50,60);
	loadimage(&bkimg, _T("D:\\picture_temp\\bk.png"),swidth,sheight*2);
	loadimage(&bulletimg, _T("D:\\picture_temp\\butt.png"),8,10);

	loadimage(&eboom[0], _T("D:\\picture_temp\\boom1.jpg"), 30, 40);
	loadimage(&eboom[1], _T("D:\\picture_temp\\boom2.jpg"), 40, 50);
	loadimage(&eboom[2], _T("D:\\picture_temp\\boom3.jpg"), 50, 60);

	BK bk = BK(bkimg);
	Hero hp = Hero(heroimg);

	vector<Enemy*> es;
	vector<Bullet*> bs;
	vector<EBullet*> ebs;
	int bsing=0;

	clock_t hurtlast = clock();

	unsigned long long killcount = 0;

	for (int i = 0; i < 3; i++)//�ͷŵл�
	{
		AddEnemy(es, enemyimg,eboom);
	}

	while (is_play)
	{
		bsing++;
		
		if (bsing %5== 0)//���ڼ����ӵ��ܶ�
		{
			bs.push_back(new Bullet(bulletimg, hp.GetRect())); 
		}
		if (bsing == 60)//���ڵл��ӵ��ܶ�
		{
			bsing = 0;
			for (auto& i : es)
			{
				ebs.push_back(new EBullet(bulletimg, i->GetRect()));
			}
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

		auto bsit = bs.begin();//��ʾ�����ӵ�
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

		auto ebsit = ebs.begin();//��ʾ�л��ӵ�
		while (ebsit != ebs.end())
		{
			if (RectHit((*ebsit)->GetRect(), hp.GetRect()))
			{
				is_play = false;
				/*if (clock() - hurtlast >= hurttime)
				{
					is_play = hp.hurt();
					hurtlast = clock();
				}*/
				break;
			}
			if (!(*ebsit)->show())
			{
				ebsit = ebs.erase(ebsit);
			}
			
			else
			{
				ebsit++;
			}
		}
		 
		auto it = es.begin();
		while (it!=es.end())//�����л�ͼƬ
		{
			if (RectHit((*it)->GetRect(),hp.GetRect()))//���Ӣ�ۺ͵л���������ʧ��
			{
				is_play = false;
			}

			auto bit = bs.begin();
			while (bit!=bs.end())//�����ӵ�ͼƬ
			{
				if (RectHit((*bit)->GetRect(), (*it)->GetRect()))//����ӵ��͵л����������ӵ��͵л�����ʧ
				{
					(*it)->Isdie();
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

		for (int i = 0; i < 3 - es.size(); i++)//��䱻���ܵĵл�
		{
			AddEnemy(es, enemyimg,eboom);
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
		Welcome();//��Ϸ��ʼ������
		is_live=Play();//��Ϸ��ʼ���г���

	}

	return 0;

}