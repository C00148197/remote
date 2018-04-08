#include "Node.h"

Node::Node(SDL_Point pos, int id) {
	m_pos = pos;
	m_id = id;
}

void Node::addArc(Node *n) {
	Arc arc;

	arc.setNode(n);
	arc.setWeight(calculateArcWeight(n->getPos()));

	m_arcs.push_back(arc);
}

float Node::calculateArcWeight(SDL_Point otherNodePos) {
	/********************************************//**
 *  ...  weight is the distance between the nodes
 ***********************************************/

	return sqrt(((otherNodePos.x - m_pos.x) * (otherNodePos.x - m_pos.x)) + ((otherNodePos.y - m_pos.y) * (otherNodePos.y - m_pos.y)));
}

SDL_Point Node::getPos() {
	return m_pos;
}

int Node::getID() {
	return m_id;
}

bool Node::getMarked() {
	return m_marked;
}

void Node::setMarked(bool marked) {
	m_marked = marked;
}

Node* Node::getPrevious() {
	return m_previous;
}

void Node::setPrevious(Node* previous) {
	m_previous = previous;
}

float Node::getHeuristic() {
	return m_heuristic;
}

void Node::setHeuristic(float heuristic) {
	m_heuristic = heuristic;
}

float Node::getCost() {
	return m_cost;
}

void Node::setCost(float cost) {
	m_cost = cost;
}

std::list<Arc>& Node::getArcs() {
	return m_arcs;
}