#include "defaults.hpp"
#include "window.hpp"

int main() {
  State state;
  Window window(windowDefaults.width, windowDefaults.height, &state);

  while (window.update(state)) {
  }
  
  return 0;
}
