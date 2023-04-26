//
// Created by Junkang on 2023/4/26.
//

#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <bone.h>
#include <functional>
//#include <animdata.h>
#include <model_skeleton.h>

struct AssimpNodeData {
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation {
private:
    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;

public:
    Animation() = default;

    Animation(const std::string& animationPath, Model* model) {
        Assimp::Importer importer;
        const auto* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
        assert(scene && scene->mRootNode);

        auto* animation = scene->mAnimations[0];
        m_Duration = animation->mDuration;
        m_TicksPerSecond = animation->mTicksPerSecond;

        ReadHeirarchyData(m_RootNode, scene->mRootNode);

        ReadMissingBones(animation, model);
    }

    ~Animation() = default;

    Bone* FindBone(const std::string& name) {
        auto iter = std::find_if(m_Bones.begin(), m_Bones.end(), [&](const Bone& bone) {
           return bone.GetBoneName() == name;
        });

        if (iter == m_Bones.end()) return nullptr;
        else return &(*iter);
    }

    inline float GetTicksPerSecond() { return m_TicksPerSecond; }

    inline float GetDuration() { return m_Duration;}

    inline const AssimpNodeData& GetRootNode() { return m_RootNode; }

    inline const std::map<std::string, BoneInfo>& GetBoneInfoMap()
    {
        return m_BoneInfoMap;
    }

private:
    void ReadMissingBones(const aiAnimation* animation, Model* model) {
        // a animation node can refer to multiple bones
        auto channelNum = animation->mNumChannels;

        // a model can have more than one animation node
        // different animation can partly or completely refer to the same bones
        auto& boneInfoMap = model->GetBoneInfoMap();
        auto& boneCount = model->GetBoneCount();

        // a channel correspond to a single bone
        // bones egaged in an animation and their key frames
        for (unsigned int i = 0; i < channelNum; i++) {
            auto* channel = animation->mChannels[i];
            std::string boneName = channel->mNodeName.data;

            // Is the following if expression really matters?
            if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
                boneInfoMap[boneName].id = boneCount;
                boneCount++;
            }
            // the data used to initialize a bone comes from the channel
            // means the extra boneInfo does not have a offset matrix

            m_Bones.emplace_back(Bone(boneName, boneInfoMap[boneName].id, channel));
        }

        m_BoneInfoMap = boneInfoMap;
    }

    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src) {
        dest.name = src->mName.data;
        dest.transformation = glm::transpose(glm::make_mat4(&src->mTransformation.a1));
        dest.childrenCount = src->mNumChildren;

        for (int i = 0; i < src->mNumChildren; i++) {
            AssimpNodeData newData;
            ReadHeirarchyData(newData, src->mChildren[i]);

            dest.children.emplace_back(std::move(newData));
        }

    }
};


