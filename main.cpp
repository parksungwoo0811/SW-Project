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


vector<Texture> textures;
for(int=1;i<=8;i++) {
    Texture texture;
    stringstream ss;
    ss << "images/r" << i <<".png";
    texture.loadFromFile(ss.str());
    textures.push_back(texture);
}

int main(void)
{
    Texture t1;
    Texture t2;
}

//이 커밋은 12:56에 만들어짐
