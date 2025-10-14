#ifndef SSAK_UTIL_HPP
#define SSAK_UTIL_HPP

#include <initializer_list>
#include <list>

namespace ssak {

template<typename T>
class tree {
  public:

  tree(T v) {
    this->node_vec.push_back({v, this});
  }

  class node {
    public:
    node(T val, tree* tree_ptr) : val(val), tree_ptr(tree_ptr), first_child(nullptr), next_sibling(nullptr) {}
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
    bool operator==(const node& rhs) const {
      return this->val == rhs.val;
    }
    bool operator==(const T& rhs) const {
      return this->val == rhs;
    }
    T& operator*() {
      return this->val;
    }
    node& get_first_child() {
      return *(this->first_child);
    }
    bool is_leaf() const {
      return this->first_child == nullptr;
    }

    class SiblingItr {
      public:
      SiblingItr(node* start_node) : current_node(start_node) {}
      node& operator*() {return *(this->current_node);}
      node& operator++() {
        this->current_node = this->current_node->next_sibling;
        return *current_node;
      }
      node& operator++(int) {
        node *prev_node = this->current_node;
        ++(*this);
        return *prev_node;
      }
      bool operator==(const SiblingItr& rhs) const {
        return this->current_node == rhs->current_node;
      }
      bool operator==(const node& rhs) const {
        return this->current_node == &rhs;
      }
      bool done() const {
        return this->current_node == nullptr;
      }
      private:
      node* current_node;
    };

    SiblingItr sibling_itr() {return SiblingItr(this);}

    private:
    node& add_node(T& v) {
      node new_node(v, this->tree_ptr);
      tree_ptr->node_vec.push_back(new_node);
      return tree_ptr->node_vec.back();
    }
    node* traverse_row(node* n) {
      node* node_itr = n;
      while (node_itr->next_sibling != nullptr) node_itr = node_itr->next_sibling;
      return node_itr;
    }
    T val;
    node* first_child;
    node* next_sibling;
    tree* tree_ptr;
  };
  
  node& get_root() {return this->node_vec.front();}
  
  private:
  std::list<node> node_vec;
  //ssak::forward_list<node> node_vec;
};

} // namespace ssak
#endif