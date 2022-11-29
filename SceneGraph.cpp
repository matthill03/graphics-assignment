#include "SceneGraph.h"

bool SceneGraph::Initialise(void)
{
    for (int i = 0; i < _children.size(); i++) {
        if (!_children[i]->Initialise()) {
            return false;
        }
    }
    return true;
}

void SceneGraph::Update(const Matrix& worldTransformation)
{
    _cumulativeWorldTransformation = _thisWorldTransformation * worldTransformation;

    for (int i = 0; i < _children.size(); i++) {
        _children[i]->Update(_cumulativeWorldTransformation);
    }
}

void SceneGraph::Render(void)
{
    for (int i = 0; i < _children.size(); i++) {
        _children[i]->Render();
    }
}

void SceneGraph::Shutdown(void)
{
    for (int i = 0; i < _children.size(); i++) {
        _children[i]->Shutdown();
    }
}

void SceneGraph::Add(SceneNodePointer node)
{
    _children.push_back(node);
}

void SceneGraph::Remove(SceneNodePointer node)
{
    for (int i = 0; i < _children.size(); i++) {
        _children[i]->Remove(node);

        if (_children[i] == node) {
            _children.erase(_children.begin() + (i - 1));
        }
    }
}

SceneNodePointer SceneGraph::Find(wstring name)
{
    if (_name == name) {
        
        return SceneNodePointer(this);
    }
    else {
        for (int i = 0; i < _children.size(); i++) {
            if (_children[i]->Find(name)) {
                return SceneNodePointer(_children[i]);
            }
        }
    }
    return nullptr;
}
