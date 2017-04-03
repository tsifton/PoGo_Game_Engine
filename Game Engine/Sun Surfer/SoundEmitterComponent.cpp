#include "SoundEmitterComponent.h"

#include <cstring>

bool SoundEmitterComponent::Initialize()
{
	m_sound.create(m_filepath, m_channel, m_onLoop, m_volume, false);
	m_sound.setRelative(m_relative);
	m_sound.setSize(m_range);
	m_sound.setOcclusion(m_occlusion);
	m_sound.setDoppler(m_doppler);
	Play();
	return true;
}

bool SoundEmitterComponent::Update(Entity & entity, float /*m_dt*/)
{
	if (m_relative) m_sound.setPosition(YSE::Vec(m_relativePos.x, m_relativePos.y, -m_relativePos.z));
	else			m_sound.setPosition(YSE::Vec(entity.worldSpace.position.x + m_relativePos.x, entity.worldSpace.position.y + m_relativePos.y, -entity.worldSpace.position.z + m_relativePos.z));
	return true;
}

void SoundEmitterComponent::Play()
{
	m_sound.play();
}

void SoundEmitterComponent::Pause()
{
	m_sound.pause();
}

void SoundEmitterComponent::Stop()
{
	m_sound.stop();
}

void SoundEmitterComponent::Restart()
{
	m_sound.restart();
}

void SoundEmitterComponent::EnableOcclusion()
{
	m_occlusion = true;
}

void SoundEmitterComponent::DisableOcclusion()
{
	m_occlusion = false;
}

void SoundEmitterComponent::EnableDoppler()
{
	m_doppler = true;
}

void SoundEmitterComponent::DisableDoppler()
{
	m_doppler = false;
}

void SoundEmitterComponent::SetFilePath(const char * filepath)
{
	if (filepath != nullptr) strncpy_s(m_filepath, filepath, 256);
}

void SoundEmitterComponent::SetOnLoop(bool onLoop)
{
	m_onLoop = onLoop;
}

void SoundEmitterComponent::SetVolume(float volume)
{
	m_volume = volume;
}

void SoundEmitterComponent::SetChannel(YSE::channel * channel)
{
	m_channel = channel;
}

void SoundEmitterComponent::SetIs3D(bool is3D)
{
	m_relative = !is3D;
}

void SoundEmitterComponent::SetPosition(glm::vec3 position)
{
	m_relativePos = position;
}

void SoundEmitterComponent::SetRange(float distance)
{
	m_range = distance;
}
