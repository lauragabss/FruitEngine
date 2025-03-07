#pragma once

#include <glm/glm.hpp>

struct KeyboardControlledComponent
{
	glm::vec2 UpVelocity;
	glm::vec2 RightVelocity;
	glm::vec2 DownVelocity;
	glm::vec2 LeftVelocity;

	KeyboardControlledComponent(glm::vec2 upVelocity = glm::vec2(0), glm::vec2 rightVelocity = glm::vec2(0), glm::vec2 downVelocity = glm::vec2(0), glm::vec2 leftVelocity = glm::vec2(0)) {
		this->UpVelocity = upVelocity;
		this->RightVelocity = rightVelocity;
		this->DownVelocity = downVelocity;
		this->LeftVelocity = leftVelocity;
	}
};
