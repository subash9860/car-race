#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

int width = 1024;
int height = 768;
int roadW = 2000;
int segL = 200;    //segment length
float camD = 0.84; //camera depth
FloatRect sprite_bounds;

// for darawing quadilateral for road
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

void drawGameOverState(RenderWindow &w)
{
    Font f;
    Text game_over;
    FloatRect gameRect = game_over.getLocalBounds();

    f.loadFromFile("Fonts/raider.ttf");
    game_over.setFont(f);
    game_over.setOrigin(gameRect.left + gameRect.width / 2.0f, gameRect.top + gameRect.height / 2.0f);
    game_over.setPosition(width / 2 - 400, height / 2 - 200);
    game_over.setFillColor(Color(155, 34, 48));
    game_over.setString("GAME OVER\n");
    game_over.setCharacterSize(100);
    w.draw(game_over);
}

struct Line
{
    float x, y, z;              //3d center of line
    float X, Y, W;              //screen coord
    float spriteX, clip, scale; // curve,
    Sprite sprite, opCar;

    FloatRect getSpriteBounds()
    {
        return sprite_bounds;
    }

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

    void CarSprite(RenderWindow &window)
    {
        Sprite s = opCar;
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

        sprite_bounds = s.getGlobalBounds();

        window.draw(s);
    }
};

int main()
{
    RenderWindow window(VideoMode(width, height), "Car Race");
    window.setFramerateLimit(60);

    SoundBuffer buffer;
    if (!buffer.loadFromFile("sound.wav"))
        return -1; // error

    Sound sound;
    sound.setBuffer(buffer);
    sound.setLoop(true);

    Music music;
    if (!music.openFromFile("game_over.wav"))
        return -1;

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
            line.opCar = object[8];
        }

        if (i % 329 == 0)
        {
            line.spriteX = -1.1;
            line.sprite.setTextureRect(IntRect(0, 0, 100, 50));
            line.opCar = object[8];
        }

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
            // if (Event::EventType::KeyPressed)
            //     if (e.key.code == Keyboard::Up)
            //         sound.play();
        }

        int speed = 0;

        if (Keyboard::isKeyPressed(Keyboard::Right))
            playerX += 0.04;
        if (Keyboard::isKeyPressed(Keyboard::Left))
            playerX -= 0.04;
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            speed = 100;
            sound.play();
        }
        if (Event::EventType::KeyReleased && e.key.code == Keyboard::Up)
            sound.pause();

        pos += speed;
        while (pos >= N * segL)
            pos -= N * segL;
        while (pos < 0)
            pos += N * segL;

        window.clear(Color(105, 205, 4));
        window.draw(sBackground);
        int startPos = pos / segL;
        int camH = lines[startPos].y + H;

        int maxy = height;
        float x = 0, dx = 0;

        ///////draw road////////
        for (int n = startPos; n < startPos + 300; n++)
        {
            Line &l = lines[n % N];
            l.project(playerX * roadW - x, camH, startPos * segL - (n >= N ? N * segL : 0));
            x += dx;

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

        for (int n = startPos + 300; n > startPos; n--)
        {
            lines[n % N].CarSprite(window);
        }

        window.draw(Rectcar);

        // for  collision with opponent car
        if (lines[startPos].getSpriteBounds().intersects(Rectcar.getGlobalBounds()))
        {
            sound.~Sound();
            window.clear();
            Texture over;
            over.loadFromFile("images/over.png");

            RectangleShape overgame(Vector2f(width, height));
            overgame.setTexture(&over);

            Font f, d;
            Text game_over;
            Text doneBY;
            f.loadFromFile("Fonts/raider.ttf");
            game_over.setFont(f);
            game_over.setPosition(width / 2 - 250, height / 2 - 210);
            game_over.setFillColor(Color(155, 34, 48));
            game_over.setString("GAME OVER\n");
            game_over.setCharacterSize(100);

            d.loadFromFile("Fonts/OpenSans.ttf");
            doneBY.setFont(d);
            doneBY.setString("Project by: Subash Kc");
            doneBY.setCharacterSize(80);
            doneBY.setPosition(50, height / 2 + 200);
            doneBY.setFillColor(Color(255, 255, 255));

            sound.stop();
            music.play();
            window.draw(overgame);
            window.draw(game_over);
            window.draw(doneBY);
        }

        window.display();
    }

    return 0;
}

// g++ game.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio