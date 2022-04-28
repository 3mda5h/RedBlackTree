#include <iostream>
#include "Tree.h"
#include <cstring>

using namespace std;

Tree::Tree()
{
  root = NULL;
}

void Tree::insert(int number)
{
  insert_impl(root, number); 
}

//insert a node into tree maintaining red black property - with sibling pointer
void Tree::insert_impl(Node* current, int number)
{
  //1. inserting root - root must be black
  if(root == NULL)
  {
    Node* newRoot = new Node();
    newRoot->number = number;
    newRoot->color = "black";
    root = newRoot;
    cout << "case 1" << endl;
    return;
  }
  if(current != NULL)
  {
    if(number > current->number)
    {
      if(current->right != NULL)
      {
        insert_impl(current->right, number);
      }
      else //insert new node in empty slot
      {
        Node* newChild = new Node();
        newChild->number = number;
        newChild->parent = current;
        newChild->sibling = current->left;
        if(current->left != NULL) current->left->sibling = newChild;
        current->right = newChild;
        if(strcmp(current->color, "black") != 0) fixInsert(newChild);  //else: case 2. parent is red, don't need to fix colors
        else cout << "case 2" << endl;
      }
    }
    else //number is less than or equal to current
    {
      if(current->left != NULL)
      {
        insert_impl(current->left, number);
      }
      else
      {
        Node* newChild = new Node();
        newChild->number = number;
        newChild->parent = current;
        newChild->sibling = current->right;
        if(current->right != NULL) current->right->sibling = newChild;
        current->left = newChild;
        if(strcmp(current->color, "black") != 0) fixInsert(newChild);
        else cout << "case 2" << endl;
      }
    }
  }
}

//with sibling pointer
void Tree::fixInsert(Node* node)
{
  Node* parent = node->parent;
  Node* grandpa = parent->parent;
  //3. parent and uncle of node are red - change parent and uncle to black, grandparent to red
  if(strcmp(parent->color, "red") == 0 && parent->sibling != NULL && strcmp(parent->sibling->color, "red") == 0)
  {
    display();
    cout << "case 3" << endl;
    caseThree(node);
    //fix tree higher up
    Node* current = grandpa;
    if(grandpa->parent != NULL && grandpa->parent->parent != NULL) fixInsert(current);
    while(current->parent != NULL && current->parent->parent != NULL)
    {
      current = current->parent;
      fixInsert(current);
    }
  }
  else if(strcmp(node->color, "red") == 0 && strcmp(parent->color, "red") == 0 && (parent->sibling == NULL || strcmp(parent->sibling->color, "black") == 0)) //NULL is black
  {
    //4. uncle is black 
    if(parent == grandpa->left && node == parent->right) //parent is left of grandpa and node is right of parent
    {
      display();
      cout << "case 4 heheh" << endl;
      //tree rotation - node goes to parent's spot 
      Node* oldParent = parent;
      grandpa->left = node;
      parent = grandpa;
      node->sibling = grandpa->right;
      if(grandpa->right != NULL) grandpa->right->sibling = node;
      //left child of node becomes right child of parent
      oldParent->right = node->left;
      if(oldParent->right != NULL) oldParent->right->sibling = oldParent->left;
      if(oldParent->left != NULL) oldParent->left->sibling = oldParent->right;
      //parent becomes the right child of node
      node->left = oldParent;
      oldParent->parent = node;
      oldParent->sibling = node->right;
      if(node->right != NULL) node->right->sibling = oldParent;
      node = node->left; //node is now the child of node (what used to be the parent of node)
      display();
    }
    else if(parent == grandpa->right && node == parent->left) //parent is right of grandpa and node is left of parent
    {
      display();
      cout << "case 4 heheh" << endl;
      //tree rotation - node goes to parent's spot 
      Node* oldParent = parent;
      grandpa->right = node;
      parent = grandpa;
      node->sibling = grandpa->left;
      if(grandpa->left != NULL) grandpa->left->sibling = node;
      //right child of node becomes left child of parent
      oldParent->left = node->right;
      if(oldParent->left != NULL) oldParent->left->sibling = oldParent->right;
      if(oldParent->right != NULL) oldParent->right->sibling = oldParent->left;
      //parent becomes the right child of node
      node->right = oldParent;
      oldParent->parent = node;
      oldParent->sibling = node->left;
      if(oldParent->left != NULL) node->left->sibling = oldParent;
      node = node->right; //node is now the child of node (what used to be the parent of node)
      display();
    }
    if(parent == grandpa->left && node == parent->left) // parent left node left
    {
      display();
      cout << "case 5" << endl;
      //5 - (happens after case 4) - grandparent and uncle are black
      //grandparent becomes red, parent becomes black
      grandpa->color = "red";
      parent->color = "black";
      //parent takes the place of grandpa
      if(grandpa->parent != NULL)
      {
        if(grandpa == grandpa->parent->right) grandpa->parent->right = parent; //if grandpa was right of his parent
        else grandpa->parent->left = parent; //grandpa was left of his parent
        parent->sibling = grandpa->sibling;
        if(grandpa->sibling->sibling != NULL) grandpa->sibling->sibling = parent;
      }
      parent->parent = grandpa->parent;
      //right child of parent transfers to left child of grandpa
      grandpa->left = parent->right;
      if(parent->right != NULL) parent->right->sibling = grandpa->right;
      grandpa->right->sibling = parent->right;
      //right child of parent becomes left child of grandpa
      parent->right = grandpa;
      grandpa->sibling = node;
      grandpa->parent = parent;
      node->sibling = grandpa;
      if(root == grandpa) root = parent; //if granpa was old root parent is now root
      display();
    }
    if(parent == grandpa->right && node == parent->right) //parent rght node right
    {
      display();
      cout << "case 5" << endl;
      //5 - (happens after case 4) - grandparent and uncle are black
      //grandparent becomes red, parent becomes black
      grandpa->color = "red";
      parent->color = "black";
      //parent takes the place of grandpa
      if(grandpa->parent != NULL)
      {
        if(grandpa == grandpa->parent->right) //if grandpa was right of his parent
        {
          grandpa->parent->right = parent;  
          parent->sibling = grandpa->parent->left;
        }
        else
        {
          grandpa->parent->left = parent;
          parent->sibling = grandpa->parent->right;
        }
        parent->parent = grandpa->parent;
      }
      //left child of parent transfers to right child of grandpa
      grandpa->right = parent->left;
      if(parent->left != NULL) parent->left->sibling = grandpa->left;
      grandpa->left->sibling = parent->left;
      //left child of parent becomes grandpa
      parent->left = grandpa;
      //fix family tree stuff
      grandpa->sibling = node;
      grandpa->parent = parent;
      node->sibling = grandpa;
      if(root == grandpa) //if granpa was old root
      {
        root = parent;
        parent->parent = NULL;
      }
      display();
    }
  }
}

//3. parent and uncle of node are red - change parent and uncle to black, grandparent to red
void Tree::caseThree(Node* node)
{
if(node != NULL && node->parent != NULL && node->parent->sibling != NULL && node->parent != root && strcmp(node->parent->color, "red") == 0 && strcmp(node->parent->sibling->color, "red") == 0)
  {
    cout << "current node is " << node->number << endl;
    cout << "parent is " << node->parent->number << endl;
    cout << "uncle is " << node->parent->sibling->number << endl;
    //cout << ""
    node->parent->color = "black"; 
    if(node->parent->sibling != NULL) node->parent->sibling->color = "black"; //uncle
    Node* grandpa = node->parent->parent;
    if(grandpa != NULL && grandpa != root) grandpa->color = "red";
    display();
    caseThree(grandpa);
  }
  else cout << "does not meet conditions" << endl;
}

void Tree::display()
{
  display_impl(root, 0);
}

//displays tree visually using tabs
void Tree::display_impl(Node* current, int level)
{ 
  if(current == NULL && level == 0)
  {
    cout << "Tree is empty :(" << endl;
    return;
  }
  else if(current != NULL)
  {
    display_impl(current->right, level + 1);
    printTabs(level);
    if(strcmp(current->color, "red") == 0) cout << "\033[31m";
    cout << current->number << endl;
    cout << "\033[37m";
    display_impl(current->left, level + 1);
  }
  else return;
}

//prints tabs according to level of tree that value is at
void Tree::printTabs(int level)
{
  for(int i = 0; i < level; i++)
  {
    cout << "     ";    
  }
}