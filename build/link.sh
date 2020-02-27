#!/bin/bash

cd ~/Desktop/dev/cpp/IATE/Second_Term/DataBase/build/
g++ -g ./obj/*.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
./sfml-app
cd ~/Desktop/dev/cpp/IATE/Second_Term/DataBase/