#include <olymbits/mcts/NodePool.h>

namespace olymbits::mcts
{
    // constructor *************************************************************
    NodePool::NodePool():
        // node
        __node_stack()
    {
    }

    // instance ****************************************************************
    NodePool& NodePool::get_node_pool_instance()
    {
        static NodePool node_pool;
        return node_pool;
    }

    // node ********************************************************************
    std::unique_ptr<Node> NodePool::get_node()
    {
        NodePool& r_node_pool(get_node_pool_instance());

        if (!__node_stack.empty())
        {
            std::unique_ptr<Node> up_node(std::move(__node_stack.top()));
            __node_stack.pop();
            return up_node;
        }

        return std::make_unique<Node>();
    }

    void NodePool::give_node(std::unique_ptr<Node>&& rv_up_node)
    {
        get_node_pool_instance().__node_stack.push(std::move(rv_up_node));
    }
}
