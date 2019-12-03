//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "qlight"
#define   MESH_PASSWORD   "da-this-is-party"
#define   MESH_PORT       5151

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

String blinkCommand = "light_blink";
String onCommand = "light_on";
String offCommand = "light_off";

String sdata = "";

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

/* Setup the arduino */
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.setRoot(true);
  mesh.setContainsRoot(true);

  // initialize connection
  Serial.println("HELLO!");
}

void processNodeCommand(char* cmdRest) {
	char* token = strtok_r(cmdRest, " ", &cmdRest);
	if (strcmp(token, "ALL")==0) {
		Serial.println("sending to all");
		mesh.sendBroadcast(cmdRest, false);
	} else {
		uint32_t nodeId = strtoul(token, NULL, 10);
		Serial.println("sending to all " + nodeId);
		if (nodeId != 0) {
			mesh.sendSingle(nodeId, cmdRest);
		}
	}
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  byte ch;
  while(Serial.available() != 0) {
	  ch = Serial.read();
	  sdata += (char)ch;

	  if (ch=='\r' || ch=='\n') {
		  sdata.trim();
		  Serial.println("received command "+sdata);

		  // process command
		  char *rest = const_cast<char*>(sdata.c_str());
		  
		  char * token = strtok_r(rest, " ", &rest);
		  if(token != NULL) {
			  if (strcmp(token, "NODE") == 0) {
				processNodeCommand(rest);
			  }
			  // invalid command
		  }

		  // clear command data
		  sdata = "";
	  }
  }
}