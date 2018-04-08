//#include "SweeperBoid.h"
//
//SweeperBoid::SweeperBoid(NodeLayout &nodes, Player* player, std::vector<Wall*> &walls, std::vector<Worker*> &workers, int pathNo) : m_nodeLayout(nodes), m_player(player), m_walls(walls), m_workers(workers) {
//	/********************************************//**
//												  *  ...  // initialises sweeper boid
//												  ***********************************************/
//	m_maxSpeed = 3;
//
//	m_astar = new AStar(nodes);
//
//	m_maxAccel = 30;
//
//	setupPatrol(pathNo);
//
//	// sets initial position to be the start of the patrol path
//	m_pos = sf::Vector2f(m_patrolPath.at(0)->getPos().x - 16, m_patrolPath.at(0)->getPos().y - 16);
//
//	m_nextPosX = m_pos;
//	m_nextPosY = m_pos;
//
//	m_image.loadFromFile("assets/SweeperBoid.png");
//	m_texture.loadFromImage(m_image);
//	m_sprite.setTexture(m_texture);
//	m_sprite.setPosition(m_pos);
//	m_sprite.setOrigin(sf::Vector2f(m_texture.getSize().x / 2, m_texture.getSize().y / 2));
//
//	m_width = m_texture.getSize().x;
//	m_height = m_texture.getSize().y;
//
//	m_currentPatrolNode = 0;
//
//	m_behaviour = PATROL;
//	m_targetChosen = false;
//	m_fleeing = false;
//	m_alive = true;
//}
//
//void SweeperBoid::render(sf::RenderWindow &window) {
//	window.draw(m_sprite);
//}
//
//void SweeperBoid::update(float deltaTime)
//{
//	// chooses whether to seek towards worker or next node
//	chooseBehaviour();
//
//	if (m_behaviour == PATROL) {
//		patrol(deltaTime);
//	}
//	else if (m_behaviour == ABDUCT) {
//		if (m_targetChosen == false) {
//			chooseTarget();
//		}
//		else {
//			setupSeekPath();
//			abduct(deltaTime);
//		}
//	}
//	else if (m_behaviour == RETURN) {
//		setupReturnPath();
//
//		// if the behaviour has been changed to patrol
//		if (m_behaviour != PATROL) {
//			returnToPatrol(deltaTime);
//		}
//	}
//	else {
//		if (m_fleeing == false) {
//			setupFleePath(0);
//		}
//		flee(deltaTime, m_fleePath.at(0)->getPos());
//	}
//
//	// checks if the velocity is greater than its max velocity
//	if (calculateMagnitude(m_vel) > m_maxSpeed) {
//		normalise(m_vel);
//		m_vel *= m_maxSpeed;
//	}
//
//	m_nextPosX.x += m_vel.x;
//	m_nextPosY.y += m_vel.y;
//
//	// check collisions with wall
//	for (std::vector<Wall*>::iterator i = m_walls.begin(); i != m_walls.end(); i++) {
//		checkWallCollisions((*i), deltaTime);
//	}
//
//	for (int i = 0; i < m_player->getBullets().size(); i++) {
//		checkBulletCollision(m_player->getBullets().at(i));
//	}
//
//	m_pos.x = m_nextPosX.x;
//	m_pos.y = m_nextPosY.y;
//
//	m_nextPosX.y = m_pos.y;
//	m_nextPosY.x = m_pos.x;
//
//	m_sprite.setPosition(m_pos.x + m_texture.getSize().x / 2, m_pos.y + m_texture.getSize().y / 2);
//
//	m_orientation = (atan2(m_vel.x, -m_vel.y) * 180 / 3.14159265);
//	m_sprite.setRotation(m_orientation);
//}
//
//void SweeperBoid::chooseBehaviour() {
//	/********************************************//**
//												  *  ...  // chooses which behaviour the sweeper follows
//												  ***********************************************/
//
//												  // if any workers are in sight, it will swap to abducting behaviour
//	if (m_behaviour != ABDUCT && m_behaviour != FLEE) {
//		for (int i = 0; i < m_workers.size(); i++) {
//			if (calculateMagnitude(m_workers.at(i)->getCenter(), m_pos) < 150) {
//				float angle = (atan2(m_workers.at(i)->getCenter().y - m_pos.y, m_workers.at(i)->getCenter().x - m_pos.x) * 180 / 3.14) + 90;
//
//				if (angle > m_orientation - 22.5 && angle < m_orientation + 22.5) {
//					m_behaviour = ABDUCT;
//					break;
//				}
//			}
//		}
//	}
//
//	// fleeing behaviour takes precedance if player is in sight
//	if (m_behaviour != FLEE) {
//		if (calculateMagnitude(m_player->getPosition(), m_pos) < 150) {
//			float angle = (atan2(m_player->getPosition().y - m_pos.y, m_player->getPosition().x - m_pos.x) * 180 / 3.14) + 90;
//
//			if (angle > m_orientation - 22.5 && angle < m_orientation + 22.5) {
//				m_behaviour = FLEE;
//			}
//		}
//	}
//}
//
//void SweeperBoid::chooseTarget() {
//	/********************************************//**
//												  *  ...  // chooses target to abduct
//												  ***********************************************/
//	float closestDistWorker = 99999;
//
//	for (int i = 0; i < m_workers.size(); i++) {
//		float dist = calculateMagnitude(m_workers.at(i)->getPos(), m_pos);
//
//		// if within vision range
//		if (dist < 150) {
//			float angle = (atan2(m_workers.at(i)->getCenter().y - m_pos.y, m_workers.at(i)->getCenter().x - m_pos.x) * 180 / 3.14) + 90;
//			// if within cone of vision
//			if (angle > m_orientation - 22.5 && angle < m_orientation + 22.5) {
//				// if closest
//				if (dist < closestDistWorker) {
//					// if not already abducted
//					if (m_workers.at(i)->getAbducted() == false && m_workers.at(i)->getRescued() == false) {
//						closestDistWorker = dist;
//						m_targetIndex = i;
//						m_targetChosen = true;
//					}
//				}
//			}
//		}
//	}
//}
//
//void SweeperBoid::setupSeekPath() {
//	/********************************************//**
//												  *  ...  // setup path to seek to worker
//												  ***********************************************/
//	int indexClosestToWorker;
//	int indexClosestToSweeper;
//
//	float closestDistWorker = 99999;
//	float closestDistSweeper = 99999;
//
//	for (int i = 0; i < m_nodeLayout.getNoOfNodes() - 1; i++) {
//		float distWorker = calculateMagnitude(m_nodeLayout.getNodes()[i]->getPos(), m_workers.at(m_targetIndex)->getPos());
//
//		if (distWorker < closestDistWorker) {
//			closestDistWorker = distWorker;
//			indexClosestToWorker = i;
//		}
//
//		float distSweeper = calculateMagnitude(m_nodeLayout.getNodes()[i]->getPos(), m_pos);
//
//		if (distSweeper < closestDistSweeper) {
//			closestDistSweeper = distSweeper;
//			indexClosestToSweeper = i;
//		}
//	}
//
//	if (!m_seekPath.empty()) {
//		// if the node that is closest (the destination) to the player has changed
//		if (m_nodeLayout.getNodes()[indexClosestToWorker] != m_seekPath.at(m_seekPath.size() - 1)) {
//			m_seekPath.clear();
//			m_astar->calculatePath(m_nodeLayout.getNodes()[indexClosestToWorker], m_nodeLayout.getNodes()[indexClosestToSweeper], m_seekPath);
//		}
//	}
//	else {
//		// create initial path
//		m_astar->calculatePath(m_nodeLayout.getNodes()[indexClosestToWorker], m_nodeLayout.getNodes()[indexClosestToSweeper], m_seekPath);
//	}
//}
//
//void SweeperBoid::abduct(float deltaTime) {
//	/********************************************//**
//												  *  ...  // chooeses whether to follow astar path or directly to target, depending on which is closer
//												  ***********************************************/
//												  // directional vector to worker
//	sf::Vector2f vecToWorker = m_workers.at(m_targetIndex)->getPos() - m_pos;
//	m_distToWorker = calculateMagnitude(vecToWorker);
//
//	//// if there are nodes to seek to
//	if (!m_seekPath.empty()) {
//		// directional vector to next node
//		sf::Vector2f vecToNextPoint = m_seekPath.at(0)->getPos() - m_pos;
//
//		// distance to next node
//		m_distToSeekNode = calculateMagnitude(vecToNextPoint);
//
//		// if the next node is closer than the worker
//		if (m_distToSeekNode < m_distToWorker) {
//			seek(deltaTime, vecToNextPoint, m_distToSeekNode, false);
//
//			if (m_distToSeekNode < 70) {
//				m_seekPath.erase(m_seekPath.begin());
//			}
//		}
//		// if the worker is closer than the next node
//		else {
//			seek(deltaTime, vecToWorker, m_distToWorker, true);
//		}
//	}
//	// if there aren't nodes to seek to
//	else {
//		seek(deltaTime, vecToWorker, m_distToWorker, true);
//	}
//}
//
//void SweeperBoid::setupReturnPath() {
//	/********************************************//**
//												  *  ...  // sets up path to return to patrol path
//												  ***********************************************/
//	int indexClosestToSweeper;
//	float closestDistSweeper = 99999;
//
//	for (int i = 0; i < m_nodeLayout.getNoOfNodes() - 1; i++) {
//		float distSweeper = calculateMagnitude(m_nodeLayout.getNodes()[i]->getPos(), m_pos);
//
//		if (distSweeper < closestDistSweeper) {
//			closestDistSweeper = distSweeper;
//			indexClosestToSweeper = i;
//		}
//	}
//
//	// if the closest node is not the next node on the patrol
//	if (indexClosestToSweeper != m_patrolPath.at(m_currentPatrolNode)->getID()) {
//		if (!m_returnPath.empty()) {
//			// if the node that is closest (the destination) to the player has changed
//			m_returnPath.clear();
//			m_astar->calculatePath(m_nodeLayout.getNodes()[m_patrolPath.at(m_currentPatrolNode)->getID()], m_nodeLayout.getNodes()[indexClosestToSweeper], m_returnPath);
//		}
//	}
//	else {
//		m_behaviour = PATROL;
//	}
//}
//
//void SweeperBoid::returnToPatrol(float deltaTime) {
//	/********************************************//**
//												  *  ...  // seeks back to the next node that it was going to patrol to
//												  ***********************************************/
//	float targetSpeed = 0;
//	sf::Vector2f v = m_patrolPath.at(m_currentPatrolNode)->getPos() - m_pos;
//	m_distToNode = calculateMagnitude(v);
//
//	if (m_distToNode < 65) {
//		// change to seek to next node upon reaching current node
//		m_returnPath.erase(m_returnPath.begin());
//
//		if (m_returnPath.size() == 0) {
//			m_behaviour = PATROL;
//		}
//	}
//	else if (m_distToNode > 200) {
//		targetSpeed = m_maxSpeed;
//	}
//	else {
//		targetSpeed = m_maxSpeed * (m_distToNode / 200);
//	}
//
//	normalise(v);
//	v *= targetSpeed;
//
//	float timeToTarget = 4;
//
//	m_accel = v - (m_vel / timeToTarget);
//
//	if (calculateMagnitude(m_accel) > m_maxAccel) {
//		normalise(m_accel);
//	}
//
//	m_vel += m_accel * deltaTime;
//}
//
//void SweeperBoid::setupFleePath(float minDist) {
//	/********************************************//**
//												  *  ...  // sets up path to run away from player
//												  ***********************************************/
//
//												  // checks which node is closest to the player and the predator
//	int indexClosestToPlayer;
//	int indexClosestToSweeper;
//
//	float closestDistPlayer = 99999;
//	float closestDistSweeper = 99999;
//
//	for (int i = 0; i < m_nodeLayout.getNoOfNodes() - 1; i++) {
//		float distPlayer = calculateMagnitude(m_nodeLayout.getNodes()[i]->getPos(), (m_player)->getPosition());
//
//		if (distPlayer < closestDistPlayer) {
//			closestDistPlayer = distPlayer;
//			indexClosestToPlayer = i;
//		}
//
//		float distSweeper = calculateMagnitude(m_nodeLayout.getNodes()[i]->getPos(), m_pos);
//
//		if (distSweeper < closestDistSweeper && distSweeper > minDist) {
//			closestDistSweeper = distSweeper;
//			indexClosestToSweeper = i;
//		}
//	}
//
//	// if the node closest to the player and sweeper aren't the same
//	if (indexClosestToSweeper == indexClosestToPlayer) {
//		setupFleePath(closestDistSweeper);
//	}
//	else {
//		if (!m_fleePath.empty()) {
//			m_fleePath.clear();
//		}
//
//		int furthestFromPlayerIndex;
//		float furthestDistFromPlayer = 0;
//
//		for (std::list<Arc>::iterator i = m_nodeLayout.getNodes()[indexClosestToSweeper]->getArcs().begin(); i != m_nodeLayout.getNodes()[indexClosestToSweeper]->getArcs().end(); i++) {
//			float distFromPlayer = calculateMagnitude(m_player->getPosition(), (*i).getNode()->getPos());
//
//			if (distFromPlayer > furthestDistFromPlayer) {
//				furthestDistFromPlayer = distFromPlayer;
//				furthestFromPlayerIndex = (*i).getNode()->getID();
//			}
//		}
//
//		m_astar->calculatePath(m_nodeLayout.getNodes()[furthestFromPlayerIndex], m_nodeLayout.getNodes()[indexClosestToSweeper], m_fleePath);
//	}
//
//	m_fleeing = true;
//}
//
//void SweeperBoid::flee(float deltaTime, sf::Vector2f v) {
//	/********************************************//**
//												  *  ...  // follows path to flee from player
//												  ***********************************************/
//	float targetSpeed = 0;
//	m_distToNextNode = calculateMagnitude(v, m_pos);
//
//	// slows down when near node
//	if (m_distToNextNode < 70) {
//		// removes the first node once close enough
//		m_fleePath.erase(m_fleePath.begin() - 1);
//
//		if (m_fleePath.size() == 0) {
//			m_behaviour = PATROL;
//			m_fleeing = false;
//		}
//	}
//	else if (m_distToNextNode > 140) {
//		targetSpeed = m_maxSpeed;
//	}
//	else {
//		targetSpeed = m_maxSpeed * (m_distToNextNode / 140);
//	}
//
//	normalise(v);
//	v *= targetSpeed;
//
//	float timeToTarget = 4;
//
//	m_accel = v - (m_vel / timeToTarget);
//
//	if (calculateMagnitude(m_accel) > m_maxAccel) {
//		normalise(m_accel);
//	}
//
//	m_vel += m_accel * deltaTime;
//}
//
//void SweeperBoid::seek(float deltaTime, sf::Vector2f v, float dist, bool seekingWorker) {
//	/********************************************//**
//												  *  ...  // seeks to a given position
//												  ***********************************************/
//	float targetSpeed;
//
//	if (seekingWorker == false) {
//		if (m_workers.at(m_targetIndex)->getAbducted() == true) {
//			// if someone else catches their target
//			m_behaviour = RETURN;
//			m_targetChosen = false;
//		}
//
//		if (dist < 70) {
//			targetSpeed = 0;
//		}
//		else if (dist > 200) {
//			targetSpeed = m_maxSpeed;
//		}
//		else {
//			targetSpeed = m_maxSpeed * (dist / 200);
//		}
//	}
//	else {
//		if (dist < 30) {
//			// abducts player and returns to patrol
//			m_workers.at(m_targetIndex)->setAbducted(true);
//			m_indexesOfAbducted.push_back(m_targetIndex);
//			m_behaviour = RETURN;
//		}
//	}
//
//	normalise(v);
//	v *= targetSpeed;
//
//	float timeToTarget = 4;
//
//	m_accel = v - (m_vel / timeToTarget);
//
//	if (calculateMagnitude(m_accel) > m_maxAccel) {
//		normalise(m_accel);
//	}
//
//	m_vel += m_accel * deltaTime;
//}
//
//void SweeperBoid::setupPatrol(int pathNo) {
//	/********************************************//**
//												  *  ...  // sets up patrol path
//												  ***********************************************/
//
//												  // sets up nodes on patrol path
//	if (pathNo == 1) {
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[0]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[1]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[9]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[21]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[20]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[19]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[4]);
//
//		// faces next node in path
//		m_orientation = 90;
//	}
//	else if (pathNo == 2) {
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[10]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[6]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[7]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[2]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[1]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[9]);
//
//		// faces next node in path
//		m_orientation = 0;
//	}
//	else if (pathNo == 3) {
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[16]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[15]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[10]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[9]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[21]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[22]);
//
//		// faces next node in path
//		m_orientation = 270;
//	}
//	else if (pathNo == 4) {
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[13]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[17]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[16]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[22]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[23]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[18]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[14]);
//
//		// faces next node in path
//		m_orientation = 180;
//	}
//	else {
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[7]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[8]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[13]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[14]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[5]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[3]);
//		m_patrolPath.push_back(m_nodeLayout.getNodes()[2]);
//
//		// faces next node in path
//		m_orientation = 90;
//	}
//}
//
//void SweeperBoid::patrol(float deltaTime) {
//	/********************************************//**
//												  *  ...  // follows patrol path
//												  ***********************************************/
//
//	float targetSpeed = 0;
//	sf::Vector2f v = m_patrolPath.at(m_currentPatrolNode)->getPos() - m_pos;
//	m_distToNode = calculateMagnitude(v);
//
//	if (m_distToNode < 65) {
//		// change to seek to next node upon reaching current node
//		if (m_currentPatrolNode < m_patrolPath.size() - 1) {
//			m_currentPatrolNode++;
//		}
//		else {
//			m_currentPatrolNode = 0;
//		}
//
//		v = m_patrolPath.at(m_currentPatrolNode)->getPos() - m_pos;
//	}
//	else if (m_distToNode > 200) {
//		targetSpeed = m_maxSpeed;
//	}
//	else {
//		targetSpeed = m_maxSpeed * (m_distToNode / 200);
//	}
//
//	normalise(v);
//	v *= targetSpeed;
//
//	float timeToTarget = 4;
//
//	m_accel = v - (m_vel / timeToTarget);
//
//	if (calculateMagnitude(m_accel) > m_maxAccel) {
//		normalise(m_accel);
//	}
//
//	m_vel += m_accel * deltaTime;
//}
//
//void SweeperBoid::checkWallCollisions(Wall* wall, float deltaTime) {
//	/********************************************//**
//												  *  ...  // checks collisions with walls
//												  ***********************************************/
//												  // checks for intersection along x between the predator and the wall
//	if (m_nextPosX.x < wall->getRight()
//		&& m_nextPosX.x + m_width > wall->getPos().x
//		&& m_nextPosX.y < wall->getBottom()
//		&& m_nextPosX.y + m_height > wall->getPos().y)
//	{
//		// left of predator collides with the right of the wall
//		if (m_nextPosX.x < wall->getRight() && m_nextPosX.x > wall->getPos().x) {
//			m_vel.x = 0;
//			m_nextPosX.x = wall->getPos().x + wall->getWidth();
//		}
//		// right of predator collides with the left of the wall
//		else if (m_nextPosX.x + m_width > wall->getPos().x && m_nextPosX.x + m_width < wall->getRight()) {
//			m_vel.x = 0;
//			m_nextPosX.x = wall->getPos().x - m_width;
//		}
//	}
//
//	// checks for intersection along y between the predator and the wall
//	if (m_nextPosY.x < wall->getRight()
//		&& m_nextPosY.x + m_width > wall->getPos().x
//		&& m_nextPosY.y < wall->getBottom()
//		&& m_nextPosY.y + m_height > wall->getPos().y)
//	{
//		// bottom of predator collides with top of the wall
//		if (m_nextPosY.y + m_height > wall->getPos().y && m_nextPosY.y + m_height < wall->getBottom()) {
//			m_vel.y = 0;
//			m_nextPosY.y = wall->getPos().y - m_height;
//		}
//		// top of predator collides with bottom of the wall
//		else if (m_nextPosY.y < wall->getBottom() && m_nextPosY.y > wall->getPos().y) {
//			m_vel.y = 0;
//			m_nextPosY.y = wall->getPos().y + wall->getHeight();
//		}
//	}
//}
//
//void SweeperBoid::checkBulletCollision(Projectile* p) {
//	/********************************************//**
//												  *  ...  // checks collision between sweeper and player bullets
//												  ***********************************************/
//
//	if (p->getPosition().x < m_pos.x + m_width
//		&& p->getPosition().x + p->getWidth() >m_pos.x
//		&& p->getPosition().y < m_pos.y + m_height
//		&& p->getPosition().y + p->getHeight() >m_pos.y)
//	{
//		p->setAlive(false);
//		m_alive = false;
//	}
//}
//
//void SweeperBoid::normalise(sf::Vector2f &v) {
//	float magnitude = calculateMagnitude(v);
//
//	if (magnitude > 0)
//	{
//		v.x = v.x / magnitude;
//		v.y = v.y / magnitude;
//	}
//}
//
//float SweeperBoid::calculateMagnitude(sf::Vector2f vec) {
//	return sqrt((vec.x * vec.x) + (vec.y * vec.y));
//}
//
//float SweeperBoid::calculateMagnitude(sf::Vector2f vec1, sf::Vector2f vec2) {
//	return sqrt(((vec2.x - vec1.x) * (vec2.x - vec1.x)) + ((vec2.y - vec1.y) * (vec2.y - vec1.y)));
//}
//
//bool SweeperBoid::getAlive() {
//	return m_alive;
//}
//
//std::vector<int> SweeperBoid::getIndexesOfAbducted() {
//	return m_indexesOfAbducted;
//}