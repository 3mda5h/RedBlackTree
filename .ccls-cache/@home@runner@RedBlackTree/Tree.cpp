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
      }
    }
  }
}

void fixInsertion(Node* newChild)
{
  //3. parent and uncle of newChild are red - change parent and uncle to black, grandparent to red
  if(strcmp(newChild->parent->color, "red") == 0 && strcmp(newChild->parent->sibling->color, "red") == 0)
  {
    caseThree(newChild);
  }
  //4. uncle is black 
  else if(strcmp(newChild->sibling->color, "black") == 0)
  {
    
  }
}

//3. parent and uncle of newChild are red - change parent and uncle to black, grandparent to red
void caseThree(Node* newChild)
{
  if(newChild != NULL)
  {
    newChild->parent->color = "black"; 
    newChild->parent->sibling->color = "black"; //uncle
    newChild->parent->parent->color = "red"; //grandpa
    caseThree(newChild->parent->parent);
  }
}