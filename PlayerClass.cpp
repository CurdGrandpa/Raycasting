#include <SFML/Graphics.hpp>
# define M_PI           3.14159265358979323846
#include <cmath>

#include "MapClass.cpp"

class Ray {
public:
    Ray() {
    }

    void castRay(sf::Vector2f pos0, float angle) {
        rayBody[0] = sf::Vertex(pos0);

        float posX = cos(angle * M_PI / 180) * rayMaxLength, posY = sin(angle * M_PI / 180) * rayMaxLength;

        // Initialising variables needed to check collision with OX axis
        float oxDeltaX, oxDeltaY, oxLen;
        float oxStepSizeY, oxStepSizeX;
        if (sin(angle * M_PI / 180) == 0) oxLen = rayMaxLength;
        else {
            if (angle < 180) {
                oxDeltaY = TileMap::tileSize - fmod(pos0.y, TileMap::tileSize);
                oxStepSizeY = TileMap::tileSize;
            }
            else {
                oxDeltaY = -fmod(pos0.y, TileMap::tileSize) - 0.001f;
                oxStepSizeY = -TileMap::tileSize;
            }

            if (cos(angle * M_PI / 180) == 0) {
                oxDeltaX = 0; 
                oxStepSizeX = 0;
            }
            else {
                oxDeltaX = 1 / tanf(angle * M_PI / 180) * oxDeltaY;//
                oxStepSizeX = 1 / tanf(angle * M_PI / 180) * oxStepSizeY;//
            }
            oxLen = sqrt(pow(oxDeltaX, 2) + pow(oxDeltaY, 2));
        }


        // Initialising variables needed to check collision with OX axis
        float oyDeltaX, oyDeltaY, oyLen;
        float oyStepSizeY, oyStepSizeX;
        if (cos(angle * M_PI / 180) == 0) oyLen = rayMaxLength;
        else {
            if (cos(angle * M_PI / 180) > 0) {
                oyDeltaX = TileMap::tileSize - fmod(pos0.x, TileMap::tileSize);
                oyStepSizeX = TileMap::tileSize;
            }
            else {
                oyDeltaX = -fmod(pos0.x, TileMap::tileSize) - 0.001f;
                oyStepSizeX = -TileMap::tileSize;
            }

            if (sin(angle * M_PI / 180) == 0) {
                oyDeltaY = 0;
                oyStepSizeY = 0;
            }
            else {
                oyDeltaY = tanf(angle * M_PI / 180) * oyDeltaX;//
                oyStepSizeY = tanf(angle * M_PI / 180) * oyStepSizeX;//
            }
            oyLen = sqrt(pow(oyDeltaX, 2) + pow(oyDeltaY, 2));
        }

        int obstacleType = 0;


        // Casting ray
        while (oxLen < rayMaxLength || oyLen < rayMaxLength) {
            if (oxLen < oyLen) {
                obstacleType = TileMap::getWall(pos0.x + oxDeltaX, pos0.y + oxDeltaY);
                if (obstacleType != 0) {
                    posX = oxDeltaX; posY = oxDeltaY;
                    rayLength = oxLen;
                    axisContact = 'x';
                    break;
                }
                oxDeltaY += oxStepSizeY;    oxDeltaX += oxStepSizeX;
                oxLen = sqrt(pow(oxDeltaX, 2) + pow(oxDeltaY, 2));
            }
            else {
                obstacleType = TileMap::getWall(pos0.x + oyDeltaX, pos0.y + oyDeltaY);
                if (obstacleType != 0) {
                    posX = oyDeltaX; posY = oyDeltaY;
                    rayLength = oyLen;
                    axisContact = 'y';
                    break;
                }
                oyDeltaY += oyStepSizeY;    oyDeltaX += oyStepSizeX;
                oyLen = sqrt(pow(oyDeltaX, 2) + pow(oyDeltaY, 2));
            }
        }


        rayBody[1] = sf::Vertex(rayBody[0].position + sf::Vector2f(posX, posY));
        this->obstacleType = obstacleType;
        if (obstacleType == 0) {
            rayLength = rayMaxLength;
            axisContact = ' ';
        }

        rayBody[0].color = sf::Color::Red;
        rayBody[1].color = sf::Color::Red;

    }

    sf::Vertex rayBody[2];

    float rayLength;
    int obstacleType;
    char axisContact;

    static float rayMaxLength;

    static float getMaxLen() { return rayMaxLength; };
};


class Player : public sf::Drawable, public sf::Transformable {
public:
    Player() {
        body = sf::RectangleShape(sf::Vector2f(sideLength, sideLength));
        body.setFillColor(sf::Color::Green);
        body.setOrigin(sideLength / 2, sideLength / 2);
        body.setPosition(TileMap::tileSize, TileMap::tileSize);
    };


    sf::RectangleShape body;
    Ray rays[180]; // Rays that player uses for looking at objects (not less than two, please!)



    void action(sf::Event event) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            float   xMove = cos(body.getRotation() * M_PI / 180) * runningSpeed,
                    yMove = sin(body.getRotation() * M_PI / 180) * runningSpeed;
            if (TileMap::getWall(xMove + body.getPosition().x, body.getPosition().y) > 0) { xMove = 0.f; }
            if (TileMap::getWall(body.getPosition().x, yMove + body.getPosition().y) > 0) { yMove = 0.f; }

            body.move(xMove, yMove);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            float   xMove = -cos(body.getRotation() * M_PI / 180) * runningSpeed,
                    yMove = -sin(body.getRotation() * M_PI / 180) * runningSpeed;
            if (TileMap::getWall(xMove + body.getPosition().x, body.getPosition().y) > 0) { xMove = 0.f; }
            if (TileMap::getWall(body.getPosition().x, yMove + body.getPosition().y) > 0) { yMove = 0.f; }

            body.move(xMove, yMove);
            //body.move(-cos(body.getRotation() * M_PI / 180) * runningSpeed, -sin(body.getRotation() * M_PI / 180) * runningSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            body.rotate(turningSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            body.rotate(-turningSpeed);
        }
    }

    void lookAround() {
        int raysNumber = sizeof(rays) / sizeof(Ray);
        if (raysNumber == 1) {
            rays[0].castRay(body.getPosition(), body.getRotation());
            return;
        }


        float angleStep = (float)fieldOfView / (raysNumber - 1);

        int i = 0;
        for (i; i < sizeof(rays) / sizeof(Ray); i++) {
            float rayAngle = fmod(360 + body.getRotation() - fieldOfView / 2 + i * angleStep, 360);
            rays[i].castRay(body.getPosition(), rayAngle);
            rays[i].rayLength *= fabs(cos((i * angleStep - fieldOfView / 2) * M_PI / 180));
        }
    }


private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        //states.transform *= getTransform();

        // apply the tileset texture
        //states.texture = &m_tileset;

        // draw player and rays
        target.draw(body);
        for (int i = 0; i < sizeof(rays) / sizeof(Ray); i++) {
            target.draw(rays[i].rayBody, 2, sf::Lines);
        }

        int windowWidthStart = 400;
        int windowHeigth = 500;
        float wallWidth = 600 / (sizeof(rays) / sizeof(Ray));
        float fadeÑoefficient = 160 / Ray::getMaxLen();

        // draw walls based on casted rays
        for (int i = 0; i < sizeof(rays) / sizeof(Ray); i++) {
            if (rays[i].obstacleType == 1) {
                sf::RectangleShape wallPiece;
                wallPiece = sf::RectangleShape(sf::Vector2f(wallWidth, windowHeigth / rays[i].rayLength * 20));
                if (rays[i].axisContact == 'x') { wallPiece.setFillColor(sf::Color::Color(230.f - rays[i].rayLength * fadeÑoefficient, 0, 0)); }
                else { wallPiece.setFillColor(sf::Color::Color(160 - rays[i].rayLength * fadeÑoefficient, 0, 0)); }
                //wallPiece.setOrigin(windowWidthStart + i * wallWidth, sideLength / 2);
                wallPiece.setPosition(windowWidthStart + i * wallWidth, (windowHeigth - wallPiece.getSize().y) / 2);
                target.draw(wallPiece);
            }
            if (rays[i].obstacleType == 0) {
                sf::RectangleShape wallPiece;
                wallPiece = sf::RectangleShape(sf::Vector2f(wallWidth, windowHeigth / rays[i].rayLength * 20));
                wallPiece.setFillColor(sf::Color::Black);
                //wallPiece.setOrigin(windowWidthStart + i * wallWidth, sideLength / 2);
                wallPiece.setPosition(windowWidthStart + i * wallWidth, (windowHeigth - wallPiece.getSize().y) / 2);
                target.draw(wallPiece);
            }
        }

    }
    float runningSpeed = 1.f;
    float turningSpeed = 2.5f;
    const float sideLength = 16.f;

    // We will have a 60 degrees field of view
    int fieldOfView = 60;
};