// 贪吃蛇.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<easyx.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<conio.h>
//表示蛇的节点
typedef struct {
    int x;
    int y;
}node;
#define NODE_WIDTH 40//NODE_WIDTH代表每一个网格的数据，其中网格的长宽均为40
void paintGrid();//在窗体上绘制网格的函数
void paintSnake(node* snake, int n);//绘制贪吃蛇的函数
//四个方向的移动
enum direction {
    eUp,
    eDown,
    eLeft,
    eRight
};
node snakeMove(node* sanke, int length, int direction);//根据传入的direction的值来确定贪吃蛇的移动方向
void changeDirection(enum direction* pD);//由键盘来控制贪吃蛇的运动方向
node creatFood(node* snake, int length);//随机生成食物的函数
void paintFood(node food);//绘制食物的函数
bool isGameOver(node* snake, int length);//判断游戏结束的条件
//游戏结束;1蛇头触碰墙壁
// 2蛇头碰到蛇身
//游戏结束返回true，否则返回false
void reset(node* snake, int* pLength, enum direction* d);//游戏结束后复位所有的数据
int main()
{
    //创建窗体,并添加颜色
    initgraph(800, 600);
    setbkcolor(RGB(164, 225, 202));
    cleardevice();
    node snake[100] = { {5,7},{4,7},{3,7},{2,7},{1,7} };//蛇的初始位置,其中5,7之类的为坐标
    int length = 5;//蛇的初始长度为5
    enum direction d = eRight;
    paintSnake(snake, length);//调用函数绘制贪吃蛇
    paintGrid();//调用paintGrid函数，来绘制网格
    srand(unsigned int(time(NULL)));//随机生成（调用当前的时间）
    node food = creatFood(snake, length);

    //让贪吃蛇动起来
    while (1) {
        cleardevice();//清空窗体
        paintGrid();//绘制网格
        paintSnake(snake, length);//绘制蛇的节点
        paintFood(food);
        Sleep(500);
        changeDirection(&d);//检查是否有键盘输入
        node lastTail = snakeMove(snake, length, d);
        //snakeMove(snake, length, d);//用键盘控制贪吃蛇的运动
        //判断蛇头是否与食物重合
        if (snake[0].x == food.x && snake[0].y == food.y) {
            if (length < 100) {
                snake[length] = lastTail;//蛇吃了食物以后，将蛇尾节点重新加回去
                length++;
            }
            food = creatFood(snake, length);//蛇吃了食物以后重新生成一个
        }
        //如果游戏结束，复位所有的数据
        if (isGameOver(snake, length) == true) {
            reset(snake, &length, &d);
            food = creatFood(snake, length);
        }
    }
    getchar();
    closegraph();
}
void paintGrid() {
    for (int x = 0;x < 800;x += NODE_WIDTH) {
        line(x, 0, x, 600);
    }
    for (int y = 0;y < 600;y += NODE_WIDTH) {
        line(0, y, 800, y);
    }
}
void paintSnake(node* snake, int n) {
    int left, right, top, bottom;
    for (int i = 0;i < n;i++) {
        left = snake[i].x * NODE_WIDTH;
        top = snake[i].y * NODE_WIDTH;
        right = (snake[i].x + 1) * NODE_WIDTH;
        bottom = (snake[i].y + 1) * NODE_WIDTH;
        solidrectangle(left, top, right, bottom);
    }
}
node snakeMove(node* snake, int length, int direction) {
    node tail = snake[length - 1];//记录蛇尾节点的数据并返回
    for (int i = length - 1;i > 0;i--) {
        snake[i] = snake[i - 1];//在蛇移动的过程中，后一点的坐标会覆盖前一点的坐标
    }
    node newHead;//node是结构体,newHead就是结构体的变量,表示贪吃蛇在移动时头的位置
    newHead = snake[0];
    if (direction == eUp) {
        newHead.y--;//向上移动，纵坐标减小，接下来同理
    }
    else if (direction == eDown) {
        newHead.y++;
    }
    else if (direction == eLeft) {
        newHead.x--;
    }
    else {//else代表的情况是向右移动
        newHead.x++;
    }
    snake[0] = newHead;//将新的头节点更新到数组上
    return tail;
}
void changeDirection(enum direction* pD) {
    if (_kbhit() != 0) {
        char c = _getch();
        switch (c) {
        case 'w':
           if(*pD!=eDown)//不能让贪吃蛇往后退,后面同理
            *pD = eUp;
            break;
        case 's':
            if(*pD!=eUp)
            *pD = eDown;
            break;
        case 'a':
            if(*pD!=eRight)
            *pD = eLeft;
            break;
        case'd':
            if(*pD!=eLeft)
            *pD = eRight;
            break;
        }
    }
}
node creatFood(node* snake, int length) {//第一个参数表示蛇的节点数据，第二个参数表示蛇的节点数量
    node food;
    while (1) {
        //保证食物的随机生成
        food.x = rand() % (800 / NODE_WIDTH);
        food.y = rand() % (600 / NODE_WIDTH);
        int i;
        for (i = 0;i < length;i++) {
            //如果食物和贪吃蛇重叠，跳出循环，继续生成
            if (snake[i].x == food.x && snake[i].y == food.y) {
                break;
            }
        }
        //如果i小于length，表示食物和蛇重叠，重新开始循环
        if (i < length)
            continue;
        else
            break;
    }
    return food;
}
void paintFood(node food) {
    int left, right, top, bottom;
    left = food.x * NODE_WIDTH;
    top = food.y * NODE_WIDTH;
    right = (food.x + 1) * NODE_WIDTH;
    bottom = (food.y + 1) * NODE_WIDTH;
    setfillcolor(YELLOW);
    solidrectangle(left, top, right, bottom);
    setfillcolor(WHITE);
}
bool isGameOver(node* snake, int length) {
    //蛇头碰壁
    if (snake[0].x < 0 || snake[0].x>800 / NODE_WIDTH)
        return true;
    if (snake[0].y < 0 || snake[0].y>600 / NODE_WIDTH)
        return true;
    //蛇头碰蛇身
    for (int i = 1;i < length;i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            return true;
    }
    return false;//游戏继续
}
void reset(node* snake, int* pLength, enum direction* d) {
    snake[0] = node{ 5,7 };
    snake[1] = node{ 4,7 };
    snake[2] = node{ 3,7 };
    snake[3] = node{ 2,7 };
    snake[4] = node{ 1,7 };
    *pLength = 5;
    *d = eRight;
}