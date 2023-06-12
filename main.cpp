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
bool isAttacking = false; // 플레이어의 공격 여부를 나타내는 변수
    bool isCooling = false; // 플레이어의 쿨타임 여부를 나타내는 변수
    sf::RectangleShape attackRectangle; // 공격에 사용할 직사각형
    sf::Clock attackClock; // 공격 직사각형의 지속 시간을 측정하기 위한 시계
    sf::Clock cooldownClock; // 쿨타임을 측정하기 위한 시계
    const float cooldownDuration = 3.0f; // 쿨타임의 지속 시간


vector<Texture> textures;
for(int=1;i<=8;i++) {
    Texture texture;
    stringstream ss;
    ss << "images/r" << i <<".png";
    texture.loadFromFile(ss.str());
    textures.push_back(texture);
}

// 장애물

// 장애물 생성 함수.
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

// 충돌 감지 함수.
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


 // 시간 기반으로 점수 증가
            Time elapsedTime = clock.getElapsedTime();
            if (elapsedTime.asSeconds() >= 1.0f) // 1초마다 점수 증가
            {
                score += 1; // 점수 증가량 설정
                clock.restart();
            }

            if (score == 20)
            {
                end++;
            }

            // Set positions
            tree.setPosition(treePos.x, treePos.y);
            playerFrames[index].setPosition(playerPos.x, playerPos.y);

            // Update score text
            scoreText.setString("Score: " + to_string(score));

            // Draw
            window.clear(Color::White);
            window.draw(playerFrames[index]);
            if (isAttacking)
            {
                window.draw(attackRectangle);
            }
            window.draw(cooldownGauge); // 쿨타임 게이지 그리기
            window.draw(tree);
            window.draw(scoreText); // 점수 텍스트 출력
            window.draw(rectangle);
        }


int main(void)
{
    Font arial;
    arial.loadFromFile("arial.ttf");

    // SFML 윈도우
    RenderWindow window(VideoMode(Width, Height), "Open Run");
    window.setFramerateLimit(60); // 프레임

    //Ground
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(1200.f, 55.f));

    int windowHeight = window.getSize().y;
    rectangle.setPosition(0.f, windowHeight - rectangle.getSize().y);

    rectangle.setFillColor(sf::Color::Black);

    // Player
    vector<Texture> textures; // 텍스처를 벡터로 저장
    for (int i = 1; i <= 8; i++) {
        Texture texture;
        stringstream ss;
        ss << "images/r" << i << ".png";
        texture.loadFromFile(ss.str());
        textures.push_back(texture);
    }

    bool isAttacking = false; // 플레이어의 공격 여부를 나타내는 변수
    bool isCooling = false; // 플레이어의 쿨타임 여부를 나타내는 변수
    sf::RectangleShape attackRectangle; // 공격에 사용할 직사각형
    sf::Clock attackClock; // 공격 직사각형의 지속 시간을 측정하기 위한 시계
    sf::Clock cooldownClock; // 쿨타임을 측정하기 위한 시계
    const float cooldownDuration = 3.0f; // 쿨타임의 지속 시간

    attackRectangle.setSize(sf::Vector2f(100.f, 20.f));
    attackRectangle.setFillColor(sf::Color::Yellow);

    float cooldownGaugeFullWidth = 200.0;

    // CoolDown gauge
    sf::RectangleShape cooldownGauge;
    cooldownGauge.setSize(sf::Vector2f(200.f, 20.f)); // 게이지의 크기 설정
    cooldownGauge.setPosition(900.f, 20.f); // 게이지의 위치 설정
    cooldownGauge.setFillColor(sf::Color::Blue); // 게이지의 색상 설정

    //장애물
    float obstacleSpawnTime = 15.0f;

    // 스프라이트로
    vector<Sprite> playerFrames; // 스프라이트를 벡터로 저장
    for (int i = 0; i < textures.size(); i++) {
        Sprite sprite(textures[i]);
        sprite.setScale(1.0f, 1.0f); // 크기 조절
        playerFrames.push_back(sprite);
    }

    static const int PLAYER_Y_BOTTOM = Height - textures[0].getSize().y; // 바닥 위치

    Position playerPos;
    playerPos.x = 50;
    playerPos.y = PLAYER_Y_BOTTOM;

    // 프레임에 따라 이미지 변경을 위한 변수들
    int index = 0; // 이미지 인덱스
    float frame = 0.f;
    float frameSpeed = 1.0f; // 이미지 변경 속도
    const int changeCount = 8; // 몇 프레임마다 변경할지 (수정된 부분)

    const int gravity = 5; // 중력. 점프할 때 사용
    bool isJumping = false; // 점프 중인지
    bool isBottom = true; // 바닥에 발이 닿았는지
    bool canDoubleJump = true; // 더블 점프 가능한지

    // Tree
    Texture treeTexture;
    treeTexture.loadFromFile("images/obj.png");
    Sprite tree(treeTexture);
    static const int TREE_Y_BOTTOM = Height - treeTexture.getSize().y; // 나무 바닥
    Position treePos;
    treePos.x = Width - 20;
    treePos.y = TREE_Y_BOTTOM; // 나무의 y 위치 수정
    const int treeSpeed = 4; // 장애물 속도
    
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


    // Score
    Font font;
    font.loadFromFile("arial.ttf");

    int score = 0; // 점수

    ostringstream sScore;
    sScore << "SCORE : " << score;

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color::Black);

    Clock clock; // SFML 시계

     //게임 시작

    Text GameStart;
    GameStart.setCharacterSize(50);
    GameStart.setFillColor(Color::Red);
    GameStart.setPosition(390, 220);
    GameStart.setFont(arial);
    GameStart.setString(" Start to press 'A' ");

    //게임 이름

    Text GameName;
    GameName.setCharacterSize(100);
    GameName.setFillColor(Color::White);
    GameName.setPosition(315, 100);
    GameName.setFont(arial);
    GameName.setString(" Open Run! ");

    //게임오버

    Text GameOver;
    GameOver.setCharacterSize(80);
    GameOver.setFillColor(Color::Red);
    GameOver.setPosition(390, 150);
    GameOver.setFont(arial);
    GameOver.setString("Game Over");

    Text GameOver2;
    GameOver2.setCharacterSize(30);
    GameOver2.setFillColor(Color::White);
    GameOver2.setPosition(460, 300);
    GameOver2.setFont(arial);
    GameOver2.setString(" Quit to press 'ESC' ");

    Text GameOver3;
    GameOver3.setCharacterSize(30);
    GameOver3.setFillColor(Color::White);
    GameOver3.setPosition(460, 350);
    GameOver3.setFont(arial);
    GameOver3.setString(" Restart to press 'S' ");

    int start = 0;
    int end = 0;

    while (window.isOpen()) // 윈도우가 열렸으면
    {
        window.clear();

        if (start == 0)
        {
            window.draw(GameStart);
            window.draw(GameName);
        }
        else if (end != 0)
        {
            window.draw(GameOver);

            sScore.str("");
            sScore << "SCORE : " << score;
            scoreText.setString(sScore.str());
            scoreText.setPosition(520, 250);
            scoreText.setFillColor(Color::Red);

            window.draw(scoreText);
            window.draw(GameOver2);
            window.draw(GameOver3);

            if (Keyboard::isKeyPressed(Keyboard::Escape))
            {
                window.close();
            }

            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                // 'S' 키를 누르면 게임을 다시 시작합니다.
                start = 0;
                end = 0;
                score = 0;
                clock.restart();
                playerPos.y = PLAYER_Y_BOTTOM;
                isBottom = true;

                // 점수 텍스트를 다시 설정합니다.
                scoreText.setString("Score: " + to_string(score));
            }

        }
        else
        {
            // 점수 위치 설정
            scoreText.setPosition(10, 10);
            window.draw(scoreText);
            scoreText.setCharacterSize(30);
            scoreText.setFillColor(Color::Black);

            // Player jump
            if (Keyboard::isKeyPressed(Keyboard::Space)) // 스페이스 입력 감지
            {
                if (isBottom && !isJumping) // 바닥이고, 점프 중이 아닐 때 점프 가능
                {
                    // Make jumping stage
                    isJumping = true;
                    isBottom = false;
                    canDoubleJump = true; // 더블 점프 가능하도록 설정
                }
                else if (canDoubleJump) // 점프 중이면서 더블 점프 가능할 때
                {
                    // Make double jump
                    canDoubleJump = false; // 더블 점프 불가능하도록 설정
                    playerPos.y -= gravity * 2; // 중력의 2배만큼 올라감
                }
            }

            // Player jump (up and down)
            if (isJumping)
            {
                playerPos.y -= gravity; // 점프 중일 때는 y에서 중력을 뺌 (올라감)
            }
            else
            {
                playerPos.y += gravity;
            }

            // Player jump limit, Player bottom limit
            if (playerPos.y >= PLAYER_Y_BOTTOM) // 바닥에서 지하로 가지 않도록 설정
            {
                playerPos.y = PLAYER_Y_BOTTOM;
                isBottom = true;
                canDoubleJump = false; // 점프 후에는 더블 점프 불가능하도록 설정
            }
            if (playerPos.y <= PLAYER_Y_BOTTOM - 100) // 점프해서 우주로 가지 않도록 설정
            {
                isJumping = false;
            }

            // Player step
            frame += frameSpeed;
            if (frame > changeCount)
            {
                frame -= changeCount;
                ++index;
                if (index >= playerFrames.size()) {
                    index = 0;
                }
            }

            // Player attack
            if (Keyboard::isKeyPressed(Keyboard::X))
            {
                if (!isAttacking && !isCooling)
                {
                    isAttacking = true;

                    // 직사각형 위치 설정 (플레이어의 앞에)
                    int playerWidth = playerFrames[index].getGlobalBounds().width;
                    attackRectangle.setPosition(playerPos.x + playerWidth, playerPos.y);

                    // 공격 시간 측정 시작
                    attackClock.restart();
                }
            }

            // Check if attack duration is over
            if (isAttacking && attackClock.getElapsedTime().asSeconds() >= 0.5f)
            {
                isAttacking = false;
                isCooling = true;

                // Start cooldown measurement
                cooldownClock.restart();

                // Reset cooldown gauge
                cooldownGauge.setSize(sf::Vector2f(cooldownGaugeFullWidth, cooldownGauge.getSize().y));
            }

            // Check if cooldown is over
            if (isCooling && cooldownClock.getElapsedTime().asSeconds() >= cooldownDuration)
            {
                isCooling = false;
            }

            // Update cooldown gauge
            if (isCooling)
            {
                float elapsedCooldownTime = cooldownClock.getElapsedTime().asSeconds();
                float remainingCooldownTime = cooldownDuration - elapsedCooldownTime;
                float currentCooldownWidth = cooldownGaugeFullWidth * (1 - remainingCooldownTime / cooldownDuration);
                cooldownGauge.setSize(sf::Vector2f(currentCooldownWidth, cooldownGauge.getSize().y));
            }

            // Tree move
            if (treePos.x <= 0)
            {
                treePos.x = Width;
            }
            else
            {
                treePos.x -= treeSpeed;
            }


            // 시간 기반으로 점수 증가
            Time elapsedTime = clock.getElapsedTime();
            if (elapsedTime.asSeconds() >= 1.0f) // 1초마다 점수 증가
            {
                score += 1; // 점수 증가량 설정
                clock.restart();
            }

            if (score == 20)
            {
                end++;
            }

            // Set positions
            tree.setPosition(treePos.x, treePos.y);
            playerFrames[index].setPosition(playerPos.x, playerPos.y);

            // Update score text
            scoreText.setString("Score: " + to_string(score));

            // Draw
            window.clear(Color::White);
            window.draw(playerFrames[index]);
            if (isAttacking)
            {
                window.draw(attackRectangle);
            }
            window.draw(cooldownGauge); // 쿨타임 게이지 그리기
            window.draw(tree);
            window.draw(scoreText); // 점수 텍스트 출력
            window.draw(rectangle);
        }

        window.display();

        Event e;
        while (window.pollEvent(e)) // 윈도우 이벤트를 받는 동안
        {
            if (e.type == Event::Closed) // 닫기 이벤트를 받으면 윈도우 닫음
            {
                window.close();
            }
            else if (e.type == Event::KeyPressed)
            {
                if (start == 0 && e.key.code == Keyboard::A)
                {
                    start++;
                }
            }
        }
    }

    return 0;
}
