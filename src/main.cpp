#include "defaults.hpp"
#include "window.hpp"

int main() {
  State state;
  const Window window(windowDefaults.width, windowDefaults.height, &state);

  while (window.is_open()) {
  }
  
  return 0;
}
