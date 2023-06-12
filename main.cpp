#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>

using namespace std;
using namespace sf;

#define Width 600
#define Height 300

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
}
// 장애물
struct Obstacle
{
    sf::RectangleShape shape;
    float speed;
};

vector<Obstacle> obstacles; // 장애물들을 저장하는 벡터
sf::Clock obstacleSpawnClock; // 장애물 생성 시간을 측정하기 위한 시계
const float obstacleSpawnDelay = 3.0f; // 장애물 생성 간격

// 장애물 생성 함수
void spawnObstacle()
{
    Obstacle obstacle;
    obstacle.shape.setSize(sf::Vector2f(50.f, 50.f));
    obstacle.shape.setFillColor(sf::Color::Red);

    int obstacleY = PLAYER_Y_BOTTOM - obstacle.shape.getSize().y;
    obstacle.shape.setPosition(Width, obstacleY);

    obstacle.speed = 5.0f;

    obstacles.push_back(obstacle);
}

// 충돌 감지 함수
bool isColliding(const sf::RectangleShape& rect1, const sf::RectangleShape& rect2)
{
    sf::FloatRect rect1Bounds = rect1.getGlobalBounds();
    sf::FloatRect rect2Bounds = rect2.getGlobalBounds();

    return rect1Bounds.intersects(rect2Bounds);
}

…

// 게임 루프 내부에서 장애물 생성 및 충돌 감지 수행

while (window.isOpen())
{
    …

    // 장애물 생성
    if (obstacleSpawnClock.getElapsedTime().asSeconds() >= obstacleSpawnDelay)
    {
        spawnObstacle();
        obstacleSpawnClock.restart();
    }

    // 장애물 이동 및 충돌 감지
    for (int i = 0; i < obstacles.size(); i++)
    {
        obstacles[i].shape.move(-obstacles[i].speed, 0.f);

        // 플레이어와 장애물 충돌 감지
        if (isColliding(playerFrames[index], obstacles[i].shape))
        {
            end++;
        }

        // 화면을 벗어난 장애물 제거
        if (obstacles[i].shape.getPosition().x + obstacles[i].shape.getSize().x < 0.f)
        {
            obstacles.erase(obstacles.begin() + i);
            i—;
        }
    }

    …

    // 장애물 그리기
    for (const Obstacle& obstacle : obstacles)
    {
        window.draw(obstacle.shape);
    }

    …
}


