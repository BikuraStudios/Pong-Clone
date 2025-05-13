#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>



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

    //Create a paddle: width 20, height 150
    sf::RectangleShape paddle2({ 20.f,150.f });
    paddle2.setFillColor(sf::Color::White);      // Color the paddle white
    paddle2.setPosition({ 725.f,250.f }); 	    // Position the paddle at (50, 250)   

  

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
		const float maxSpeed = 500.f; // Maximum speed for the ball


        //Check the paddle's position
        //std::cout << "Paddle Position: " << paddle.getPosition().y << std::endl;
        //check the balls's position
        //std::cout << "Ball Position: " << ball.getPosition().x << ", " << ball.getPosition().y << std::endl;
        std::cout << "Ball Speed: " << ballVelocity.x << ", " << ballVelocity.y << std::endl;


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

        // Movement input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            paddle2.move({ 0.f, -paddleSpeed * deltaTime });
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            paddle2.move({ 0.f, paddleSpeed * deltaTime });
        }
        // Stop the paddle from going out of bounds
        if (paddle2.getPosition().y < 0.f)
            paddle2.setPosition({ paddle2.getPosition().x, 0.f });
        else if (paddle2.getPosition().y + paddle2.getSize().y > window.getSize().y)
            paddle2.setPosition({ paddle2.getPosition().x, window.getSize().y - paddle2.getSize().y });

        // Bounce the ball off the walls
        // ball speed increases by 3% when it hits the top or bottom of the window
        if (ball.getPosition().y < 0.f)                                                                // I think this makes the ball bounce?
        {
            
            //ballVelocity.y *= -1.03f;
			ballVelocity.y *= -2.0f;              // Speed test                                          
        }
        ballVelocity.x = std::clamp(ballVelocity.x, -maxSpeed, maxSpeed);
        ballVelocity.y = std::clamp(ballVelocity.y, -maxSpeed, maxSpeed);
        if (ball.getPosition().y > window.getSize().y - ball.getRadius() * 2.f)                        // confirmed, the ball bounces of the top and bottom of the window
        {
            
            //ballVelocity.y *= -1.03f;
            ballVelocity.y *= -2.0f;              // Speed test
        }                                                  
        ballVelocity.x = std::clamp(ballVelocity.x, -maxSpeed, maxSpeed);
        ballVelocity.y = std::clamp(ballVelocity.y, -maxSpeed, maxSpeed);

        // For now, the ball bounces off the left and right, but eventually these colisions will be for scoring
        if (ball.getPosition().x < 0.f)
            ballVelocity.x *= -1.0f;                                             // ball bounces off the left side of the window
        if (ball.getPosition().x > window.getSize().x - ball.getRadius() * 2.f)
            ballVelocity.x *= -1.0f;                                                      // ball bounces off the right side of the window

        // Ball overshoot correction
		
        const float overShootTop = window.getSize().y - ball.getRadius() * 2.f;       // The top of the window minus the ball's radius
		const float overShootBottom = window.getSize().y - ball.getRadius() * 2.f;    // The bottom of the window minus the ball's radius
		const float overShootLeft = window.getSize().x - ball.getRadius() * 2.f;     // The left of the window minus the ball's radius
		const float overShootRight = window.getSize().x - ball.getRadius() * 2.f;    // The right of the window minus the ball's radius

        if (ball.getPosition().y < 0.f)
            ball.setPosition(sf::Vector2f(ball.getPosition().x, 0.f));
        else if (ball.getPosition().y > overShootTop)
            ball.setPosition(sf::Vector2f(ball.getPosition().x, overShootTop));

        if (ball.getPosition().x < 0.f)
            ball.setPosition(sf::Vector2f(0.f, ball.getPosition().y));
        else if (ball.getPosition().x > overShootLeft)
            ball.setPosition(sf::Vector2f(overShootLeft, ball.getPosition().y));
       
        

        window.clear(sf::Color::Black);         // Clear the screen

        window.draw(paddle);                    // Draw the paddle
		window.draw(paddle2);                   // Draw the paddle
        window.draw(ball);                      // Draw the ball      [Additional note, I spent 5 minutes wondering why the ball wasn't showing up only to realize this command is necessary]
        window.display();                       // Show the drawn frame
    }

    return 0;
   

}
