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
        std::unique_ptr<Node> get_node();

        void give_node(std::unique_ptr<Node>&& up_node);

    private:
        std::stack<std::unique_ptr<Node>> __node_stack;
    };
}

#endif
