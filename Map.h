//Map.h

#ifndef MAP_H
#define MAP_H
#include<stack>
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

	class Iterator : public std::iterator<std::bidirectional_iterator_tag, K, V> {
	private:
		MapNode<K, V>* current;
		std::stack<MapNode<K, V>*> store_nodes;
	public:
		Iterator(MapNode<K, V>* root) {
			MapNode<K, V>* temp = root;

			while (root) {
				store_nodes.push(root);
				root = root->getLeft();
			}

			if (store_nodes.size() > 0) {
				this->current = store_nodes.top();
				store_nodes.pop();
			}
			else
				current = nullptr;
		}
		Iterator& operator++() {
			if (current->getRight()) {
				store_nodes.push(current->getRight());
				if (current->getRight()->getLeft())
					store_nodes.push(current->getRight()->getLeft());
			}
			if (store_nodes.size() == 0) {
				this->current = nullptr;
				return *this;
			}
			current = store_nodes.top();
			store_nodes.pop();
			return *this;
		}
	};
public:
	Map();
	void RotateLeft(MapNode<K, V>* node);
	void RotateRight(MapNode<K, V>* node);
	void FixInsert(MapNode<K, V>* leaf_node);
	void Insert(pair<K, V>& data);
	void Print();
	void InOrder(MapNode<K, V>* root);
	unsigned int size();
	unsigned int count(MapNode<K, V>* root);
	MapNode<K, V>* Search(MapNode<K, V>* root, K key);
	V& operator[](const K& key);
};


template<class K, class V>
MapNode<K, V>::MapNode() :key(0), value(0), parent(0), left(0), right(0), color(RED) {}

template<class K, class V>
MapNode<K, V>::MapNode(K key, V value) {
	this->key = key;
	this->value = value;
	parent = left = right = nullptr;
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
Map<K, V>::Map() :root(nullptr) {}

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
	MapNode<K, V>* uncle = nullptr;
	MapNode<K, V>* parent = nullptr;
	MapNode<K, V>* grandparent = nullptr;

	while (node != NULL && node->getParent() != NULL && node->getParent()->getColor() == RED && node->getColor() != BLACK && node != this->root) {

		parent = node->getParent();
		grandparent = node->getParent()->getParent();

		if (parent == grandparent->getRight()) {
			uncle = grandparent->getLeft();

			if (uncle != 0 && uncle->getColor() == RED) {
				//case 1

				uncle->setColor(BLACK);
				parent->setColor(BLACK);
				grandparent->setColor(RED);
				node = grandparent;
			}

			else {
				//case 2

				if (node == parent->getLeft()) {
					RotateRight(parent);
					node = parent;
					parent = node->getParent();
				}
				
				RotateLeft(parent);
				Color cl = parent->getColor();
				parent->setColor(grandparent->getColor());
				grandparent->setColor(cl);
				node = parent;
			}
		}

		else {
			uncle = grandparent->getRight();

			if (uncle != 0 && uncle->getColor() == RED) { //mirror case 1
				uncle->setColor(BLACK);
				parent->setColor(BLACK);
				grandparent->setColor(RED);
				node = grandparent;
			}

			else {
				if (node == parent->getRight()) {
					RotateLeft(parent);
					node = parent;
					parent = node->getParent();
				}

				RotateRight(grandparent);
				Color cl = parent->getColor();
				parent->setColor(grandparent->getColor());
				grandparent->setColor(cl);
				node = parent;
			}
		}
	}

	this->root->setColor(BLACK);
	
}

template<class K, class V>
void Map<K, V>::Insert(pair<K, V>& data) {
	MapNode<K, V>* new_node = new MapNode<K, V>(data.first, data.second);

	if (!this->root) {
		new_node->setColor(BLACK);
		this->root = new_node;
	}

	else {
		MapNode<K, V>* current = nullptr;
		MapNode<K, V>* parent = nullptr;
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

		else if (new_node->getKey() > parent->getKey())
			parent->setRight(new_node);
		else
			parent->setLeft(new_node);

		FixInsert(new_node);
	}
}

template<class K, class V>
unsigned int  Map<K, V>::size() {
	unsigned int count_node = 0;
	if (this->root)
		count_node = count(this->root);

	return count_node;
}

template<class K, class V>
unsigned int  Map<K, V>::count(MapNode<K, V>* root) {
	unsigned int count_node = 1;
	if (root->getLeft())
		count_node += count(root->getLeft());
	if (root->getRight())
		count_node += count(root->getRight());

	return count_node;
}


template<class K, class V>
MapNode<K, V>* Map<K, V>::Search(MapNode<K, V>* root, K key) {
	if (!root)
		return NULL;

	if (root->getKey() == key)
		return root;

	if (root->getKey() < key)
		Search(root->getRight(), key);
	else
		Search(root->getLeft(), key);
}

template<class K, class V>
V& Map<K, V>::operator[](const K& key) {
	MapNode<K, V>* search_node = Search(this->root, key);
	return search_node->getValue();
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