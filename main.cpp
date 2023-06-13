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

int getRandomNumber(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}


bool checkCollisionCircle(const sf::CircleShape& player, const sf::CircleShape& obstacle) {
    sf::Vector2f playerPosition = player.getPosition();
    sf::Vector2f obstaclePosition = obstacle.getPosition();

    // 플레이어와 장애물의 중심점 사이의 거리를 계산합니다.
    float distance = std::sqrt(std::pow(playerPosition.x - obstaclePosition.x, 2) + std::pow(playerPosition.y - obstaclePosition.y, 2));

    // 플레이어와 장애물의 반지름을 계산합니다.
    float playerRadius = player.getRadius()/2;
    float obstacleRadius = obstacle.getRadius()/2;

    // 플레이어와 장애물의 중심점 사이의 거리가 플레이어와 장애물의 반지름 합보다 작으면 충돌이 발생한 것으로 간주합니다.
    return distance < playerRadius + (obstacleRadius/4);
}

// spriteToCircle 함수 정의
sf::CircleShape spriteToCircle(const sf::Sprite& sprite) {
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    sf::Vector2f spritePosition = sprite.getPosition();
    float spriteRadius = std::min(spriteBounds.width, spriteBounds.height) / 8.0f;

    sf::CircleShape circle(spriteRadius);
    circle.setOrigin(spriteRadius, spriteRadius);
    circle.setPosition(spritePosition);

    return circle;
}

// spritesToCircles 함수 정의
std::vector<sf::CircleShape> spritesToCircles(const std::vector<sf::Sprite>& sprites) {
    std::vector<sf::CircleShape> circles;
    circles.reserve(sprites.size());

    for (const auto& sprite : sprites) {
        circles.push_back(spriteToCircle(sprite));
    }

    return circles;
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
        //sprite.setScale(1.0f, 1.0f); // 크기 조절
        playerFrames.push_back(sprite);
    }

    static const int PLAYER_Y_BOTTOM = Height - textures[0].getSize().y - 40; // 바닥 위치

    Position playerPos;
    playerPos.x = 100;
    playerPos.y = PLAYER_Y_BOTTOM;

    // 프레임에 따라 이미지 변경을 위한 변수들
    int index = 0; // 이미지 인덱스
    float frame = 0.f;
    float frameSpeed = 1.0f; // 이미지 변경 속도
    const int changeCount = 8; // 몇 프레임마다 변경할지 (수정된 부분)

    // ---------------------------------------------------------------------------

    const int gravity = 4;    //중력. 점프할때 사용
    bool isJumping = false;    //점프 중인지
    bool isBottom = true;    //바닥에 발이 닿았는지

    // ---------------------------------------------------------------------------



    vector<Texture> treeTextures;
    vector<string> treeImages = { "images/s1.png", "images/s2.png", "images/s3.png" };
    for (const auto& image : treeImages) {
        Texture texture;
        if (!texture.loadFromFile(image)) {
            cerr << "Failed to load tree image: " << image << endl;
            return -1;
        }
        treeTextures.push_back(texture);
    }

    vector<Sprite> trees;
    for (const auto& texture : treeTextures) {
        Sprite tree(texture);
        //tree.setScale(1.0f, 1.0f);
        trees.push_back(tree);
    }

    const int obstacleWidth = treeTextures[0].getSize().x * 0.3f;
    const int obstacleHeight = treeTextures[0].getSize().y;

    vector<Position> treePositions;
    for (int i = 0; i < 10; i++) {
        Position position;
        position.x = Width + i * getRandomNumber(300, 800);
        position.y = PLAYER_Y_BOTTOM - obstacleHeight;
        treePositions.push_back(position);
    }



    Clock clock;

    // ---------------------------------------------------------------------------

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
            // 함수 선언
            sf::CircleShape spriteToCircle(const sf::Sprite & sprite);
            std::vector<sf::CircleShape> spritesToCircles(const std::vector<sf::Sprite>&sprites);
            bool checkCollisionCircle(const sf::CircleShape & player, const sf::CircleShape & obstacle);

            // ...

            // Move trees
            for (int i = 0; i < trees.size(); i++) {
                trees[i].setPosition(treePositions[i].x, treePositions[i].y + 52);
                treePositions[i].x -= 6;

                if (treePositions[i].x < -obstacleWidth) {
                    treePositions[i].x = Width + getRandomNumber(300, 800);
                }

                // Check collision
                if (checkCollisionCircle(spriteToCircle(playerFrames[index]), spriteToCircle(trees[i]))) {
                    // Handle collision (e.g. game over)
                    end++;
                    cout << "Game over!" << endl;
                    // ...
                }
            }

            // ...

            


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
            //tree.setPosition(treePos.x, treePos.y);
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
