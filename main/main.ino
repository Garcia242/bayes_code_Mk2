#include "tvc.hpp"

TVC my_tvc = TVC(10, 11);

void setup()
{
    my_tvc.init();
}

void loop()
{
    my_tvc.update(45, 45);
}