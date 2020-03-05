/************************/
/*  FILE NAME: model.h  */
/************************/
#ifndef _MODEL_H_
#define _MODEL_H_

/**************/
/*  INCLUDES  */
/**************/
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "mesh.h"
#include "node.h"
#include "skin.h"
#include "animation.h"
#include "texture.h"
#include "material.h"

/***********************/
/*  CLASS NAME: Model  */
/***********************/
class Model
{
public:
    Model(const std::string& directory, const std::string& filename);
    Model(const Model& other);
    ~Model();

public:
    void SetupModel();
    void CleanupModel();
    void Update(double delta_time);
    void Render(unsigned int shader_program);

public:
    bool IsAnimated() const;
    void ChangeAnimation(int num);

private:
    void LoadModel(const std::string& file);
    orca::mat4<float> GetNodeMatrix(int node_id);
    void UpdateAnimation(double duration);
    void UpdateNode(int node_id);

private:
    size_t curr_animation;
    std::string directory;
    std::string filename;
    std::map<int, Mesh> meshes;
    std::map<int, Node> nodes;
    std::map<int, Skin> skins;
    std::map<int, Animation> animations;
    std::map<int, Texture> textures;
    std::map<int, Material> materials;
}; // class Model
#endif // !_MODEL_H_