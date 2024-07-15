void setup() {
  // put your setup code here, to run once:
  DynamicJsonDocument doc(1024);
  JsonObject res = doc.to<JsonObject>(1024);

  res["tagUID"] = "12 34 56 78";
  res["lockID"] = "123456789012";

  String output;
  serializeJson(doc, output);

  Serial.print("OUTPUT: ");
  Serial.println(output);
}

void loop() {
  // put your main code here, to run repeatedly:

}
