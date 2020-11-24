#include "stdafx.h"
#include "OrthographicCamera.h"

///Start Area To Declare Static Variables..
std::once_flag Ermine::OrthographicCamera::InitializedFlag;
Ermine::OrthographicCamera* Ermine::OrthographicCamera::Camera = nullptr;
///Ended Area To Declare Static Variables..

Ermine::OrthographicCamera::OrthographicCamera()
    :
    Object()
{
    auto Mutex = GetCameraUniqueLock();

    this->ProjectionMatrix = glm::ortho<float>(0.0f, (float)Ermine::GetScreenWidth(), (float)Ermine::GetScreenHeight(), 0.0f,-5.0f,5.0f);
    
    CameraPosition = { 0.0f,0.0f,0.0f }; 
    RotationInDegrees = 0.0f;

    //OnTickEventTicket = new Ermine::SubscriptionTicket(std::move(Ermine::RecieverComponent::Bind(GenCallableFromMethod(&OrthographicCamera::OnTickFunctionMessageReciever), CameraReadyToRecieveEvents, Ermine::EventType::OnTickEvent)));

    //Must Also Initilize The View Matrix Right :>
    HelperRecalculateViewMatrix(); 
}

Ermine::OrthographicCamera::~OrthographicCamera()
{
    auto Mutex = GetCameraUniqueLock();
    //Destriy The Subscription..
    auto Station = Ermine::EventBroadcastStation::GetStation();
    Station->DestroySubscription(std::move(*OnTickEventTicket));

    //Delete The Allocated Space Even If The Variable Is Moved As Per The Norm..
    delete OnTickEventTicket;
}

Ermine::OrthographicCamera* Ermine::OrthographicCamera::Get()
{
    std::call_once(InitializedFlag, []() {
        Camera = new OrthographicCamera(); //Create An Orthographic Camera..
    });

    return Camera;
}

void Ermine::OrthographicCamera::Deinitialize()
{
    //Calling The Destructor On Camera On Hopes That It Will Deinitialize It..
    if (Camera == nullptr)
        delete Camera;
    else STDOUTDefaultLog_Info("Trying To Delete Orthographic Camera Twice.. This Maybe Part Of A Bigger Semantic Problem.. Please Do Check..");
}

glm::mat4 Ermine::OrthographicCamera::GetProjectionViewMatrix()
{
    auto Mutex = GetCameraUniqueLock();

    if (CacheUptoDate)
        return ProjectionViewMatrix;
    else
    {
        HelperRecalculateViewMatrix();
        return GetProjectionViewMatrix();
    }
}

glm::mat4 Ermine::OrthographicCamera::GetViewMatrix()
{
    auto Mutex = GetCameraUniqueLock();

    if(CacheUptoDate)
        return ViewMatrix;
    else
    {
        HelperRecalculateViewMatrix();
        return GetViewMatrix();
    }
}

glm::mat4 Ermine::OrthographicCamera::GetProjectionMatrix()
{
    auto Mutex = GetCameraUniqueLock();

    if(CacheUptoDate)
        return ProjectionMatrix;
    else
    {
        HelperRecalculateViewMatrix();
        return GetProjectionMatrix();
    }
}

glm::vec3 Ermine::OrthographicCamera::GetCameraLocation()
{
    auto Mutex = GetCameraUniqueLock();
    return CameraPosition;
}

void Ermine::OrthographicCamera::SetCameraLocation(glm::vec2 Point)
{
    auto Mutex = GetCameraUniqueLock();
    SetCameraLocation(glm::vec3(Point, 0.0f));
}

void Ermine::OrthographicCamera::SetCameraLocation(glm::vec3 Point)
{
    auto Mutex = GetCameraUniqueLock();
    CameraPosition = Point;
    CacheUptoDate = false;
}

void Ermine::OrthographicCamera::TranslateCamera(glm::vec2 TranslateByAmount)
{
    auto Mutex = GetCameraUniqueLock();
    TranslateCamera(glm::vec3(TranslateByAmount, 0.0f));
}

void Ermine::OrthographicCamera::TranslateCamera(glm::vec3 TranslateByAmount)
{
    auto Mutex = GetCameraUniqueLock();
    CameraPosition = CameraPosition + TranslateByAmount;
    CacheUptoDate = false;
}

void Ermine::OrthographicCamera::SetTranslateVelocityToCamera(glm::vec2 Velocity)
{
    auto Mutex = GetCameraUniqueLock();
    this->CameraVelocity = Velocity;
}

float Ermine::OrthographicCamera::GetCameraRotation(bool Degrees)
{
    auto Mutex = GetCameraUniqueLock();
    if (Degrees)
    {
        return RotationInDegrees;
    }
    else return glm::radians<float>(RotationInDegrees);
}
void Ermine::OrthographicCamera::SetCameraRotation(float RotationInDegrees)
{
    auto Mutex = GetCameraUniqueLock();
    this->RotationInDegrees = RotationInDegrees;

    CacheUptoDate = false;
}

void Ermine::OrthographicCamera::RotateCameraBy(float RotationInDegrees)
{
    auto Mutex = GetCameraUniqueLock();
    this->RotationInDegrees = RotationInDegrees;

    if (this->RotationInDegrees >= 360.0f)
    {
        this->RotationInDegrees = this->RotationInDegrees - 360.0f;
    }

    CacheUptoDate = false;
}

void Ermine::OrthographicCamera::SetProjectionMatrix(glm::mat4 ProjectionMatrix)
{
    auto Mutex = GetCameraUniqueLock();
    //Set The Internal Projection Matrix To The Recieved Projection Matrix
    this->ProjectionMatrix = ProjectionMatrix;

    CacheUptoDate = false;
}

void Ermine::OrthographicCamera::SetFOV(float Left, float Right, float Bottom, float Top)
{
    auto Mutex = GetCameraUniqueLock();
    this->ProjectionMatrix = glm::ortho<float>(Left, Right, Bottom, Top, -5.0f, 5.0f);

    CacheUptoDate = false;
}

void Ermine::OrthographicCamera::SetDepth(float NearLimit, float FarLimit)
{
    auto Mutex = GetCameraUniqueLock();
    //This Function Is Absolutely Not Tested And I Would Not Recommend Using This Function Without First Testing It Thoroughly..
    //Furthermore This Function I Think Is Pretty Expensive (Instrumentation Not Done :<) PLease Dont USe This Often.. Not Using It Ever Is Better
    
    float Top = 0.0f;
    float Bottom = 0.0f;

    float Right = 0.0f;
    float Left = 0.0f;

    //The Scopes Are USed So That Variable Names Could Be Used :>

    {
        float Value = this->ProjectionMatrix[3].x; //(R+L) / (R-L) Value..
        float Value2 = this->ProjectionMatrix[0].x; // 2.0f/(R-L) Value..

        float Intermediary = 2.0f / Value2; //This IS The R - L Value..

        float Intermediary2 = Value * Intermediary; // This Is The R + L Value

        Right = (Intermediary2 + Intermediary) / 2.0f;

        Left = (Intermediary2 - Intermediary) / 2.0f;
    }
    {
        float Value = this->ProjectionMatrix[3].y; //(T+B) / (T-B) Value..
        float Value2 = this->ProjectionMatrix[1].y; // 2.0f/(T-B) Value..

        float Intermediary = 2.0f / Value2; //This IS The T - B Value..

        float Intermediary2 = Value * Intermediary; // This Is The T + B Value

        Top = (Intermediary2 + Intermediary) / 2.0f;

        Bottom = (Intermediary2 - Intermediary) / 2.0f;
    }

    this->ProjectionMatrix = glm::ortho<float>(Left, Right, Bottom, Top, NearLimit, FarLimit);

    CacheUptoDate = false;
}


void Ermine::OrthographicCamera::CentreOnActor(Actor2DBase* Act, glm::vec2 OffsetOfCameraFromActorCentre, float CentreSpeed, bool InheritRotation)
{
    auto Mutex = GetCameraUniqueLock();

    //Making SUre This Set To False In The First Place So That There Will Be No Problem When Trying To Switch Connections Id You Are Doing That I Reckon
    TemperoryPauseCentreOnActor = false;

    //From The Next Frame On Will Centre On This Actor
    ActorToCentreOn = Act;

    //This Is The Speed At Which The Camera Centres On The Actor.. It Always Makes A Beeline As Of Now.. Other modes Are Not Yet Implemented..
    if (CentreSpeed >= 0.0f && CentreSpeed <= 1.0f)
        this->CentreSpeed = CentreSpeed;
    else if (CentreSpeed < 0.0f)
        this->CentreSpeed = 0.0f;
    else if (CentreSpeed > 1.0f)
        this->CentreSpeed = 1.0f;

    //Inherit Rotation Flag To Ask Wether The Camera Wishes To Inherit The Rotation Of The Actor..
    this->InheritActorRotation = InheritActorRotation;

    //Need This To Start Tracking..
    ActorDefaultPosition = Act->GetScreenLocation() + OffsetOfCameraFromActorCentre;
    this->OffsetOfCameraFromActorCentre = OffsetOfCameraFromActorCentre;
}


void Ermine::OrthographicCamera::SetCentreSpeed(float CentreSpeed)
{
    auto Mutex = GetCameraUniqueLock();

    if (CentreSpeed >= 0.0f && CentreSpeed <= 1.0f)
        this->CentreSpeed = CentreSpeed;
    else if (CentreSpeed < 0.0f)
        this->CentreSpeed = 0.0f;
    else if (CentreSpeed > 1.0f)
        this->CentreSpeed = 1.0f;
}
void Ermine::OrthographicCamera::SetInheritRotation(bool InheritRotation)
{
    auto Mutex = GetCameraUniqueLock();

    this->InheritActorRotation = InheritRotation;
}


void Ermine::OrthographicCamera::StopCentreOnActor()
{
    auto Mutex = GetCameraUniqueLock();
    TemperoryPauseCentreOnActor = true;
}
void Ermine::OrthographicCamera::ResumeCentreOnActor()
{
    auto Mutex = GetCameraUniqueLock();
    TemperoryPauseCentreOnActor = false;
}


std::queue<glm::vec2> StepsBufferX;
std::queue<glm::vec2> StepsBufferY;

void Ermine::OrthographicCamera::OnUpdate()
{
    auto Mutex = GetCameraUniqueLock();
    if (ActorToCentreOn != nullptr)
    {
        if (TemperoryPauseCentreOnActor == false)
        {
            /*glm::vec2 MovementBuffer = glm::vec2(1.0f);

            glm::vec2 ActorCurrentLocation = ActorToCentreOn->GetScreenLocation();
            glm::vec2 MovementLimit = (ActorCurrentLocation - ActorDefaultPosition) * CentreSpeed;

            MovementBuffer = (ActorCurrentLocation - ActorDefaultPosition);

            glm::vec2 StepCounter = glm::vec2(0.0f);

            //The Problem Is Most Likely Here.. Its Not TAking Y Into Account..
            while (StepCounter.x < abs(MovementBuffer.x))
            {
                StepsBufferX.push(glm::vec2(MovementLimit.x,0.0f));
                StepCounter = StepCounter + abs(MovementLimit);
            }

            StepCounter = glm::vec2(0.0f);

            while (StepCounter.y < abs(MovementBuffer.y))
            {
                StepsBufferY.push(glm::vec2(0.0f, MovementLimit.y));
                StepCounter = StepCounter + abs(MovementLimit);
            }

            ActorDefaultPosition = ActorCurrentLocation;

            if (StepsBufferX.size() != 0)
            {
                TranslateCamera(StepsBufferX.front());
                StepsBufferX.pop();
            }

            if (StepsBufferY.size() != 0)
            {
                TranslateCamera(StepsBufferY.front());
                StepsBufferY.pop();
            }*/

            /* //Initialiation.. Well We Need Some Value Right..
            static glm::vec2 LastPos = ActorToCentreOn->GetScreenLocation();
            glm::vec2 Velocity = ActorToCentreOn->GetScreenLocation() - LastPos;
            
            OffsetOfCameraFromActorCentre = OffsetOfCameraFromActorCentre + Velocity;

            static float CentreSpeedApplied = CentreSpeed;

            glm::vec2 CameraVelocity = CentreSpeedApplied * Velocity;

            if (CentreSpeedApplied < 1.0f);
            {
                CentreSpeedApplied = CentreSpeedApplied + 0.005f;
                if (CentreSpeedApplied > 1.0f)
                    CentreSpeedApplied = 1.0f;
            }

            if (CameraVelocity == glm::vec2(0.0f, 0.0f))
            {
                if (OffsetOfCameraFromActorCentre.x != 0.0f || OffsetOfCameraFromActorCentre.y != 0.0f)
                {
                    CameraVelocity.x = OffsetOfCameraFromActorCentre.x /10;
                    CameraVelocity.y = OffsetOfCameraFromActorCentre.y / 10;
                }
            }

            if (OffsetOfCameraFromActorCentre.x == 0.0f && OffsetOfCameraFromActorCentre.y == 0.0f)
            {
                CentreSpeedApplied = CentreSpeed;
            }

            SetTranslateVelocityToCamera(CameraVelocity);
            OffsetOfCameraFromActorCentre = OffsetOfCameraFromActorCentre - CameraVelocity;

            //This Variable Will Again Be Used In The Next Frame With Previous Value.. Hehehe
            LastPos = ActorToCentreOn->GetScreenLocation();*/

            static glm::vec2 LastPos = ActorToCentreOn->GetScreenLocation();
            static glm::vec2 LastVelocity = ActorToCentreOn->GetScreenLocation() - LastPos;

            glm::vec2 Velocity =  ActorToCentreOn->GetScreenLocation() - LastPos;

            glm::vec2 Acc = Velocity - LastVelocity;

            static glm::vec2 EffectOffset = glm::vec2(0.0f);

            if(Acc.x > 0.0f)
                if(EffectOffset.x <= 50.0f)
                    EffectOffset.x = EffectOffset.x + 0.1f;

            if (Acc.x < 0.0f)
                if (EffectOffset.x <= -50.0f)
                    EffectOffset.x = EffectOffset.x + 0.1f;

            if (Acc.y > 0.0f)
                if (EffectOffset.y <= 50.0f)
                    EffectOffset.y = EffectOffset.y + 0.1f;

            if (Acc.y < 0.0f)
                if (EffectOffset.y <= -50.0f)
                    EffectOffset.y = EffectOffset.y + 0.1f;

            OffsetOfCameraFromActorCentre = OffsetOfCameraFromActorCentre + EffectOffset;

            glm::vec2 ActorCurrentScreenLocation = ActorToCentreOn->GetScreenLocation() + OffsetOfCameraFromActorCentre;
            glm::vec2 Movement = ActorCurrentScreenLocation - ActorDefaultPosition;
            TranslateCamera(Movement);
            ActorDefaultPosition = ActorCurrentScreenLocation;

            OffsetOfCameraFromActorCentre = OffsetOfCameraFromActorCentre - EffectOffset;

            LastPos = ActorToCentreOn->GetScreenLocation();
            LastVelocity = Velocity;
        }
    }

    //Translate The Camera Using The Camera Velocity Set.. (Maybe Ill Hook Up Camera To Box2D ha If It Needs Collisions I Would Have Most Probably Done It.. MAYBE
    TranslateCamera(CameraVelocity);
}


void Ermine::OrthographicCamera::HelperRecalculateViewMatrix()
{
    auto Mutex = GetCameraUniqueLock();
    //Construct Identity Matrix
    glm::mat4 IdentityMat(1.0f);

    //Recalculate The View Matrix Using The Below Formulae.. 
    this->ViewMatrix = glm::translate(IdentityMat, CameraPosition) * glm::rotate(IdentityMat, glm::radians<float>(RotationInDegrees), glm::vec3(0.0f, 0.0f, 1.0f));

    this->ViewMatrix = glm::inverse(this->ViewMatrix);

    //The Projection View Matrix Must Also Be Updated Since It So Clearly Depends On The View Matrix..
    this->ProjectionViewMatrix = ProjectionMatrix * ViewMatrix;

    CacheUptoDate = true;
}

void Ermine::OrthographicCamera::OnTickFunctionMessageReciever(Event* Eve)
{
    if (Eve->GetEventType() != Ermine::EventType::OnUpdateTickEvent)
    {
        STDOUTDefaultLog_Critical("Orthographic Camera Recieved An Event Which Is Not The Event Tick Into The Event Tick Reciever.. Please See That Such A Thing Is Not Repeated..");
        //Exitting As This IS An Unrecoverable Catestrophic Error..
        exit(-1);
    }

    Ermine::OnUpdateTickEvent* TickEvent = (Ermine::OnUpdateTickEvent*)Eve;
    OnTickCameraDefaultProcessing();

    if (OnUpdateTickFunction != nullptr)
        OnUpdateTickFunction();
}

void Ermine::OrthographicCamera::OnTickCameraDefaultProcessing()
{
    //This Function IS Not Really Needed In Most Cases As OnUpdate Serves The Same Purpose I Guess..
}

