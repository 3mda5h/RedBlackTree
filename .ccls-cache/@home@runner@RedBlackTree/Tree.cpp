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
        current->right = newChild;
        if(strcmp(current->color, "black") != 0) fixInsert(newChild);  //else: case 2. parent is red, don't need to fix colors
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
        current->left = newChild;
        if(strcmp(current->color, "black") != 0) fixInsert(newChild);
      }
    }
  }
}

//re-establish red-black property
void Tree::fixInsert(Node* node)
{
  Node* parent = node->parent;
  Node* grandpa = parent->parent;
  Node* uncle;
  if(parent == grandpa->left) uncle = grandpa->right;
  else uncle = grandpa->left; 
  //3. parent and uncle of node are red - change parent and uncle to black, grandparent to red
  if(strcmp(node->color, "red") == 0 && strcmp(parent->color, "red") == 0 && uncle != NULL && strcmp(uncle->color, "red") == 0)
  {    
    caseThree(node);
    //fix tree higher up
    Node* current = grandpa;
    while(current != NULL && current->parent != NULL && current->parent->parent != NULL)
    {
      fixInsert(current);
      current = current->parent;
    }
  }
  else if(strcmp(node->color, "red") == 0 && strcmp(parent->color, "red") == 0 && (uncle == NULL || strcmp(uncle->color, "black") == 0)) //NULL is black
  {
    //4. uncle is black 
    if(parent == grandpa->left && node == parent->right) //parent is left of grandpa and node is right of parent
    {
      //tree rotation - node goes to parent's spot 
      grandpa->left = node;
      node->parent = grandpa;
      //left child of node becomes right child of parent
      parent->right = node->left;
      if(parent->right != NULL) parent->right->parent = parent;
      //parent becomes the left child of node
      node->left = parent;
      parent->parent = node;
      //update parent and node pointers
      node = node->left; 
      parent = node->parent;
      //update uncle
      if(parent == grandpa->left) uncle = grandpa->right;
      else uncle = grandpa->left;
    }
    else if(parent == grandpa->right && node == parent->left) //parent is right of grandpa and node is left of parent
    {
      //tree rotation - node goes to parent's spot 
      grandpa->right = node;
      node->parent = grandpa;
      //right child of node becomes left child of parent
      parent->left = node->right;
      if(parent->left != NULL) parent->left->parent = parent;
      //parent becomes the right child of node
      node->right = parent;
      parent->parent = node;
      //update parent and node pointers
      node = node->right; 
      parent = node->parent;
      //update uncle
      if(parent == grandpa->left) uncle = grandpa->right;
      else uncle = grandpa->left;
    }
    if(node->parent == grandpa->left && node == parent->left) // parent left node left
    {
      //5 - (happens after case 4) - grandparent and uncle are black
      //grandparent becomes red, parent becomes black
      grandpa->color = "red";
      parent->color = "black";
      //parent takes the place of grandpa
      if(grandpa->parent != NULL)
      {
        if(grandpa == grandpa->parent->right) grandpa->parent->right = parent; //if grandpa was right of his parent
        else grandpa->parent->left = parent; //grandpa was left of his parent
      }
      parent->parent = grandpa->parent;
      //right child of parent transfers to left child of grandpa
      grandpa->left = parent->right;
      if(grandpa->left != NULL) grandpa->left->parent = grandpa;
      //fix family tree stuff
      parent->right = grandpa;
      grandpa->parent = parent;
      if(root == grandpa) root = parent; //if granpa was old root parent is now root
    }
    if(node->parent == grandpa->right && node == parent->right) //parent rght node right
    {
      //5 - (happens after case 4) - grandparent and uncle are black
      //grandparent becomes red, parent becomes black
      grandpa->color = "red";
      parent->color = "black";
      //parent takes the place of grandpa
      if(grandpa->parent != NULL)
      {
        if(grandpa == grandpa->parent->right) grandpa->parent->right = parent; //if grandpa was right of his parent
        else grandpa->parent->left = parent; //grandpa was left of his parent
      }
      parent->parent = grandpa->parent;
      //left child of parent transfers to right child of grandpa
      grandpa->right = parent->left;
      if(grandpa->right != NULL) grandpa->right->parent = grandpa;
      //fix family tree stuff
      parent->left = grandpa;
      grandpa->parent = parent;
      if(root == grandpa) root = parent; //if granpa was old root parent is now root
    }
  }
}

//3. parent and uncle of node are red - change parent and uncle to black, grandparent to red
void Tree::caseThree(Node* node)
{
  if(node != NULL && node->parent != NULL && node->parent->parent != NULL)
  {
    Node* grandpa = node->parent->parent;
    Node* uncle;
    if(node->parent == grandpa->left) uncle = grandpa->right;
    else uncle = grandpa->left;
    if(uncle != NULL && strcmp(node->parent->color, "red") == 0 && strcmp(uncle->color, "red") == 0)
    {
      node->parent->color = "black";       
      uncle->color = "black";
      if(grandpa != NULL && grandpa != root) grandpa->color = "red";
      caseThree(grandpa);
    }
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