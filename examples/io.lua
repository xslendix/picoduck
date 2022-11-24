LED_PIN = 25 -- Built-in LED

pinMode(LED_PIN, OUTPUT)
digitalWrite(LED_PIN, HIGH)
wait(2000)
digitalWrite(LED_PIN, LOW)
wait(2000)
blinkLed(100) -- Default rate is 1000

