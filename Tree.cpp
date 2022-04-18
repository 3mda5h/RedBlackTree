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
  insert_impl(root, NULL, number); 
}

//insert a node into tree maintaining red black property
void Tree::insert_impl(Node* current, Node* parent, int number)
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
        insert_impl(current->right, current, number);
      }
      //2. parent is red
      else if(strcmp(current->color, "red") == 0) //insert new node in empty slot
      {
        Node* newNode = new Node();
        newNode->number = number;
        current->right = newNode;
        if(current->left != NULL) newNode->sibling = current->left;
      }
      //3. parent and uncle are red - change parent and uncle to black, grandparent to red
      else
      {
        
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
        Node* newNode = new Node();
        newNode->number = number;
        current->left = newNode;
        if(current->right != NULL) newNode->sibling = current->right;
      }
    }
  }
}