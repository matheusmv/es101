#include "state.h"

#define RED_PIN 13
#define YELLOW_PIN 12
#define GREEN_PIN 11

#define BUTTON_PIN 10

#define BUTTON_UP LOW
#define BUTTON_DOWN HIGH

void exec_button_action(State*);
void set_next_state(State*);

struct State g_traffic_light;

void setup() {
  Serial.begin(9600);

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT);

  g_traffic_light = {
    .led_state = RED,
    .led = { .r = HIGH, .y = LOW, .g = LOW },
    .button = { .state = BUTTON_UP, .action = request_yellow_led },
    .timer = millis(),
    .interval = 3000,
    .next = yellow_up
  };
}

void loop() {
  g_traffic_light.button.state = digitalRead(BUTTON_PIN);

  set_next_state(&g_traffic_light);
}

void set_next_state(State* state) {
  if (state == NULL || !state->next)
    return;

  if (state->button.state == BUTTON_DOWN)
    exec_button_action(state);

  state->next(state);

  digitalWrite(RED_PIN, state->led.r);
  digitalWrite(YELLOW_PIN, state->led.y);
  digitalWrite(GREEN_PIN, state->led.g);
}

void exec_button_action(State* state) {
  if (!state || !state->button.action)
    return;

  state->button.action(state);
}
