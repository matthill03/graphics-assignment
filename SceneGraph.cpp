#include "SceneGraph.h"

// Implement Initialise()
bool SceneGraph::Initialise(void)
{
    // For all children of scene graph
    for (int i = 0; i < _children.size(); i++) {
        // Call Initialise on each child
        // If Initialise returns false, also return false
        if (!_children[i]->Initialise()) {
            return false;
        }
    }
    // else return true
    return true;
}

// Implement Update()
void SceneGraph::Update(const Matrix& worldTransformation)
{
    // Set the cumulative world transfromation to node world transformation to other nodes world transformation
    // that has been passed in
    _cumulativeWorldTransformation = _thisWorldTransformation * worldTransformation;

    // recursivly call update on all children
    for (int i = 0; i < _children.size(); i++) {
        _children[i]->Update(_cumulativeWorldTransformation);
    }
}

// Implement Render()
void SceneGraph::Render(void)
{
    // Recursivly call render on all children
    for (int i = 0; i < _children.size(); i++) {
        _children[i]->Render();
    }
}

// Implement Shutdown()
void SceneGraph::Shutdown(void)
{
    // Recursivly call shutdown on all children
    for (int i = 0; i < _children.size(); i++) {
        _children[i]->Shutdown();
    }
}

// Implement Add()
void SceneGraph::Add(SceneNodePointer node)
{
    // Add node that has been passed in onto the back of the children vector (list)
    _children.push_back(node);
}

// Implement Remove()
void SceneGraph::Remove(SceneNodePointer node)
{
    // Recursivly call remove on all children nodes
    for (int i = 0; i < _children.size(); i++) {
        _children[i]->Remove(node);

        // if current child node is the node that has been passed in 
        if (_children[i] == node) {

            // take out the child from the vector, using the begining of the vector minus the current index - 1
            // (index - 1) ensures that it takes out the value below i as vectors start at index 0
            _children.erase(_children.begin() + (i - 1));
        }
    }
}

// Implement Find()
SceneNodePointer SceneGraph::Find(wstring name)
{
    // if the current node is the one being searched for
    // return pointer to current node
    if (_name == name) {
        
        return SceneNodePointer(this);
    }
    else {
        // else recursivly call find on all child nodes
        for (int i = 0; i < _children.size(); i++) {

            // if current child node is the one being searched for
            // return a pointer to that child node
            if (_children[i]->Find(name)) {
                return SceneNodePointer(_children[i]);
            }
        }
    }
    // if node does not exist then return a null pointer
    return nullptr;
}
