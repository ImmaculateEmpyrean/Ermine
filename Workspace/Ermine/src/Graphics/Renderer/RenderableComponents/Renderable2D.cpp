#include "stdafx.h"
#include "Renderable2D.h"

#include "EventSystem/Event.h"

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

void Ermine::Renderable2D::SetVertexArray(VertexArray& Vao)
{ 
    auto Lock = GetObjectMutex();
    this->Vao = std::make_shared<Ermine::VertexArray>(Vao);
}
std::shared_ptr<Ermine::VertexArray> Ermine::Renderable2D::GetVertexArray()
{
    auto Lock = GetObjectMutex();
    return Vao; 
}


std::shared_ptr<Ermine::Material> Ermine::Renderable2D::GetMaterial()
{
    auto Lock = GetObjectMutex();
    return Mat;
}
void Ermine::Renderable2D::SetMaterial(Material& Mat)
{ 
    auto Lock = GetObjectMutex();
    this->Mat = std::make_shared<Ermine::Material>(Mat);
}
std::shared_ptr<Ermine::Material> Ermine::Renderable2D::GetMaterialBeingUsed()
{
    auto Lock = GetObjectMutex();
    return Mat; 
}

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

void Ermine::Renderable2D::RecieveEvents(Ermine::Event* Eve)
{
    if (ObjectInitialized)
    {
        //Start From Here.. Beware OF THE MUTEXES..
        Ermine::EventType EventType = Eve->GetEventType();
        if (EventType == Ermine::EventType::OnTickEvent)
        {
            auto Lock = GetObjectMutex();
            //First Time Initialize The Renderable Before Refreshing..
            std::call_once(CallInitializeFlag,&Ermine::Renderable2D::Initialize, this); //Well Since This IS The First Time.. How Abt We Let THe Evnt THread Handle It :> "well nothing is final.."

            //Refresh The Renderable Every Tick.. //This Will BE Done In A Seperate THread For Now..  Donot Forget To Lock The Mutex..
            //Also Creating Ridiculous Number Of HElper Threads IS An Absolutely Terrible Idea.. In The Future Switch To A Centrally Planned Refresh..
            std::thread Obj(&Ermine::Renderable2D::Refresh, this);
            Obj.detach();
        }
    }
}
