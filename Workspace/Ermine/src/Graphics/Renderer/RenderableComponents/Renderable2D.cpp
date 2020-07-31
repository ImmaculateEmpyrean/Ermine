#include "stdafx.h"
#include "Renderable2D.h"

Ermine::Renderable2D::Renderable2D()
{}

Ermine::Renderable2D::Renderable2D(VertexArray Vao)
    :
    Vao(std::make_shared<Ermine::VertexArray>(std::move(Vao)))
 {}

Ermine::Renderable2D::Renderable2D(Material Mat)
    :
    Mat(std::make_shared<Ermine::Material>(std::move(Mat)))
{}

Ermine::Renderable2D::Renderable2D(VertexArray Vao, Material Mat)
    :
    Vao(std::make_shared<Ermine::VertexArray>(std::move(Vao))),
    Mat(std::make_shared<Ermine::Material>(std::move(Mat)))
{}

Ermine::Renderable2D::~Renderable2D()
{}

void Ermine::Renderable2D::SetVertexArray(VertexArray Vao)
{ this->Vao.reset(new VertexArray(Vao));}
std::shared_ptr<Ermine::VertexArray> Ermine::Renderable2D::GetVertexArray()
{ return Vao; }

void Ermine::Renderable2D::SetMaterial(Material Mat)
{ this->Mat.reset(new Material(Mat)); }
std::shared_ptr<Ermine::Material> Ermine::Renderable2D::GetMaterialBeingUsed()
{ return Mat; }

void Ermine::Renderable2D::Bind()
{
    Vao->Bind();
    Mat->Bind();
}

void Ermine::Renderable2D::Clear()
{
    Vao.reset();
    Mat.reset();
}
