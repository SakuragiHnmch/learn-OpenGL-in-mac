//
// Created by Junkang on 2023/4/26.
//

#pragma once

#include <utility>
#include <vector>
#include <assimp/scene.h>
#include <list>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

struct KeyPosition {
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation {
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale {
    glm::vec3 scale;
    float timeStamp;
};


class Bone {
private:
    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScale> m_Scales;
    unsigned int m_NumPositions;
    unsigned int m_NumRotations;
    unsigned int m_NumScalings;

    glm::mat4 m_LocalTransform;
    std::string m_Name;
    int m_ID;

public:
    /* read keyFrames fron aiNodeAnim */
    Bone(std::string name, int ID, const aiNodeAnim* channel)
        :m_Name(std::move(name)), m_ID(ID), m_LocalTransform(1.0)
    {
        m_NumPositions = channel->mNumPositionKeys;
        for (int positionIndex = 0; positionIndex < m_NumPositions; positionIndex++) {
            KeyPosition data;
            data.position = glm::make_vec3(&channel->mPositionKeys[positionIndex].mValue.x);
            data.timeStamp = channel->mPositionKeys[positionIndex].mTime;

            m_Positions.emplace_back(data);
        }

        m_NumRotations = channel->mNumRotationKeys;
        for (int rotationIndex = 0; rotationIndex < m_NumRotations; rotationIndex++) {
            KeyRotation data;

            auto quat = channel->mRotationKeys[rotationIndex].mValue;
            data.orientation.x = quat.x;
            data.orientation.y = quat.y;
            data.orientation.z = quat.z;
            data.orientation.w = quat.w;

            data.timeStamp = channel->mRotationKeys[rotationIndex].mTime;

            m_Rotations.emplace_back(data);
        }

        m_NumScalings = channel->mNumScalingKeys;
        for (int scaleIndex = 0; scaleIndex < m_NumScalings; scaleIndex++) {
            KeyScale data;
            data.scale = glm::make_vec3(&channel->mScalingKeys[scaleIndex].mValue.x);
            data.timeStamp = channel->mScalingKeys[scaleIndex].mTime;

            m_Scales.emplace_back(data);
        }
    }

    /**
     *  interpolate postions rotations and scales keys based on the current time of animation
     *  and prepare the local matrix
    * */
    void Update(float animationTime) {
        auto translation = InterpolatePosition(animationTime);
        auto rotation = InterpolateRotation(animationTime);
        auto scale = InterpolateScaling(animationTime);

        m_LocalTransform = translation * rotation * scale;
    }

    glm::mat4 GetLocalTransform() { return m_LocalTransform; }
    std::string GetBoneName() const { return m_Name; }
    int GetBoneID() { return m_ID; }

    int GetPositionIndex(float animationTime)
    {
        for (int index = 0; index < m_NumPositions - 1; ++index)
        {
            if (animationTime < m_Positions[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }

    // Gets the current index on mKeyPositions to interpolate to based on the current animation time
    int GetRotationIndex(float animationTime)
    {
        for (int index = 0; index < m_NumRotations - 1; ++index)
        {
            if (animationTime < m_Rotations[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }

    int GetScaleIndex(float animationTime)
    {
        for (int index = 0; index < m_NumScalings - 1; ++index)
        {
            if (animationTime < m_Scales[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }

private:
    // Gets normalized value for lerp and Slerp
    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) {
        float midWayLength = animationTime - lastTimeStamp;
        float frameDiff = nextTimeStamp - lastTimeStamp;
        return midWayLength / frameDiff;
    }

    glm::mat4 InterpolatePosition(float animationTime) {
        if (m_NumPositions == 1) {
            return glm::translate(glm::mat4(1.0f), m_Positions[0].position);
        }

        int pos0 = GetPositionIndex(animationTime);
        int pos1 = pos0 + 1;
        float scaleFactor = GetScaleFactor(m_Positions[pos0].timeStamp, m_Positions[pos1].timeStamp, animationTime);
        auto finalPos = glm::mix(m_Positions[pos0].position, m_Positions[pos1].position, scaleFactor);

        return glm::translate(glm::mat4(1.0f), finalPos);
    }

    glm::mat4 InterpolateRotation(float animationTime) {
        if (m_NumRotations == 1) {
            return glm::toMat4(glm::normalize(m_Rotations[0].orientation));
        }

        int rot0 = GetRotationIndex(animationTime);
        int rot1 = rot0 + 1;
        float scaleFactor = GetScaleFactor(m_Rotations[rot0].timeStamp, m_Rotations[rot1].timeStamp, animationTime);
        auto finalRot = glm::slerp(m_Rotations[rot0].orientation, m_Rotations[rot1].orientation, scaleFactor);

        return glm::toMat4(glm::normalize(finalRot));
    }

    glm::mat4 InterpolateScaling(float animationTime) {
        if (m_NumScalings == 1) {
            return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);
        }

        int scale0 = GetScaleIndex(animationTime);
        int scale1 = scale0 + 1;
        float scaleFactor = GetScaleFactor(m_Scales[scale0].timeStamp, m_Scales[scale1].timeStamp, animationTime);
        auto finalScale = glm::mix(m_Scales[scale0].scale, m_Scales[scale1].scale, scaleFactor);

        return glm::scale(glm::mat4(1.0f), finalScale);
    }
};