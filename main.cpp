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

// Wicket Count
int wicketCount = 0;

void resetGame()
{
    runCount = 0;
    ballCount = 0;
    wicketCount = 0;
    // Reset other game variables as needed
}

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
        // runCount += 1;
        break;
    case 1:
        runEvent = "2 runs";
        // runCount += 2;
        break;
    case 2:
        runEvent = "4 runs";
        // runCount += 4;
        break;
    case 3:
        runEvent = "6 runs";
        // runCount += 6;
        break;
    }
    return runEvent;
}

// Enumeration for the different game states
enum GameState
{
    MAIN_MENU,
    GAME,
    GUIDELINES,
    GAME_OVER

};

// Class for the main menu
class MainMenu
{
public:
    MainMenu(float width, float height)
    {
        if (!font.loadFromFile("arial.ttf"))
        {
            // handle error
        }

        menu[0].setFont(font);
        menu[0].setFillColor(sf::Color::Red);
        menu[0].setString("Play Now");
        menu[0].setPosition(sf::Vector2f(width / 2, height / (2 + 1) * 1));

        menu[1].setFont(font);
        menu[1].setFillColor(sf::Color::White);
        menu[1].setString("Guidelines");
        menu[1].setPosition(sf::Vector2f(width / 2, height / (2 + 1) * 2));

        selectedItemIndex = 0;
    }

    void draw(sf::RenderWindow &window)
    {
        for (int i = 0; i < 2; i++)
        {
            window.draw(menu[i]);
        }
    }

    void moveUp()
    {
        if (selectedItemIndex - 1 >= 0)
        {
            menu[selectedItemIndex].setFillColor(sf::Color::White);
            selectedItemIndex--;
            menu[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }

    void moveDown()
    {
        if (selectedItemIndex + 1 < 2)
        {
            menu[selectedItemIndex].setFillColor(sf::Color::White);
            selectedItemIndex++;
            menu[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }

    int getSelectedItemIndex()
    {
        return selectedItemIndex;
    }

private:
    int selectedItemIndex;
    sf::Font font;
    sf::Text menu[2];
};

/* -----------------------------------------------------------------------------FOR GAME OVER FEATURE-----------------------------------------------------------------------------------------*/
class GameOverScreen
{
public:
    GameOverScreen(float width, float height)
    {
        if (!font.loadFromFile("arial.ttf"))
        {
            // Handle error
        }

        menu[0].setFont(font);
        menu[0].setFillColor(sf::Color::Red);
        menu[0].setString("Play Again");
        menu[0].setPosition(sf::Vector2f(width / 2, height / (3 + 1) * 1));

        menu[1].setFont(font);
        menu[1].setFillColor(sf::Color::White);
        menu[1].setString("Exit");
        menu[1].setPosition(sf::Vector2f(width / 2, height / (3 + 1) * 2));

        selectedItemIndex = 0;
    }

    void draw(sf::RenderWindow &window)
    {
        for (int i = 0; i < 2; i++)
        {
            window.draw(menu[i]);
        }
    }

    void moveUp()
    {
        if (selectedItemIndex - 1 >= 0)
        {
            menu[selectedItemIndex].setFillColor(sf::Color::White);
            selectedItemIndex--;
            menu[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }

    void moveDown()
    {
        if (selectedItemIndex + 1 < 2)
        {
            menu[selectedItemIndex].setFillColor(sf::Color::White);
            selectedItemIndex++;
            menu[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }

    int getSelectedItemIndex()
    {
        return selectedItemIndex;
    }

private:
    int selectedItemIndex;
    sf::Font font;
    sf::Text menu[2];
};

void showGameOver(sf::RenderWindow &window, GameState &gameState)
{
    GameOverScreen gameOverScreen(window.getSize().x, window.getSize().y);
    bool keyPressed = false;

    while (window.isOpen() && gameState == GAME_OVER)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return; // Ensure we exit the function
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (!keyPressed) // Process the key press only if it hasn’t been processed yet
                {
                    keyPressed = true; // Set the flag to avoid handling the key press multiple times
                    if (event.key.code == sf::Keyboard::Up)
                    {
                        gameOverScreen.moveUp();
                    }
                    else if (event.key.code == sf::Keyboard::Down)
                    {
                        gameOverScreen.moveDown();
                    }
                    else if (event.key.code == sf::Keyboard::Return)
                    {
                        int selectedItem = gameOverScreen.getSelectedItemIndex();
                        if (selectedItem == 0)
                        {
                            resetGame();      // Ensure the game is properly reset
                            gameState = GAME; // Update the game state
                            return;           // Exit to restart the game
                        }
                        else if (selectedItem == 1)
                        {
                            gameState = MAIN_MENU; // Update the game state
                            return;                // Exit to return to the main menu
                        }
                    }
                }
            }
        }

        // Reset key press flag after processing events
        keyPressed = false;

        window.clear();
        gameOverScreen.draw(window);
        window.display();
    }
}

/* -----------------------------------------------------------------------------FOR GAME OVER FEATURE-------------------------------------------------*/

// Function to show the game scene
int showGame(sf::RenderWindow &window, GameState &gameState)
{
    // sf::RenderWindow window(sf::VideoMode(1200, 900), "Full Window Pitch");
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

    // Create text for displaying Runs
    sf::Text runText;
    runText.setFont(font);
    runText.setCharacterSize(30); // Set character size
    runText.setFillColor(sf::Color::White);
    runText.setPosition(10, 70); // Position text in the top-left corner

    // Create text for displaying Wickets
    sf::Text wicketText;
    wicketText.setFont(font);
    wicketText.setCharacterSize(30); // Set character size
    wicketText.setFillColor(sf::Color::White);
    wicketText.setPosition(10, 100); // Position text in the top-left corner

    // Game loop
    while (window.isOpen() && gameState == GAME)
    {

        sf::Event event;

        if (ballCount == 35 || wicketCount == 11)
        {
            // Load texture for the splash screen
            sf::Texture splashTexture;
            if (!splashTexture.loadFromFile("gameover.jpg"))
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

            // Enable alpha blending
            window.clear(sf::Color(0, 0, 0, 0)); // Clear with transparent color

            // Display the splash screen for 5 seconds
            sf::Clock splashClock;
            while (splashClock.getElapsedTime().asSeconds() < 1.0f)
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }

                window.clear(sf::Color(0, 0, 0, 0)); // Clear with transparent color
                window.draw(splashSprite);
                window.display();
            }
            // ball count
            int ballCount = 0;

            // Run count
            int runCount = 0;

            // Wicket Count
            int wicketCount = 0;

            gameState = GAME_OVER;
            return 0;

            // window.close();
            // exit(0);
        }
        // sf::Event event;
        while (window.pollEvent(event))
        {
            // Code to take back to main menu from game
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                gameState = MAIN_MENU; // Change state to return to Main Menu
                return 0;              // Exit the function to return to the main loop
            }
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

                if (ballSprite.getPosition().x > 700)
                {
                    currentEvent = "Dot ball";
                }
                else if (ballSprite.getPosition().x <= 600 || ballSprite.getPosition().x >= window.getSize().x)
                {
                    // Check if the ball is out of screen and hasn't collided with the bat
                    currentEvent = "Wide ball";
                }
                else if (ballSprite.getPosition().x >= 650 || ballSprite.getPosition().x <= 700)
                {
                    currentEvent = "Wicket";
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
            if (currentEvent == "Wicket")
            {
                wicketCount++;

                // Load texture for the splash screen
                sf::Texture splashTexture;
                if (!splashTexture.loadFromFile("out.jpg"))
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

                // Display the splash screen for 5 seconds
                sf::Clock splashClock;
                while (splashClock.getElapsedTime().asSeconds() < 2.0f)
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
            }
            if (currentEvent != "Wide ball")
            {
                // runCount++;
                ballCount++;
            }

            if (currentEvent == "Wide ball")
            {
                runCount++;

                // Load texture for the splash screen
                sf::Texture splashTexture;
                if (!splashTexture.loadFromFile("wide.jpg"))
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

                // Display the splash screen for 5 seconds
                sf::Clock splashClock;
                while (splashClock.getElapsedTime().asSeconds() < 2.0f)
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
            }
            if (currentEvent == "1 run")
            {
                runCount++;
            }

            if (currentEvent == "2 runs")
            {
                runCount += 2;
            }

            if (currentEvent == "4 runs")
            {
                runCount += 4;

                // Load textures for splash screens
                sf::Texture splashTexture1, splashTexture2;
                if (!splashTexture1.loadFromFile("four.jpg") || !splashTexture2.loadFromFile("cheerLeader.jpg"))
                {
                    std::cerr << "Failed to load splash textures!" << std::endl;
                    return -1;
                }

                // Create sprites for splash screens
                sf::Sprite splashSprite1(splashTexture1);
                sf::Sprite splashSprite2(splashTexture2);

                // Scale the splash sprites to fit the window
                float splashScaleX1 = static_cast<float>(window.getSize().x) / splashTexture1.getSize().x;
                float splashScaleY1 = static_cast<float>(window.getSize().y) / splashTexture1.getSize().y;
                splashSprite1.setScale(splashScaleX1, splashScaleY1);

                float splashScaleX2 = static_cast<float>(window.getSize().x) / splashTexture2.getSize().x;
                float splashScaleY2 = static_cast<float>(window.getSize().y) / splashTexture2.getSize().y;
                splashSprite2.setScale(splashScaleX2, splashScaleY2);

                // Load music for the splash screen
                sf::Music splashMusic;
                if (!splashMusic.openFromFile("cheerSound1.ogg"))
                {
                    std::cerr << "Failed to load splash music!" << std::endl;
                    // Continue without playing music if loading fails
                }
                else
                {
                    splashMusic.play(); // Play the splash music
                }

                // Display the first splash screen for 2 seconds
                sf::Clock splashClock;
                while (window.isOpen())
                {
                    sf::Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }

                    window.clear();
                    if (splashClock.getElapsedTime().asSeconds() < 2.0f)
                    {
                        window.draw(splashSprite1);
                    }
                    else if (splashClock.getElapsedTime().asSeconds() < 4.0f)
                    {
                        window.draw(splashSprite2);
                    }
                    else
                    {
                        // Break the loop and proceed to the main game
                        break;
                    }
                    window.display();
                }
                // Stop and release resources for splash music
                splashMusic.stop();
            }

            if (currentEvent == "6 runs")
            {
                runCount += 6;

                // Load textures for splash screens
                sf::Texture splashTexture1, splashTexture2;
                if (!splashTexture1.loadFromFile("six.jpg") || !splashTexture2.loadFromFile("cheerLeader.jpg"))
                {
                    std::cerr << "Failed to load splash textures!" << std::endl;
                    return -1;
                }

                // Create sprites for splash screens
                sf::Sprite splashSprite1(splashTexture1);
                sf::Sprite splashSprite2(splashTexture2);

                // Scale the splash sprites to fit the window
                float splashScaleX1 = static_cast<float>(window.getSize().x) / splashTexture1.getSize().x;
                float splashScaleY1 = static_cast<float>(window.getSize().y) / splashTexture1.getSize().y;
                splashSprite1.setScale(splashScaleX1, splashScaleY1);

                float splashScaleX2 = static_cast<float>(window.getSize().x) / splashTexture2.getSize().x;
                float splashScaleY2 = static_cast<float>(window.getSize().y) / splashTexture2.getSize().y;
                splashSprite2.setScale(splashScaleX2, splashScaleY2);

                // Load music for the splash screen
                sf::Music splashMusic;
                if (!splashMusic.openFromFile("cheerSound1.ogg"))
                {
                    std::cerr << "Failed to load splash music!" << std::endl;
                    // Continue without playing music if loading fails
                }
                else
                {
                    splashMusic.play(); // Play the splash music
                }
                // Display the first splash screen for 2 seconds
                sf::Clock splashClock;
                while (window.isOpen())
                {
                    sf::Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }

                    window.clear();
                    if (splashClock.getElapsedTime().asSeconds() < 2.0f)
                    {
                        window.draw(splashSprite1);
                    }
                    else if (splashClock.getElapsedTime().asSeconds() < 4.0f)
                    {
                        window.draw(splashSprite2);
                    }
                    else
                    {
                        // Break the loop and proceed to the main game
                        break;
                    }
                    window.display();
                }
                // Stop and release resources for splash music
                splashMusic.stop();
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

        // Update the runs text
        runText.setString("Runs: " + std::to_string(runCount));

        // Update the runs text
        wicketText.setString("Wickets: " + std::to_string(wicketCount));

        // Draw the event text on top of everything
        window.draw(eventText);

        // Draw the overs text on the screen
        window.draw(overText);

        // Draw the overs text on the screen
        window.draw(runText);

        // Draw the overs text on the screen
        window.draw(wicketText);

        window.display();
    }
}

void showGuidelines(sf::RenderWindow &window, GameState &gameState)
{
    // Define the guidelines
    const std::string guidelines[] = {
        "Guidelines for Cricket Game:",
        "1. Press 'A' for leg shots.",
        "2. Press 'D' for off shots.",
        "3. Press 'W' for straight shots.",
        "4. The run, over, wicket, and previous ball event are displayed",
        "   at the top left of the game screen.",
        "5. Press 'Esc' to go back to the Main Menu."};

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        // Handle error
    }

    sf::Text guidelinesText;
    guidelinesText.setFont(font);
    guidelinesText.setFillColor(sf::Color::White);
    guidelinesText.setCharacterSize(24); // Increased font size for better readability

    float xOffset = 50; // Adjusted to fit within window width
    float yOffset = 50; // Adjusted to fit within window height
    window.clear();
    for (const auto &line : guidelines)
    {
        guidelinesText.setString(line);
        guidelinesText.setPosition(xOffset, yOffset);
        window.draw(guidelinesText);
        yOffset += 40; // Increased spacing between lines for readability
    }
    window.display();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                gameState = MAIN_MENU; // Change state to return to Main Menu
                return;                // Exit the function to return to the main loop
            }
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "SFML Cricket Game");

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

    GameState gameState = MAIN_MENU;
    MainMenu mainMenu(window.getSize().x, window.getSize().y);
    GameOverScreen gameOverScreen(window.getSize().x, window.getSize().y);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0; // Exit the application
            }

            if (gameState == MAIN_MENU)
            {
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::Up)
                    {
                        mainMenu.moveUp();
                    }
                    else if (event.key.code == sf::Keyboard::Down)
                    {
                        mainMenu.moveDown();
                    }
                    else if (event.key.code == sf::Keyboard::Return)
                    {
                        int selectedItem = mainMenu.getSelectedItemIndex();
                        if (selectedItem == 0)
                        {
                            gameState = GAME;
                            resetGame();                 // Ensure the game is properly reset
                            showGame(window, gameState); // Pass the gameState to showGame
                            gameState = MAIN_MENU;       // Ensure we return to the main menu state
                        }
                        else if (selectedItem == 1)
                        {
                            gameState = GUIDELINES;
                            showGuidelines(window, gameState); // Pass the gameState to showGuidelines
                            gameState = MAIN_MENU;             // Ensure we return to the main menu state
                        }
                    }
                }
            }
            else if (gameState == GAME_OVER)
            {
                showGameOver(window, gameState);
                if (gameState == MAIN_MENU)
                {
                    // Reinitialize the main menu
                    mainMenu = MainMenu(window.getSize().x, window.getSize().y);
                }
            }
        }

        window.clear();
        if (gameState == MAIN_MENU)
        {
            mainMenu.draw(window);
        }
        window.display();
    }

    return 0;
}
