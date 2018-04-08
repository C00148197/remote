#pragma once

#include "stdafx.h"
#include "Arc.h"

#include <list>

class Node {
public:
	Node() {}
	Node(SDL_Point pos, int id);

	void addArc(Node *n);
	float calculateArcWeight(SDL_Point otherNodePos);

	SDL_Point getPos();

	int getID();

	bool getMarked();
	void setMarked(bool marked);

	Node* getPrevious();
	void setPrevious(Node *previous);

	float getHeuristic();
	void setHeuristic(float heuristic);

	float getCost();
	void setCost(float cost);

	std::list<Arc>& getArcs();

private:
	int m_id;

	SDL_Point m_pos;

	bool m_marked; // if the node has been visited

	Node *m_previous; // pointer to previous node

	float m_cost;
	float m_heuristic;
	
	std::list<Arc> m_arcs; // list containing arcs to other nodes
};