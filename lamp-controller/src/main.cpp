//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"
#include "light_control/light_control.h"

#define   MESH_PREFIX     "qlight"
#define   MESH_PASSWORD   "da-this-is-party"
#define   MESH_PORT       5151

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
LightControl lightControl;
long timed = 0;
// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("Message received from %u msg=%s\n", from, msg.c_str());

  // read COMMAND
  char *cmdRest = const_cast<char*>(msg.c_str());
  char* token;

  while((token = strtok_r(cmdRest, " ", &cmdRest))) {
    if (strcmp(token,"SOLID")==0 || strcmp(token, "BLINK")==0) {
      String command(token);
      // read RGB
      token = strtok_r(cmdRest, " ", &cmdRest);
      char* rgbRest;
      char* R = strtok_r(token, ",", &rgbRest);
      int red = atoi(R);
      char* G = strtok_r(rgbRest, ",", &rgbRest);
      int green = atoi(G);
      char* B = strtok_r(rgbRest, ",", &rgbRest);
      int blue = atoi(B);

      lightControl.setRGB(red, green, blue);
      Serial.println(command);
      if (command.equals("SOLID")) {
        lightControl.setMode(SOLID);
      } else {
        lightControl.setMode(BLINK);
      }
    } else if(strcmp(token, "RAINBOW") ==0 || strcmp(token, "OFF")==0) {
      String command(token);
      if (command.equals("RAINBOW")) {
        lightControl.setMode(RAINBOW);
      } else {
        lightControl.setMode(OFF);
      }
    } else if(strcmp(token, "BRIGHTNESS")==0) {
      String command(token);
      token = strtok_r(cmdRest, " ", &cmdRest);
      int brightness = atoi(token);
    } else {
      Serial.print("Unknown command\n");
    }
  }
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

/* Setup the arduino */
void setup() {
  Serial.begin(115200);
  lightControl.init();

// mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.setContainsRoot(true);
  // lightControl.setMode(RAINBOW);
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  lightControl.step();
  if (millis()-timed > 10000) {
    timed = millis();
    Serial.println(ESP.getFreeHeap());
    Serial.println(ESP.getVcc());
    Serial.println("----------");
  }
}