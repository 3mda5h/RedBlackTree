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

//insert a node into tree maintaining red black property
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

void Tree::fixInsert(Node* node)
{
  Node* grandpa = node->parent->parent;
  //3. parent and uncle of node are red - change parent and uncle to black, grandparent to red
  if(strcmp(node->parent->color, "red") == 0 && node->parent->sibling != NULL && strcmp(node->parent->sibling->color, "red") == 0)
  {
    caseThree(node);
    cout << "case 3" << endl;
  }
  //4. uncle is black 
  else if(node->parent->sibling == NULL || strcmp(node->parent->sibling->color, "black") == 0) //NULL is black
  {
    if(node->parent == grandpa->left && node == node->parent->right) //parent is left of grandpa and node is right of parent
    {
    cout << "case 4 heheh" << endl;
      //tree rotation - node goes to parent's spot 
      Node* oldParent = node->parent;
      grandpa->left = node;
      node->parent = grandpa;
      node->sibling = grandpa->right;
      //left child of node becomes right child of parent
      oldParent->right = node->left;
      //parent becomes the left child of node
      node->left = oldParent;
      oldParent->parent = node;
      oldParent->sibling = node->right; 
    }
    if(grandpa != NULL && node->parent == grandpa->left && node == node->parent->left) // parent left node left
    {
      
      cout << "case 5" << endl;
      //5 - after case 4 - grandparent and uncle are black
      //grandparent becomes red, parent becomes black
      grandpa->color = "red";
      node->parent->color = "black";
      //parent takes the place of grandpa
      if(grandpa->parent != NULL)
      {
        if(grandpa == grandpa->parent->right) grandpa->parent->right = node->parent; //if grandpa was right of his parent
        else grandpa->parent->left = node->parent;
        node->parent->parent = grandpa->parent;
      }
      //right child of parent transfers to left child of grandpa
      grandpa->left = node->parent->right;
      //right child of parent becomes grandpa
      node->parent->right = grandpa;
      //fix family tree stuff
      grandpa->sibling = node;
      grandpa->parent = node->parent;
      node->sibling = grandpa;
      if(root == grandpa) //if granpa was old root
      {
        root = node->parent;
        node->parent->parent = NULL;
      }    
    }
    else if(grandpa != NULL && node->parent == grandpa->right && node == node->parent->left) //parent is right of grandpa and node is left of parent
    {
    cout << "case 4 heheh" << endl;
      //tree rotation - node goes to parent's spot 
      Node* oldParent = node->parent;
      grandpa->right = node;
      node->parent = grandpa;
      node->sibling = grandpa->left;
      //right child of node becomes left child of parent
      oldParent->left = node->right;
      //parent becomes the right child of node
      node->right = oldParent;
      oldParent->parent = node;
      oldParent->sibling = node->left;
    }
    if(node->parent == grandpa->right && node == node->parent->right) //parent rght node right
    {
      cout << "case 5" << endl;
      //5 - after case 4 - grandparent and uncle are black
      //grandparent becomes red, parent becomes black
      grandpa->color = "red";
      node->parent->color = "black";
      //parent takes the place of grandpa
      if(grandpa->parent != NULL)
      {
        if(grandpa == grandpa->parent->right) grandpa->parent->right = node->parent; //if grandpa was right of his parent
        else grandpa->parent->left = node->parent;
        node->parent->parent = grandpa->parent;
      }
      //left child of parent transfers to right child of grandpa
      grandpa->right = node->parent->left;
      //left child of parent becomes grandpa
      node->parent->left = grandpa;
      //fix family tree stuff
      grandpa->sibling = node;
      grandpa->parent = node->parent;
      node->sibling = grandpa;
      if(root == grandpa) //if granpa was old root
      {
        root = node->parent;
        node->parent->parent = NULL;
      }
    }
  }
}

//3. parent and uncle of node are red - change parent and uncle to black, grandparent to red
void Tree::caseThree(Node* node)
{
  if(node != NULL && node->parent != NULL && node->parent != root)
  {
    node->parent->color = "black"; 
    node->parent->sibling->color = "black"; //uncle
    Node* grandpa = node->parent->parent;
    if(grandpa != NULL && grandpa != root) grandpa->color = "red";
    caseThree(grandpa);
  }
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