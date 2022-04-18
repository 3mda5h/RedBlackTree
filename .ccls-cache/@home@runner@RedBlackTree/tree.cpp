#include <iostream>
#include "Tree.h"

using namespace std;

Tree::Tree()
{
  root = NULL;
}

void Tree::insert(int number)
{
  insert_impl(root, number); 
}


//insert a node into tree maintaining bst property
void Tree::insert_impl(Node* current, int number)
{
  if(root == NULL)
  {
    Node* newRoot = new Node();
    newRoot->number = number;
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
          Node* newNode = new Node();
          newNode->number = number;
          current->right = newNode;
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
        }
      }
    }
}