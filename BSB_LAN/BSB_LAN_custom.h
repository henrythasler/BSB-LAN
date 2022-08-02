/* 
 * If configuration definement CUSTOM_COMMANDS is active, this code is run 
 * at the end of each main loop and utilizes the main loop variables 
 * custom_timer (set each loop to millis()) and custom_timer_compare.
*/

if (custom_timer > (custom_timer_compare + 30000)) {    // every 20 seconds  
  custom_timer_compare = millis();

  float mean = 32.2;
  float temp = 42.4;

  uint8_t txBuffer[256];

  snprintf((char *)txBuffer, sizeof(txBuffer), "{\"raw\": %.1f, \"value\": %.1f, \"timestamp\": " PRIi64 ", \"unit\": \"\u00b0C\"}", mean, temp, (esp_timer_get_time() / 1000000LL));
  MQTTPubSubClient->publish("home/out/temp", txBuffer, true);
  

  Serial.print(F("Absolute humidity inside: "));
}
