//#pragma once
//
//#include "stdafx.h"
//#include "AStar.h"
//#include "Wall.h"
//#include "Player.h"
//#include "Worker.h"
//
//class SweeperBoid {
//public:
//	SweeperBoid();
//	SweeperBoid(NodeLayout &nodes, SDL_Rect* player, int pathNo);
//
//	void render();
//
//	void update();
//
//	void chooseTarget();
//
//	void seek(float deltaTime, sf::Vector2f v, float dist, bool seekingWorker);
//
//	void checkWallCollisions(Wall* wall, float deltaTime);
//
//	void chooseBehaviour();
//
//	void checkBulletCollision(Projectile* p);
//
//	void setupPatrol(int pathNo);
//
//	void patrol(float deltaTime);
//
//	void setupSeekPath();
//
//	void abduct(float deltaTime);
//
//	void setupReturnPath();
//	void returnToPatrol(float deltaTime);
//
//	void setupFleePath(float minDist);
//	void flee(float deltaTime, sf::Vector2f v);
//
//	void normalise(sf::Vector2f &v);
//	float calculateMagnitude(sf::Vector2f v);
//	float calculateMagnitude(sf::Vector2f v1, sf::Vector2f v2);
//
//	bool getAlive();
//	std::vector<int> getIndexesOfAbducted();
//
//private:
//	enum Behaviour { PATROL, FLEE, ABDUCT, RETURN };
//	Behaviour m_behaviour;
//
//	sf::Vector2f m_pos;
//	sf::Vector2f m_nextPosX;
//	sf::Vector2f m_nextPosY;
//
//	float m_width;
//	float m_height;
//
//	sf::Vector2f m_vel;
//	float m_maxSpeed;
//	sf::Vector2f m_accel;
//	float m_magnitudeAccel;
//	float m_maxAccel;
//
//	sf::Image m_image;
//	sf::Texture m_texture;
//	sf::Sprite m_sprite;
//
//	float m_orientation;
//
//	bool m_alive;
//
//	Player* m_player;
//
//	std::vector<Wall*>& m_walls;
//	std::vector<Worker*>& m_workers;
//
//	NodeLayout& m_nodeLayout;
//
//	std::vector<Node*> m_patrolPath;
//	int m_currentPatrolNode;
//	float m_distToNode;
//
//	std::vector<Node*> m_seekPath;
//	bool m_targetChosen;
//	int m_targetIndex;
//	float m_distToWorker;
//	float m_distToSeekNode;
//	std::vector<int> m_indexesOfAbducted;
//
//	std::vector<Node*> m_returnPath;
//
//	std::vector<Node*> m_fleePath;
//	float m_distToNextNode;
//	bool m_fleeing;
//
//	AStar* m_astar;
//};