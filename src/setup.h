//Setup your credentials and mqtt info here:
#define WIFI_SSID "romiot"//**Your SSID here**
#define WIFI_PWD "RomIOT!!"//**Your password here** leave empty if open (bad!)

#define MQTT_SERVER "192.168.1.46"//**Your IP address here**
#define MQTT_USERNAME ""//leave empty if not set (bad!)
#define MQTT_PASSWORD ""//leave empty if not set (bad!)
#define MQTT_PORT 1883

#define FREQUENCY 10000 //query values every 30 sec
#define RX_PIN    36// Pin connected to the TX pin of X10A 
#define TX_PIN    26// Pin connected to the RX pin of X10A
#define PIN_THERM 0// Pin connected to the thermosta relay (normally open)

#define MAX_MSG_SIZE 4096//max size of the json message sent in mqtt 

//Uncomment *ONE* of the following according to your installation.
//Then, open and edit the selected file in the include folder and uncomment each values you are interested in.

//#include "def/ALTHERMA(BIZONE_CB_04-08KW).h"
//#include "def/ALTHERMA(BIZONE_CB_11-16KW).h"
//#include "def/ALTHERMA(GSHP).h"
//#include "def/ALTHERMA(GSHP2).h"
//#include "def/ALTHERMA(HPSU6_ULTRA).h"
//#include "def/ALTHERMA(HYBRID).h"
//#include "def/ALTHERMA(LT-D7_E_BML).h"
#include "def/ALTHERMA(LT_11-16KW_HYDROSPLIT_HYDRO_UNIT).h"
//#include "def/ALTHERMA(LT_CA_CB_04-08KW).h"
//#include "def/ALTHERMA(LT_CA_CB_11-16KW).h"
//#include "def/ALTHERMA(LT_DA_04-08KW).h"
//#include "def/ALTHERMA(LT_DA_PAIR_BML).h"
//#include "def/ALTHERMA(LT_GAS_INJ)20200702.h"
//#include "def/ALTHERMA(LT_MULTI_DHWHP).h"
//#include "def/ALTHERMA(LT_MULTI_HYBRID).h"
//#include "def/ALTHERMA(MONOBLOC_CA_05-07KW).h"
//#include "def/ALTHERMA(TOP-GRADE).h"
//#include "def/DAIKIN_MINI_INVERTER_CHILLER04-08KW.h"
//#include "def/DEFAULT.h"
