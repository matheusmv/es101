#include <stdbool.h>
#include <stdint.h>
#include <time.h>

typedef enum CurrentState {
  RED,
  YELLOW,
  GREEN
} CurrentState;

typedef struct State State;

typedef void (*transition_func)(State*);

struct State {
  CurrentState led_state;

  struct {
    uint8_t r;
    uint8_t y;
    uint8_t g;
  } led;

  struct {
    uint8_t state;

    transition_func action;
  } button;

  time_t timer;
  time_t interval;

  transition_func next;
};

void red_up(State* state);
void yellow_up(State* state);
void green_up(State* state);

bool reach_interval(State* state) {
  if (!state)
    return false;

  return millis() - state->timer >= state->interval;
}

void red_up(State* state) {
  if (!state)
    return;

  if (!reach_interval(state))
    return;

  state->timer = millis();

  state->led_state = RED;

  state->led.r = HIGH;
  state->led.y = LOW;
  state->led.g = LOW;

  state->next = yellow_up;
}

void yellow_up(State* state) {
  if (!state)
    return;

  if (!reach_interval(state))
    return;

  state->timer = millis();

  state->led_state = YELLOW;

  state->led.r = LOW;
  state->led.y = HIGH;
  state->led.g = LOW;

  state->next = green_up;
}

void green_up(State* state) {
  if (!state)
    return;

  if (!reach_interval(state))
    return;

  state->timer = millis();

  state->led_state = GREEN;

  state->led.r = LOW;
  state->led.y = LOW;
  state->led.g = HIGH;

  state->next = red_up;
}

void request_yellow_led(State* state) {
  if (!state)
    return;

  if (state->led_state == GREEN)
    state->next = yellow_up;

  state->button.state = LOW;
}
