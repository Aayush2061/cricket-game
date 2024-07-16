#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Full Window Pitch");

    // Load texture for the splash screen
    sf::Texture splashTexture;
    if (!splashTexture.loadFromFile("poster.png"))
    {
        std::cerr << "Failed to load splash texture!" << std::endl;
        return -1;
    }

    // Create sprite for the splash screen
    sf::Sprite splashSprite;
    splashSprite.setTexture(splashTexture);

    // Scale the splash sprite to fit the window
    float splashScaleX = static_cast<float>(window.getSize().x) / splashTexture.getSize().x;
    float splashScaleY = static_cast<float>(window.getSize().y) / splashTexture.getSize().y;
    splashSprite.setScale(splashScaleX, splashScaleY);

    // Load music for the splash screen
    sf::Music splashMusic;
    if (!splashMusic.openFromFile("an.ogg"))
    {
        std::cerr << "Failed to load splash music!" << std::endl;
        // Continue without playing music if loading fails
    }
    else
    {
        splashMusic.play(); // Play the splash music
    }

    // Display the splash screen for 5 seconds
    sf::Clock splashClock;
    while (splashClock.getElapsedTime().asSeconds() < 5.0f)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(splashSprite);
        window.display();
    }

    // Stop and release resources for splash music
    splashMusic.stop();

    // Load texture for the pitch
    sf::Texture pitchTexture;
    if (!pitchTexture.loadFromFile("ground.jpg"))
    {
        std::cerr << "Failed to load pitch texture!" << std::endl;
        return -1;
    }

    // Create sprite for the pitch
    sf::Sprite pitchSprite;
    pitchSprite.setTexture(pitchTexture);

    // Scale the pitch sprite to fit the window
    float scaleX = static_cast<float>(window.getSize().x) / pitchTexture.getSize().x;
    float scaleY = static_cast<float>(window.getSize().y) / pitchTexture.getSize().y;
    pitchSprite.setScale(scaleX, scaleY);

    // Load textures for different batsmen positions
    sf::Texture batsmanStanceTexture, batsmanRunningTexture, batsmanBattingTexture, batsmanFieldingTexture;
    if (!batsmanStanceTexture.loadFromFile("lhb_stance.png") ||
        !batsmanRunningTexture.loadFromFile("lhb_ondrive.png") ||
        !batsmanBattingTexture.loadFromFile("lhb_cut.png") ||
        !batsmanFieldingTexture.loadFromFile("lhb_straightdrive.png"))
    {
        std::cerr << "Failed to load batsmen textures!" << std::endl;
        return -1;
    }

    // Create sprite for the batsmen with default texture
    sf::Sprite batsmenSprite;
    batsmenSprite.setTexture(batsmanStanceTexture); // Set default texture to stance
    batsmenSprite.setPosition(600, 10);             // Initial position of batsmen

    // Load texture for the ball
    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("cball.png"))
    {
        std::cerr << "Failed to load ball texture!" << std::endl;
        return -1;
    }

    // Create sprite for the ball
    sf::Sprite ballSprite;
    ballSprite.setTexture(ballTexture);
    ballSprite.setPosition(630, 880); // Initial position of the ball

    // Scale the ball sprite to make it larger
    ballSprite.setScale(1.5f, 1.5f); // Scale the ball sprite by a factor of 2

    // Ball velocity (initial)
    sf::Vector2f ballVelocity(0.0f, -0.1f); // Moves upwards with speed 0.5 pixel per frame

    // Random number generator setup
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Time variables for delay between bowls
    sf::Clock clock;
    sf::Time timeSinceLastBowling = sf::seconds(0.0f);
    sf::Time bowlingInterval = sf::seconds(2.0f); // Interval of 2 seconds between bowls

    // Game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Check for key press to change batsmen sprite
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::A: // Key A for running batsmen
                    batsmenSprite.setTexture(batsmanRunningTexture);
                    break;
                case sf::Keyboard::D: // Key S for batting batsmen
                    batsmenSprite.setTexture(batsmanBattingTexture);
                    break;
                case sf::Keyboard::W: // Key D for fielding batsmen
                    batsmenSprite.setTexture(batsmanFieldingTexture);
                    break;
                case sf::Keyboard::S: // Key W for resetting to default batsmen
                    batsmenSprite.setTexture(batsmanStanceTexture);
                    break;
                default:
                    break;
                }
            }
        }

        // Move the ball
        ballSprite.move(ballVelocity);

        // Check if ball has reached the top (y <= 10)
        if (ballSprite.getPosition().y <= 10)
        {
            // Reset ball to initial position (630, 880)
            ballSprite.setPosition(630, 880);

            // Add randomness to the ball's velocity
            float randomXVelocity = (std::rand() % 5 - 2) / 100.0f;      // Random x velocity between -0.02 and 0.02
            float randomYVelocity = -(0.1f + std::rand() % 10 / 100.0f); // Random y velocity between -0.1 and -0.19
            ballVelocity = sf::Vector2f(randomXVelocity, randomYVelocity);
        }

        window.clear();

        // Draw the pitch sprite (which now fills the window)
        window.draw(pitchSprite);

        // Draw the ball sprite on top of the pitch
        window.draw(ballSprite);

        // Draw the batsmen sprite on top of the pitch
        window.draw(batsmenSprite);

        window.display();
    }

    return 0;
}
