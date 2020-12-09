#include "application/Application.h"

int main(int argc, char** argv) {
  term_engine::application::Init();
  term_engine::application::Run();
  term_engine::application::CleanUp();

  return 0;
}
