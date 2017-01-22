#include "audio_effect_compressor.h"
#include "servers/audio_server.h"

void AudioEffectCompressorInstance::process(const AudioFrame *p_src_frames,AudioFrame *p_dst_frames,int p_frame_count) {


	float treshold = Math::db2linear(base->treshold);
	float sample_rate=AudioServer::get_singleton()->get_mix_rate();

	float ratatcoef = exp(-1 / (0.00001f * sample_rate));
	float ratrelcoef = exp(-1 / (0.5f * sample_rate));
	float attime = base->attack_us / 1000000.0;
	float reltime = base->release_ms / 1000.0;
	float atcoef = exp(-1 / (attime * sample_rate));
	float relcoef = exp(-1 / (reltime * sample_rate));

	float makeup = Math::db2linear(base->gain);

	float mix = base->mix;
	float gr_meter_decay = exp(1 / (1 * sample_rate));


	for(int i=0;i<p_frame_count;i++) {

		AudioFrame s = p_src_frames[i];
		//convert to positive
		s.l = Math::abs(s.l);
		s.r = Math::abs(s.r);

		float peak = MAX(s.l,s.r);

		float overdb = 2.08136898f * Math::linear2db(peak/treshold);

		if (overdb<0.0) //we only care about what goes over to compress
			overdb=0.0;

		if(overdb-rundb>5) // diffeence is too large
			averatio = 4;

		if(overdb > rundb) {
			rundb = overdb + atcoef * (rundb - overdb);
			runratio = averatio + ratatcoef * (runratio - averatio);
		} else 	{
			rundb = overdb + relcoef * (rundb - overdb);
			runratio = averatio + ratrelcoef * (runratio - averatio);
		}

		overdb = rundb;
		averatio = runratio;

		float cratio;

		if(false) { //rato all-in
			cratio = 12 + averatio;
		} else {
			cratio = base->ratio;
		}

		float gr = -overdb * (cratio-1)/cratio;
		float grv = Math::db2linear(gr);

		runmax = maxover + relcoef * (runmax - maxover);  // highest peak for setting att/rel decays in reltime
		maxover = runmax;

		if (grv < gr_meter) {
			gr_meter=grv;
		} else {
			gr_meter*=gr_meter_decay;
			if(gr_meter>1)
				gr_meter=1;
		}


		p_dst_frames[i] = p_src_frames[i] * grv * makeup * mix + p_src_frames[i] * (1.0-mix);

	}

}


Ref<AudioEffectInstance> AudioEffectCompressor::instance() {
	Ref<AudioEffectCompressorInstance> ins;
	ins.instance();
	ins->base=Ref<AudioEffectCompressor>(this);
	ins->rundb=0;
	ins->runratio=0;
	ins->averatio=0;
	ins->runmax=0;
	ins->maxover=0;
	ins->gr_meter=1.0;
	return ins;
}


void AudioEffectCompressor::set_treshold(float p_treshold) {

	treshold=p_treshold;
}

float AudioEffectCompressor::get_treshold() const {

	return treshold;
}

void AudioEffectCompressor::set_ratio(float p_ratio) {

	ratio=p_ratio;
}
float AudioEffectCompressor::get_ratio() const {

	return ratio;
}

void AudioEffectCompressor::set_gain(float p_gain) {

	gain=p_gain;
}
float AudioEffectCompressor::get_gain() const {

	return gain;
}

void AudioEffectCompressor::set_attack_us(float p_attack_us) {

	attack_us=p_attack_us;
}
float AudioEffectCompressor::get_attack_us() const {

	return attack_us;
}

void AudioEffectCompressor::set_release_ms(float p_release_ms) {

	release_ms=p_release_ms;
}
float AudioEffectCompressor::get_release_ms() const {

	return release_ms;
}

void AudioEffectCompressor::set_mix(float p_mix) {

	mix=p_mix;
}
float AudioEffectCompressor::get_mix() const {

	return mix;
}


void AudioEffectCompressor::_bind_methods() {

	ClassDB::bind_method(_MD("set_treshold","treshold"),&AudioEffectCompressor::set_treshold);
	ClassDB::bind_method(_MD("get_treshold"),&AudioEffectCompressor::get_treshold);

	ClassDB::bind_method(_MD("set_ratio","ratio"),&AudioEffectCompressor::set_ratio);
	ClassDB::bind_method(_MD("get_ratio"),&AudioEffectCompressor::get_ratio);

	ClassDB::bind_method(_MD("set_gain","gain"),&AudioEffectCompressor::set_gain);
	ClassDB::bind_method(_MD("get_gain"),&AudioEffectCompressor::get_gain);

	ClassDB::bind_method(_MD("set_attack_us","attack_us"),&AudioEffectCompressor::set_attack_us);
	ClassDB::bind_method(_MD("get_attack_us"),&AudioEffectCompressor::get_attack_us);

	ClassDB::bind_method(_MD("set_release_ms","release_ms"),&AudioEffectCompressor::set_release_ms);
	ClassDB::bind_method(_MD("get_release_ms"),&AudioEffectCompressor::get_release_ms);

	ClassDB::bind_method(_MD("set_mix","mix"),&AudioEffectCompressor::set_mix);
	ClassDB::bind_method(_MD("get_mix"),&AudioEffectCompressor::get_mix);

	ADD_PROPERTY(PropertyInfo(Variant::REAL,"treshold",PROPERTY_HINT_RANGE,"-60,0,0.1"),_SCS("set_treshold"),_SCS("get_treshold"));
	ADD_PROPERTY(PropertyInfo(Variant::REAL,"ratio",PROPERTY_HINT_RANGE,"1,48,0.1"),_SCS("set_ratio"),_SCS("get_ratio"));
	ADD_PROPERTY(PropertyInfo(Variant::REAL,"gain",PROPERTY_HINT_RANGE,"-20,20,0.1"),_SCS("set_gain"),_SCS("get_gain"));
	ADD_PROPERTY(PropertyInfo(Variant::REAL,"attack_us",PROPERTY_HINT_RANGE,"20,2000,1"),_SCS("set_attack_us"),_SCS("get_attack_us"));
	ADD_PROPERTY(PropertyInfo(Variant::REAL,"release_ms",PROPERTY_HINT_RANGE,"20,2000,1"),_SCS("set_release_ms"),_SCS("get_release_ms"));
	ADD_PROPERTY(PropertyInfo(Variant::REAL,"mix",PROPERTY_HINT_RANGE,"0,1,0.01"),_SCS("set_mix"),_SCS("get_mix"));

}

AudioEffectCompressor::AudioEffectCompressor()
{
	treshold=0;
	ratio=4;
	gain=0;
	attack_us=20;
	release_ms=250;
	mix=1;
}