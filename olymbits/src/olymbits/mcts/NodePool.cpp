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
    std::shared_ptr<Node> NodePool::get_node()
    {
        NodePool& r_node_pool(get_node_pool_instance());

        if (!r_node_pool.__node_stack.empty())
        {
            std::shared_ptr<Node> sp_node(
                std::move(r_node_pool.__node_stack.top())
            );
            r_node_pool.__node_stack.pop();
            return sp_node;
        }

        return std::make_unique<Node>();
    }

    void NodePool::give_node(std::shared_ptr<Node> sp_node)
    {
        get_node_pool_instance().__node_stack.push(std::move(sp_node));
    }
}
