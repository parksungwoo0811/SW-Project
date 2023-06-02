#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>

using namespace std;
using namespace sf;

#define Width 1200
#define Height 600

struct Position
{
    int x;
    int y;
};
 
int main(void)
{
    //SFML 윈도우을 열어주고, title을 정해줍니다.
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Dinosaur Game. By BlockDMask");
    window.setFramerateLimit(60);    //프레임
 
    //dino
    Texture t1;
    Texture t2;
    t1.loadFromFile("images/boo1.png");
    t2.loadFromFile("images/boo2.png");
 
    //스프라이트로
    Sprite dinoArr[2];
    BOOi[0] = Sprite(t1);
    BOOi[1] = Sprite(t2);
 
    static const int DINO_Y_BOTTOM = HEIGHT - t1.getSize().y;    //바닥위치
    
    Position dinoPos;
    BooPos.x = 50;
    BooPos.y = DINO_Y_BOTTOM;
 
    //프레임에 따라 왼발 오른발 해주기 위한 변수 네개
    int index            = 0;    //왼발 오른발 인덱스
    float frame            = 0.f;
    float frameSpeed    = 0.4f;
    const int changeCount = 5;    //몇 프레임에 체인지 할지
 
    const int gravity = 5;    //중력. 점프할때 사용
    bool isJumping = false;    //점프 중인지
    bool isBottom = true;    //바닥에 발이 닿았는지
    
    //tree
    Texture t3;
    t3.loadFromFile("images/obj.png");
    Sprite obj(t3);
    static const int OBJ_Y_BOTTOM = HEIGHT - t3.getSize().y;    //나무 바닥
    Position objPos;
    objPos.x = WIDTH - 20;
    objPos.y = TREE_Y_BOTTOM;
    const int objSpeed = 4;    //장애물 속
    while (window.isOpen())        //윈도우가 열렸으면
    {
        Event e;
        while (window.pollEvent(e))    //윈도우 이벤트를 받는데
        {
            if (e.type == Event::Closed)    //닫기면 윈도우 닫음
            {
                window.close();
            }
        }
        //logic.
        //boo jump.
        if (Keyboard::isKeyPressed(Keyboard::Space)) //스페이스 입력 감지
        {
            if (isBottom && !isJumping)    //바닥이고, 점프중이 아닐때 점프 가능
            {
                //make jumping stage;
                isJumping = true;
                isBottom = false;
            }
        }
        //boo jump(up and down)
        if (isJumping)    
        {
            BooPos.y -= gravity; //점프중일때는 y에서 중력을 뺴줌 그럼 올라감
        }
        else
        {
            BooPos.y += gravity;
        }
        //boo jump limit, Boo bottom limit.
        if (BooPos.y >= BOO_Y_BOTTOM)    //바닥에서 지하로 가지 않도록 설정
        {
            BooPos.y = BOO_Y_BOTTOM;
            isBottom = true;
        }
        if (BooPos.y <= BOO_Y_BOTTOM - 100)    //점프해서 우주로 가지 않도록 설정
        {
            isJumping = false;
        }
        //boo step.
        frame += frameSpeed;
        if (frame > changeCount)
        {
            frame -= changeCount;
            ++index;
            if (index >= 2) { index = 0; }
        }
        //obj move.
        if (objPos.x <= 0)
        {
            objPos.x = WIDTH;
        }
        else
        {
            objPos.x -= objSpeed;
        }
        //obj Position.
        obj.setPosition(objPos.x, objPos.y);
        //boo Position.
        Booi[index].setPosition(BooPos.x, BooPos.y);
        //draw.
        window.clear(Color::White);
        window.draw(BOOi[index]);
        window.draw(obj);
        window.display();
    }
    return 0;
}