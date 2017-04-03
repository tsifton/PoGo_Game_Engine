#ifndef SOUNDEMITTERCOMPONENT_H_
#define SOUNDEMITTERCOMPONENT_H_

#include "Component.h"

#pragma warning(push)
#pragma warning(disable: 4458)
#include "yse.hpp"
#pragma warning(pop)

class SoundEmitterComponent : public Component
{
public:
	SoundEmitterComponent() {};
	~SoundEmitterComponent() {};

public:
	bool Initialize() override;
	bool Update(Entity& entity, float dt) override;

	void Play();
	void Pause();
	void Stop();
	void Restart();

	void EnableOcclusion();
	void DisableOcclusion();

	void EnableDoppler();
	void DisableDoppler();

	void SetFilePath(const char* filepath);
	void SetOnLoop(bool onLoop);
	void SetVolume(float volume);
	void SetChannel(YSE::channel* channel);
	void SetIs3D(bool is3D = true);
	void SetPosition(glm::vec3 position);
	void SetRange(float distance);

private:
	char m_filepath[256];

	bool m_relative = false;
	bool m_onLoop;
	bool m_occlusion = false;
	bool m_doppler = false;

	float m_volume = 1.0f;
	float m_range;

	glm::vec3 m_relativePos;
	YSE::channel* m_channel;
	YSE::sound m_sound;
};
#endif // !SOUNDEMITTERCOMPONENT_H_
