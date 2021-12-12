#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(500, 500), "this is title");

    const unsigned int kwidth = 5, kheight = 5;

    // array size = width *height *4(RBGA)

    Uint8 pixels[kwidth * kheight * 4] = {
        255, 255, 255, 255, // white
        0, 0, 0, 255,       // black
        255, 0, 0, 255,     // Red
        128, 128, 128, 255, // grey

        //.. all other pixel
    };

    Image image;
    if (!image.loadFromFile("mg.jpg"))
        return -1;
    image.create(kheight, kwidth, pixels);

    Texture texture;
    texture.loadFromFile("mg.jpg");
    // texture.loadFromImage(image);
    // if ( !texture.loadFromFile("mg.jpg"), sf::IntRect(0,0,32,32))
    // return -1;

    RectangleShape rectangleShape(Vector2f(300, 150));
    rectangleShape.setTexture(&texture);

    Vector2u textureSize = texture.getSize();
    float rectWidth = static_cast<float>(textureSize.x);
    float rectheight = static_cast<float>(textureSize.y);
    RectangleShape rectShape(Vector2f(rectWidth,rectheight));
    rectShape.setTexture(&texture);
    

    while (window.isOpen())
    {
        Event event;                    // check the event
        while (window.pollEvent(event)) // Events can be polled from the window
        {
            switch (event.type)
            {
            // check event type wheather it is close or not
            case Event::EventType ::Closed:
                window.close();
                break;
            }
        }

        window.clear(Color::Black);

        window.draw(rectangleShape);

        window.display();
    }
    return 0;
}

// g++ texture.cpp -o texture -lsfml-graphics -lsfml-window -lsfml-system