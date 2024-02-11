#include <SFML/Graphics.hpp>

class TileMap : public sf::Drawable, public sf::Transformable {
public:
	TileMap() {

	}

	void create() {
		map_vertices.setPrimitiveType(sf::Quads);
		map_vertices.resize(sizeX * sizeY * 4);

		for (int i = 0; i < sizeY; i++) {
			for (int j = 0; j < sizeX; j++) {
				int tileNumber = levelWalls[i * sizeX + j];
				if (tileNumber == 0) continue;

				sf::Vertex* quad = &map_vertices[(i * sizeX + j) * 4];

				quad[0].position = sf::Vector2f(j * tileSize, i * tileSize);
				quad[1].position = sf::Vector2f((j + 1) * tileSize, i * tileSize + 0.5f);
				quad[2].position = sf::Vector2f((j + 1) * tileSize + 0.5f, (i + 1) * tileSize + 0.5f);
				quad[3].position = sf::Vector2f(j * tileSize + 0.5f, (i + 1) * tileSize);

				for (int k = 0; k < 4; k++) {
					quad[k].color = sf::Color::White;
					/*
					if (tileNumber == 1) {
						quad[k].color = sf::Color::White;
					}
					else if (tileNumber == 0) {
						quad[k].color = sf::Color(100, 107, 99);
					}
					*/
					 
				}
			}
		}
	}

	static int getTile(float positionX, float positionY) {
		int levelPosX = positionX / tileSize;	int levelPosY = positionY / tileSize;
		if (levelPosX + levelPosY * sizeX > sizeof(levelWalls) / sizeof(int))	return -1;
		return levelPosX + levelPosY * sizeX;
	}

	static int getWall(float positionX, float positionY) {
		int tile = getTile(positionX, positionY);
		if (tile == -1) { return 0; }
		return levelWalls[tile];
	}

	static int getFloor(float positionX, float positionY) {
		int tile = getTile(positionX, positionY);
		if (tile == -1) { return 0; }
		return levelFloor[tile];
	}


	static int tileSize;


	///*
	static int sizeX, sizeY;
	static int levelWalls[90];
	static int levelFloor[90];
	//*/

	/*
	const int sizeX = 6, sizeY = 5;
	const int level[30] =
	{
		1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 1, 1,
		1, 0, 1, 1, 0, 1,
		1, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1,
	};
	*/

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		//states.transform *= getTransform();

		// apply the tileset texture
		//states.texture = &m_tileset;

		// draw the vertex array
		target.draw(map_vertices);
	}


	sf::VertexArray map_vertices;
};