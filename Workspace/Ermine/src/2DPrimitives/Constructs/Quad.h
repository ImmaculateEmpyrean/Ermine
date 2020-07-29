#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
    class Quad
    {
    public:
        //For Now This Is Default Might Change In The Future..
        Quad() = default;

    public:
        static std::vector<float> GetModelCoordinates() {
            return {
                // positions            // colors                 // texture coords   //Texture Number
                 50.0f,  50.0f, 0.0f,   100.0f, 0.0f  , 0.0f  ,   -99.0f, -99.0f,         0.0f,
                 50.0f, -50.0f, 0.0f,   0.0f  , 100.0f, 0.0f  ,   -99.0f, -99.0f,         0.0f,
                -50.0f, -50.0f, 0.0f,   0.0f  , 0.0f  , 100.0f,   -99.0f, -99.0f,         0.0f,
                -50.0f,  50.0f, 0.0f,   100.0f, 100.0f, 0.0f  ,   -99.0f, -99.0f,         0.0f
            };
        }

        static std::vector<uint32_t> GetModelIndices() {
            return {
                    0, 1, 3, // first triangle
                    1, 2, 3  // second triangle
            };
        }
    

    protected:

    protected:

    private:

    private:
        /*std::vector<float> vertices = {
            // positions          // colors           // texture coords
             50.0f,  50.0f, 0.0f,   100.0f, 0.0f, 0.0f,   100.0f, 100.0f,   // top right
             50.0f, -50.0f, 0.0f,   0.0f, 100.0f, 0.0f,   100.0f, 0.0f,   // bottom right
            -50.0f, -50.0f, 0.0f,   0.0f, 0.0f, 100.0f,   0.0f, 0.0f,   // bottom left
            -50.0f,  50.0f, 0.0f,   100.0f, 100.0f, 0.0f,   0.0f, 100.0f    // top left 
        };*/

	};
}