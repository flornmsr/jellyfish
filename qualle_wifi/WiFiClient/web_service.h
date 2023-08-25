#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

WiFiServer server(80);

pthread_t web_thread_t;
String header;
byte QUALLE = 27;
// 0 = off, 1 = on
int qualle_state = 1;

void initialize_web_server()
{
  Serial.println("INIT.\n");
  server.begin();
}

void *web_server_loop_my( void *ptr )
{
  char *message;
  message = (char *) ptr;
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
            
            if(qualle_state == 0){
              digitalWrite(QUALLE, HIGH); 
              qualle_state = 1;
            } else {
              digitalWrite(QUALLE, LOW);
              qualle_state = 0;
            }
            webclient.print("{\"state\": \"");
            webclient.print(qualle_state);
            webclient.println("\"}");
          } else if("GET /getState"){
            webclient.print("{\"state\": \"");
            webclient.print(qualle_state);
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

void initialize_web_server_loop()
{
  if (NULL == web_thread_t) {
    char *type = "Thread 2: webserver";
    int  result;

    result = pthread_create( &web_thread_t, NULL, web_server_loop_my, (void*) type);
  }

}
