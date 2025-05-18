#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <string>
#include <SFML/Audio.hpp>

int main()
{
    // Create the window with size 800x600 and title "Pong"
   // float resX = 800.f;
    //float resY = 600.f;
    sf::RenderWindow window(sf::VideoMode({ 800,600 }), "Pong");

    

    //Load sound files into memory
    sf::SoundBuffer buffer_pause("pause.wav");
    sf::SoundBuffer buffer_goal("goal.wav");
    sf::SoundBuffer buffer_match("match.wav");
    sf::SoundBuffer buffer_leftPaddle("leftPaddle.wav");
    sf::SoundBuffer buffer_rightPaddle("rightPaddle.wav");
    sf::SoundBuffer buffer_unpause("unpause.wav");
    sf::SoundBuffer buffer_wallHit("wallHit.wav");

    // sound
    sf::Sound sound_pause(buffer_pause);
    sf::Sound sound_goal(buffer_goal);
    sf::Sound sound_match(buffer_match);
    sf::Sound sound_leftPaddle(buffer_leftPaddle);
    sf::Sound sound_rightPaddle(buffer_rightPaddle);
    sf::Sound sound_unpause(buffer_unpause);
    sf::Sound sound_wallHit(buffer_wallHit);
    

    //color pallettes courtessy of lospec.com
    

    struct ColorPalette {
        sf::Color background;
        sf::Color ball;
        sf::Color paddle;
        sf::Color text;
    };
    //bkr (Bikura) color pallete
    static const ColorPalette bkrPalette = {
        sf::Color(6, 45, 96),
        sf::Color(199, 202, 206),
        sf::Color(132, 136, 140),
        sf::Color(244, 244, 244)
    };

    // DMG gameboy theme
    static const ColorPalette dmgPalette = {
        sf::Color(27, 37, 61),
        sf::Color(154, 173, 92),
        sf::Color(55, 122, 100),
        sf::Color(227, 226, 213)
    };
    // Off black and white theme (newsprint)
    static const ColorPalette bnwPalette = {
        sf::Color(46,38,34),
        sf::Color(204, 206, 199),
        sf::Color(160, 159, 151),
        sf::Color(119, 116, 111)
    };
    // Gooseberry thistle (purple and blue) theme
    static const ColorPalette gbtPalette = {
        sf::Color(72, 55, 108),
        sf::Color(184, 131, 185),
        sf::Color(108, 86, 171),
        sf::Color(191, 220, 225)
    };



    
    // Font

    sf::Font font("PressStart2P.ttf");
    int player1Score = 0;
    std::string player1ScoreString = std::to_string(player1Score);
    int player2Score = 0;
    std::string player2ScoreString = std::to_string(player2Score);
    // Left score text
    sf::Text scoreTextLeft(font);
    scoreTextLeft.setCharacterSize(40);
    scoreTextLeft.setFillColor(bkrPalette.text);
    scoreTextLeft.setPosition({ 200.f, 20.f });
    scoreTextLeft.setString(player2ScoreString);

    // Right score text
    sf::Text scoreTextRight(font);
    scoreTextRight.setCharacterSize(40);
    scoreTextRight.setFillColor(bkrPalette.text);
    scoreTextRight.setPosition({ 580.f, 20.f });
    scoreTextRight.setString(player2ScoreString);

    //Tells who the winning player is
    sf::Text winText(font);
    winText.setCharacterSize(45);
    winText.setFillColor(bkrPalette.text);
    winText.setPosition({ 100.f, 175.f });
    winText.setString("PLAYER 1 WINS");

    sf::Text winText2(font);
    winText2.setCharacterSize(45);
    winText2.setFillColor(bkrPalette.text);
    winText2.setPosition({ 100.f, 175.f });
    winText2.setString("PLAYER 2 WINS");


    sf::Text pauseText(font);
    pauseText.setCharacterSize(50);
    pauseText.setFillColor(bkrPalette.text);
    pauseText.setPosition({ 275.f, 250.f });
    pauseText.setString("PAUSE");

    sf::Text titleText(font);
    titleText.setCharacterSize(30);
    titleText.setFillColor(bkrPalette.text);
    titleText.setPosition({ 100.f, 175.f });
    titleText.setString("PRESS ENTER TO START");


    sf::Text titleEndText(font);
    titleEndText.setCharacterSize(25);
    titleEndText.setFillColor(bkrPalette.text);
    titleEndText.setPosition({ 120.f, 450.f });
    titleEndText.setString("PRESS ENTER TO RESTART");


    //Create a paddle: width 20, height 150
    sf::RectangleShape paddle({ 20.f,150.f });
    paddle.setFillColor(bkrPalette.paddle);      // Color the paddle white
    paddle.setPosition({ 50.f,250.f }); 	    // Position the paddle at (50, 250)   

    // Paddle speed set to 300 pixels per second
    const float paddleSpeed = 300.f;

    //Create a paddle: width 20, height 150
    sf::RectangleShape paddle2({ 20.f,150.f });
    paddle2.setFillColor(bkrPalette.paddle);      // Color the paddle white
    paddle2.setPosition({ 725.f,250.f }); 	    // Position the paddle at (50, 250)   



    // Create a ball: Radius 10
    sf::CircleShape ball(10.f);
    ball.setFillColor(bkrPalette.ball);        // Color the ball white
    ball.setPosition({ 400.f, 300.f });         // Position the ball in the center of the window

    // how fast the ball moves (200 pixels per second)
    sf::Vector2f ballVelocity(200.f, 200.f);    // Moves diagonally

    // Clock used to track frame time
    sf::Clock clock;

    // Game state flags
    bool Pause{ true };
    bool matchEnd{ false };
    bool titleScreen{ true };
    bool startNewGame = false;
    bool playMatch = true;

    if (((titleScreen == true) && player1Score >= 10) || ((titleScreen == true) && (player2Score >= 10)))
    {
        player1Score = 0;
        player2Score = 0;
        matchEnd = false;
    }
	//Ball starting velocity randomizer?
    //float iniBallvelocity = abs((rand() % 400 + 200));

    //sf::Vector2f initialBallVelocity(iniBallvelocity, iniBallvelocity);

    //ballVelocity = initialBallVelocity;
    

    // Game loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
                
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
                {
                    sound_pause.play();
                    Pause = !Pause;
                }
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
                {
                    if (titleScreen || matchEnd) {
                        startNewGame = true;
                    }
                    Pause = false;
                    titleScreen = false;
                    matchEnd = false;
                }
                    

            }
            if (player1Score >= 10)
            {

                matchEnd = true;
                Pause = true;
                if (playMatch == true)
                {
                    sound_match.play();
                    playMatch = false;
                }
                
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
                        titleScreen = true;
                        playMatch = true;
                }

            }
            if (player2Score >= 10)
            {

                matchEnd = true;
                Pause = true;
                if (playMatch == true)
                {
                    sound_match.play();
                    playMatch = false;
                }
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
                        titleScreen = true;
                        playMatch = true;
                }
            }

                
        }
        //Pause check

        //std::cout << Pause << " \n";

        

        // With the correct way to check for a key press event:
        /* if (bool sf::Event::KeyReleased::Space)
            Pause = true;
        }
                
        if (Pause == true)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                Pause = false;
                */

        // Get the time elapsed since last frame
        float deltaTime = clock.restart().asSeconds();

        if (startNewGame) {
            player1Score = 0;
            player2Score = 0;
            player1ScoreString = std::to_string(player1Score);
            player2ScoreString = std::to_string(player2Score);
            scoreTextLeft.setString(player1ScoreString);
            scoreTextRight.setString(player2ScoreString);

            // Reset ball and paddle positions
            ball.setPosition({ 400.f, 300.f });
            paddle.setPosition({ 50.f,250.f });
            paddle2.setPosition({ 725.f,250.f });

            // Reset velocity
            ballVelocity = { 200.f, 200.f };

            startNewGame = false;
        }

        

        // Move the ball based on its velocity and deltaTime
        if (Pause == false)
           ball.move(ballVelocity * deltaTime);

        //Maxspeed variable
          const float maxSpeed = 500.f; // Maximum speed for the ball
        
        // AABB collision detection
            // first, we create the bounding box variables
        sf::FloatRect leftPaddleBounds = paddle.getGlobalBounds();
        sf::FloatRect rightPaddleBounds = paddle2.getGlobalBounds();
        sf::FloatRect ballBounds = ball.getGlobalBounds();
        // then we check for collisions
        if (const std::optional intersection = ballBounds.findIntersection(leftPaddleBounds))
        {
            // Collision detected
            //std::cout << "Collision detected!" << std::endl;
            sound_leftPaddle.play();
            ballVelocity.x *= -1.1f; // Reverse the ball's x velocity
        }
        if (const std::optional intersection = ballBounds.findIntersection(rightPaddleBounds))
        {
            // Collision detected
            //std::cout << "Collision detected!" << std::endl;
            sound_rightPaddle.play();
            ballVelocity.x *= -1.1f; // Reverse the ball's x velocity
        }

        //Check the paddle's position
        //std::cout << "Paddle Position: " << paddle.getPosition().y << std::endl;
        //check the balls's position
        //std::cout << "Ball Position: " << ball.getPosition().x << ", " << ball.getPosition().y << std::endl;
        //std::cout << "Ball Speed: " << ballVelocity.x << ", " << ballVelocity.y << std::endl;

      

            // Movement input
        if (Pause == false)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                paddle.move({ 0.f, -paddleSpeed * deltaTime });
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            {
                paddle.move({ 0.f, paddleSpeed * deltaTime });
            }
        }
        // Stop the paddle from going out of bounds
        if (paddle.getPosition().y < 0.f)
        {
            sound_wallHit.play();
            paddle.setPosition({ paddle.getPosition().x, 0.f });
        }
        else if (paddle.getPosition().y + paddle.getSize().y > window.getSize().y)
        {
            sound_wallHit.play();
            paddle.setPosition({ paddle.getPosition().x, window.getSize().y - paddle.getSize().y });
        }
        // Movement input
        if (Pause == false)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            {
                paddle2.move({ 0.f, -paddleSpeed * deltaTime });
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
                paddle2.move({ 0.f, paddleSpeed * deltaTime });
            }
        }
        // Stop the paddle from going out of bounds
        if (paddle2.getPosition().y < 0.f)
        {
            sound_wallHit.play();
            paddle2.setPosition({ paddle2.getPosition().x, 0.f });
        }
        else if (paddle2.getPosition().y + paddle2.getSize().y > window.getSize().y)
        {
            sound_wallHit.play();
            paddle2.setPosition({ paddle2.getPosition().x, window.getSize().y - paddle2.getSize().y });
         }
        // Bounce the ball off the walls
        // ball speed increases by 3% when it hits the top or bottom of the window
        if (ball.getPosition().y < 0.f)                                                                // I think this makes the ball bounce?
        {
            sound_wallHit.play();
            ballVelocity.y *= -1.03f;
            //ballVelocity.y *= -2.0f;              // Speed test                                          
        }
        ballVelocity.x = std::clamp(ballVelocity.x, -maxSpeed, maxSpeed);
        ballVelocity.y = std::clamp(ballVelocity.y, -maxSpeed, maxSpeed);
        if (ball.getPosition().y > window.getSize().y - ball.getRadius() * 2.f)                        // confirmed, the ball bounces of the top and bottom of the window
        {
            sound_wallHit.play();
            ballVelocity.y *= -1.03f;
            //ballVelocity.y *= -2.0f;              // Speed test
        }
        ballVelocity.x = std::clamp(ballVelocity.x, -maxSpeed, maxSpeed);
        ballVelocity.y = std::clamp(ballVelocity.y, -maxSpeed, maxSpeed);

        
        if (ball.getPosition().x < 0.f)                      // enters left goal
        {
            sound_goal.play();
            player2Score += 1;
            ball.setPosition({ 400.f, 300.f });
            ballVelocity = { 200.f, 200.f };
            player2ScoreString = std::to_string(player2Score);
            scoreTextLeft.setString(player2ScoreString);
        }

        if (ball.getPosition().x > window.getSize().x - ball.getRadius() * 2.f)
        {
            sound_goal.play();                                                    // ball enters right goal
            player1Score += 1;
            ball.setPosition({ 400.f, 300.f });
            ballVelocity = { 200.f, 200.f };
			player1ScoreString = std::to_string(player1Score);
			scoreTextRight.setString(player1ScoreString);
        }

        

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



        window.clear(bkrPalette.background);         // Clear the screen
        if (titleScreen == true)
            window.draw(titleText);
        if ((Pause == true) && (matchEnd == false) && (titleScreen == false))
                window.draw(pauseText);             // When the game is paused, draw PAUSE in the middle of the screen as long as the game has not finished
        if (matchEnd == true)
        {
            if (player1Score >= 10)
            {
                window.draw(winText);
                window.draw(titleEndText);
            }
            if (player2Score >= 10)
            {
                window.draw(winText2);
                window.draw(titleEndText);
            }
                
        }
       
            
		window.draw(scoreTextLeft);               // Draw the left score
		window.draw(scoreTextRight);              // Draw the right score
        window.draw(paddle);                    // Draw the paddle
        window.draw(paddle2);                   // Draw the paddle
        window.draw(ball);                      // Draw the ball      [Additional note, I spent 5 minutes wondering why the ball wasn't showing up only to realize this command is necessary]
        //window.draw(titleEndText);              // comment out after testing
        window.display();                       // Show the drawn frame
        
    }

    return 0;
    

}
