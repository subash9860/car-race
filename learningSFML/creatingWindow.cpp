#include <SFML/Graphics.hpp>

using namespace sf;

void initShape(RectangleShape &shape, Vector2f const &pos, Color const &color)
{
    shape.setFillColor(color);
    shape.setPosition(pos);
    shape.setOrigin(shape.getSize() * 0.5f);
}

int main()
{
    RenderWindow window(VideoMode(500, 500), "this is title");
    // buffer to store enter text
    // String buffer;
    // set traget Frame per second;
    window.setFramerateLimit(60);

    // for controlling moving of rectangle by key press
    // bool moving = false;

    // // shape Drawing circle
    // CircleShape circleShape(50);
    // circleShape.setFillColor(Color::Red);
    // circleShape.setOutlineColor(Color::White);
    // circleShape.setOutlineThickness(3);

    // RectangleShape rectangleShape(Vector2f(50, 50));
    // rectangleShape.setFillColor(Color::Green);
    // // shape tranformation
    // rectangleShape.setPosition(Vector2f(50, 150));
    // rectangleShape.setRotation(30);
    // rectangleShape.setScale(Vector2f(2, 1));

    // ConvexShape triangle;
    // triangle.setPointCount(3);
    // triangle.setPoint(0, Vector2f(100, 0));
    // triangle.setPoint(1, Vector2f(100, 100));
    // triangle.setPoint(2, Vector2f(0, 100));
    // triangle.setPosition(Vector2f(100, 100));
    // triangle.setFillColor(Color::Magenta);

    // initialize the initshape

    Vector2f startPos = Vector2f(50, 50);
    RectangleShape playerRect(Vector2f(50, 50));
    initShape(playerRect, startPos, Color::Green);

    RectangleShape targetRect(Vector2f(50, 50));
    initShape(targetRect, startPos, Color::Blue);

    RectangleShape badRect(Vector2f(50, 50));
    initShape(badRect, startPos, Color::Red);

    // Game loop
    while (window.isOpen())
    {
        /* game loop stages
        handle input
        update frame 
        render frame
        */
        Event event;                    // check the event
        while (window.pollEvent(event)) // Events can be polled from the window
        {
            switch (event.type)
            {
            // check event type wheather it is close or not
            case Event::EventType ::Closed:
                window.close();
                break;

                // case Event::EventType ::Resized:
                //     // window.setSize();
                //     break;

                // case Event::EventType ::KeyPressed:
                //     // change the title if the space is pressed;
                //     if (event.key.code == sf::Keyboard::Key::Space)
                //         window.setTitle("space pressed");

                //     // else if (event.key.code == sf::Keyboard::Key::Right)
                //     // moving = true;

                // case Event::EventType ::KeyReleased:
                //     // change the title again if the space is released;
                //     if (event.key.code == sf::Keyboard::Key::Space)
                //         window.setTitle("space released");

                //     // else if (event.key.code == sf::Keyboard::Key::Right)
                //     // moving = false;

                //     else if (event.key.code == sf::Keyboard::Key::Escape)
                //         window.close();

                // else if (event.key.code == Keyboard::Key::Return)
                // {
                // window.setTitle(buffer);
                // buffer.clear();
                // }
                // break;

                // case Event::EventType ::TextEntered:
                // add the charater directly to string
                // buffer += event.text.unicode;
                // break;

            case Event::EventType ::KeyReleased:
                playerRect.move(0, 1);
                if (event.key.code == Keyboard::Up)
                    playerRect.move(0, -1);
 
                // target reached . you win . exit game
                if (playerRect.getGlobalBounds().intersects(targetRect.getGlobalBounds()))
                    window.close();

                // bad square intersect. you lose restart

                if (playerRect.getGlobalBounds().intersects(badRect.getGlobalBounds()))
                    playerRect.setPosition(startPos);

            default:
                break;
            }
        }

        // update frame
        playerRect.move(1, 0);
        // if (moving)
        // {
        // if (Keyboard::isKeyPressed(Keyboard::Key::Right))
        // {
        //  move object
        // always moving right

        // }

        // }

        // rectangleShape.rotate(1.3f);
        // rectangleShape.move(Vector2f(1, 1));

        // render cycle
        window.clear(Color::Blue);
        // render object
        window.draw(playerRect);
        window.draw(targetRect);
        window.draw(badRect);
        // window.draw(circleShape);
        // window.draw(rectangleShape);
        // window.draw(triangle);

        window.display();
    }

    return 0;
}

// gcc sfm.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
//./sfml-app