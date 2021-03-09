#ifndef APP_VARIABLES_H
#define APP_VARIABLES_H


namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

constexpr int kTensorArenaSize = 8000;
uint8_t tensor_arena[kTensorArenaSize];
}

float x, y, z;
float inc_mag;
float mean = 0.;
float variation = 0.;
float maxi;
float mini;
int max_idx;
int min_idx;
bool cond;
float sample;
float new_sample;

float thresh = 2.5;
float thresh_crit = 8.;

bool crit_trig = false;
bool trig = false;

float magnitudes[MAGNITUDE_BUFFER_LEN] = {1.};
float conditions[N_WINDOWS] = {0.};
float variations[N_WINDOWS] = {0.};

cbuf_handle_t mag_buf = circular_buf_init(magnitudes, MAGNITUDE_BUFFER_LEN);
cbuf_handle_t cond_buf = circular_buf_init(conditions, N_WINDOWS);
cbuf_handle_t var_buf = circular_buf_init(variations, N_WINDOWS);

gps_coords coords = gps_coords_init();
char_buf ttn_payload = char_buf_init(64);

ThreadedTimer t1;
int task1;
bool t1_flag = false;

#endif
