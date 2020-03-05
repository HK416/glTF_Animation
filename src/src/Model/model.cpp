/**************************/
/*  FILE NAME: model.cpp  */
/**************************/
#include "model.h"

/**************/
/*  INCLUDES  */
/**************/
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
#include <vector_functions.hpp>
#include <matrix_functions.hpp>
#include <quaternion_functions.hpp>

/*************************/
/*  FUNCTION PROTOTYPES  */
/*************************/
bool IsBinaryFile(const std::string& file);
void LoadglTFModel(tinygltf::Model& gltf_model, const std::string& file);
Mesh LoadglTFMesh(const tinygltf::Model& gltf_model, const tinygltf::Mesh& gltf_mesh);
void LoadglTFNode(const tinygltf::Model& gltf_model, const tinygltf::Node& gltf_node, int parent_id, int current_id, std::map<int, Node>& nodes);
Skin LoadglTFSkin(const tinygltf::Model& gltf_model, const tinygltf::Skin& gltf_skin);
Animation LoadglTFAnimation(const tinygltf::Model& gltf_model, const tinygltf::Animation& gltf_animation);
Texture LoadglTFTexture(const tinygltf::Model& gltf_model, const tinygltf::Texture& gltf_texture);
Material LoadglTFMaterial(const tinygltf::Model& gltf_model, const tinygltf::Material& gltf_material);


/* a function that checks if a file in glTF format is in binary format */
/* returns ture if it is in binary format                              */
bool IsBinaryFile(const std::string& file)
{
    bool binary = false;
    std::size_t ext_pos = file.rfind('.', file.length());
    if(ext_pos != std::string::npos)
    {
        binary = (file.substr(ext_pos + 1, file.length() - ext_pos) == "glb");
    }
    return binary;
}

/* function to load glTF model instance from glTF format file */
void LoadglTFModel(tinygltf::Model& gltf_model, const std::string& file)
{
    std::string error;
    std::string warning;
    tinygltf::TinyGLTF gltf_loader;
    bool is_loaded;

    /* load from glTF file */
    if(IsBinaryFile(file) == true)
        is_loaded = gltf_loader.LoadBinaryFromFile(&gltf_model, &error, &warning, file);
    else
        is_loaded = gltf_loader.LoadASCIIFromFile(&gltf_model, &error, &warning, file);

    /* check if it is loaded */
    if(is_loaded == false)
        throw std::runtime_error("Failed to load glTF file(" + file + "), error: " + error);

    /* check for warning messages */
    if(warning.empty() == false)
        std::cout << "glTF warning: " << warning << std::endl;
}

/* a function that loads mesh data */
/* return the loaded mesh          */
Mesh LoadglTFMesh(const tinygltf::Model& gltf_model, const tinygltf::Mesh& gltf_mesh)
{
    Mesh mesh;
    mesh.name = gltf_mesh.name;

    /* NOTE: Assume that there is only one primitive in a mesh */
    /* TODO: Modify it to work on more than one primitive. */
    for(const auto& primitive : gltf_mesh.primitives)
    {
        /* save material id */
        mesh.material_id = primitive.material;

        /* load indices data */
        {
            const auto& accessor = gltf_model.accessors[primitive.indices];
            const auto& buffer_view = gltf_model.bufferViews[accessor.bufferView];
            const auto& buffer = gltf_model.buffers[buffer_view.buffer];

            const auto data_address = buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset;
            const auto byte_stride = accessor.ByteStride(buffer_view);
            const auto count = accessor.count;

            mesh.indices.clear();
            mesh.indices.reserve(count);
            if(accessor.type == TINYGLTF_TYPE_SCALAR)
            {
                if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_BYTE
                    || accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
                {
                    for(std::size_t i = 0; i < count; ++i)
                        mesh.indices.emplace_back(*(reinterpret_cast<const char*>(data_address + i * byte_stride)));
                }
                else if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_SHORT
                        || accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
                {
                    for(std::size_t i = 0; i < count; ++i)
                        mesh.indices.emplace_back(*(reinterpret_cast<const short*>(data_address + i * byte_stride)));
                }
                else if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_INT
                        || accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
                {
                    for(std::size_t i = 0; i < count; ++i)
                        mesh.indices.emplace_back(*(reinterpret_cast<const int*>(data_address + i * byte_stride)));
                }
                else { throw std::runtime_error("Undefined indices component type."); }
            }
            else { throw std::runtime_error("Undefined indices type."); }
        }


        if(mesh.indices.size() > 0)
        {
            /* converts an indices array into the 'TRIANGLES' mode indices array */
            if(primitive.mode == TINYGLTF_MODE_TRIANGLES) { /* empty */ }
            else if(primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN)
            {
                auto triangle_fan = std::move(mesh.indices);
                mesh.indices.clear();

                for(std::size_t i = 2; i < triangle_fan.size(); ++i)
                {
                    mesh.indices.push_back(triangle_fan[0]);
                    mesh.indices.push_back(triangle_fan[i - 1]);
                    mesh.indices.push_back(triangle_fan[i - 0]);
                }
            }
            else if(primitive.mode == TINYGLTF_MODE_TRIANGLE_STRIP)
            {
                auto triangle_strip = std::move(mesh.indices);
                mesh.indices.clear();

                for(std::size_t i = 2; i < triangle_strip.size(); ++i)
                {
                    mesh.indices.push_back(triangle_strip[i - 2]);
                    mesh.indices.push_back(triangle_strip[i - 1]);
                    mesh.indices.push_back(triangle_strip[i - 0]);
                }
            }
            else { throw std::runtime_error("Undefined primitive mode."); }
        }


        mesh.vertices.resize(mesh.indices.size());
        for(const auto& attribute : primitive.attributes)
        {
            /* load vertices data */
            /* NOTE: render witout using an index buffer      */
            /* TODO: change to use index buffer for rendering */
            {
                const auto& accessor = gltf_model.accessors[attribute.second];
                const auto& buffer_view = gltf_model.bufferViews[accessor.bufferView];
                const auto& buffer = gltf_model.buffers[buffer_view.buffer];

                const auto data_address = buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset;
                const auto byte_stride = accessor.ByteStride(buffer_view);

                if(attribute.first == "POSITION")
                {
                    if(accessor.type == TINYGLTF_TYPE_VEC3)
                    {
                        if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
                        {
                            for(std::size_t i = 0; i < mesh.indices.size(); ++i)
                            {
                                auto face = mesh.indices[i];
                                mesh.vertices[i].position.x = *(reinterpret_cast<const float*>(data_address + 0 * sizeof(float) + face * byte_stride));
                                mesh.vertices[i].position.y = *(reinterpret_cast<const float*>(data_address + 1 * sizeof(float) + face * byte_stride));
                                mesh.vertices[i].position.z = *(reinterpret_cast<const float*>(data_address + 2 * sizeof(float) + face * byte_stride));
                            }
                        }
                        else if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
                        {
                            for(std::size_t i = 0; i < mesh.indices.size(); ++i)
                            {
                                auto face = mesh.indices[i];
                                mesh.vertices[i].position.x = *(reinterpret_cast<const double*>(data_address + 0 *sizeof(double) + face * byte_stride));
                                mesh.vertices[i].position.y = *(reinterpret_cast<const double*>(data_address + 0 *sizeof(double) + face * byte_stride));
                                mesh.vertices[i].position.z = *(reinterpret_cast<const double*>(data_address + 0 *sizeof(double) + face * byte_stride));
                            }
                        }
                        else { throw std::runtime_error("Undefined \'POSITION\' attribute component type."); }
                    }  
                    else { throw std::runtime_error("Undefined \'POSITION\' attribute type."); }
                } // endif 'POSITION'
                else if(attribute.first == "NORMAL")
                {
                    if(accessor.type == TINYGLTF_TYPE_VEC3)
                    {
                        if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
                        {
                            for(std::size_t i = 0; i < mesh.indices.size(); ++i)
                            {
                                auto face = mesh.indices[i];
                                mesh.vertices[i].normal.x = *(reinterpret_cast<const float*>(data_address + 0 * sizeof(float) + face * byte_stride));
                                mesh.vertices[i].normal.y = *(reinterpret_cast<const float*>(data_address + 1 * sizeof(float) + face * byte_stride));
                                mesh.vertices[i].normal.z = *(reinterpret_cast<const float*>(data_address + 2 * sizeof(float) + face * byte_stride));
                            }
                        }
                        else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
                        {
                            for(std::size_t i = 0; i < mesh.indices.size(); ++i)
                            {
                                auto face = mesh.indices[i];
                                mesh.vertices[i].normal.x = *(reinterpret_cast<const double*>(data_address + 0 * sizeof(double) + face * byte_stride));
                                mesh.vertices[i].normal.y = *(reinterpret_cast<const double*>(data_address + 1 * sizeof(double) + face * byte_stride));
                                mesh.vertices[i].normal.z = *(reinterpret_cast<const double*>(data_address + 2 * sizeof(double) + face * byte_stride));
                            }
                        }
                        else { throw std::runtime_error("Undefined \'NORMAL\' attribute component type."); }
                    }
                    else { throw std::runtime_error("Undefined \'NORMAL\' attribute type."); }
                } // endif 'NORMAL'
                else if(attribute.first == "TEXCOORD_0")
                {
                    if(accessor.type == TINYGLTF_TYPE_VEC2)
                    {
                        if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
                        {
                            for(std::size_t i = 0; i < mesh.indices.size(); ++i)
                            {
                                auto face = mesh.indices[i];
                                mesh.vertices[i].texcoord.x = *(reinterpret_cast<const float*>(data_address + 0 * sizeof(float) + face * byte_stride));
                                mesh.vertices[i].texcoord.y = *(reinterpret_cast<const float*>(data_address + 1 * sizeof(float) + face * byte_stride));
                            }
                        }
                        else if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
                        {
                            for(std::size_t i = 0; i < mesh.indices.size(); ++i)
                            {
                                auto face = mesh.indices[i];
                                mesh.vertices[i].texcoord.x = *(reinterpret_cast<const double*>(data_address + 0 * sizeof(double) + face * byte_stride));
                                mesh.vertices[i].texcoord.y = *(reinterpret_cast<const double*>(data_address + 1 * sizeof(double) + face * byte_stride));
                            }
                        }
                        else { throw std::runtime_error("Undefined \'TEXCOORD_0\' attribute component type."); }
                    }
                    else { throw std::runtime_error("Undefined \'TEXCOORD_0\' attribute type."); }
                } // endif 'TEXCOORD_0'
                else if(attribute.first == "JOINTS_0")
                {
                    if(accessor.type == TINYGLTF_TYPE_VEC4)
                    {
                        if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_SHORT
                            || accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
                        {   
                            for(std::size_t i = 0; i < mesh.indices.size(); ++i)
                            {
                                auto face = mesh.indices[i];
                                mesh.vertices[i].joint.x = *(reinterpret_cast<const short*>(data_address + 0 * sizeof(short) + face * byte_stride));
                                mesh.vertices[i].joint.y = *(reinterpret_cast<const short*>(data_address + 1 * sizeof(short) + face * byte_stride));
                                mesh.vertices[i].joint.z = *(reinterpret_cast<const short*>(data_address + 2 * sizeof(short) + face * byte_stride));
                                mesh.vertices[i].joint.w = *(reinterpret_cast<const short*>(data_address + 3 * sizeof(short) + face * byte_stride));
                            }
                        }
                        else { throw std::runtime_error("Undefined \'JOINTS_0\' attribute component type."); }
                    }
                    else { throw std::runtime_error("Undefined \'JOINTS_0\' attribute type."); }
                } // endif 'JOINTS_0'
                else if(attribute.first == "WEIGHTS_0")
                {
                    if(accessor.type == TINYGLTF_TYPE_VEC4)
                    {
                        if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
                        {
                            for(std::size_t i = 0; i < mesh.indices.size(); ++i)
                            {
                                auto face = mesh.indices[i];
                                mesh.vertices[i].weight.x = *(reinterpret_cast<const float*>(data_address + 0 * sizeof(float) + face * byte_stride));
                                mesh.vertices[i].weight.y = *(reinterpret_cast<const float*>(data_address + 1 * sizeof(float) + face * byte_stride));
                                mesh.vertices[i].weight.z = *(reinterpret_cast<const float*>(data_address + 2 * sizeof(float) + face * byte_stride));
                                mesh.vertices[i].weight.w = *(reinterpret_cast<const float*>(data_address + 3 * sizeof(float) + face * byte_stride));
                            }
                        }
                        else if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
                        {
                            for(std::size_t i = 0; i < mesh.indices.size(); ++i)
                            {
                                auto face = mesh.indices[i];
                                mesh.vertices[i].weight.x = *(reinterpret_cast<const double*>(data_address + 0 * sizeof(double) + face * byte_stride));
                                mesh.vertices[i].weight.y = *(reinterpret_cast<const double*>(data_address + 1 * sizeof(double) + face * byte_stride));
                                mesh.vertices[i].weight.z = *(reinterpret_cast<const double*>(data_address + 2 * sizeof(double) + face * byte_stride));
                                mesh.vertices[i].weight.w = *(reinterpret_cast<const double*>(data_address + 3 * sizeof(double) + face * byte_stride));
                            }
                        }
                        else { throw std::runtime_error("Undefined \'WEIGHTS_0\' attribute type."); }
                    }
                    else { throw std::runtime_error("Undefined \'WEIGHTS_0\' attribute type."); }
                } // endif 'WEIGHTS_0'
            }
        } // for each attribute in primitive
    } // for each primitive in glTF mesh

    return mesh;
}

/* recursively load node data */
void LoadglTFNode(const tinygltf::Model& gltf_model, const tinygltf::Node& gltf_node, int parent_id, int current_id, std::map<int, Node>& nodes)
{
    Node node;
    node.name = gltf_node.name;
    node.child_ids = gltf_node.children;
    node.node_id = current_id;
    node.parent_id = parent_id;
    node.mesh_id = gltf_node.mesh;
    node.skin_id = gltf_node.skin;

    /* save matrix data */
    if(gltf_node.matrix.size() == 16)
    {
        node.matrix = orca::MakeMatrix4X4<float>(gltf_node.matrix.data());
    }

    /* save translation data */
    if(gltf_node.translation.size() == 3)
    {
        node.translate = orca::MakeVector3<float>(gltf_node.translation.data());
    }

    /* save rotation data */
    if(gltf_node.rotation.size() == 4)
    {
        /* store quaternion information in vec4 type */
        node.rotate = orca::MakeVector4<float>(gltf_node.rotation.data());
    }

    /* save sacle data */
    if(gltf_node.scale.size() == 3)
    {
        node.scale = orca::MakeVector3<float>(gltf_node.scale.data());
    }

    /* save the loaded node to the node map */
    nodes.insert(std::make_pair(current_id, node));

    /* stores child nodes */
    for(std::size_t i = 0; i < node.child_ids.size(); ++i)
    {
        LoadglTFNode(gltf_model, gltf_model.nodes[node.child_ids[i]], current_id, node.child_ids[i], nodes);
    }
}

/* a function that loads skin data */
/* return the loaded skin          */
Skin LoadglTFSkin(const tinygltf::Model& gltf_model, const tinygltf::Skin& gltf_skin)
{
    Skin skin;
    skin.name = gltf_skin.name;
    skin.joints = gltf_skin.joints;
    skin.skeleton_root_id = gltf_skin.skeleton;

    /* store inverse bind matrices */
    if(gltf_skin.inverseBindMatrices > -1)
    {
        const auto& accessor = gltf_model.accessors[gltf_skin.inverseBindMatrices];
        const auto& buffer_view = gltf_model.bufferViews[accessor.bufferView];
        const auto& buffer = gltf_model.buffers[buffer_view.buffer];

        const auto data_address = buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset;
        const auto byte_stride = accessor.ByteStride(buffer_view);
        const auto count = accessor.count;

        if(accessor.type == TINYGLTF_TYPE_MAT4)
        {
            if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
            {
                skin.inverse_bind_matrices.reserve(count);
                for(std::size_t i = 0; i < count; ++i)
                {
                    orca::mat4<float> mat = orca::MakeMatrix4X4<float, float>(reinterpret_cast<const float*>(data_address + i * byte_stride));
                    skin.inverse_bind_matrices.emplace_back(mat);
                }
            }
            else if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
            {
                skin.inverse_bind_matrices.reserve(count);
                for(std::size_t i = 0; i < count; ++i)
                {
                    orca::mat4<float> mat = orca::MakeMatrix4X4<float, double>(reinterpret_cast<const double*>(data_address + i * byte_stride));
                    skin.inverse_bind_matrices.emplace_back(mat);
                }
            }
            else { throw std::runtime_error("Undefined inverse bind matrices component type."); }
        }
        else { throw std::runtime_error("Undefined inverse bind matrices type."); }
    }

    return skin;
}

/* a function that loads animation data */
/* return the loaded animation          */
Animation LoadglTFAnimation(const tinygltf::Model& gltf_model, const tinygltf::Animation& gltf_animation)
{
    Animation animation;
    animation.name = gltf_animation.name;

    /* save animation sampler information */
    animation.samplers.reserve(gltf_animation.samplers.size());
    for(const auto& sampler : gltf_animation.samplers)
    {
        AnimationSampler anim_sampler;

        /* save the animation sampler interpolation information */
        if(sampler.interpolation == "LINEAR")
        {
            anim_sampler.interpolation = INTERPOLATION_TYPE::LINEAR;
        }
        else if(sampler.interpolation == "STEP")
        {
            anim_sampler.interpolation = INTERPOLATION_TYPE::STEP;
        }
        else if(sampler.interpolation == "CUBICSPLINE")
        {
            anim_sampler.interpolation = INTERPOLATION_TYPE::CUBICSPLINE;
        }
        else 
        { 
            anim_sampler.interpolation = INTERPOLATION_TYPE::UNKNOWN;
            std::cout << "Undefined animation sampler interpolation. "; 
            std::cout << "(interpolation: " << sampler.interpolation << ")" << std::endl;
        }

        /* save animation sampler input information */
        {
            const auto& accessor = gltf_model.accessors[sampler.input];
            const auto& buffer_view = gltf_model.bufferViews[accessor.bufferView];
            const auto& buffer = gltf_model.buffers[buffer_view.buffer];

            const auto data_address = buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset;
            const auto byte_stride = accessor.ByteStride(buffer_view);
            const auto count = accessor.count;

            if(accessor.type == TINYGLTF_TYPE_SCALAR)
            {
                if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
                {
                    anim_sampler.inputs.reserve(count);
                    for(std::size_t i = 0; i < count; ++i)
                    {
                        anim_sampler.inputs.emplace_back(*(reinterpret_cast<const float*>(data_address + i * byte_stride)));
                    }
                }
                else if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
                {
                    anim_sampler.inputs.reserve(count);
                    for(std::size_t i = 0; i < count; ++i)
                    {
                        anim_sampler.inputs.emplace_back(*(reinterpret_cast<const double*>(data_address + i * byte_stride)));
                    }
                }
                else { throw std::runtime_error("Warning::Undefined animation sampler inputs component type."); }
            }
            else { throw std::runtime_error("Undefined animation sampler inputs type."); }
        }

        /* save start time and end time of animation */
        for(const auto& input : anim_sampler.inputs)
        {
            if(input < animation.start_time)
            {
                animation.start_time = input;
            }

            if(input > animation.end_time)
            {
                animation.end_time = input;
            }
        }

        /* save the animation sampler outputs information */
        {
            const auto& accessor = gltf_model.accessors[sampler.output];
            const auto& buffer_view = gltf_model.bufferViews[accessor.bufferView];
            const auto& buffer = gltf_model.buffers[buffer_view.buffer];

            const auto data_address = buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset;
            const auto byte_stride = accessor.ByteStride(buffer_view);
            const auto count = accessor.count;

            /* FIXME: the animation sampler output type of some modeling files is not processed */
            if(accessor.type == TINYGLTF_TYPE_VEC3)
            {
                if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
                {
                    anim_sampler.outputs.reserve(count);
                    for(std::size_t i = 0; i < count; ++i)
                    {
                        float x = *(reinterpret_cast<const float*>(data_address + 0 * sizeof(float) + i * byte_stride));
                        float y = *(reinterpret_cast<const float*>(data_address + 1 * sizeof(float) + i * byte_stride));
                        float z = *(reinterpret_cast<const float*>(data_address + 2 * sizeof(float) + i * byte_stride));
                        float w = 0.0f;
                        anim_sampler.outputs.emplace_back(x, y, z, w);
                    }
                }
                else if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
                {
                    anim_sampler.outputs.reserve(count);
                    for(std::size_t i = 0; i < count; ++i)
                    {
                        double x = *(reinterpret_cast<const double*>(data_address + 0 * sizeof(double) + i * byte_stride));
                        double y = *(reinterpret_cast<const double*>(data_address + 1 * sizeof(double) + i * byte_stride));
                        double z = *(reinterpret_cast<const double*>(data_address + 2 * sizeof(double) + i * byte_stride));
                        double w = 0.0f;
                        anim_sampler.outputs.emplace_back(x, y, z, w);
                    }
                }
                else { throw std::runtime_error("Undefined animation sampler outputs component type."); }
            }
            else if(accessor.type == TINYGLTF_TYPE_VEC4)
            {
                if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
                {
                    anim_sampler.outputs.reserve(count);
                    for(std::size_t i = 0; i < count; ++i)
                    {
                        float x = *(reinterpret_cast<const float*>(data_address + 0 * sizeof(float) + i * byte_stride));
                        float y = *(reinterpret_cast<const float*>(data_address + 1 * sizeof(float) + i * byte_stride));
                        float z = *(reinterpret_cast<const float*>(data_address + 2 * sizeof(float) + i * byte_stride));
                        float w = *(reinterpret_cast<const float*>(data_address + 3 * sizeof(float) + i * byte_stride));
                        anim_sampler.outputs.emplace_back(x, y, z, w);
                    }
                }
                else if(accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
                {
                    anim_sampler.outputs.reserve(count);
                    for(std::size_t i = 0; i < count; ++i)
                    {
                        double x = *(reinterpret_cast<const double*>(data_address + 0 * sizeof(double) + i * byte_stride));
                        double y = *(reinterpret_cast<const double*>(data_address + 1 * sizeof(double) + i * byte_stride));
                        double z = *(reinterpret_cast<const double*>(data_address + 2 * sizeof(double) + i * byte_stride));
                        double w = *(reinterpret_cast<const double*>(data_address + 3 * sizeof(double) + i * byte_stride));
                        anim_sampler.outputs.emplace_back(x, y, z, w);
                    }
                }
                else { throw std::runtime_error("Undefined animation sampler outputs component type."); }
            }
            else { throw std::runtime_error("Undefined animation sampler outputs type."); }
        }

        animation.samplers.emplace_back(anim_sampler);
    } // for each sampler in glTF animation


    /* save animation channel information */
    animation.channels.reserve(gltf_animation.channels.size());
    for(auto& channel : gltf_animation.channels)
    {
        AnimationChannel anim_channel;

        /* save animation channel path type information */
        if(channel.target_path == "rotation")
        {
            anim_channel.path_type = PATH_TYPE::ROTATION;
        }
        else if(channel.target_path == "translation")
        {
            anim_channel.path_type = PATH_TYPE::TRANSLATION;
        }
        else if(channel.target_path == "scale")
        {
            anim_channel.path_type = PATH_TYPE::SCALE;
        }
        else if(channel.target_path == "weights")
        {
            anim_channel.path_type = PATH_TYPE::WEIGHTS;
        }
        else 
        {
            anim_channel.path_type = PATH_TYPE::UNKNOWN;
            std::cout << "Warning::Undefined animation channel path type. ";
            std::cout << "(path type: " << channel.target_path << ")" << std::endl;
        }

        anim_channel.node_id = channel.target_node;
        anim_channel.sampler_id = channel.sampler;

        animation.channels.emplace_back(anim_channel);
    } // for each channel in glTF animation

    return animation;
}

/* a function that loads texture data */
/* return the loaded texture          */
Texture LoadglTFTexture(const tinygltf::Model& gltf_model, const tinygltf::Texture& gltf_texture)
{
    Texture texture;
    
    /* save the glTF image */
    {
        const tinygltf::Image& gltf_image = gltf_model.images[gltf_texture.source];
        texture.image.name = gltf_image.uri;
        texture.image.width = gltf_image.width;
        texture.image.height = gltf_image.height;
        texture.image.component = gltf_image.component;
        texture.image.data = gltf_image.image;
    }

    /* save the glTF image sampler */
    {
        if(gltf_texture.sampler > -1)
        {
            const tinygltf::Sampler& gltf_sampler = gltf_model.samplers[gltf_texture.sampler];
            texture.sampler.name = gltf_sampler.name;
            texture.sampler.min_filter = GetFilterMode(gltf_sampler.minFilter);
            texture.sampler.mag_filter = GetFilterMode(gltf_sampler.magFilter);
            texture.sampler.wrap_R = GetWrapMode(gltf_sampler.wrapR);
            texture.sampler.wrap_S = GetWrapMode(gltf_sampler.wrapS);
            texture.sampler.wrap_T = GetWrapMode(gltf_sampler.wrapT);
        }
    }

    return texture;
}

/* a function that loads material data */
/* return the loaded material          */
Material LoadglTFMaterial(const tinygltf::Model& gltf_model, const tinygltf::Material& gltf_material)
{
    Material material;

    {
        auto base_color_texture_id = gltf_material.values.find("baseColorTexture");
        if(base_color_texture_id != gltf_material.values.end())
        {
            material.base_color_texture_id = base_color_texture_id->second.TextureIndex();
            material.texture_coordinate_sets.base_color = static_cast<unsigned char>(base_color_texture_id->second.TextureTexCoord());
        }
    }

    {
        auto metallic_roughness_texture = gltf_material.values.find("metallicRoughnessTexture");
        if(metallic_roughness_texture != gltf_material.values.end())
        {
            material.metallic_roughness_texture_id = metallic_roughness_texture->second.TextureIndex();
            material.texture_coordinate_sets.metallic_roughness = static_cast<unsigned char>(metallic_roughness_texture->second.TextureTexCoord());
        }
    }

    {
        auto roughness_factor = gltf_material.values.find("roughnessFactor");
        if(roughness_factor != gltf_material.values.end())
        {
            material.roughness_factor = static_cast<float>(roughness_factor->second.Factor());
        }
    }

    {
        auto metallic_factor = gltf_material.values.find("metallicFactor");
        if(metallic_factor != gltf_material.values.end())
        {
            material.metallic_factor = static_cast<float>(metallic_factor->second.Factor());
        }
    }

    {
        auto base_color_factor = gltf_material.values.find("baseColorFactor");
        if(base_color_factor != gltf_material.values.end())
        {
            material.base_color_factor = orca::MakeVector4<float>(base_color_factor->second.ColorFactor().data());
        }
    }

    {
        auto normal_texture = gltf_material.additionalValues.find("normalTexture");
        if(normal_texture != gltf_material.additionalValues.end())
        {
            material.normal_texture_id = normal_texture->second.TextureIndex();
            material.texture_coordinate_sets.normal = static_cast<unsigned char>(normal_texture->second.TextureTexCoord());
        }
    }

    {
        auto emissive_texture = gltf_material.additionalValues.find("emissiveTexture");
        if(emissive_texture != gltf_material.additionalValues.end())
        {
            material.emissive_texture_id = emissive_texture->second.TextureIndex();
            material.texture_coordinate_sets.emissive = static_cast<unsigned char>(emissive_texture->second.TextureTexCoord());
        }
    }

    {
        auto occlusion_texture = gltf_material.additionalValues.find("occlusionTexture");
        if(occlusion_texture != gltf_material.additionalValues.end())
        {
            material.occlusion_texture_id = occlusion_texture->second.TextureIndex();
            material.texture_coordinate_sets.occlusion = static_cast<unsigned char>(occlusion_texture->second.TextureTexCoord());
        }
    }

    {
        auto alpha_mode = gltf_material.additionalValues.find("alphaMode");
        if(alpha_mode != gltf_material.additionalValues.end())
        {
            const tinygltf::Parameter& param = alpha_mode->second;
            if(param.string_value == "BLEND")
            {
                material.alpha_mode = ALPHA_MODE::ALPHA_MODE_BLEND;
            }
            else if(param.string_value == "MASK")
            {
                material.alpha_mode = ALPHA_MODE::ALPHA_MODE_MASK;
                material.alpha_cutoff = 0.5f;
            }
        }
    }

    {
        auto alpha_cutoff = gltf_material.additionalValues.find("alphaCutoff");
        if(alpha_cutoff != gltf_material.additionalValues.end())
        {
            material.alpha_cutoff = static_cast<float>(alpha_cutoff->second.Factor());
        }
    }

    {
        auto emissive_factor = gltf_material.additionalValues.find("emissiveFactor");
        if(emissive_factor != gltf_material.additionalValues.end())
        {
            material.emissive_factor = orca::vec4<float>(orca::MakeVector3<float>(emissive_factor->second.ColorFactor().data()), 1.0f);
        }
    }

    // extensions
    {
        auto extension = gltf_material.extensions.find("KHR_materials_pbrSpecularGlossiness");
        if(extension != gltf_material.extensions.end())
        {
            if(extension->second.Has("specularGlossinessTexture"))
            {
                material.extension.specular_glossiness_texture_id = extension->second.Get("specularGlossinessTexture").Get("index").Get<int>();
                material.texture_coordinate_sets.specular_glossiness = static_cast<unsigned char>(extension->second.Get("specularGlossinessTexture").Get("texCoord").Get<int>());
                material.work_flow = PBR_WORK_FLOW::SPECULAR_GLOSSINESS;
            }

            if(extension->second.Has("diffuseTexture"))
            {
                material.extension.diffuse_texture_id = extension->second.Get("diffuseTexture").Get("index").Get<int>();
            }

            if(extension->second.Has("diffuseFactor"))
            {
                auto factor = extension->second.Get("diffuseFactor");
                for(std::size_t i = 0; i < factor.ArrayLen(); ++i)
                {
                    auto value = factor.Get(i);
                    material.extension.diffuse_factor[i] = value.IsNumber() ? static_cast<float>(value.Get<double>()) : static_cast<float>(value.Get<int>());
                }
            }

            if(extension->second.Has("specularFactor"))
            {
                auto factor = extension->second.Get("specularFactor");
                for(std::size_t i = 0; i < factor.ArrayLen(); ++i)
                {
                    auto value = factor.Get(i);
                    material.extension.specular_factor[i] = value.IsNumber() ? static_cast<float>(value.Get<double>()) : static_cast<float>(value.Get<int>());
                }
            }
        }
    }

    return material;
}


/* constructor */
Model::Model(const std::string& directory, const std::string& filename)
    : curr_animation(0)
    , directory(directory)
    , filename(filename)
    , meshes()
    , nodes()
    , skins()
    , animations()
{
    LoadModel(directory + '/' + filename);
    SetupModel();
}

/* copy constructor */
Model::Model(const Model& other)
    : curr_animation(other.curr_animation)
    , directory(other.directory)
    , filename(other.filename)
    , meshes(other.meshes)
    , nodes(other.nodes)
    , skins(other.skins)
    , animations(other.animations)
    , textures(other.textures)
{ /* empty */ }

/* destructor */
Model::~Model()
{
    CleanupModel();
}

/* function that does initial work so that the model can be used */
void Model::SetupModel()
{
    for(std::size_t i = 0; i < textures.size(); ++i)
        textures[i].SetupTexture();

    for(std::size_t i = 0; i < meshes.size(); ++i)
        meshes[i].SetupMesh();
}

/* function to clean up the model used */
void Model::CleanupModel()
{
    for(std::size_t i = 0; i < textures.size(); ++i)
        textures[i].CleanupTexture();

    for(std::size_t i = 0; i < meshes.size(); ++i)
        meshes[i].CleanupMesh();
}

/* function to update the state of the model */
/* (mainly update animation)                 */
void Model::Update(double delta_time)
{
    static double total_time = 0.0f;
    total_time += delta_time;

    if(animations.size() > 0)
    {
        /* NOTE: only the first animation is used */
        UpdateAnimation(total_time);
    }
}

/* function to render model */
void Model::Render(unsigned int shader_program)
{
    for(std::size_t i = 0; i < meshes.size(); ++i)
    {  
        if(meshes[i].material_id > -1)
        {
            const Material& material = materials[meshes[i].material_id];
            
            /* NOTE: use only diffuse texture */
            if(material.base_color_texture_id > -1)
            {
                textures[material.base_color_texture_id].BindTexture();
            }
        }

        meshes[i].Render(shader_program, IsAnimated());
    }
}

bool Model::IsAnimated() const
{
    return (animations.size() > 0 && skins.size() > 0);
}

void Model::ChangeAnimation(int num)
{
    curr_animation = std::clamp<size_t>(curr_animation + num, 0, animations.size() - 1);
}

/* load a file in glTF format */
void Model::LoadModel(const std::string& file)
{
    tinygltf::Model gltf_model;
    LoadglTFModel(gltf_model, file);

    /* load the mesh data */
    for(std::size_t i = 0; i < gltf_model.meshes.size(); ++i)
    {
        meshes.insert(std::make_pair(i, LoadglTFMesh(gltf_model, gltf_model.meshes[i])));
    }

    /* load the node data                                */
    /* NOTE: only store node data from the default scene */
    const tinygltf::Scene& scene = gltf_model.scenes[gltf_model.defaultScene];
    for(std::size_t i = 0; i < scene.nodes.size(); ++i)
    {
        LoadglTFNode(gltf_model, gltf_model.nodes[scene.nodes[i]], -1, scene.nodes[i], nodes);
    }

    /* save the matrix information of the mesh */
    for(std::size_t i= 0; i < nodes.size(); ++i)
    {
        if(nodes[i].mesh_id > -1)
        {
            meshes[nodes[i].mesh_id].matrix = nodes[i].matrix;
        }
    }

    /* load the texture data */
    for(std::size_t i = 0; i < gltf_model.textures.size(); ++i)
    {
        textures.insert(std::make_pair(i, LoadglTFTexture(gltf_model, gltf_model.textures[i])));
    }
    
    /* load the material data */
    for(std::size_t i = 0; i < gltf_model.materials.size(); ++i)
    {
        materials.insert(std::make_pair(i, LoadglTFMaterial(gltf_model, gltf_model.materials[i])));
    }
    
    /* load the animation data */
    for(std::size_t i = 0; i < gltf_model.animations.size(); ++i)
    {
        animations.insert(std::make_pair(i, LoadglTFAnimation(gltf_model, gltf_model.animations[i])));
    }

    /* load the skin data */
    for(std::size_t i = 0; i < gltf_model.skins.size(); ++i)
    {
        skins.insert(std::make_pair(i, LoadglTFSkin(gltf_model, gltf_model.skins[i])));
    }
}

/* function to return matrix information of node */
orca::mat4<float> Model::GetNodeMatrix(int node_id)
{
    auto& node = nodes[node_id];
    auto matrix = node.LocalMatrix();

    for(auto id = node.parent_id; id != -1; id = nodes[id].parent_id)
    {
        matrix = matrix * nodes[id].LocalMatrix();
    }
    return matrix;
}

/* function to update the animation of the model */
void Model::UpdateAnimation(double duration)
{
    if (curr_animation >= animations.size())   
        throw std::runtime_error("animation id error: out of range.");

    bool updated = false;
    Animation& animation = animations[curr_animation];
    float time = std::fmod(static_cast<float>(duration), animation.end_time - animation.start_time);

    for (auto& channel : animation.channels)
    {
        AnimationSampler& sampler = animation.samplers[channel.sampler_id];
        if(sampler.inputs.size() > sampler.outputs.size())
            continue;

        
        for(std::size_t i = 0; i < sampler.inputs.size() - 1; ++i)
        {
            if((time >= sampler.inputs[i]) && (time <= sampler.inputs[i + 1]))
            {
                float u = std::max(0.0f, time - sampler.inputs[i]) / (sampler.inputs[i + 1] - sampler.inputs[i]);
                if(u <= 1.0f)
                {
                    /* NOTE: use the previous sampler output information of the current frame              */
                    /* TODO: interpolate the sampler output information before and after the current frame */
                    /*       using the interpolation information stored in the sampler                     */
                    if(channel.path_type == PATH_TYPE::TRANSLATION)
                    {
                        auto translation = sampler.outputs[i];
                        // auto translation = orca::Lerp(sampler.outputs[i], sampler.outputs[i + 1], u);
                        nodes[channel.node_id].translate = orca::vec3<float>(translation);
                    }
                    else if(channel.path_type == PATH_TYPE::SCALE)
                    {
                        auto scale = sampler.outputs[i];
                        // auto scale = orca::Lerp(sampler.outputs[i], sampler.outputs[i + 1], u);
                        nodes[channel.node_id].scale = orca::vec3<float>(scale);
                    }
                    else if(channel.path_type == PATH_TYPE::ROTATION)
                    {
                        auto rotation = sampler.outputs[i];
                        // auto rotation = orca::Slerp(sampler.outputs[i], sampler.outputs[i + 1], u);
                        nodes[channel.node_id].rotate = orca::Normalize(rotation);
                    }

                    updated = true;
                }
            }
        }
    }
    
    if(updated == true)
    {
        UpdateNode(0);
    }
}

/* function to update the transformation information of a node */
void Model::UpdateNode(int node_id)
{
    
    auto& node = nodes[node_id];
    if(node.mesh_id > -1)
    {
        auto& mesh = meshes[node.mesh_id];
        mesh.matrix = GetNodeMatrix(node_id);
        if(node.skin_id > -1)
        {
            auto& skin = skins[node.skin_id];
            auto inverse_transform = orca::Inverse(mesh.matrix);
            unsigned int num_joints = std::min(static_cast<unsigned int>(skin.joints.size()), MAX_NUM_JOINTS);
            for(unsigned int i = 0; i < num_joints; ++i)
            {
                /* NOTE: Reference: https://github.com/KhronosGroup/glTF-Tutorials/blob/master/gltfTutorial/gltfTutorial_020_Skins.md */
                auto joint_mat = skin.inverse_bind_matrices[i] * GetNodeMatrix(skin.joints[i]) * inverse_transform;
                mesh.joint_matrices[i] = joint_mat;
            }
        }
    }

    for(auto child : node.child_ids)
    {
        UpdateNode(child);
    }
}