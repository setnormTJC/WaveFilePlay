#pragma once
/*Attack, decay, sustain, release - see (for example):
https://www.muzines.co.uk/articles/back-to-basics/1882
*/
class adsrEnvelope
{
	float attackTime; 
	float decayTime; 
	float sustainLevel; //NOTE: this is an amplitude (not units of time, I think)
	float releaseTime; 

public: 
	adsrEnvelope() = delete; 

	adsrEnvelope(float attackTime, float decayTime, float sustainLevel, float releaseTime); 
};

