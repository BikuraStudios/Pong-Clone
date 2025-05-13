#include <SFML/Graphics.hpp>
#include <iostream>


int main()
{
    // Create the window with size 800x600 and title "Pong"
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Pong");

    //Create a paddle: width 20, height 150
    sf::RectangleShape paddle({ 20.f,150.f });
    paddle.setFillColor(sf::Color::White);      // Color the paddle white
    paddle.setPosition({ 50.f,250.f }); 	    // Position the paddle at (50, 250)   

    // Paddle speed set to 300 pixels per second
    const float paddleSpeed = 300.f;

    // Create a ball: Radius 10
    sf::CircleShape ball(10.f);
    ball.setFillColor(sf::Color::Blue);        // Color the ball white
    ball.setPosition({ 400.f, 300.f });         // Position the ball in the center of the window

    // how fast the ball moves (200 pixels per second)
    sf::Vector2f ballVelocity(200.f, 200.f);    // Moves diagonally

    // Clock used to track frame time
    sf::Clock clock;

    // Game loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Get the time elapsed since last frame
        float deltaTime = clock.restart().asSeconds();

        // Move the ball based on its velocity and deltaTime
        ball.move(ballVelocity * deltaTime);

        //Check the paddle's position
        //std::cout << "Paddle Position: " << paddle.getPosition().y << std::endl;
        //check the balls's position
        std::cout << "Ball Position: " << ball.getPosition().x << ", " << ball.getPosition().y << std::endl;



        // Movement input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            paddle.move({ 0.f, -paddleSpeed * deltaTime });
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            paddle.move({ 0.f, paddleSpeed * deltaTime });
        }
        // Stop the paddle from going out of bounds
        if (paddle.getPosition().y < 0.f)
            paddle.setPosition({ paddle.getPosition().x, 0.f });
        else if (paddle.getPosition().y + paddle.getSize().y > window.getSize().y)
            paddle.setPosition({ paddle.getPosition().x, window.getSize().y - paddle.getSize().y });

        // stop the ball from going out of bounds
        if (ball.getPosition().y < 0.f)
            ballVelocity.y = -ballVelocity.y;                                       // I think this makes the ball bounce?
        if (ball.getPosition().y > window.getSize().y - ball.getRadius() * 2.f)
            ballVelocity.y = -ballVelocity.y;                                       // confirmed, the ball bounces of the top and bottom of the window
        if (ball.getPosition().x < 0.f)
            ballVelocity.x = -ballVelocity.x;                                       // ball bounces off the left side of the window
        if (ball.getPosition().x > window.getSize().x - ball.getRadius() * 2.f)
            ballVelocity.x = -ballVelocity.x;                                       // ball bounces off the right side of the window

        window.clear(sf::Color::Black);         // Clear the screen

        //window.draw(paddle);                    // Draw the paddle
        window.draw(ball);                      // Draw the ball      [Additional note, I spent 5 minutes wondering why the ball wasn't showing up only to realize this command is necessary]
        window.display();                       // Show the drawn frame
    }

    return 0;
}