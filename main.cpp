#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>

#include <random>

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
//장애물  생성 파트
// 장애물
struct Obstacle
{
    sf::RectangleShape shape;
    float speed;
};

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


int getRandomNumber(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}


bool checkCollision(const Sprite& player, const Sprite& obstacle) {
    FloatRect playerBounds = player.getGlobalBounds();
    FloatRect obstacleBounds = obstacle.getGlobalBounds();

    // 플레이어가 장애물 위에 있는지 확인
    bool isPlayerAboveObstacle = player.getPosition().y + playerBounds.height < obstacle.getPosition().y + obstacleBounds.height;

    // 플레이어와 장애물의 전역 경계 상자가 겹치고, 플레이어가 장애물 위에 없는 경우에만 충돌 감지
    return playerBounds.intersects(obstacleBounds) && !isPlayerAboveObstacle;
}

int main(void)
{
    // SFML 윈도우
    RenderWindow window(VideoMode(Width, Height), "Open Run");
    window.setFramerateLimit(144); // 프레임
    // ---------------------------------------------------------------------------

    //폰트 설정
    Font arial;
    arial.loadFromFile("arial.ttf");

    // ---------------------------------------------------------------------------
    
     // 배경 이미지 로드
    const int numBackgrounds = 30; // 배경 이미지 개수
    std::vector<sf::Texture> b(numBackgrounds);
    for (int i = 0; i < numBackgrounds; ++i) {
        std::stringstream ss;
        ss << "images/b" << (i + 1) << ".png";
        if (!b[i].loadFromFile(ss.str())) {
            // 이미지 로드 실패
            std::cerr << "Failed to load background image: " << ss.str() << std::endl;
            return -1;
        }
    }

    sf::Sprite backgroundSprite(b[0]);

    // 배경 이미지 변경을 위한 변수들
    int currentBackgroundIndex = 0; // 현재 보여지고 있는 배경 이미지 인덱스
    int currentBackgroundScore = 2; // 현재 보여지고 있는 배경 이미지의 점수


    // ---------------------------------------------------------------------------
    
    // Ground 이미지 로드
    sf::Texture groundTexture;
    if (!groundTexture.loadFromFile("images/G.png")) {
        // 이미지 로드 실패
        std::cerr << "Failed to load Ground image!" << std::endl;
        return -1;
    }

    // Ground 스프라이트 생성
    sf::Sprite groundSprite(groundTexture);

    // Ground 크기 조정
    float scaleFactor = 2.5f;
    groundSprite.setScale(1.8*scaleFactor, scaleFactor);

    // Ground 위치 설정
    int windowHeight = window.getSize().y - 17;
    groundSprite.setPosition(-15.f, windowHeight - groundTexture.getSize().y);

    // ---------------------------------------------------------------------------

    // 플레이어
    vector<Texture> textures; // 텍스처를 벡터로 저장
    for (int i = 1; i <= 8; i++) {
        Texture texture;
        stringstream ss;
        ss << "images/r" << i << ".png";
        texture.loadFromFile(ss.str());
        textures.push_back(texture);
    }

    // 스프라이트
    vector<Sprite> playerFrames; // 스프라이트를 벡터로 저장
    for (int i = 0; i < textures.size(); i++) {
        Sprite sprite(textures[i]);
        sprite.setScale(1.5f, 1.5f); // 크기 조절
        playerFrames.push_back(sprite);
    }

    static const int PLAYER_Y_BOTTOM = Height - textures[0].getSize().y - 60; // 바닥 위치

    Position playerPos;
    playerPos.x = 100;
    playerPos.y = PLAYER_Y_BOTTOM ;

    // 프레임에 따라 이미지 변경을 위한 변수들
    int index = 0; // 이미지 인덱스
    float frame = 0.f;
    float frameSpeed = 1.0f; // 이미지 변경 속도
    const int changeCount = 8; // 몇 프레임마다 변경할지 (수정된 부분)

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

    //Clock clock; // SFML 시계

    // ---------------------------------------------------------------------------

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

    // ---------------------------------------------------------------------------

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
            window.clear();
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

            // ---------------------------------------------------------------------------
        }
        else
        {
            // 점수 위치 설정
            scoreText.setPosition(10, 10);
            window.draw(scoreText);
            scoreText.setCharacterSize(30);
            scoreText.setFillColor(Color::Black);
            //logic.
            //dino jump.
            if (Keyboard::isKeyPressed(Keyboard::Space)) //스페이스 입력 감지
            {
                if (isBottom && !isJumping)    //바닥이고, 점프중이 아닐때 점프 가능
                {
                    //make jumping stage;
                    isJumping = true;
                    isBottom = false;
                }
            }
            //dino jump(up and down)
            if (isJumping)
            {
                playerPos.y -= gravity; //점프중일때는 y에서 중력을 뺴줌 그럼 올라감
            }
            else
            {
                playerPos.y += gravity;
            }
            //dino jump limit, dino bottom limit.
            if (playerPos.y >= PLAYER_Y_BOTTOM)    //바닥에서 지하로 가지 않도록 설정
            {
                playerPos.y = PLAYER_Y_BOTTOM;
                isBottom = true;
            }
            if (playerPos.y <= PLAYER_Y_BOTTOM - 150)    //점프해서 우주로 가지 않도록 설정
            {
                isJumping = false;
            }
            // ---------------------------------------------------------------------------
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
            // ---------------------------------------------------------------------------
            // Move trees
            for (int i = 0; i < trees.size(); i++) {
                trees[i].setPosition(treePositions[i].x, treePositions[i].y - 53);
                treePositions[i].x -= 6;

                // Check if tree is out of screen
                if (treePositions[i].x < -obstacleWidth) {
                    // Remove the old tree
                    trees.erase(trees.begin() + i);
                    treePositions.erase(treePositions.begin() + i);
                    i--; // Adjust the loop index

                    // Generate a new tree
                    const int fixedYPosition = PLAYER_Y_BOTTOM - obstacleHeight; // Fixed y position
                    Position position;
                    position.x = Width + getRandomNumber(300, 800);
                    position.y = fixedYPosition;
                    treePositions.push_back(position);

                    // Create a new tree sprite and add it to the vector
                    sf::Sprite newTree;
                    // ... Set the properties of the new tree sprite (position, texture, etc.)
                    trees.push_back(newTree);

                }
                else {
                    // Check collision
                    if (checkCollision(playerFrames[index], trees[i])) {
                        // Handle collision (e.g. game over)
                        end++;
                        cout << "Game over!" << endl;
                        // ...
                    }
                }
            }

            // Generate a new tree if needed
            if (trees.size() < 3) {
                const int fixedYPosition = PLAYER_Y_BOTTOM - obstacleHeight; // Fixed y position
                Position position;
                position.x = Width + getRandomNumber(300, 800);
                position.y = fixedYPosition;
                treePositions.push_back(position);

                // Create a new tree sprite and add it to the vector
                sf::Sprite newTree;
                newTree.setPosition(position.x, position.y);
                trees.push_back(newTree);
            }

            // ---------------------------------------------------------------------------
            // 시간 기반으로 점수 증가
            Time elapsedTime = clock.getElapsedTime();
            if (elapsedTime.asSeconds() >= 1.0f) // 1초마다 점수 증가
            {
                score += 1; // 점수 증가량 설정
                clock.restart();
            }

            // ---------------------------------------------------------------------------

            
            
            // ---------------------------------------------------------------------------
            
            // 배경 이미지 변경
            if (score >= currentBackgroundScore) {
                currentBackgroundIndex = (currentBackgroundIndex + 1) % numBackgrounds;
                currentBackgroundScore += 2;
                backgroundSprite.setTexture(b[currentBackgroundIndex]);
            }

            // ---------------------------------------------------------------------------

            
            // Set positions
            //tree.setPosition(treePos.x, treePos.y-54);
            playerFrames[index].setPosition(playerPos.x, playerPos.y);

            // Update score text
            scoreText.setString("Score: " + to_string(score));

            // ---------------------------------------------------------------------------
            
            //draw.
            window.clear(Color::White);
            window.draw(backgroundSprite);
            window.draw(groundSprite);
            window.draw(playerFrames[index]);
            for (const auto& tree : trees) {
                window.draw(tree);
            }
            window.draw(scoreText); // 점수 텍스트 출력

            window.display();
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
