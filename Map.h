//Map.h

#ifndef MAP_H
#define MAP_H
#include <utility>
using namespace std;

template<class K, class V>
class MapNode;

template<class K, class V>
class Map;

enum Color { RED, BLACK };

template<class K, class V>
class MapNode {
private:
	K key;
	V value;
	MapNode<K, V>* parent;
	MapNode<K, V>* left;
	MapNode<K, V>* right;
	Color color;

public:
	MapNode();
	MapNode(K key, V value);
	K getKey();
	V getValue();
	MapNode<K, V>* getParent();
	MapNode<K, V>* getLeft();
	MapNode<K, V>* getRight();
	Color getColor();
	void setKey(K key);
	void setValue(V value);
	void setParent(MapNode<K, V>* parent);
	void setLeft(MapNode<K, V>* left);
	void setRight(MapNode<K, V>* right);
	void setColor(Color color);
};


template<class K, class V>
class Map {
private:
	MapNode<K, V>* root;

public:
	Map();
	void RotateLeft(MapNode<K, V>* node);
	void RotateRight(MapNode<K, V>* node);
	void FixInsert(MapNode<K, V>* leaf_node);
	void Insert(pair<K, V>& data);
	void Print();
	void InOrder(MapNode<K, V>* root);
};


template<class K, class V>
MapNode<K, V>::MapNode() :key(0), value(0), parent(0), left(0), right(0), color(RED) {}

template<class K, class V>
MapNode<K, V>::MapNode(K key, V value) {
	this->key = key;
	this->value = value;
	parent = left = right = 0;
	color = RED;
}

template<class K, class V>
K MapNode<K, V>::getKey() {
	return key;
}

template<class K, class V>
V MapNode<K, V>::getValue() {
	return value;
}

template<class K, class V>
MapNode<K, V>* MapNode<K, V>::getParent() {
	return parent;
}

template<class K, class V>
MapNode<K, V>* MapNode<K, V>::getLeft() {
	return left;
}

template<class K, class V>
MapNode<K, V>* MapNode<K, V>::getRight() {
	return right;
}

template<class K, class V>
Color MapNode<K, V>::getColor() {
	return color;
}


template<class K, class V>
void MapNode<K, V>::setKey(K key) {
	this->key = key;
}


template<class K, class V>
void MapNode<K, V>::setValue(V value) {
	this->value = value;
}


template<class K, class V>
void MapNode<K, V>::setParent(MapNode<K, V>* parent) {
	this->parent = parent;
}


template<class K, class V>
void MapNode<K, V>::setLeft(MapNode<K, V>* left) {
	this->left = left;
}


template<class K, class V>
void MapNode<K, V>::setRight(MapNode<K, V>* right) {
	this->right = right;
}


template<class K, class V>
void MapNode<K, V>::setColor(Color color) {
	this->color = color;
}




template<class K, class V>
Map<K, V>::Map() :root(0) {}

template<class K, class V>
void Map<K, V>::RotateLeft(MapNode<K, V>* node) {
	MapNode<K, V>* temp_node;
	temp_node = node->getRight();
	node->setRight(temp_node->getLeft());

	if (node->getRight())
		node->getRight()->setParent(node);

	temp_node->setParent(node->getParent());

	if (!node->getParent())	this->root = temp_node;
	else if (node == node->getParent()->getLeft())	node->getParent()->setLeft(temp_node);
	else	node->getParent()->setRight(temp_node);

	temp_node->setLeft(node);
	node->setParent(temp_node);
}

template<class K, class V>
void Map<K, V>::RotateRight(MapNode<K, V>* node) {
	MapNode<K, V>* temp_node;
	temp_node = node->getLeft();
	node->setLeft(temp_node->getRight());

	if (node->getLeft())
		node->getLeft()->setParent(node);

	temp_node->setParent(node->getParent());

	if (!node->getParent())		this->root = temp_node;
	else if (node == node->getParent()->getRight())	node->getParent()->setRight(temp_node);
	else	node->getParent()->setLeft(temp_node);

	temp_node->setRight(node);
	node->setParent(temp_node);
}

template<class K, class V>
void Map<K, V>::FixInsert(MapNode<K, V>* node) {
	MapNode<K, V>* uncle = 0;

	while (node->getParent()->getColor() == RED) {

		if (node->getParent() == node->getParent()->getParent()->getRight()) {
			uncle = node->getParent()->getParent()->getLeft();

			if (uncle->getColor() == RED) {
				//case 1

				uncle->setColor(BLACK);
				node->getParent()->setColor(BLACK);
				node->getParent()->getParent()->setColor(RED);
				node = node->getParent()->getParent();
			}

			else {
				//case 2

				if (node == node->getParent()->getLeft()) {
					node = node->getParent();
					RotateRight(node);
				}
				node->getParent()->setColor(BLACK);
				node->getParent()->getParent()->setColor(RED);
				RotateLeft(node->getParent()->getParent());
			}
		}

		else {
			uncle = node->getParent()->getParent()->getRight();

			if (uncle->getColor() == RED) { //mirror case 1
				uncle->setColor(BLACK);
				node->getParent()->setColor(BLACK);
				node->getParent()->getParent()->setColor(RED);
				node = node->getParent()->getParent();
			}

			else {
				if (node == node->getParent()->getRight()) {
					node = node->getParent();
					RotateLeft(node);
				}
				node->getParent()->setColor(BLACK);
				node->getParent()->getParent()->setColor(RED);
				RotateRight(node->getParent()->getParent());

			}
		}

		if (node == this->root) break;
	}

	this->root->setColor(BLACK);
	
}

template<class K, class V>
void Map<K, V>::Insert(pair<K, V>& data) {
	MapNode<K, V>* new_node = new MapNode<K, V>(data.first, data.second);


	cout << "inserted value";
	if (!this->root) {
		new_node->setColor(BLACK);
		this->root = new_node;
	}

	else {
		MapNode<K, V>* current = 0;
		MapNode<K, V>* parent = 0;
		current = this->root;

		while (current) { 
			parent = current;
			if (new_node->getKey() < current->getKey())
				current = current->getLeft();
			else
				current = current->getRight();
		}

		new_node->setParent(parent);

		if (!parent) this->root = new_node;

		if (new_node->getKey() > parent->getKey())
			parent->setRight(new_node);
		else
			parent->setLeft(new_node);

		new_node->setColor(RED);
		if (!new_node->getParent()) {
			new_node->setColor(BLACK);
			return;
		}

		if (!new_node->getParent()->getParent())
			return;
		cout << "we reached here\n";
		FixInsert(new_node);
	}
}

template<class K, class V>
void Map<K, V>::Print() {
	InOrder(this->root);
}

template<class K, class V>
void Map<K, V>::InOrder(MapNode<K, V>* root) {
	if (root) {
		InOrder(root->getLeft());
		cout << root->getKey()<<endl;
		InOrder(root->getRight());
	}
}

#endif // !MAP_H