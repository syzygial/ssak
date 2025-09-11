#ifndef SSAK_UTIL_HPP
#define SSAK_UTIL_HPP

#include <vector>

template<typename T>
class tree {
  public:
  class node {
    public:
    node(T val, tree* tree_ptr) : val(val), tree_ptr(tree_ptr) {}
    node& add_child(T v) {
      node& new_node = this->add_node(v);
      if (this->first_child == nullptr) {
        this->first_child = &new_node;
      }
      else {
        node* last_child = this->traverse_row(this->first_child);
        last_child->next_sibling = &new_node;
      }
      return new_node;
    }
    node& add_sibling(T v) {
      node& new_node = this->add_node(v);
      if (this->next_sibling == nullptr) {
        this->next_sibling = &new_node;
      }
      else {
        node *last_sibling = this->traverse_row(this->next_sibling);
        last_sibling->next_sibling = &new_node;
      }
      return new_node;
    }

    private:
    node& add_node(T& v) {
      tree_ptr->node_vec.push_back(v);
      return tree_ptr->node_vec.back();
    }
    node* traverse_row(node& n) {
      node* node_itr = &n;
      while (node_itr->next_sibling != nullptr) node_itr++;
      return node_itr;
    }
    T val;
    T* first_child;
    T* next_sibling;
    tree* tree_ptr;
  };

  private:
  std::vector<node> node_vec;
};

#endif