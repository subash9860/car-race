#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int width = 1024;
int height = 768;
int roadW = 2000;
int segL = 200;    //segment length
float camD = 0.84; //camera depth

// for darawing road
void drawQuad(RenderWindow &w, Color c, int x1, int y1, int w1, int x2, int y2, int w2)
{
    ConvexShape shape(4);
    shape.setFillColor(c);
    shape.setPoint(0, Vector2f(x1 - w1, y1));
    shape.setPoint(1, Vector2f(x2 - w2, y2));
    shape.setPoint(2, Vector2f(x2 + w2, y2));
    shape.setPoint(3, Vector2f(x1 + w1, y1));
    w.draw(shape);
}

struct Line
{
    float x, y, z;              //3d center of line
    float X, Y, W;              //screen coord
    float spriteX, clip, scale; // curve,
    Sprite sprite;

    Line()
    {
        spriteX = x = y = z = 0; // = curve
    }

    // for scaling according to window size
    void project(int camX, int camY, int camZ)
    {
        scale = camD / (z - camZ);
        X = (1 + scale * (x - camX)) * width / 2;
        Y = (1 - scale * (y - camY)) * height / 2;
        W = scale * roadW * width / 2;
    }

    void drawSprite(RenderWindow &window)
    {
        Sprite s = sprite;
        int w = s.getTextureRect().width;
        int h = s.getTextureRect().height;

        float destX = X + scale * spriteX * width / 2;
        float destY = Y + 4;
        float destW = w * W / 266;
        float destH = h * W / 266;

        destX += destW * spriteX; //offsetX
        destY += destH * (-1);    //offsetY

        float clipH = destY + destH - clip;
        if (clipH < 0)
            clipH = 0;

        if (clipH >= destH)
            return;
        s.setTextureRect(IntRect(0, 0, w, h - h * clipH / destH));
        s.setScale(destW / w, destH / h);
        s.setPosition(destX, destY);
        window.draw(s);
    }
};

int main()
{
    RenderWindow window(VideoMode(width, height), "Car Race");
    window.setFramerateLimit(60);

    // drawing object in the side of road
    Texture t[50];
    Sprite object[50];
    for (int i = 1; i <= 8; i++)
    {
        t[i].loadFromFile("images/" + to_string(i) + ".png");
        t[i].setSmooth(true);
        object[i].setTexture(t[i]);
    }

    // background image
    Texture bg;
    bg.loadFromFile("images/bg3.png");
    bg.setRepeated(true);
    Sprite sBackground(bg);
    sBackground.setTextureRect(IntRect(0, 0, 5000, 411));
    sBackground.setPosition(-2000, 0);
    // sBackground.setPosition(0, 0);

    Texture texture;
    texture.loadFromFile("images/car.png");

    RectangleShape Rectcar(Vector2f(200, 150));
    Rectcar.setPosition(320, 470);
    Rectcar.setRotation(1);
    Rectcar.setTexture(&texture);

    std::vector<Line> lines;

    for (int i = 0; i < 1600; i++)
    {
        Line line;
        line.z = i * segL;

        // if (i > 300 && i < 700)
        //     line.curve = 0.5;
        // if (i > 1100)
        //     line.curve = -0.7;

        if (i < 300 && i % 20 == 0)
        {
            line.spriteX = -2.5;
            line.sprite = object[5];
        }
        if (i % 17 == 0)
        {
            line.spriteX = 0.6;
            line.sprite = object[6];
        }
        if (i % 45 == 0)
        {
            line.spriteX = 0.4;
            line.sprite = object[5];
        }
        if (i % 19 == 0)
        {
            line.spriteX = -3.7;
            line.sprite = object[6];
        }
        if (i > 300 && i % 20 == 0)
        {
            line.spriteX = -0.7;
            line.sprite = object[4];
        }
        if (i > 800 && i % 20 == 0)
        {
            line.spriteX = -1.2;
            line.sprite = object[1];
        }
        if (i == 400)
        {
            line.spriteX = -1.3;
            line.sprite = object[7];
        }

        if (i % 127 == 0)
        {
            line.spriteX = 0;
            line.sprite.setTextureRect(IntRect(0, 0, 100, 50));
            line.sprite = object[8];
            // collision detection
            if (line.sprite.getGlobalBounds().intersects(Rectcar.getGlobalBounds()))
            {
                cout << "Collision" << endl;
            }
        }

        if (i % 329 == 0)
        {
            line.spriteX = -1.1;
            line.sprite.setTextureRect(IntRect(0, 0, 100, 50));
            line.sprite = object[8];

            // collision detection
            if (line.sprite.getGlobalBounds().intersects(Rectcar.getGlobalBounds()))
            {
                cout << "Collision" << endl;
            }
        }
        //     line.y = sin(i / 30.0) * 1500;

        lines.push_back(line);
    }

    int N = lines.size();
    float playerX = 0;
    int pos = 0;
    int H = 1500;

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        int speed = 0;

        if (Keyboard::isKeyPressed(Keyboard::Right))
            playerX += 0.05;
        if (Keyboard::isKeyPressed(Keyboard::Left))
            playerX -= 0.05;
        if (Keyboard::isKeyPressed(Keyboard::Up))
            speed = 100;
        // if (Keyboard::isKeyPressed(Keyboard::Down))
        // speed = -200;
        // if (Keyboard::isKeyPressed(Keyboard::Tab))
        // speed *= 3;
        // if (Keyboard::isKeyPressed(Keyboard::W))
        // H += 100;
        // if (Keyboard::isKeyPressed(Keyboard::S))
        // H -= 100;

        pos += speed;
        while (pos >= N * segL)
            pos -= N * segL;
        while (pos < 0)
            pos += N * segL;

        window.clear(Color(105, 205, 4));
        window.draw(sBackground);
        int startPos = pos / segL;
        int camH = lines[startPos].y + H;
        // if (speed > 0)
        //     sBackground.move(-lines[startPos].curve * 2, 0);
        // if (speed < 0)
        //     sBackground.move(lines[startPos].curve * 2, 0);

        int maxy = height;
        float x = 0, dx = 0;

        ///////draw road////////
        for (int n = startPos; n < startPos + 300; n++)
        {
            Line &l = lines[n % N];
            l.project(playerX * roadW - x, camH, startPos * segL - (n >= N ? N * segL : 0));
            x += dx;
            // dx += l.curve;

            l.clip = maxy;
            if (l.Y >= maxy)
                continue;
            maxy = l.Y;

            Color grass = (n / 3) % 2 ? Color(16, 200, 16) : Color(0, 154, 0);
            Color roadSide = (n / 3) % 2 ? Color(255, 255, 255) : Color(0, 0, 0);
            Color roadMid = (n / 3) % 2 ? Color(107, 107, 107) : Color(255, 255, 0);
            Color road = (n / 3) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);

            Line p = lines[(n - 1) % N]; //previous line

            drawQuad(window, grass, 0, p.Y, width, 0, l.Y, width);
            drawQuad(window, roadSide, p.X, p.Y, p.W * 1.2, l.X, l.Y, l.W * 1.2);
            drawQuad(window, road, p.X, p.Y, p.W, l.X, l.Y, l.W);
            drawQuad(window, roadMid, p.X, p.Y, p.W * 0.04, l.X, l.Y, l.W * 0.04);
        }

        ////////draw objects////////
        for (int n = startPos + 300; n > startPos; n--)
        {
            lines[n % N].drawSprite(window);
        }

        window.draw(Rectcar);

        

        // for sprite collision
        // if(Rectcar.getGlobalBounds().intersects(lines[].sprite.getGlobalBounds()))
        // {
        //     cout << "collision" << endl;
        // }

        window.display();
    }

    return 0;
}

// Texture texture;
// texture.loadFromFile("bb.png");

// RectangleShape Rectcar(Vector2f(200, 150));
// Rectcar.setPosition(320, 470);
// Rectcar.setRotation(1);
// Rectcar.setTexture(&texture);

//window.draw(Rectcar);
