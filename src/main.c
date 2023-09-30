#include "pd_api.h"

PlaydateAPI* pd;

#define THRESHOLD 100
#define HYSTERESIS 10
#define MIC_BUFLEN 4096
int16_t micdata[MIC_BUFLEN];
int micdatapos = 0;
int running = 0;
double frequency = 0;

int micCallback(void* context, int16_t* data, int len) {
	const int remaining = MIC_BUFLEN - micdatapos;
	len = (len > remaining) ? remaining : len;
	memcpy(&micdata[micdatapos], data, len * sizeof(int16_t));
	micdatapos += len;

	if (micdatapos < MIC_BUFLEN) return 1;

	int zeroCrossings = 0;
	int prev_sign = micdata[0] > 0 ? 1 : -1;

	for (int i = 1; i < MIC_BUFLEN; ++i) {
		const int cur_sign = micdata[i] > 0 ? 1 : -1;
		const int delta = micdata[i] - micdata[i - 1];

		if (abs(delta) <= THRESHOLD) continue;

		if (cur_sign != prev_sign) {
			zeroCrossings++;
			prev_sign = (cur_sign > 0 && micdata[i] > HYSTERESIS) ? 1 :
						(cur_sign < 0 && micdata[i] < -HYSTERESIS) ? -1 : -prev_sign;
		}
	}

	const double sample_rate = 44100.0;
	frequency = (zeroCrossings / 2.0) * (sample_rate / MIC_BUFLEN);
	micdatapos = 0;

	return 1;
}

static int currentFrequency(lua_State *L) {
	pd->lua->pushFloat(frequency);
	return 1;
}

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg) {
	(void)arg;
	if ( event == kEventInit ) {
		pd = playdate;
		pd->sound->setMicCallback(micCallback, NULL, 0);
	} else if (event == kEventInitLua) {
		const char* err;
		if (playdate->lua->addFunction(currentFrequency, "currentFrequency", &err) == 0) {
			playdate->system->logToConsole("Failed to register Lua function: %s", err);
		}
	}
	return 0;
}
