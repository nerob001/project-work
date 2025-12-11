#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <random>
#include <string>
#include <sstream>

struct Fruit {
    sf::CircleShape shape;
    float speed;
    bool isNegative;

    Fruit(float radius = 16.f)
        : shape(radius), speed(50.f), isNegative(false) {
    }
};

int main()
{
    const unsigned int WINDOW_W = 800;
    const unsigned int WINDOW_H = 600;

    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Catch The Circle");
    window.setFramerateLimit(60);

    
    sf::RectangleShape basket(sf::Vector2f(120.f, 20.f));
    basket.setFillColor(sf::Color(139, 69, 19)); 
    basket.setOrigin(basket.getSize().x / 2.f, basket.getSize().y / 2.f);
    basket.setPosition(WINDOW_W / 2.f, WINDOW_H - 40.f);

    
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> xdist(20.f, WINDOW_W - 20.f);
    std::uniform_real_distribution<float> sizeDist(10.f, 22.f);
    std::uniform_int_distribution<int> chanceDist(1, 100);

    std::vector<Fruit> fruits;

    float spawnTimer = 0.f;
    float spawnInterval = 1.0f;
    float timeElapsed = 0.f;
    int score = 0;
    int lives = 5;

    const float DIFFICULTY_INCREASE_EVERY = 10.f;

    
    sf::Font font;
    bool fontLoaded = false;
    if (font.loadFromFile("C:\\Users\\Lenovo\\source\\repos\\Game Project\\Game Project\\sfml font\\Paul-le1V.ttf"))
        fontLoaded = true;
    else
        printf("Failed to load font from path\n");

    sf::Text scoreText, livesText, gameOverText;
    if (fontLoaded) {
        scoreText.setFont(font); scoreText.setCharacterSize(20); scoreText.setPosition(10.f, 10.f);
        livesText.setFont(font); livesText.setCharacterSize(20); livesText.setPosition(10.f, 36.f);
        gameOverText.setFont(font); gameOverText.setCharacterSize(48);
        gameOverText.setOrigin(0.f, 0.f);
    }

   
    sf::SoundBuffer pointBuffer, negativeBuffer, gameOverBuffer;
    sf::Sound pointSound, negativeSound, gameOverSound;

    if (!pointBuffer.loadFromFile("point.wav")) printf("Failed to load point.wav\n");
    pointSound.setBuffer(pointBuffer);

    if (!negativeBuffer.loadFromFile("negative.wav")) printf("Failed to load negative.wav\n");
    negativeSound.setBuffer(negativeBuffer);

    if (!gameOverBuffer.loadFromFile("gameover.wav")) printf("Failed to load gameover.wav\n");
    gameOverSound.setBuffer(gameOverBuffer);

    sf::Clock clock;
    bool isGameOver = false;
    bool gameOverSoundPlayed = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            if (isGameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                fruits.clear();
                spawnInterval = 1.0f;
                spawnTimer = 0.f;
                timeElapsed = 0.f;
                score = 0;
                lives = 5;
                isGameOver = false;
                gameOverSoundPlayed = false;
            }
        }

        float dt = clock.restart().asSeconds();
        if (isGameOver) dt = 0.f;
        timeElapsed += dt;

        
        float basketSpeed = 450.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            basket.move(-basketSpeed * dt, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            basket.move(basketSpeed * dt, 0.f);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mpos = sf::Mouse::getPosition(window);
            basket.setPosition((float)mpos.x, basket.getPosition().y);
        }

        |
        if (basket.getPosition().x - basket.getSize().x / 2.f < 0.f)
            basket.setPosition(basket.getSize().x / 2.f, basket.getPosition().y);
        if (basket.getPosition().x + basket.getSize().x / 2.f > WINDOW_W)
            basket.setPosition(WINDOW_W - basket.getSize().x / 2.f, basket.getPosition().y);

        
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.f;
            Fruit f;
            float r = sizeDist(mt);
            f.shape.setRadius(r);
            f.shape.setOrigin(r, r);
            f.shape.setPosition(xdist(mt), -r - 5.f);

            int chance = chanceDist(mt);
            if (chance <= 15) { 
                f.isNegative = true;
                f.shape.setFillColor(sf::Color::White);
            }
            else { 
                f.isNegative = false;
                sf::Color col(rand() % 256, rand() % 256, rand() % 256);
                f.shape.setFillColor(col);
            }

            f.speed = 50.f; 
            fruits.push_back(std::move(f));
        }

        
        for (auto& f : fruits) f.speed += dt * 10.f;

        
        if (timeElapsed > 0.f) {
            float factor = 1.f + timeElapsed / DIFFICULTY_INCREASE_EVERY * 0.05f;
            spawnInterval = std::max(0.25f, 1.0f / factor);
        }

        
        for (auto it = fruits.begin(); it != fruits.end();) {
            it->shape.move(0.f, it->speed * dt);

            
            if (it->shape.getGlobalBounds().intersects(basket.getGlobalBounds())) {
                if (it->isNegative) {
                    score -= 10; 
                    if (score < 0) score = 0;
                    negativeSound.play();
                }
                else {
                    score += 10;
                    pointSound.play();
                }
                it = fruits.erase(it);
                continue;
            }

            
            if (it->shape.getPosition().y - it->shape.getRadius() > WINDOW_H) {
                if (!it->isNegative) { 
                    lives--;
                    if (lives <= 0) isGameOver = true;
                }
                it = fruits.erase(it);
                continue;
            }

            ++it;
        }

        
        if (fontLoaded) {
            scoreText.setString("Score: " + std::to_string(score));
            livesText.setString("Lives: " + std::to_string(lives));
            if (isGameOver) {
                gameOverText.setString("GAME OVER! Press R to restart");
                sf::FloatRect textRect = gameOverText.getLocalBounds();
                gameOverText.setOrigin(textRect.width / 2.f, textRect.height / 2.f);
                gameOverText.setPosition(WINDOW_W / 2.f, WINDOW_H / 2.f);

                if (!gameOverSoundPlayed) {
                    gameOverSound.play();
                    gameOverSoundPlayed = true;
                }
            }
        }

        
        window.clear(sf::Color::Black);
        window.draw(basket);
        for (auto& f : fruits) window.draw(f.shape);
        if (fontLoaded) {
            window.draw(scoreText);
            window.draw(livesText);
            if (isGameOver) window.draw(gameOverText);
        }
        window.display();
    }

    return 0;
}
