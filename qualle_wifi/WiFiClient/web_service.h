#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

WiFiServer server(80);

pthread_t web_thread_t;
pthread_t party_thread_t;
String header;
byte JELLY_LED_PIN = 27;
// 0 = off, 1 = on, 2 = party
int qualle_state = 1;
// 0 = no loop
int party_on_time = 100;
int party_off_time = 100;

void *party_loop_my(void *ptr) {
  while (true) {
    if (qualle_state == 2) {
      digitalWrite(JELLY_LED_PIN, HIGH);
      delay(party_on_time);
      digitalWrite(JELLY_LED_PIN, LOW);
      delay(party_off_time);
    } else {
      delay(500);
    }
  }
}

void initialize_party_mode_loop() {
  if (NULL == party_thread_t) {
    char *type = "Thread 3: PartyThread";
    int result;

    result = pthread_create(&party_thread_t, NULL, party_loop_my, (void *)type);
  }
}


void initialize_web_server() {
  Serial.println("INIT.\n");
  server.begin();
}

void *web_server_loop_my(void *ptr) {
  char *message;
  message = (char *)ptr;
  printf("%s \n", message);

  while (true) {
    delay(500);
    WiFiClient webclient = server.available();

    if (webclient) {
      Serial.println("New Webclient.");
      while (webclient.connected()) {
        if (webclient.available()) {
          header = webclient.readString();
          int n = header.length();

          // declaring character array
          char char_array[n + 1];
          for (int i = 0; i < sizeof(char_array); i++) {
            char_array[i] = header[i];
          }

          Serial.println(header);
          webclient.println("HTTP/1.1 200 OK");
          webclient.println("Content-type:application/json");
          webclient.println("Access-Control-Allow-Origin:*");
          webclient.println("Connection: close");
          webclient.println();
          if (header.indexOf("GET /switch") >= 0) {
            if (qualle_state == 2) {
              digitalWrite(JELLY_LED_PIN, LOW);
              qualle_state = 0;
            } else if (qualle_state == 0) {
              digitalWrite(JELLY_LED_PIN, HIGH);
              qualle_state = 1;
            } else {
              digitalWrite(JELLY_LED_PIN, LOW);
              qualle_state = 0;
            }
            webclient.print("{\"state\": \"");
            webclient.print(qualle_state);
            webclient.println("\"}");
          } else if (header.indexOf("GET /getState") >= 0) {
            webclient.print("{\"state\": \"");
            webclient.print(qualle_state);
            webclient.println("\"}");
          } else if (header.indexOf("GET /party") >= 0) {
            qualle_state = 2;
            int bpmValue;

            String queryParams = "";
            int start = header.indexOf("?");

            if (start >= 0) {
              int end = header.indexOf(" ", start);
              if (end >= 0) {
                queryParams = header.substring(start + 1, end);
              }
            }

            if (queryParams.length() > 0) {
              // Split the query parameters into key-value pairs
              String keyValuePairs = queryParams;
              while (keyValuePairs.length() > 0) {
                int separatorIndex = keyValuePairs.indexOf("&");
                String keyValuePair;
                if (separatorIndex >= 0) {
                  keyValuePair = keyValuePairs.substring(0, separatorIndex);
                  keyValuePairs = keyValuePairs.substring(separatorIndex + 1);
                } else {
                  keyValuePair = keyValuePairs;
                  keyValuePairs = "";
                }

                // Split the key-value pair into key and value
                int equalsIndex = keyValuePair.indexOf("=");
                if (equalsIndex >= 0) {
                  String key = keyValuePair.substring(0, equalsIndex);
                  String value = keyValuePair.substring(equalsIndex + 1);

                  // Check if the key is "bpm" and extract its value
                  if (key.equals("bpm")) {
                    bpmValue = value.toInt();
                    int calc = 60000/bpmValue;
                    party_off_time = calc * 1/3;
                    party_on_time = calc - party_off_time;
                    // Now, `bpmValue` contains the integer value of "bpm"
                    // You can use it as needed
                  }
                }
              }
            }


            webclient.print("{\"state\": \"");
            webclient.print(qualle_state);
            webclient.print("\",\n");
            webclient.print("\"bpm\": \"");
            webclient.print(party_on_time);
            webclient.println("\"}");
          }
          // else if (header.indexOf("POST /alarm/email") >= 0) {
          //   // Extract the first token
          //   char * token = strtok(char_array, "\n");
          //   // loop through the string to extract all other tokens
          //   char * post_data = NULL;
          //   while ( token != NULL ) {
          //     post_data = token;
          //     token = strtok(NULL, "\n");
          //   }
          //   emails = (char *)malloc(strlen(post_data));
          //   strcpy(emails, post_data);
          // } else if (header.indexOf("POST /alarm/off") >= 0) {
          //   // Extract the first token
          //   char * token = strtok(char_array, "\n");
          //   // loop through the string to extract all other tokens
          //   char * post_data = NULL;
          //   while ( token != NULL ) {
          //     post_data = token;
          //     token = strtok(NULL, "\n");
          //   }
          //   Serial.println(atoi(post_data));
          //   switch_off_alarm(atoi(post_data));
          // }
          // give the web browser time to receive the data
          delay(300);
          // Close the connection,
          webclient.stop();
          Serial.println("Webclient disconnected.");
          Serial.println("");
        }
      }
    }
  }
}

void initialize_web_server_loop() {
  if (NULL == web_thread_t) {
    char *type = "Thread 2: webserver";
    int result;

    result = pthread_create(&web_thread_t, NULL, web_server_loop_my, (void *)type);
  }
}
