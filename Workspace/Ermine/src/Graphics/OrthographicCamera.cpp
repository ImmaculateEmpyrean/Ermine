#include "stdafx.h"
#include "OrthographicCamera.h"

///Start Area To Declare Static Variables..
std::once_flag Ermine::OrthographicCamera::InitializedFlag;
Ermine::OrthographicCamera* Ermine::OrthographicCamera::Camera = nullptr;
///Ended Area To Declare Static Variables..

Ermine::OrthographicCamera::OrthographicCamera()
{
    this->ProjectionMatrix = glm::ortho<float>(0.0f, (float)Ermine::GetScreenWidth(), (float)Ermine::GetScreenHeight(), 0.0f,-5.0f,5.0f);
    
    CameraPosition = { 0.0f,0.0f,0.0f };
    RotationInDegrees = 0.0f;

    //Must Also Initilize The View Matrix Right :>
    HelperRecalculateViewMatrix();
}

Ermine::OrthographicCamera::~OrthographicCamera()
{
    if (Camera != nullptr)
    {
        delete Camera;
        Camera = nullptr;
        STDOUTDefaultLog_Info("Orthographic Deleted Successfully..");
    }
    else
    {
        STDOUTDefaultLog_Info("Attempted To Delete Camera Twice Or Tried To Delete An Unitialized Camera Object!..");
    }
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
    Camera->~OrthographicCamera();
}

glm::mat4 Ermine::OrthographicCamera::GetProjectionViewMatrix()
{
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
    return CameraPosition;
}

void Ermine::OrthographicCamera::SetCameraLocation(glm::vec2 Point)
{
    SetCameraLocation(glm::vec3(Point, 0.0f));
}

void Ermine::OrthographicCamera::SetCameraLocation(glm::vec3 Point)
{
    CameraPosition = Point;
    CacheUptoDate = false;
}

void Ermine::OrthographicCamera::TranslateCamera(glm::vec2 TranslateByAmount)
{
    TranslateCamera(glm::vec3(TranslateByAmount, 0.0f));
}

void Ermine::OrthographicCamera::TranslateCamera(glm::vec3 TranslateByAmount)
{
    CameraPosition = CameraPosition + TranslateByAmount;
    CacheUptoDate = false;
}

void Ermine::OrthographicCamera::SetTranslateVelocityToCamera(glm::vec2 Velocity)
{
    this->CameraVelocity = Velocity;
}

float Ermine::OrthographicCamera::GetCameraRotation(bool Degrees)
{
    if (Degrees)
    {
        return RotationInDegrees;
    }
    else return glm::radians<float>(RotationInDegrees);
}
void Ermine::OrthographicCamera::SetCameraRotation(float RotationInDegrees)
{
    this->RotationInDegrees = RotationInDegrees;

    CacheUptoDate = false;
}

void Ermine::OrthographicCamera::RotateCameraBy(float RotationInDegrees)
{
    this->RotationInDegrees = RotationInDegrees;

    if (this->RotationInDegrees >= 360.0f)
    {
        this->RotationInDegrees = this->RotationInDegrees - 360.0f;
    }

    CacheUptoDate = false;
}

void Ermine::OrthographicCamera::SetProjectionMatrix(glm::mat4 ProjectionMatrix)
{
    //Set The Internal Projection Matrix To The Recieved Projection Matrix
    this->ProjectionMatrix = ProjectionMatrix;

    CacheUptoDate = false;
}

void Ermine::OrthographicCamera::SetFOV(float Left, float Right, float Bottom, float Top)
{
    this->ProjectionMatrix = glm::ortho<float>(Left, Right, Bottom, Top, -5.0f, 5.0f);

    CacheUptoDate = false;
}

void Ermine::OrthographicCamera::SetDepth(float NearLimit, float FarLimit)
{
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
    if (CentreSpeed >= 0.0f && CentreSpeed <= 1.0f)
        this->CentreSpeed = CentreSpeed;
    else if (CentreSpeed < 0.0f)
        this->CentreSpeed = 0.0f;
    else if (CentreSpeed > 1.0f)
        this->CentreSpeed = 1.0f;
}
void Ermine::OrthographicCamera::SetInheritRotation(bool InheritRotation)
{
    this->InheritActorRotation = InheritRotation;
}


void Ermine::OrthographicCamera::StopCentreOnActor()
{
    TemperoryPauseCentreOnActor = true;
}
void Ermine::OrthographicCamera::ResumeCentreOnActor()
{
    TemperoryPauseCentreOnActor = false;
}


std::queue<glm::vec2> StepsBufferX;
std::queue<glm::vec2> StepsBufferY;

void Ermine::OrthographicCamera::OnUpdate()
{
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
    //Construct Identity Matrix
    glm::mat4 IdentityMat(1.0f);

    //Recalculate The View Matrix Using The Below Formulae.. 
    this->ViewMatrix = glm::translate(IdentityMat, CameraPosition) * glm::rotate(IdentityMat, glm::radians<float>(RotationInDegrees), glm::vec3(0.0f, 0.0f, 1.0f));

    this->ViewMatrix = glm::inverse(this->ViewMatrix);

    //The Projection View Matrix Must Also Be Updated Since It So Clearly Depends On The View Matrix..
    this->ProjectionViewMatrix = ProjectionMatrix * ViewMatrix;

    CacheUptoDate = true;
}
