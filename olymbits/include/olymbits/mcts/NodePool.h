#ifndef OLYMBITS_MCTS_NODE_POOL_H
#define OLYMBITS_MCTS_NODE_POOL_H

#include <memory>
#include <stack>

#include <olymbits/mcts/Node.h>

namespace olymbits::mcts
{
    class NodePool
    {
    // constructor *************************************************************
    public:
        NodePool(const NodePool& cr_node_pool) = delete;

        NodePool& operator=(const NodePool& cr_node_pool) = delete;

    private:
        NodePool();
        
        ~NodePool() = default;

    // instance ****************************************************************
    private:
        static NodePool& get_node_pool_instance();

    // node ********************************************************************
    public:
        static std::shared_ptr<Node> get_node();

        static void give_node(std::shared_ptr<Node> sp_node);

    private:
        std::stack<std::shared_ptr<Node>> __node_stack;
    };
}

#endif
