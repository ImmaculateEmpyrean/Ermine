#include "stdafx.h"
#include "Renderable2D.h"

#include "EventSystem/Event.h"

#include "2DPrimitives/Constructs/VertexTextured.h"
#include "2DPrimitives/Constructs/Quad.h"

#include "EventSystem/Components/BroadcastComponent.h"

Ermine::Renderable2D::Renderable2D(std::shared_ptr<Ermine::Actor2D> Ptr)
{
    //Well We Would Love To Recieve Event Tick Right..
    Object::RecieveEvents(true, Ermine::EventType::OnTickEvent);
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

    Vao = rhs.Vao;
    Mat = rhs.Mat;
    Specification = rhs.Specification;

    Actor_Bound = rhs.Actor_Bound;

    //The Initialize Function Will Not Be Called As The Renderable Form Which We Are Initializing Was Already Initialized Hopefullty Before This Function Was Called..
    InitializeFunctionPtr = rhs.InitializeFunctionPtr;
    CallDefaultInitializer = rhs.CallDefaultInitializer;

    RefreshFunction = rhs.RefreshFunction;
    CallDefaultRefresh = rhs.CallDefaultRefresh;

    ObjectInitialized = rhs.ObjectInitialized;
}

Ermine::Renderable2D& Ermine::Renderable2D::operator=(Renderable2D& rhs)
{
    auto ForeignLock = rhs.GetObjectMutex();
    auto Lock = Object::GetObjectMutex();
    
    Object::operator=(rhs);

    Vao = rhs.Vao;
    Mat = rhs.Mat;
    Specification = rhs.Specification;

    Actor_Bound = rhs.Actor_Bound;

    //The Initialize Function Will Not Be Called As The Renderable Form Which We Are Initializing Was Already Initialized Hopefullty Before This Function Was Called..
    InitializeFunctionPtr = rhs.InitializeFunctionPtr;
    CallDefaultInitializer = rhs.CallDefaultInitializer;

    RefreshFunction = rhs.RefreshFunction;
    CallDefaultRefresh = rhs.CallDefaultRefresh;

    ObjectInitialized = rhs.ObjectInitialized;

    return *this;
}

Ermine::Renderable2D::Renderable2D(Renderable2D&& rhs)
    :
    Object(std::move(rhs))
{
    auto ForeignLock = rhs.GetObjectMutex();
    auto Lock = Object::GetObjectMutex();

    Vao = std::move(rhs.Vao);
    Mat = std::move(rhs.Mat);
    Specification = std::move(rhs.Specification);

    Actor_Bound = std::move(rhs.Actor_Bound);

    //The Initialize Function Will Not Be Called As The Renderable Form Which We Are Initializing Was Already Initialized Hopefullty Before This Function Was Called..
    InitializeFunctionPtr  = std::move(rhs.InitializeFunctionPtr);
    CallDefaultInitializer = std::move(rhs.CallDefaultInitializer);

    RefreshFunction    = std::move(rhs.RefreshFunction);
    CallDefaultRefresh = std::move(rhs.CallDefaultRefresh);

    ObjectInitialized = std::move(rhs.ObjectInitialized);
}

Ermine::Renderable2D& Ermine::Renderable2D::operator=(Renderable2D&& rhs)
{
    auto ForeignLock = rhs.GetObjectMutex();
    auto Lock = Object::GetObjectMutex();

    Object::operator=(std::move(rhs));

    Vao = std::move(rhs.Vao);
    Mat = std::move(rhs.Mat);
    Specification = std::move(rhs.Specification);

    Actor_Bound = std::move(rhs.Actor_Bound);

    //The Initialize Function Will Not Be Called As The Renderable Form Which We Are Initializing Was Already Initialized Hopefullty Before This Function Was Called..
    InitializeFunctionPtr  = std::move(rhs.InitializeFunctionPtr);
    CallDefaultInitializer = std::move(rhs.CallDefaultInitializer);

    RefreshFunction    = std::move(rhs.RefreshFunction);
    CallDefaultRefresh = std::move(rhs.CallDefaultRefresh);

    ObjectInitialized = std::move(rhs.ObjectInitialized);

    return *this;
}

void Ermine::Renderable2D::SetVertexArray(VertexArray& Vao)
{ 
    auto Lock = GetObjectMutex();
    this->Vao = std::make_shared<Ermine::VertexArray>(Vao);
}
std::unique_ptr<Ermine::Renderable2D> Ermine::Renderable2D::Generate(std::shared_ptr<Actor2DBase> Act)
{
    if (Act->GetActorFamilyIdentifier() == Ermine::ActorFamilyIdentifier::Actor2D)
    {
        std::shared_ptr<Ermine::Actor2D> Act2D = std::dynamic_pointer_cast<Ermine::Actor2D>(Act);
        std::unique_ptr<Renderable2D> Module(new Renderable2D(Act2D));

        std::shared_ptr<void> st = std::make_shared<void*>();
        Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::OnBeginEvent>(st));

        return Module;
    }
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

void Ermine::Renderable2D::Initialize()
{
    auto Lock = Object::GetObjectMutex();

    if (ObjectInitialized == false)
    {
        //Initializes The Renderable..
        if (Actor_Bound->GetActorFamilyIdentifier() == Ermine::ActorFamilyIdentifier::Actor2D)
            GenerateActor2DRenderable(std::dynamic_pointer_cast<Ermine::Actor2D>(Actor_Bound));
    }
}

void Ermine::Renderable2D::Refresh()
{
    auto Lock = GetObjectMutex();

    if (ObjectInitialized == true)
    {
        if (Actor_Bound->GetObjectHealth() == Ermine::ObjectStatus::StatusMarkedForDeletion || Object::GetObjectHealth() == Ermine::ObjectStatus::StatusMarkedForDeletion)
        {
            Object::SetObjectHealth(Ermine::ObjectStatus::StatusMarkedForDeletion);
            return;
        }

        if (Actor_Bound->GetActorFamilyIdentifier() == Ermine::ActorFamilyIdentifier::Actor2D)
            Vao = std::make_shared<VertexArray>(CalculateModelSpaceVertexesActor2D(std::dynamic_pointer_cast<Ermine::Actor2D>(Actor_Bound)), Quad::GetModelIndices());
    }
}

std::shared_ptr<Ermine::Actor2DBase> Ermine::Renderable2D::GetBoundActor()
{
    return Actor_Bound;
}

void Ermine::Renderable2D::OnTickEventRecieved(float DeltaTime)
{
    auto Lock = GetObjectMutex();
    
    if (ObjectInitialized == true)
    {
        std::thread Obj(&Ermine::Renderable2D::Refresh, this);
        Obj.detach();
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

#pragma region Meat
void Ermine::Renderable2D::GenerateActor2DRenderable(std::shared_ptr<Ermine::Actor2D> Ptr)
{
    //Get The Lock Of The Object Since We Are Accessing The Variable..
    auto Lock = GetObjectMutex();

    static std::vector<VertexAttribPointerSpecification> Spec = {
                {3,GL_FLOAT,false},
                {3,GL_FLOAT,false},
                {2,GL_FLOAT,false},
                {1,GL_FLOAT,false}
    };

    Vao = std::make_shared<VertexArray>(CalculateModelSpaceVertexesActor2D(Ptr), Quad::GetModelIndices());
    Mat = std::make_shared<Ermine::Material>(std::filesystem::path("Shader/Vertex/Actor2DUpdatedWithRenderableTextureModuleVertexShader.vert"),
                                             std::filesystem::path("Shader/Fragment/Actor2DUpdatedWithRenderableTextureModuleFragmentShader.frag"));
    Specification = Spec;

    Actor_Bound = Ptr;
}

std::vector<float> Ermine::Renderable2D::CalculateModelSpaceVertexesActor2D(std::shared_ptr<Ermine::Actor2D> Act)
{
    Ermine::VertexTextured TopRight(Quad::GetModelCoordinatesTopRight());
    Ermine::VertexTextured BottomRight(Quad::GetModelCoordinatesBottomRight());
    Ermine::VertexTextured BottomLeft(Quad::GetModelCoordinatesBottomLeft());
    Ermine::VertexTextured TopLeft(Quad::GetModelCoordinatesTopLeft());

    glm::vec3 TopRightPos = TopRight.GetPositionCoordinates();
    glm::vec4 TopRightPos4 = glm::vec4(TopRightPos, 0.0f);

    glm::vec3 BottomRightPos = BottomRight.GetPositionCoordinates();
    glm::vec4 BottomRightPos4 = glm::vec4(BottomRightPos, 0.0f);

    glm::vec3 BottomLeftPos = BottomLeft.GetPositionCoordinates();
    glm::vec4 BottomLeftPos4 = glm::vec4(BottomLeftPos, 0.0f);

    glm::vec3 TopLeftPos = TopLeft.GetPositionCoordinates();
    glm::vec4 TopLeftPos4 = glm::vec4(TopLeftPos, 0.0f);

    TopRight.SetPositonCoordinates(TopRightPos4);
    BottomRight.SetPositonCoordinates(BottomRightPos4);
    BottomLeft.SetPositonCoordinates(BottomLeftPos4);
    TopLeft.SetPositonCoordinates(TopLeftPos4);

    //Get The Mutex As We Are Starting To Use Shared Memory
    auto Lock = Object::GetObjectMutex();

    TopRight.SetVertexUV(   glm::vec2(Act->Actorsprite->GetTopRightUV().x  , Act->Actorsprite->GetBottomLeftUV().y));
    BottomRight.SetVertexUV(glm::vec2(Act->Actorsprite->GetTopRightUV().x  , Act->Actorsprite->GetTopRightUV().y)  );
    BottomLeft.SetVertexUV( glm::vec2(Act->Actorsprite->GetBottomLeftUV().x, Act->Actorsprite->GetTopRightUV().y)  );
    TopLeft.SetVertexUV(    glm::vec2(Act->Actorsprite->GetBottomLeftUV().x, Act->Actorsprite->GetBottomLeftUV().y));

    //We Are Done Using Shared Resources..
    Lock.unlock();

    std::vector<float> ModelCoordinates;
    ModelCoordinates = TopRight;
    ModelCoordinates = ModelCoordinates + BottomRight;
    ModelCoordinates = ModelCoordinates + BottomLeft;
    ModelCoordinates = ModelCoordinates + TopLeft;

    return ModelCoordinates;
}
#pragma endregion