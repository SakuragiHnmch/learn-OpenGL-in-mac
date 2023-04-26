//
// Created by 兰俊康 on 2023/4/26.
//

#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <animation.h>
#include <bone.h>

class Animator {
private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime = 0.0f;

public:
    Animator(Animation* animation)
        :m_CurrentTime(0.0f), m_CurrentAnimation(animation)
    {
        m_FinalBoneMatrices.reserve(100);
        for (int i = 0; i < 100; i++) {
            m_FinalBoneMatrices.emplace_back(glm::mat4(1.0f));
        }
    }

    void UpdateAnimation(float dt) {
        m_DeltaTime = dt;
        if (m_CurrentAnimation) {
            m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
            std::cout << "*************************************\n";
            std::cout << "**** delta time: " << m_DeltaTime << "\n"
                << "**** current time: " << m_CurrentTime << "\n";

            m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
            std::cout << "**** post mode current time: " << m_CurrentTime << "\n";

            CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
        }
    }

    void PlayAnimation(Animation* pAnimation) {
        m_CurrentAnimation = pAnimation;
        m_CurrentTime = 0.0f;
    }

    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform) {
        std::string nodeName = node->name;
        auto nodeTransform = node->transformation;

        auto* bone = m_CurrentAnimation->FindBone(nodeName);
        if (bone) {
            bone->Update(m_CurrentTime);
            nodeTransform = bone->GetLocalTransform();
        }

        auto globalTransform = parentTransform * nodeTransform;

        auto boneInfoMap = m_CurrentAnimation->GetBoneInfoMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
            int index = boneInfoMap[nodeName].id;
            auto offsetMat = boneInfoMap[nodeName].offsetMat;

            m_FinalBoneMatrices[index] = globalTransform * offsetMat;
        }

        for (int i = 0; i < node->childrenCount; i++) {
            CalculateBoneTransform(&node->children[i], globalTransform);
        }
    }

    std::vector<glm::mat4> GetFinalBoneMatrices() {
        return m_FinalBoneMatrices;
    }
};
