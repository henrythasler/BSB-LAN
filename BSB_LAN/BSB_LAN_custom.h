/*
 * If configuration definement CUSTOM_COMMANDS is active, this code is run
 * at the end of each main loop and utilizes the main loop variables
 * custom_timer (set each loop to millis()) and custom_timer_compare.
 */

if (custom_timer > (custom_timer_compare_500ms + 500))
{
  custom_timer_compare_500ms = millis();

  if (adc_data.size() >= 60)
  {
    adc_data.erase(adc_data.begin());
  }
  adc_data.push_back(float(analogRead(PIN_ADC)));  
}

if (custom_timer > (custom_timer_compare_30s + 1000 * 30))
{ // every 10 seconds
  custom_timer_compare_30s = millis();

  float mean = 0;
  if (adc_data.size())
  {
    for (float &p : adc_data)
    {
      mean += p;
    }
    mean = mean / adc_data.size();
  }

  float B = 3528.01, R_N = 1000, T_N = 298.15;
  float a = 8611895, b = -2230.0;
  float RT = a / mean + b;
  float temp = B * T_N / (B + log(RT / R_N) * T_N) - 273.15;

  if (MQTTPubSubClient != nullptr)
  {
    snprintf(textBuffer, sizeof(textBuffer), "{\"raw\": %.1f, \"value\": %.1f, \"timestamp\": %u, \"unit\": \"\u00b0C\"}", mean, temp, 0);
    MQTTPubSubClient->publish(PSTR("home/out/temp"), textBuffer, true);

    snprintf(textBuffer, sizeof(textBuffer), "%.1f", temp);
    MQTTPubSubClient->publish("home/out/temp/value", textBuffer, true);
  }
}
