#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Paddle {
public:
    Paddle(float startX, float startY, float speed)
        : position(startX, startY), shape(sf::Vector2f(10, 80)), speed(speed) {
        shape.setFillColor(sf::Color::White);
        shape.setPosition(position);
    }

    void move(float deltaTime, bool up) {
        if (up && position.y > 0) {
            position.y -= speed * deltaTime;
        } else if (!up && position.y < 600 - shape.getSize().y) {
            position.y += speed * deltaTime;
        }
        shape.setPosition(position);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::FloatRect getBounds() {
        return shape.getGlobalBounds();
    }

private:
    sf::Vector2f position;
    sf::RectangleShape shape;
    float speed;
};

class Ball {
public:
    Ball(float startX, float startY, float speed)
        : position(startX, startY), velocity(speed, -speed), shape(10), speed(speed) {
        shape.setFillColor(sf::Color::White);
        shape.setPosition(position);
    }

    void update(float deltaTime) {
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
        shape.setPosition(position);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    void reset(float startX, float startY) {
        position.x = startX;
        position.y = startY;
        velocity.x = speed;
        velocity.y = -speed;
    }

    sf::FloatRect getBounds() {
        return shape.getGlobalBounds();
    }

    void bounceOffPaddle() {
        velocity.x = -velocity.x;
    }

    bool isOutOfBounds() {
        return position.x < 0 || position.x > 800;
    }

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::CircleShape shape;
    float speed;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong Game");
    sf::Clock clock;
    float deltaTime = 0.0f;

    Paddle paddle1(10, 260, 300.0f);
    Paddle paddle2(780, 260, 300.0f);
    Ball ball(400, 300, 300.0f);

    int player1Score = 0;
    int player2Score = 0;

    sf::Music backgroundMusic;
    if (backgroundMusic.openFromFile("background_music.mp3")) {
        backgroundMusic.setLoop(true);
        backgroundMusic.play();
    }

    sf::SoundBuffer collisionBuffer;
    if (collisionBuffer.loadFromFile("collision.mp3")) {
        sf::Sound collisionSound;
        collisionSound.setBuffer(collisionBuffer);

        while (window.isOpen()) {
            deltaTime = clock.restart().asSeconds();

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                paddle1.move(deltaTime, true);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                paddle1.move(deltaTime, false);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                paddle2.move(deltaTime, true);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                paddle2.move(deltaTime, false);
            }

            paddle1.draw(window);
            paddle2.draw(window);
            ball.update(deltaTime);

            if (ball.getBounds().intersects(paddle1.getBounds()) || ball.getBounds().intersects(paddle2.getBounds())) {
                ball.bounceOffPaddle();
                collisionSound.play();
            }

            if (ball.isOutOfBounds()) {
                if (ball.getBounds().left < 0) {
                    player2Score++;
                } else {
                    player1Score++;
                }
                ball.reset(400, 300);
            }

            if (player1Score >= 5) {
                window.close();
            }
			
            if (player2Score >= 5) {
                window.close();
            }


            window.clear();


            paddle1.draw(window);
            paddle2.draw(window);
            ball.draw(window);

            window.display();
        }
    }

    return 0;
}
