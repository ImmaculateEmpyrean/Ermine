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
    return ProjectionViewMatrix;
}

glm::mat4 Ermine::OrthographicCamera::GetViewMatrix()
{
    return ViewMatrix;
}

glm::mat4 Ermine::OrthographicCamera::GetProjectionMatrix()
{
    return ProjectionMatrix;
}

glm::vec3 Ermine::OrthographicCamera::GetCameraLocation()
{
    return CameraPosition;
}

void Ermine::OrthographicCamera::SetCameraLocation(glm::vec2 Point)
{
    CameraPosition.x = Point.x;
    CameraPosition.y = Point.y;
    
    //Update View Matrix Using The New Translation Coordinates..
    HelperRecalculateViewMatrix();
}

void Ermine::OrthographicCamera::SetCameraLocation(glm::vec3 Point)
{
    CameraPosition = Point;

    //Update View Matrix Using The New Translation Coordinates..
    HelperRecalculateViewMatrix();
}

void Ermine::OrthographicCamera::TranslateCamera(glm::vec2 TranslateByAmount)
{
    CameraPosition.x = CameraPosition.x + TranslateByAmount.x;
    CameraPosition.y = CameraPosition.y + TranslateByAmount.y;

    //Update View Matrix Using The New Translation Coordinates..
    HelperRecalculateViewMatrix();
}

void Ermine::OrthographicCamera::TranslateCamera(glm::vec3 TranslateByAmount)
{
    CameraPosition = CameraPosition + TranslateByAmount;

    //Update View Matrix Using The New Translation Coordinates..
    HelperRecalculateViewMatrix();
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

    //Update View Matrix Using The New Rotation Coordinates..
    HelperRecalculateViewMatrix();
}

void Ermine::OrthographicCamera::RotateCameraBy(float RotationInDegrees)
{
    this->RotationInDegrees = RotationInDegrees;

    if (this->RotationInDegrees >= 360.0f)
    {
        this->RotationInDegrees = this->RotationInDegrees - 360.0f;
    }

    //Update View Matrix Using The New Rotation Coordinates..
    HelperRecalculateViewMatrix();
}

void Ermine::OrthographicCamera::SetProjectionMatrix(glm::mat4 ProjectionMatrix)
{
    //Set The Internal Projection Matrix To The Recieved Projection Matrix
    this->ProjectionMatrix = ProjectionMatrix;
}

void Ermine::OrthographicCamera::SetFOV(float Left, float Right, float Bottom, float Top)
{
    this->ProjectionMatrix = glm::ortho<float>(Left, Right, Bottom, Top, -5.0f, 5.0f);
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
}
