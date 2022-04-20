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
        if(current->left != NULL) newChild->sibling = current->left;
        current->right = newChild;
        newChild->color = "black";
        if(strcmp(current->color, "red") != 0) fixInsert(newChild);  //else: case 2. parent is red, don't need to fix colors
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
        if(current->right != NULL) newChild->sibling = current->right;
        current->left = newChild;
        newChild->color = "black";
        if(strcmp(current->color, "red") != 0) fixInsert(newChild);
        else cout << "case 2" << endl;
      }
    }
  }
}

void Tree::fixInsert(Node* node)
{
  //3. parent and uncle of node are red - change parent and uncle to black, grandparent to red
  if(strcmp(node->parent->color, "red") == 0 && strcmp(node->parent->sibling->color, "red") == 0)
  {
    cout << "case 3 hehe" << endl;
    caseThree(node);
  }
  //4. uncle is black 
  else if(node->parent->sibling == NULL || strcmp(node->parent->sibling->color, "black") == 0) //NULL is black
  {
    cout << "case 4 heheh" << endl;
    Node* grandpa = node->parent->parent;
    if(grandpa != NULL && node->parent == grandpa->left && node == node->parent->right) //parent is left of grandpa and node is right of parent
    {
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
       //5 - left left  - after case 4 - grandparent and uncle are black
      //grandparent becomes red, parent becomes black
      node = node->left;
      grandpa->color = "red";
      node->parent->color = "black";
      //parent takes the place of grandpa
      Node* temp = grandpa;
      grandpa = node->parent;
      //right child of grandpa becomes old left child of parent
      temp->left = node->parent->right;
      //left child of parent becomes grandpa
      node->parent->right = temp;
      //fix family tree stuff
      grandpa->sibling = node;
      grandpa->parent = node->parent;
      node->sibling = grandpa;
      if(root == grandpa) root = node->parent;
    }
    else if(grandpa != NULL && node->parent == grandpa->right && node == node->parent->left) //parent is right of grandpa and node is left of parent
    {
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
      //5 - right right - after case 4 - grandparent and uncle are black
      //grandparent becomes red, parent becomes black
      node = node->right;
      grandpa->color = "red";
      node->parent->color = "black";
      //parent takes the place of grandpa
      Node* temp = grandpa;
      grandpa = node->parent;
      //right child of grandpa becomes old left child of parent
      temp->right = node->parent->left;
      //left child of parent becomes grandpa
      node->parent->left = temp;
      //fix family tree stuff
      grandpa->sibling = node;
      grandpa->parent = node->parent;
      node->sibling = grandpa;
      if(root == grandpa) root = node->parent;
    }
  }
}

//3. parent and uncle of newChild are red - change parent and uncle to black, grandparent to red
void Tree::caseThree(Node* newChild)
{
  if(newChild != NULL)
  {
    newChild->parent->color = "black"; 
    newChild->parent->sibling->color = "black"; //uncle
    newChild->parent->parent->color = "red"; //grandpa
    caseThree(newChild->parent->parent);
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
    if(strcmp(current->color, "red")) cout << "\033[31m";
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