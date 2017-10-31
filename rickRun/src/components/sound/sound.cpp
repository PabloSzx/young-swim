#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>

//#include "gl_utils.h"

#include <AL/al.h>
#include <AL/alc.h>
#include "sound.hpp"
#include "AL/alut.h"

#include <iostream>

using namespace std;


sound::sound(const char* filename)
{
	this->filename = (char*)filename;

	ALboolean enumeration;
	ALboolean loop = AL_FALSE;
}

void sound::set_listener(const glm::vec3& o)
{
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	alListener3f(AL_POSITION, o[0], o[1], o[2]);
  alListener3f(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listenerOri);
}

void sound::definir_fuente(const glm::vec3& f, ALboolean l)
{
	alGenSources((ALuint)1, &source);
	alSourcef(this->source, AL_PITCH, 1);
	alSourcef(this->source, AL_GAIN, 1);
	alSource3f(this->source, AL_POSITION, f[0], f[1], f[2]);
	alSource3f(this->source, AL_VELOCITY, 5, 0, 0);
	alSourcei(this->source, AL_LOOPING, l);
}
void sound::set_gain(float f) {
	alSourcef(this->source, AL_GAIN, f);
}
void sound::play()
{
	alutInit(0, NULL);
	alGetError();
	this->buffer = alutCreateBufferFromFile(this->filename);
	alGenSources(1, &this->source);

	alSourcei(this->source, AL_BUFFER, this->buffer);
	alSourcePlay(this->source);
}

void sound::stop()
{
	alGenBuffers(1, &buffer);
	alBufferData(this->buffer, this->format, this->data, this->size, this->freq);

	alSourcei(this->source, AL_BUFFER, this->buffer);
	alSourceStop(this->source);
}

void sound::check() {

	ALint bufferID, bufferSize, frequency, bitsPerSample, channels;
    alGetSourcei(this->source, AL_BUFFER, &bufferID);
    alGetBufferi(bufferID, AL_SIZE, &bufferSize);
    alGetBufferi(bufferID, AL_FREQUENCY, &frequency);
    alGetBufferi(bufferID, AL_CHANNELS, &channels);    
    alGetBufferi(bufferID, AL_BITS, &bitsPerSample);    

	ALfloat durationInSeconds = ((ALfloat)bufferSize)/(frequency*channels*(bitsPerSample/8));
	
    ALfloat s;
    alGetSourcef(this->source, AL_SEC_OFFSET, &s);
    cout << "sound01 va en el segundo " << s << " de " << durationInSeconds << endl;
}

ALfloat sound::timeToEnd() {
	ALint bufferID, bufferSize, frequency, bitsPerSample, channels;
    alGetSourcei(this->source, AL_BUFFER, &bufferID);
    alGetBufferi(bufferID, AL_SIZE, &bufferSize);
    alGetBufferi(bufferID, AL_FREQUENCY, &frequency);
    alGetBufferi(bufferID, AL_CHANNELS, &channels);    
    alGetBufferi(bufferID, AL_BITS, &bitsPerSample);    

	ALfloat durationInSeconds = ((ALfloat)bufferSize)/(frequency*channels*(bitsPerSample/8));
	
    ALfloat s;
	alGetSourcef(this->source, AL_SEC_OFFSET, &s);
	
	return durationInSeconds - s;
    cout << "background va en el segundo " << s << " de " << durationInSeconds << endl;
}


ALint sound::get_source_state(){
	alGetSourcei(this->source, AL_SOURCE_STATE, &this->source_state);
	return this->source_state;
}

ALuint sound::get_source(){
	return this->source;
}

ALuint sound::get_num_obj(){
	return this->num_obj;
}

void sound::set_num_obj(ALint n){
	this->num_obj = n;
}

void sound::limpia_contexto_recursos(){
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	this->device = alcGetContextsDevice(this->context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(this->context);
	alcCloseDevice(this->device);
}
