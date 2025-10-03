#include "FIRFilter.h"
#include <stdlib.h>
//static float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGTH] = {0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f,0.05f}; //20 Point moving average filter

float* FIR_IMPULSE_RESPONSE;
void FIRFilter_Init(FIRFilter *fir) {
	FIR_IMPULSE_RESPONSE = (float*)malloc(sizeof(float)*FIR_FILTER_LENGTH);
	for(int i = 0; i < FIR_FILTER_LENGTH; i++)
	{
		FIR_IMPULSE_RESPONSE[i] = val;
	}

	/* Clear filter buffer */
	for (uint8_t n = 0; n < FIR_FILTER_LENGTH; n++) {

		fir->buf[n] = 0.0f;

	}

	/* Reset buffer index */
	fir->bufIndex = 0;

	/* Clear filter output */
	fir->out = 0.0f;

}

float FIRFilter_Update(FIRFilter *fir, float inp) {

	/* Store latest sample in buffer */
	fir->buf[fir->bufIndex] = inp;

	/* Increment buffer index and wrap around if necessary */
	fir->bufIndex++;

	if (fir->bufIndex == FIR_FILTER_LENGTH) {

		fir->bufIndex = 0;

	}

	/* Compute new output sample (via convolution) */
	fir->out = 0.0f;

	uint8_t sumIndex = fir->bufIndex;

	for (uint8_t n = 0; n < FIR_FILTER_LENGTH; n++) {

		/* Decrement index and wrap if necessary */
		if (sumIndex > 0) {

			sumIndex--;

		} else {

			sumIndex = FIR_FILTER_LENGTH - 1;

		}

		/* Multiply impulse response with shifted input sample and add to output */
		fir->out += FIR_IMPULSE_RESPONSE[n] * fir->buf[sumIndex];
		//fir->out += 1/FIR_FILTER_LENGTH * fir->buf[sumIndex];

	}

	/* Return filtered output */
	return fir->out;

}
