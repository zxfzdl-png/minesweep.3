#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<easyx.h>
#define ROW 10
#define COL 10
#define MineNum 10

//定义图片资源
IMAGE imgs[12]; 
void loadResource()
{
	for(int i=0;i<12;i++)
	{
		char imgPath[50]={0};
		sprintf(imgPath,"./bmp/%d.bmp", i);
		loadimage(&imgs[i],imgPath);
			
	}
	
}
void draw(int map[][COL]);
void init(int map[][COL]);
void show(int map[][COL]);
void mouseMsg(ExMessage*msg,int map[][COL]);
void boom(int map[][COL],int row,int col);
int judje(int map[][COL],int row,int col);

int main()
{
	//创建一个窗口
	initgraph(800,800); 
	//扫雷地图
	int map[ROW][COL]={0};
	init(map);
	while(true)
	{
		//处理消息
		ExMessage msg;
		while(peekmessage(&msg,EX_MOUSE))
		{
			switch(msg.message)
			{
				case WM_LBUTTONDOWN: //鼠标点击情况 
				case WM_RBUTTONDOWN:
			    mouseMsg(&msg,map);
			    int ret=judje(map,msg.y/80,msg.x/80);   //点击之后判断 
			    printf("judge:%d\n",ret);
			    if(ret==-1)
			    {
			    	draw(map);
			    int select =MessageBox(GetHWnd(),"很遗憾你输掉了游戏","defult",MB_OKCANCEL);
			    	if(select==IDOK)//再来一把
					{
						init(map);	
					 } 
					 else
					 {
					 	exit(0);
					 }
				}
				else if(ret==1)
				{
					 int select =MessageBox(GetHWnd(),"恭喜你赢了","win",MB_OKCANCEL);
					 if(select==IDOK)//再来一把
					{
						init(map);	
					 } 
					 else
					 {
					 	exit(0);
					 }
				}
				system("cls");
				//show(map);
			    break;
			}
		  }  
		draw(map);
		
	}
	show(map);
	//getchar();
	return 0; 
}

void show(int map[][COL])
{
	for(int i=0;i<ROW;i++)
	{
		for(int j=0;j<COL;j++)
		{
			printf("%2d ",map[i][j]);
		}
		printf("\n");
	}
}
//初始化数据
void init(int map[][COL])
{
	loadResource();
	//设置随机数种子
	srand((unsigned)time(NULL));
	
	//把数组初始化为0
	 memset(map,0,sizeof(int)*ROW*COL);
	//随机设置10个雷用-1表示
	for(int i=0;i<MineNum;)
	{
		//数组有效下表【0，9】
		int r = rand()%ROW;
		int c = rand()%COL;
		//判断当前位置是否生成雷,成功才设置雷 
		if(map[r][c] == 0)
		{  
			map[r][c]=-1;
			i++;
	    }
	 } 
	 //把以雷为中心的九宫格都加1，雷除外 
	 for(int i=0;i<ROW;i++)
	 {
	 	for(int j=0;j<COL;j++)
	 	{
	 		//找到雷,并遍历雷所在的九宫格 
			  if(map[i][j]==-1)
			  {
			  	for(int r=i-1;r<=i+1;r++)
			  	{
			  		for(int c=j-1;c<=j+1;c++)
			  		{
			  			//对周围数据+1
						  if(r>=0 && r<ROW && c>=0&& c<COL&&map[r][c]!=-1)
						  {
						  	++map[r][c];
						   } 
					  }
				  }
			  }
		 }
	 }
 	//加密格子
	for(int i=0;i<ROW;i++)
	{
		for(int j=0;j<COL;j++)
		{
			map[i][j]+=20;
		}
	  }  
 }
//绘制
void draw(int map[][COL])
{
	for(int k=0;k<ROW;k++)
	{
		for(int i=0;i<COL;i++)
		{
			
		
			if(map[k][i]>=0&&map[k][i]<=8)
			{
				putimage(i*80,k*80,&imgs[map[k][i]]);
			}
			else if(map[k][i]==-1)
			{
				putimage(i*80,k*80,&imgs[9]);
			}
			else if(map[k][i]>=19&&map[k][i]<=28)
			{
				putimage(i*80,k*80,&imgs[11]);
			}
			else if(map[k][i]>39)
			{
				putimage(i*80,k*80,&imgs[10]);
			}
		}
	 } 
}
//鼠标操作数据
void mouseMsg(ExMessage*msg,int map[][COL])
{
	//现根据鼠标点击的坐标 求出对应数组的下标 
	int c=msg->x/80;
	int r=msg->y/80;
	
	//左键打开格子
	if(msg->message == WM_LBUTTONDOWN)
	{
		//什么时候点击有效，没有打开的时候就打开 
		if(map[r][c]>=19&&map[r][c]<=28)
		{
		 
			map[r][c]-=20;
			boom(map,r,c);
		}
	}
	//右键标记格子
	else if(msg->message==WM_RBUTTONDOWN)
	{
		//判断是否能够标记
		if(map[r][c]>+19&&map[r][c]<=28)
		{
			map[r][c]+=20;
		 } 
		 else if(map[r][c]>=39)
		 {
		 	map[r][c]-=20;
		 }
	}
	
		
 } 
 //点击空白格，连环爆开周围的九宫格 row是当前行 
 void boom(int map[][COL],int row,int col)
 {
 	//判断是否为空白格子
 	if(map[row][col]==0)
 	{
 		for(int r=row-1;r<=row+1;r++)
 		{
 			for(int c=col-1;c<=col+1;c++)
 			{
 				if((r>=0&&r<ROW&&c>=0&&c<COL)&&map[r][c]>=19&&map[r][c]<=28) //每越界没打开 
				 {
				      map[r][c]-=20;
				      boom(map,r,c);
				  }
			 }
		 }
	 }
	  
 	
  }
  //游戏结束条件
  int  judje(int map[][COL],int row,int col)
  {
  	//点到雷结束
	  if(map[row][col]==-1||map[row][col]==19)
	  {
	  	return -1;
	  }
	//点完格子赢了,点开了90个格子 
	int cnt=0;
	  for(int i=0;i<ROW;i++)
	  {
	  	for(int k=0;k<COL;k++)
	  	{
	  		if(map[i][k]>=0&&map[i][k]<=8)
	  		{
	  			cnt++;
			  }
		  }
	  }
	  if(ROW*COL-MineNum==cnt)
	  {
	  	return 1;
	  }
	  return 0;
	   
   } 

 
 
