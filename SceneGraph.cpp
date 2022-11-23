#include "SceneGraph.h"

bool SceneGraph::Initialise(void)
{
    return false;
}

void SceneGraph::Update(const Matrix& worldTransformation)
{
}

void SceneGraph::Render(void)
{
}

void SceneGraph::Shutdown(void)
{
}

void SceneGraph::Add(SceneNodePointer node)
{
}

void SceneGraph::Remove(SceneNodePointer node)
{
}

SceneNodePointer SceneGraph::Find(wstring name)
{
    return SceneNodePointer();
}
