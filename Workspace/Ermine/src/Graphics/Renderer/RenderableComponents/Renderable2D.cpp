#include "stdafx.h"
#include "Renderable2D.h"

#include "EventSystem/Event.h"

#include "2DPrimitives/Constructs/VertexTextured.h"
#include "2DPrimitives/Constructs/Quad.h"

#include "EventSystem/Components/BroadcastComponent.h"

Ermine::Renderable2D::Renderable2D(std::shared_ptr<Ermine::Actor2DBase> Ptr)
    :
    Actor_Bound(Ptr)
{
    //Well We Would Love To Recieve Event Tick Right..
    Object::RecieveEvents(true, Ermine::EventType::OnRenderTickEvent);
    Object::RecieveEvents(true, Ermine::EventType::OnBeginEvent);
}

Ermine::Renderable2D::~Renderable2D()
{
    //This Destructor Is Not In Use As Renderable Does Not Explicitly Manage Any Memory.. Useful For Children Though
}

Ermine::Renderable2D::Renderable2D(Renderable2D & rhs)
    :
    Object(rhs)
{
    auto ForeignLock = rhs.GetObjectMutex();
    auto Lock = Object::GetObjectMutex();

    CopyHelper(rhs);
}

Ermine::Renderable2D& Ermine::Renderable2D::operator=(Renderable2D& rhs)
{
    auto ForeignLock = rhs.GetObjectMutex();
    auto Lock = Object::GetObjectMutex();
    
    Object::operator=(rhs);
    CopyHelper(rhs);
    return *this;
}

Ermine::Renderable2D::Renderable2D(Renderable2D&& rhs)
    :
    Object(std::move(rhs))
{
    auto ForeignLock = rhs.GetObjectMutex();
    auto Lock = Object::GetObjectMutex();

    MoveHelper(std::move(rhs));
}

Ermine::Renderable2D& Ermine::Renderable2D::operator=(Renderable2D&& rhs)
{
    auto ForeignLock = rhs.GetObjectMutex();
    auto Lock = Object::GetObjectMutex();

    Object::operator=(std::move(rhs));
    MoveHelper(std::move(rhs));
    return *this;
}

void Ermine::Renderable2D::SetVertexArray(VertexArray& Vao)
{ 
    auto Lock = GetObjectMutex();
    this->Vao = std::make_shared<Ermine::VertexArray>(Vao);
}
std::unique_ptr<Ermine::Renderable2D> Ermine::Renderable2D::Generate(std::shared_ptr<Actor2DBase> Act)
{
    std::unique_ptr<Renderable2D> Module(new Renderable2D(Act));

    std::shared_ptr<void*> st = std::make_shared<void*>();
    Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::OnBeginEvent>(st));

    return Module;
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

void Ermine::Renderable2D::Clear()
{
    Vao.reset();
    Mat.reset();
}

void Ermine::Renderable2D::Initialize()
{
    auto ForeignLock = Actor_Bound->GetObjectMutex();
    auto Lock = Object::GetObjectMutex();

    if (ObjectInitialized == false)
    {
        //Get The Model Matrix Associated With The Actor
        ModelMatrix = Actor_Bound->GetModelMatrix();
        
        //Get The Tools To Create And Bind Opengl Vertex Arrays In The Future
        VertexBufferBuffer = Actor_Bound->GenerateModelSpaceVertexBuffer();
        Specification = Actor_Bound->GetVertexArraySpecification();

        //Get The Tools To Create And Bind Opengl Index Buffer In The Future..
        IndexBufferBuffer = Actor_Bound->GenerateModelSpaceIndices();

        //Get The Material For The Actor And Initialize It inside The renderable..
        Mat = Actor_Bound->GetMaterial();
    }
}

void Ermine::Renderable2D::Refresh(float DeltaTime)
{
    auto ForeignLock = Actor_Bound->GetObjectMutex();
    auto Lock = GetObjectMutex();

    if (ObjectInitialized == true)
    {
        if (Actor_Bound->GetObjectHealth() == Ermine::ObjectStatus::StatusMarkedForDeletion || Object::GetObjectHealth() == Ermine::ObjectStatus::StatusMarkedForDeletion)
        {
            Object::SetObjectHealth(Ermine::ObjectStatus::StatusMarkedForDeletion);
            return;
        }
        //The Renderable Is Only Responsible For Updating Model Matrix Nothing Else.. All Other Properties Are Not Changed In The Midst of Execution..
        ModelMatrix = Actor_Bound->GetModelMatrix() + (Actor_Bound->GetModelMatrix() * DeltaTime);
    }
}

std::shared_ptr<Ermine::Actor2DBase> Ermine::Renderable2D::GetBoundActor()
{
    auto Lock = GetObjectMutex();
    return Actor_Bound;
}

glm::mat4 Ermine::Renderable2D::GetModelMatrix()
{
    auto Lock = GetObjectMutex();
    return ModelMatrix;
}

void Ermine::Renderable2D::BindRenderable()
{
    if (RenderableInitialized == false)
    {
        Vao = std::make_shared<VertexArray>(VertexBufferBuffer, IndexBufferBuffer);
        Vao->SetVertexAttribArray(Specification); //Set The Array Specifications Right..
        Vao->Bind();
        Mat->Bind();

        RenderableInitialized = true;
    }
    else
    {
        Vao->Bind();
        Mat->Bind();
    }
}

void Ermine::Renderable2D::OnRenderTickEventRecieved(float DeltaTime)
{
    auto Lock = GetObjectMutex();
    
    if (ObjectInitialized == true)
    {
        Refresh(DeltaTime);
        //std::thread Obj(&Ermine::Renderable2D::Refresh, this);
        //Obj.detach();
    }
}

void Ermine::Renderable2D::OnBeginEvent(std::shared_ptr<void> Packet)
{
    auto Lock = GetObjectMutex();

    if (ObjectInitialized == false)
    {
        //Since We Have Already Recieved Our Event Begin. No Need To Recieve Them In The Future
        Object::RecieveEvents(false, Ermine::EventType::OnBeginEvent);

        //Call The Initialize Function Which Initializes Stuff..
        Initialize(); //This Is A Blocking Function As Without It..It Is Difficult To Know  When The Initialization Is Complete.. An acceptable Compromise I Believe.. For Now..

        ObjectInitialized = true;
    }
}

#pragma region Helpers
void Ermine::Renderable2D::CopyHelper(Ermine::Renderable2D& rhs)
{
    auto ForeignLock = rhs.GetObjectMutex();
    auto Lock = GetObjectMutex();

    VertexBufferBuffer = rhs.VertexBufferBuffer;
    IndexBufferBuffer = rhs.IndexBufferBuffer;

    ModelMatrix = rhs.ModelMatrix;

    Vao = rhs.Vao;
    Mat = rhs.Mat;
    Specification = rhs.Specification;

    DrawMode = rhs.DrawMode;

    Actor_Bound = rhs.Actor_Bound;

    //The Initialize Function Will Not Be Called As The Renderable Form Which We Are Initializing Was Already Initialized Hopefullty Before This Function Was Called..
    InitializeFunctionPtr = rhs.InitializeFunctionPtr;
    CallDefaultInitializer = rhs.CallDefaultInitializer;

    RefreshFunction = rhs.RefreshFunction;
    CallDefaultRefresh = rhs.CallDefaultRefresh;

    ObjectInitialized = rhs.ObjectInitialized;
    RenderableInitialized = rhs.RenderableInitialized;
}
void Ermine::Renderable2D::MoveHelper(Ermine::Renderable2D&& rhs)
{
    auto ForeignLock = rhs.GetObjectMutex();
    auto Lock = GetObjectMutex();

    VertexBufferBuffer = std::move(rhs.VertexBufferBuffer);
    IndexBufferBuffer = std::move(rhs.IndexBufferBuffer);

    ModelMatrix = std::move(rhs.ModelMatrix);

    Vao = std::move(rhs.Vao);
    Mat = std::move(rhs.Mat);
    Specification = std::move(rhs.Specification);

    DrawMode = std::move(rhs.DrawMode);

    Actor_Bound = std::move(rhs.Actor_Bound);

    //The Initialize Function Will Not Be Called As The Renderable Form Which We Are Initializing Was Already Initialized Hopefullty Before This Function Was Called..
    InitializeFunctionPtr = std::move(rhs.InitializeFunctionPtr);
    CallDefaultInitializer = std::move(rhs.CallDefaultInitializer);

    RefreshFunction = std::move(rhs.RefreshFunction);
    CallDefaultRefresh = std::move(rhs.CallDefaultRefresh);

    ObjectInitialized = std::move(rhs.ObjectInitialized);
    RenderableInitialized = std::move(rhs.RenderableInitialized);
}
#pragma endregion