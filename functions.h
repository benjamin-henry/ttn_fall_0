#ifndef APP_FUNCTIONS_H
#define APP_FUNCTIONS_H

void init_model() {
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;
  model = tflite::GetModel(hidden1x8_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report(
      "Model provided is schema version %d not equal "
      "to supported version %d.",
      model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  static tflite::MicroMutableOpResolver micro_mutable_op_resolver;
  micro_mutable_op_resolver.AddBuiltin(
    tflite::BuiltinOperator_FULLY_CONNECTED,
    tflite::ops::micro::Register_FULLY_CONNECTED());
  micro_mutable_op_resolver.AddBuiltin(
    tflite::BuiltinOperator_SOFTMAX,
    tflite::ops::micro::Register_SOFTMAX());


  static tflite::MicroInterpreter static_interpreter(
    model, micro_mutable_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    error_reporter->Report("AllocateTensors() failed");
    return;
  }

  input = interpreter->input(0);
  output = interpreter->output(0);
}

void led_green() {
  analogWrite(PIN_R, 255);
  analogWrite(PIN_G, 0);
  analogWrite(PIN_B, 255);
}

void led_red() {
  analogWrite(PIN_R, 0);
  analogWrite(PIN_G, 255);
  analogWrite(PIN_B, 255);
}

void led_blue() {
  analogWrite(PIN_R, 255);
  analogWrite(PIN_G, 255);
  analogWrite(PIN_B, 0);
}

void led_yellow() {
  analogWrite(PIN_R, 0);
  analogWrite(PIN_G, 0);
  analogWrite(PIN_B, 255);
}

void led_white() {
  analogWrite(PIN_R, 0);
  analogWrite(PIN_G, 0);
  analogWrite(PIN_B, 0);
}

void t1_callback() {
  t1_flag = true;
}

unsigned int compute_n_windows(unsigned int input_length, unsigned int window_length, unsigned int stride, unsigned int padding) {
  return (input_length - window_length + 2*padding) / stride + 1;
}

void axl_acq() {
  if (t1_flag & IMU.accelerationAvailable()) {
    x = y = z = 0.;
    IMU.readAcceleration(x, y, z);
    inc_mag = sqrtf(x * x + y * y + z * z);
    circular_buf_put(mag_buf, inc_mag);
    t1_flag = false;
  }
}

void analyze_mouvement() {
  if (circular_buf_size(mag_buf) == 20) {
    circular_buf_Vinit(mag_buf);
    unsigned int n_w = compute_n_windows(circular_buf_size(mag_buf), WINDOW_SIZE, WINDOW_STRIDE, 0);
    for (unsigned int n = 0; n < n_w; n++) {
      sample = mag_buf->buffer[mag_buf->vtail];
      retreat_vpointer(mag_buf);
      maxi = sample;
      mini = sample;
      max_idx = 0;
      min_idx = 0;
      new_sample = 0.;
      cond = false;
      variation = 0.;

      for (int i = 1; i < WINDOW_SIZE; i++) {
        new_sample = mag_buf->buffer[mag_buf->vtail];
        retreat_vpointer(mag_buf);
        if (maxi <= new_sample) {
          maxi = new_sample;
          max_idx = i;
        }
        if (mini >= new_sample) {
          mini = new_sample;
          min_idx = i;
        }
        variation += abs(new_sample - sample);
        sample = new_sample;
      }
      if ((maxi - mini) > thresh_crit) {
        crit_trig = true;
        circular_buf_reset(cond_buf);
        circular_buf_reset(var_buf);
        led_red();
      }
      if ((maxi - mini) >= thresh & max_idx > min_idx) {
        cond = true;
        if (!trig) {
          trig = true;
          //Serial.println("buffering");
          led_yellow();
          circular_buf_reset(cond_buf);
          circular_buf_reset(var_buf);
        }
      }
      variation /= 20.;
      circular_buf_put(cond_buf, cond == true ? 1. : 0.);
      circular_buf_put(var_buf, variation);
      for (int decr = 0; decr < 5; decr++) {
        decrease_vtail(mag_buf);
      }
    }
    shift_left(mag_buf, WINDOW_STRIDE);
  }

  if (trig && circular_buf_full(cond_buf) && circular_buf_full(var_buf)) {
    int i = 0;
    float tmp = 0.;
    while (!circular_buf_empty(cond_buf)) {
      circular_buf_get(cond_buf, &tmp);
      input->data.f[i++] = tmp;
    }
    tmp = 0.;
    while (!circular_buf_empty(var_buf)) {
      circular_buf_get(var_buf, &tmp);
      input->data.f[i++] = tmp;
    }

    //Serial.println("Invoking model");
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
      TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed\n");
    }

    bool fall_detected = output->data.f[0] > output->data.f[1] ? false : true;
    //Serial.println(fall_detected == true ? "fall" : "not_fall");
    fall_detected == true ? led_blue() : led_green();
    
    set_gps_coords(coords, 46.9909, 3.1628);
    char_buf_append(ttn_payload, coords->bytes, 6);
    char_buf_append(ttn_payload, (unsigned char*)userUuid, 32);


    for (int i = 0; i < ttn_payload->cnt; i++) {
    Serial.print(ttn_payload->data[i]);
    Serial.print(" ");
    }
    Serial.println();

    char_buf_reset(ttn_payload);

    circular_buf_reset(cond_buf);
    circular_buf_reset(var_buf);
    trig = false;
  }
}


#endif
