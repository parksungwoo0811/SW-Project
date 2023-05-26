#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>

using namespace std;
using namespace sf;

#define Width 1200
#define Height 600

struct 
{
    int x;
    int y;
};

int main(void)
{
    RenderWindow window(VideoMode(Width, Height), "Project Game");
    window.setFramerateLimit(60);
                        
    
    //이미지 불러오기
    Texture t1;
    Texture t2;
    t1.loadFromFile("");
    t2.loadFromFile("");
    
    //이미지 교체
    int index = 0;
    float frame = 0.f;
    float framespeed = 0.4f;
    const int countChange = 5;
    
   //Boo 위치
    Sprite Boo[2];
    Boo[0] = Sprite(t1);
    Boo[1] = Sprite(t2);
    
    static const int BOO_Y_Pos = Height - t1.getSize().y;
    
    while (window.isOpen())        
    {
        Event e;
        while (window.pollEvent(e))    
        {
            if (e.type == Event::Closed)    
            {
                window.close();
            }
        }
        //logic.
        //Boo jump.
        if (Keyboard::isKeyPressed(Keyboard::Space)) 
        {
            if (isBottom && !isJumping)    
            {
                //make jumping stage;
                isJumping = true;
                isBottom = false;
            }
        }
        //Boo jump(up and down)
        if (isJumping)    
        {
            BooPos.y -= gravity; 
        }
        else
        {
            BooPos.y += gravity;
        }
        // 부 점프 한계 설정
        
        if (BooPos.y >= BOO_Y_BOTTOM)   
        {
            BooPos.y = BOO_Y_BOTTOM;
            isBottom = true;
        }
        if (BooPos.y <= BOO_Y_BOTTOM - 100)
        {
            isJumping = false;
        }
        //Boo step.
        frame += frameSpeed;
        if (frame > changeCount)
        {
            frame -= changeCount;
            ++index;
            if (index >= 2) { index = 0; }
        }
        //tree move.
        if (treePos.x <= 0)
        {
            treePos.x = WIDTH;
        }
        else
        {
            treePos.x -= treeSpeed;
        }
        //tree Position.
        tree.setPosition(treePos.x, treePos.y);
        //Boo Position.
        BooArr[index].setPosition(BooPos.x, BooPos.y);
        //draw.
        window.clear(Color::White);
        window.draw(BooArr[index]);
        window.draw(tree);
        window.display();
    }
    
    
}


