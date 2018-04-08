#include "NodeLayout.h"

NodeLayout::NodeLayout(std::vector<SDL_Point> &nodeData) {
	m_noOfNodes = nodeData.size() + 1;

	m_nodes = new Node*[m_noOfNodes];

	for (int i = 0; i < nodeData.size(); i++) {
		m_nodes[i] = new Node(nodeData.at(i), i);
		m_nodes[i]->setMarked(false);
	}

	addArcs();
}

void NodeLayout::addArcs() {
	/********************************************//**
	*  ...  add arcs between nodes
***********************************************/

	m_nodes[0]->addArc(m_nodes[1]);
	m_nodes[1]->addArc(m_nodes[0]);


	m_nodes[1]->addArc(m_nodes[2]);
	m_nodes[2]->addArc(m_nodes[1]);

	m_nodes[2]->addArc(m_nodes[3]);
	m_nodes[3]->addArc(m_nodes[2]);


	m_nodes[3]->addArc(m_nodes[4]);
	m_nodes[4]->addArc(m_nodes[3]);


	m_nodes[4]->addArc(m_nodes[5]);
	m_nodes[5]->addArc(m_nodes[4]);




	//m_nodes[0]->addArc(m_nodes[1]);
	//m_nodes[1]->addArc(m_nodes[0]);

	//m_nodes[1]->addArc(m_nodes[2]);
	//m_nodes[2]->addArc(m_nodes[1]);
	//
	//m_nodes[2]->addArc(m_nodes[3]);
	//m_nodes[3]->addArc(m_nodes[2]);

	//m_nodes[0]->addArc(m_nodes[4]);
	//m_nodes[4]->addArc(m_nodes[0]);

	//m_nodes[3]->addArc(m_nodes[5]);
	//m_nodes[5]->addArc(m_nodes[3]);

	//m_nodes[18]->addArc(m_nodes[5]);
	//m_nodes[5]->addArc(m_nodes[18]);

	//m_nodes[6]->addArc(m_nodes[7]);
	//m_nodes[7]->addArc(m_nodes[6]);

	//m_nodes[2]->addArc(m_nodes[7]);
	//m_nodes[7]->addArc(m_nodes[2]);

	//m_nodes[8]->addArc(m_nodes[7]);
	//m_nodes[7]->addArc(m_nodes[8]);

	//m_nodes[9]->addArc(m_nodes[10]);
	//m_nodes[10]->addArc(m_nodes[9]);

	//m_nodes[9]->addArc(m_nodes[1]);
	//m_nodes[1]->addArc(m_nodes[9]);

	//m_nodes[6]->addArc(m_nodes[10]);
	//m_nodes[10]->addArc(m_nodes[6]);

	//m_nodes[11]->addArc(m_nodes[10]);
	//m_nodes[10]->addArc(m_nodes[11]);

	//m_nodes[11]->addArc(m_nodes[12]);
	//m_nodes[12]->addArc(m_nodes[11]);

	//m_nodes[13]->addArc(m_nodes[12]);
	//m_nodes[12]->addArc(m_nodes[13]);

	//m_nodes[13]->addArc(m_nodes[8]);
	//m_nodes[8]->addArc(m_nodes[13]);

	//m_nodes[13]->addArc(m_nodes[14]);
	//m_nodes[14]->addArc(m_nodes[13]);

	//m_nodes[5]->addArc(m_nodes[14]);
	//m_nodes[14]->addArc(m_nodes[5]);

	//m_nodes[18]->addArc(m_nodes[14]);
	//m_nodes[14]->addArc(m_nodes[18]);

	//m_nodes[10]->addArc(m_nodes[15]);
	//m_nodes[15]->addArc(m_nodes[10]);

	//m_nodes[16]->addArc(m_nodes[15]);
	//m_nodes[15]->addArc(m_nodes[16]);

	//m_nodes[16]->addArc(m_nodes[17]);
	//m_nodes[17]->addArc(m_nodes[16]);

	//m_nodes[13]->addArc(m_nodes[17]);
	//m_nodes[17]->addArc(m_nodes[13]);

	//m_nodes[4]->addArc(m_nodes[19]);
	//m_nodes[19]->addArc(m_nodes[4]);

	//m_nodes[20]->addArc(m_nodes[19]);
	//m_nodes[19]->addArc(m_nodes[20]);

	//m_nodes[20]->addArc(m_nodes[21]);
	//m_nodes[21]->addArc(m_nodes[20]);

	//m_nodes[9]->addArc(m_nodes[21]);
	//m_nodes[21]->addArc(m_nodes[9]);

	//m_nodes[22]->addArc(m_nodes[21]);
	//m_nodes[21]->addArc(m_nodes[22]);

	//m_nodes[22]->addArc(m_nodes[16]);
	//m_nodes[16]->addArc(m_nodes[22]);

	//m_nodes[22]->addArc(m_nodes[23]);
	//m_nodes[23]->addArc(m_nodes[22]);

	//m_nodes[18]->addArc(m_nodes[23]);
	//m_nodes[23]->addArc(m_nodes[18]);
}

int NodeLayout::getNoOfNodes() {
	return m_noOfNodes;
}

Node** NodeLayout::getNodes() {
	return m_nodes;
}