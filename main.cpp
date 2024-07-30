#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <cmath>   // For trigonometric functions
#include <string>  // For std::string
using namespace std;
// ball count
int ballCount = 0;

// Run count
int runCount = 0;

// Function to get formatted overs and balls as a string
std::string getOversString()
{
    int overs = ballCount / 6;
    int balls = ballCount % 6;
    return std::to_string(overs) + "." + std::to_string(balls);
}

// Function to generate a random number up to a specified maximum value
int generateRandomNumber(int max)

{

    return std::rand() % (max + 1);
}

// Function to set ball velocity with randomness based on angle

sf::Vector2f setBallVelocity(float angleDeg, float speed)

{

    float angleRad = angleDeg * 3.14159265f / 180.0f;

    return sf::Vector2f(speed * std::cos(angleRad), speed * std::sin(angleRad));
}

std::string generateRandomRun()
{
    // Generate a random number to decide the runs
    int runs = generateRandomNumber(3); // Random number between 0 and 3
    std::string runEvent;

    // Map the random number to runs
    switch (runs)
    {
    case 0:
        runEvent = "1 run";
        runCount += 1;
        break;
    case 1:
        runEvent = "2 runs";
        runCount += 2;
        break;
    case 2:
        runEvent = "4 runs";
        runCount += 4;
        break;
    case 3:
        runEvent = "6 runs";
        runCount += 6;
        break;
    }
    return runEvent;
}

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
    ballSprite.setScale(1.5f, 1.5f); // Scale the ball sprite by a factor of 1.5

    // Ball velocity (initial)
    sf::Vector2f ballVelocity(0.0f, -0.1f); // Moves upwards with speed 0.1 pixel per frame

    // Random number generator setup
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Time variables for delay between bowls
    sf::Clock clock;
    sf::Time timeSinceLastBowling = sf::seconds(0.0f);
    sf::Time bowlingInterval = sf::seconds(2.0f); // Interval of 2 seconds between bowls

    bool batSwinging = false;     // Flag to check if bat is swinging
    bool ballOutOfScreen = false; // Flag to check if ball is out of the screen

    // Function to set ball velocity with randomness based on angle
    auto setBallVelocity = [](float angleDeg, float speed) -> sf::Vector2f
    {
        float angleRad = angleDeg * 3.14159265f / 180.0f;
        return sf::Vector2f(speed * std::cos(angleRad), speed * std::sin(angleRad));
    };

    // Load font for displaying text
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }

    // Create text for displaying event
    sf::Text eventText;
    eventText.setFont(font);
    eventText.setCharacterSize(30); // Set character size
    eventText.setFillColor(sf::Color::White);
    eventText.setPosition(10, 10); // Position text in the top-left corner
    std::string currentEvent = ""; // Variable to store the current event

    // Create text for displaying overs
    sf::Text overText;
    overText.setFont(font);
    overText.setCharacterSize(30); // Set character size
    overText.setFillColor(sf::Color::White);
    overText.setPosition(10, 40); // Position text in the top-left corner

    // Game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Check for key press to change batsmen sprite and set bat swinging flag
            if (event.type == sf::Event::KeyPressed)
            {
                batSwinging = true;
                switch (event.key.code)
                {
                case sf::Keyboard::A: // Key A for running batsmen
                    batsmenSprite.setTexture(batsmanRunningTexture);
                    break;
                case sf::Keyboard::D: // Key D for batting batsmen
                    batsmenSprite.setTexture(batsmanBattingTexture);
                    break;
                case sf::Keyboard::W: // Key W for fielding batsmen
                    batsmenSprite.setTexture(batsmanFieldingTexture);
                    break;
                case sf::Keyboard::S: // Key S for resetting to default batsmen
                    batsmenSprite.setTexture(batsmanStanceTexture);
                    break;
                default:
                    break;
                }
            }
            if (event.type == sf::Event::KeyReleased)
            {
                batSwinging = false;
            }
        }

        // Move the ball
        ballSprite.move(ballVelocity);

        // Check if ball has reached the top (y <= 10) or left/right of the screen
        if (ballSprite.getPosition().y <= 10 || ballSprite.getPosition().x <= 0 || ballSprite.getPosition().x >= window.getSize().x)
        {
            ballOutOfScreen = true;
        }

        // Check for collision between the ball and the bat only when the bat is swinging
        if (batSwinging && ballSprite.getGlobalBounds().intersects(batsmenSprite.getGlobalBounds()))
        {
            // Determine the direction based on the current batsmenSprite texture
            if (batsmenSprite.getTexture() == &batsmanRunningTexture)
            {
                // Move ball to the left with randomness
                float randomAngle = 180.0f + static_cast<float>(std::rand() % 60 - 30); // Random angle between 150 and 210 degrees
                ballVelocity = setBallVelocity(randomAngle, 0.2f);                      // Speed of 0.2
                currentEvent = generateRandomRun();
            }
            else if (batsmenSprite.getTexture() == &batsmanBattingTexture)
            {
                // Move ball to the right with randomness
                float randomAngle = 0.0f + static_cast<float>(std::rand() % 60 - 30); // Random angle between -30 and 30 degrees
                ballVelocity = setBallVelocity(randomAngle, 0.2f);                    // Speed of 0.2
                currentEvent = generateRandomRun();
            }
            else if (batsmenSprite.getTexture() == &batsmanFieldingTexture)
            {
                // Move ball straight with randomness
                float randomAngle = -90.0f + static_cast<float>(std::rand() % 30 - 15); // Random angle between -105 and -75 degrees
                ballVelocity = setBallVelocity(randomAngle, 0.2f);                      // Speed of 0.2
                currentEvent = generateRandomRun();
            }
        }
        else
        {
            // Ball is out of screen or not hitting the bat
            if (ballSprite.getPosition().y <= 10)
            {
                if (ballSprite.getPosition().x <= 600 || ballSprite.getPosition().x >= window.getSize().x)
                {
                    // Check if the ball is out of screen and hasn't collided with the bat
                    currentEvent = "Wide ball";
                }
                else if (!batSwinging)
                {
                    currentEvent = "Dot ball";
                }
            }
        }

        // Redo balling after some time and only if the ball is out of the screen
        timeSinceLastBowling += clock.restart();
        if (timeSinceLastBowling >= bowlingInterval && ballOutOfScreen)
        {
            ballSprite.setPosition(630, 880);         // Reset ball to initial position
            timeSinceLastBowling = sf::seconds(0.0f); // Reset time since last bowl
            ballOutOfScreen = false;                  // Reset flag

            // Reset ball velocity with some randomness
            float randomXVelocity = (std::rand() % 5 - 2) / 100.0f;      // Random x velocity between -0.02 and 0.02
            float randomYVelocity = -(0.1f + std::rand() % 10 / 100.0f); // Random y velocity between -0.1 and -0.19
            ballVelocity = sf::Vector2f(randomXVelocity, randomYVelocity);
            if (currentEvent != "Wide ball")
            {
                ballCount++;
            }
        }

        window.clear();

        // Draw the pitch sprite (which now fills the window)
        window.draw(pitchSprite);

        // Draw the ball sprite on top of the pitch
        window.draw(ballSprite);

        // Draw the batsmen sprite on top of the pitch
        window.draw(batsmenSprite);

        // Update the event text
        eventText.setString("Event: " + currentEvent);

        // Update the overs text
        overText.setString("Overs: " + getOversString());

        // Draw the event text on top of everything
        window.draw(eventText);

        // Draw the overs text on the screen
        window.draw(overText);

        window.display();
    }

    return 0;
}