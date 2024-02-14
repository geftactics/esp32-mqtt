#include <pgmspace.h>

#define THINGNAME "xxxxxxxxxx"

const char WIFI_SSID[] = "xxxxxxxxxx";
const char WIFI_PASSWORD[] = "xxxxxxxxxx";
const char AWS_IOT_ENDPOINT[] = "xxxxxxxxxxxxx-ats.iot.eu-west-1.amazonaws.com";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
-----END RSA PRIVATE KEY-----
)KEY";