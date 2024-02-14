# esp32_aws_iot

Playing around with ESP32 board, linked to MQTT (AWS IoT) - Sends messages in both directions.
I'm testing on a Heltec board with LCD, so displaying messages on-screen.

 
### Getting started
- AWS IoT core - Create thing (single)
- Auto generate certificates (download all!)
- Attach new policy to certificate with allow for iot:Publish, iot:Subscribe, iot:Receive, iot:Connect
- Add PlatformIO VSCode extension (manages external libs)
- Update include/config.h with certificates, wifi info and IoT endpoint (`aws iot describe-endpoint --endpoint-type iot:Data-ATS`)
- Use PlatformIO build/deploy/monitor buttons to get onto ESP device (Will need powering up holding down button to go into flashing mode!)
- Use AWS IoT MQTT client to publish json message to device
