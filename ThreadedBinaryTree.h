#include<iostream> // 12/12
using namespace std;

class node{
	public:
		node();
		node(int value);
		~node();
		friend void gothrough(node *p);

	private:
		node *left, *right; // the left child and the right child pointer
		int number; // record the node's number
		int is_threadl, is_threadr; //the flag to record whether the pointer is a thread or not

		friend class op_tree_totum;//you could access all valuables in class op_tree_totum
};

//ctor
node::node(){
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//ctor
node::node(int value){
	number = value;
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//dtor
node::~node(){

}

class op_tree_totum{

	public:
		op_tree_totum();
		~op_tree_totum();
		void insertion(int s);
		void deletion(int s);
		void inorder_run();
		void reverseorder_run();
		int size();

	private:
		node *root, *head, *tail;
		int num;//caculate how many nodes in the totum
		node* find (int s);
		node* find_p (int s, node* & parent);
		node* find_most (node* p, int mode);
		node* find_most_p (node* p, int mode, node* & parent);
		void replace_left_rightmost (node* p);
		void replace_right_leftmost (node* p);
		void del_no_sub(node* del, node* parent);
		void del_only_leftsub (node* del, node* parent);
		void del_only_rightsub (node* del, node*parent);
		void print (node* p);
};

//ctor
op_tree_totum::op_tree_totum(){
	head = new node();
	tail = new node();
	head->right = tail; //initialize the head node to connect to the tail node
	tail->left = head;
	root = NULL;
	num = 0;
}

//dtor
op_tree_totum::~op_tree_totum(){
	node *p = root;
	if(p!=NULL)	gothrough(p);
	num = 0;
	if (head != NULL)delete head;
	if (tail != NULL)delete tail;
}

void gothrough(node *p){
	if (p->is_threadl==0 && p->left!= NULL) gothrough(p->left);
	if (p->is_threadr==0 && p->right!= NULL) gothrough(p->right);
	delete p;
}

void op_tree_totum::insertion(int s){
	//TODO: fill in the code to do the insertion of the node with number s
	num++;
	if (root == NULL) {
		root = new node (s);
		root -> is_threadl = 1, root -> is_threadr = 1;
		root -> left = head, root -> right = tail;
		head -> right = root, tail -> left = root;
		return;
	}
	int dir = 0;
	node * find = NULL ;
	node * s_location = find_p (s,find);
	if (s_location != NULL) {num--; return;}
	if (s < find -> number) dir = 1;
	if (s > find -> number) dir = 2;
	
	if (dir == 1) { //insert as a left sub tree
		node* new_node = new node(s);
		if (find -> left == head) head -> right = new_node;
		new_node -> left = find -> left;
		new_node -> right = find;
		find -> is_threadl = 0;
		find -> left = new_node;
		return ;
	}
	else if (dir == 2) { //insert as a right sub tree
		node*  new_node = new node(s);
		if (find -> right == tail) tail -> left = new_node;
		new_node -> left = find;
		new_node -> right = find -> right;
		find -> is_threadr = 0;
		find -> right = new_node;
		return ;

	}
}

void op_tree_totum::deletion(int s){
	//TODO: fill in the code to do the deletion of the node with number s
	node* parent;
	node* del = find_p(s, parent);
	if (del == NULL) return ; // not found, do nothing
	num--;
	if (del -> is_threadl && del -> is_threadr) {// delete a node without sub tree
		del_no_sub (del, parent); 
		return ;		
	}	
	if (del -> is_threadl != 1 && del -> is_threadr == 1 ) { //delete a node with only left subtree
		del_only_leftsub (del, parent);
		return ;
	}
	if (del -> is_threadl == 1 && del -> is_threadr != 1) { // delete a node with only right subtree
		del_only_rightsub (del, parent);
		return ;
	}
	if (del -> is_threadl != 1 && del -> is_threadr != 1) { //delete a node with both left and right subtree
		node* right_most_parent;
		node* right_most = find_most_p (del -> left, 2, right_most_parent);
		del -> number = right_most -> number;
		if (right_most_parent == NULL) right_most_parent = del;
		if (right_most -> is_threadl == 1 && right_most -> is_threadr == 1) { // right_most has no sub tree
			del_no_sub (right_most, right_most_parent);
		}
		else if(right_most -> is_threadl != 1 && right_most -> is_threadr == 1) { // right_most has only left subtree
			del_only_leftsub (right_most, right_most_parent);
		}
	}
}

void op_tree_totum::inorder_run(){
	//TODO: travel the tree from the head node to the tail node and output the values
	//You should NOT use stack or recurtion 
	if (root == NULL) return;
	for (node * cur = head -> right, *cur_next = NULL;cur -> right != NULL; cur = cur_next) {
		cout << cur -> number;
		if (cur -> right != tail) cout << " " ;
		if (cur -> is_threadr == 1) cur_next = cur -> right; //is right sub tree is thread ,visit right
		else
			cur_next = find_most (cur -> right, 1);
		
	}
	cout << endl;
}

void op_tree_totum::reverseorder_run(){
	//TODO: travel the tree from the tail node to the head node and output the values
	//You should NOT use stack or recurtion 
	if (root == NULL) {cout << endl; return ;}
	for (node * cur = tail -> left, *cur_next = NULL; cur -> left != NULL; cur = cur_next) {
		cout << cur -> number;
		if (cur -> left != head) cout << " " ;
		if (cur -> is_threadl == 1) cur_next = cur -> left;
		else
			cur_next = find_most (cur -> left, 2);
	}	
	cout << endl;
}

int op_tree_totum::size(){
	return num;
}

node* op_tree_totum::find(int s) {
	if (root == NULL) return NULL;
	node * cur = head -> right;
	for (node* cur_next = NULL;cur -> right != NULL; cur = cur_next) {
		if (cur -> number == s ) return cur;
		if (cur -> is_threadr == 1) cur_next = cur -> right; //if right sub tree is thread ,visit right
		else cur_next = find_most (cur -> right, 1);
	}
	return NULL;
}
node* op_tree_totum::find_p(int s, node* & parent) { // find s and return its location (if nor found,  return NULL, parent stops at the last node))
	if (root == NULL){ parent = NULL ;return NULL;}
	parent = NULL; node *target = root, *target_next = NULL;
	for (;s != target -> number; parent = target, target = target_next) {
		if (s > target -> number) { // s is greater than the pointer "target"
			if (target -> is_threadr == 1) {parent = target;return NULL;} // but target -> right is thread, means no match
			target_next = target -> right;
		}
		if (s < target -> number ) {
			if (target -> is_threadl == 1) {parent = target; return NULL;}
			target_next = target -> left;
		}
	}
	return target;
} 
void op_tree_totum::print (node* p) {
	if (p == NULL) {cout << "Not Found" << endl; return ;}
	cout << "number: " << p -> number << endl;
}
node* op_tree_totum::find_most (node* p, int mode) {//if mode == 1 :find left most  if mode == 2 : find right most
	node* cur = p;
	if (mode == 1) for (;cur -> is_threadl != 1; cur = cur -> left);
	if (mode == 2) for (;cur -> is_threadr != 1; cur = cur -> right);
	return cur;	
}
node* op_tree_totum::find_most_p (node* p, int mode, node* & parent) { // if (parent == NULL) means p is the most
	node* cur = p;
	parent = NULL;
	if (mode == 1) for(;cur -> is_threadl != 1; parent = cur, cur = cur -> left);
	if (mode == 2) for(;cur -> is_threadr != 1; parent = cur, cur = cur -> right);
	return cur;
}
void op_tree_totum::del_no_sub(node* del, node* parent) {
	if (del == root) {
		root = NULL;
	//	num--;
		head -> right = tail;
		tail -> left = head;
	}
	else if(parent -> left == del) { //del is parent's left sub tree
		if (del -> left == head) {
			head -> right = parent;
		}
		parent -> is_threadl = 1;
		parent -> left = del -> left;		
	}
	else if (parent -> right == del) { // del is parent's right sub tree
		if (del -> right == tail) {
			tail -> left = parent;
		}
		parent -> is_threadr = 1;
		parent -> right = del -> right;
	}
	//num--;
	delete del;
	return ;
}

void op_tree_totum::del_only_leftsub (node* del, node* parent) { // del has only left subtree
	if (del == root) {	//deleting the root
		root = del -> left;
		node* right_most = find_most(del -> left, 2);
		right_most -> right = tail;
		tail -> left = right_most;
	}
	else if (parent -> is_threadl != 1 && parent -> left == del) { // del is parent's left subtree
		parent -> left = del -> left;
		node* right_most = find_most (del -> left, 2);
		right_most -> right = del -> right;
	}
	else if (parent -> is_threadr != 1 && parent -> right == del) { // del is parent's right subtree
		parent -> right = del -> left;	
		node* right_most = find_most (del -> left, 2);
		right_most -> right = del -> right;
		if (del -> right == tail) { // del -> right = tail
			tail -> left = right_most; 
		}
	} 
	delete del;
	return ;
}
void op_tree_totum::del_only_rightsub (node* del, node* parent) {
	if (del == root) {
		root = del -> right;
		node* left_most = find_most (del -> right, 1);
		left_most -> left = head;
		head -> right = left_most;
	}
	else if (parent -> is_threadl != 1 && parent -> left == del) { // del is parent's left sub
		parent -> left = del -> right;
		node* left_most = find_most (del -> right, 1);
		left_most -> left = del -> left;
		if (del -> left == head) {
			head -> right = left_most;
		}
	}
	else if (parent -> is_threadr != 1 && parent -> right == del) {// del is parent's right subtree
		parent -> right = del -> right;
		node* left_most = find_most (del -> right, 1);
		left_most -> left = del -> left;
	}
	delete del;
	return ;
}
