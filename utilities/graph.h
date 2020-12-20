/*! \file graph.h
*
*  \brief a kind of flat graph based data structure
*         where nodes are linked together by relationships to other nodes
*
*
*  \author Graham Riches
*/

#pragma once



/********************************** Includes *******************************************/
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>

namespace graph
{

using int64 = long long int;

/**
 * @brief enumeration of possible child node types for each graph node
*/
enum class ChildNodes : unsigned
{
   child_a,
   child_b,
   total_children
};

/**
 * @brief template graph node structure. Each node can contain a set of other nodes as children. 
 *        If a node has children, it assumes that the data from the node is empty.
 * @tparam T template type
*/
template <typename T>
struct Node
{
   int64 key{ 0 };
   std::unique_ptr<T> data;

   std::vector<std::vector<Node<T>*>> children = std::vector<std::vector<Node<T>*>>(static_cast<size_t>(ChildNodes::total_children));  

   /********************* Constructors **********************/
   /**
   * @brief default graph node constructor
   */
   Node( void )  {};

   /**
   * @brief construct a graph node with only a key
   * @param key unique key
   */
   Node( int64 key ) : key( key ) {};

   /**
   * @brief construct a node with a key and input data
   * @param key unique key
   * @param data rvalue data
   */
   Node( int64 key, T&& data )
   {
      this->key = key;
      this->data = std::make_unique<T>( data );
   }

   /**
   * @brief move constructor moves owned resources from the argument into the node
   * @param node temporary node object
   */
   Node( Node<T>&& node )   
   {
      this->data = std::move( node.data );
      this->children = std::move( node.children );
      this->key = std::move( node.key );
   }

   /**
   * @brief deleted copy constructor as the node has managed resources that should be moved by default
   * @param node reference to a node to copy
   */
   Node( Node<T>& node ) = delete;


   /********************** Operators **********************/
   /**
   * @brief move assignment operator moves resources from other node into the host
   * @param other reference to another node
   * @return
   */
   Node<T>& operator=( Node<T>&& other ) noexcept
   {
      this->data = std::move( other.data );
      this->key = std::move( other.key );
      this->children = std::move(other.children);
      return *this;
   }

   /********************** Methods **********************/   
   /**
    * @brief append a child node a specific child branch
    * @param node pointer to the node
   */
   void append_child( ChildNodes child, Node<T>* node )
   {
      if ( (node != nullptr) && ( child != ChildNodes::total_children ) )
      {
         this->children[static_cast<int>(child)].push_back(node);
      }
   }

   /**
    * @brief get a vector of child nodes from a node
    * @param child the child branch
    * @return vector of node pointers
   */
   std::vector<Node<T>*> get_child_nodes( ChildNodes child )
   {
      return this->children[static_cast<int>(child)];
   }

   /**
    * @brief check if a node has children
    * @return true if it has children
   */
   bool has_children( void )
   {
      return ( this->children[static_cast<int>(ChildNodes::child_a)].size() > 0 ) || (this->children[static_cast<int>(ChildNodes::child_b)].size() > 0);
   }

};



/**
* @brief creates distributed graph of nodes where each node is connected by child relationships
*        to other nodes
*/
template <typename T>
struct Graph
{
   std::vector<Node<T>> nodes;

   /**
    * @brief default constructor    
   */
   Graph( void ) {};

   /**
    * @brief construct a graph structure and pre-size the nodes vector
    * @param size how many nodes to set the initial container size to.
    * @note: Graph container size is not fixed, members can be added to the vector
   */
   Graph( size_t size ) : nodes( size ) {};

   /**
    * @brief move assignment operator
    * @param graph graph to move from
    * @return 
   */
   Graph& operator =( Graph&& graph )
   {
      this->nodes = std::move(graph.nodes);
      return *this;
   }


   /**
    * @brief get a pointer to a graph node by it's identifying key O(n) max
    * @param key identifier
    * @return node pointer or nullptr if not found
   */
   Node<T>* get_node_by_key( int64 key )
   {
      auto node_iterator = std::find_if(this->nodes.cbegin(), this->nodes.cend(), [key](auto& node){ return (node.key == key); } );
      auto ptr = (node_iterator != this->nodes.cend()) ? &this->nodes[node_iterator - this->nodes.cbegin()] : nullptr;
      return ptr;
   }

   /**
    * @brief assign a set of node pointers to a nodes child a branch based on a set of id keys
    * @param node_key the key of the node to assign to
    * @param child which child branch to assign to
    * @param child_keys vector of keys to assign pointers from
   */
   void assign_child_nodes( int64 node_key, ChildNodes child, std::vector<int64> child_keys )
   {
      std::vector<Node<T>*> ptrs(child_keys.size());
      std::transform( child_keys.begin(), child_keys.end(), ptrs.begin(), [this]( int64 key ) { return this->get_node_by_key( key ); } );
      
      auto node_ptr = this->get_node_by_key(node_key);
      for (auto& node : ptrs)
      {
         node_ptr->append_child( child, node);
      }
   }

   /**
    * @brief get the root data for a node
    * @param node_key 
    * @return 
   */
   T get_node_data( int64 node_key )
   {
      auto node_ptr = this->get_node_by_key( node_key );
      return *node_ptr->data.get();      
   }

   /**
    * @brief get all possible child data combinations
    * @param node_key 
    * @return 
   */
   std::vector<std::vector<T>> get_child_combinations( const int64 node_key )
   {
      auto node_ptr = this->get_node_by_key( node_key );
      std::vector<std::vector<T>> total_combinations;
      
      /* todo this is a giant nightmare */

      return total_combinations;
   }



};


};


