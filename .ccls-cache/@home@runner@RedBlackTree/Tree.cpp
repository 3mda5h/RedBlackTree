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
        if(current->left != NULL) newChild->sibling = current->left;
        current->right = newChild;
        if(strcmp(current->color, "red") != 0) fixInsert(newChild);  //else: case 2. parent is red, don't need to fix colors
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
        if(strcmp(current->color, "red") != 0) fixInsert(newChild);
      }
    }
  }
}

void Tree::fixInsert(Node* child)
{
  //3. parent and uncle of child are red - change parent and uncle to black, grandparent to red
  if(strcmp(child->parent->color, "red") == 0 && strcmp(child->parent->sibling->color, "red") == 0)
  {
    caseThree(child);
  }
  //4. uncle is black 
  else if(child->parent->sibling == NULL || strcmp(child->parent->sibling->color, "black") == 0) //NULL is black
  {
    Node* grandpa = child->parent->parent;
    if(grandpa != NULL && child->parent == grandpa->left && child == child->parent->right) //parent is left of grandpa and child is right of parent
    {
      //tree rotation - child goes to parent's spot 
      Node* oldParent = child->parent;
      grandpa->left = child;
      child->parent = grandpa;
      child->sibling = grandpa->right;
      //left child of child becomes right child of parent
      oldParent->right = child->left;
      //parent becomes the left child of child
      child->left = oldParent;
      oldParent->parent = child;
      oldParent->sibling = child->right; 
    }
    else if(grandpa != NULL && child->parent == grandpa->right && child == child->parent->left) //parent is right of grandpa and child is left of parent
    {
      //tree rotation - child goes to parent's spot 
      Node* oldParent = child->parent;
      grandpa->right = child;
      child->parent = grandpa;
      child->sibling = grandpa->left;
      //right child of child becomes left child of parent
      oldParent->left = child->right;
      //parent becomes the right child of child
      child->right = oldParent;
      oldParent->parent = child;
      oldParent->sibling = child->left; 
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
    else cout << "\033[37m";
    cout << current->number << endl;
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