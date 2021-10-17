#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <SFML/Window.hpp>
#include <math.h>
#include <cmath>                                         // для функции cos
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>

#define PI 3.14159265     

using namespace std;

const int FPS = 120;
float x = 0;
int width = 1280;
int height = 720;
int radius = 15;
float speed;
const float g = 9.8;
float y = height - radius * 2;
int angle = 60;
float t = 0;
const float start_pos = x;
int r = rand() % (width - 200) + 200;

sf::RenderWindow App(sf::VideoMode(width, height), "Physics Ball");
sf::CircleShape Rocket(radius);
sf::CircleShape Target(radius);
sf::Font font;
sf::Vertex line[2];
sf::Vertex MouseLine[2];
vector<vector<float>> Cordinates;

int target_x = width - r, target_y = height - 2 * radius;
bool isClicked = false;

float CalcAngle(sf::Vector2i MousePos){
    return atan((y - MousePos.y) / (MousePos.x - x)) * 180 / PI;
}

float CalcSpeed(float distance)
{   
    return sqrt(distance * g / sin(2 * angle * PI / 180));
}

void DrawText(string Text, int x, int y){
    sf::Text text;
    text.setFont(font); 
    text.setString(Text);
    text.setCharacterSize(15); 
    text.setFillColor(sf::Color::White);  
    text.setPosition(x,y);
    App.draw(text);   
}

void Setup(){
    App.setFramerateLimit(FPS);
    Target.setFillColor(sf::Color::Red);
    width = App.getSize().x;
    height = App.getSize().y;
    x = 0;
    y = height - radius * 2;
    t = 0;
    angle = 60;
    Cordinates.clear();
    int r = rand() % (width - 200) + 200;
    target_x = width - r;
    MouseLine[0].position = sf::Vector2f(radius,height);
}

void Fire(){
    speed = CalcSpeed((target_x + radius) - start_pos);
    y += g;
    t+=0.1;
    x = speed * cos(angle * PI / 180) * t;
    y = height - speed * sin(angle * PI / 180) * t + ((g * pow(t, 2)) / 2);

    sf::Vector2i MousePos = sf::Mouse::getPosition();

    vector<float>Cord = {x + radius,y + radius};

    Cordinates.push_back(Cord);

    if(x > width - 2*radius || y > height)
    {
        Setup();
        isClicked = false;
    }
}

void Update(){
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        isClicked = true;
    }

    if(isClicked)
    {
        Fire();
    }
    else
    {
        sf::Vector2i MousePos = sf::Mouse::getPosition(App);
        MouseLine[1].position = sf::Vector2f(MousePos.x, MousePos.y);
        angle=CalcAngle(MousePos);      
    }
    DrawText("Angle: " + to_string(angle), 0, 0);
    DrawText("Speed: " + to_string(speed), 0, 20);
    DrawText("Distance: " + to_string((target_x + radius) - start_pos), 0, 40);
}

void Render(){    
    Target.setPosition(target_x, target_y);
    App.draw(Target);
    if(isClicked)
    {
        App.draw(Rocket);

        for(int i = 0; i < Cordinates.size() - 1; i++)
        {
            line[0].position = sf::Vector2f(Cordinates[i][0], Cordinates[i][1]);
            line[1].position = sf::Vector2f(Cordinates[i + 1][0], Cordinates[i + 1][1]);
            App.draw(line, 2, sf::PrimitiveType::LinesStrip);            
        }
        Rocket.setPosition(x, y);
    }
    else
    {
        App.draw(MouseLine, 2, sf::PrimitiveType::LinesStrip);
    }
    App.display();
}

int main(){ 
    if (!font.loadFromFile("arial.ttf"))
    {
        cout << "Something Wrong With Font!" << endl;
    }
    srand(time(0));
    Setup();
    // Start game loop
    while (App.isOpen())
    {
        // Process events
        sf::Event Event;
        while (App.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                App.close();
        }
        App.clear();
        Update();
        Render();
    }
}