#include <iostream>
#include "terminal.h"
using namespace std;

int main(){
    Terminal *term = new Terminal(32);
    term->run();
    return 0;
}
