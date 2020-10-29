// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_DHT_Particle.h>

#define DHTPIN D2     // what pin we're connected to
#define TMPPIN A0
#define MOIPIN A1

// Uncomment whatever type you're using!
#define DHTTYPE DHT11		// DHT 11 

DHT dht(DHTPIN, DHTTYPE);
int loopCount;

void setup() {
	Serial.begin(9600); 

	dht.begin();
	loopCount = 0;
	delay(2000);
}

void loop() {
	float h = dht.getHumidity();
// Read temperature as Celsius
	float temp_in = dht.getTempCelcius();

// Check if any reads failed and exit early (to try again).
	if (isnan(h) || isnan(temp_in)) {
		Particle.publish("Failed to read from DHT sensor!");
		return;
	}
	float hi = dht.getHeatIndex();
	float dp = dht.getDewPoint();
	float k = dht.getTempKelvin();
	int analogValue = analogRead(TMPPIN);
	int temp_out= map(analogValue,0,2482,-50,150); // 2V max output sensor, 3.3V source Voltage on 12 bit ADC -> 2 / 3.3 * 4096 = 2482
	int deltaTemp = temp_in-temp_out;
	int moisture = analogRead(MOIPIN);
	
	// push data too the cloud
	Particle.publish("Temp outside", String(temp_out));
	Particle.publish("Temp inside", String(temp_in));
	delay(1000);
	Particle.publish("Hum", String(h));
	Particle.publish("DewPoint",String(dp));
	delay(1000);
	Particle.publish("HeatIndex",String(hi));
	Particle.publish("DeltaTemp",String(deltaTemp));
	delay(1000);
	Particle.publish("Moisture",String(moisture));

	delay(10000);
	loopCount++;

	if(loopCount >= 6){
	  Particle.publish("state", "Going to sleep for 5 minutes");
	  delay(1000);
	  System.sleep(SLEEP_MODE_DEEP, 300);  
	}

}

