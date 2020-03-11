@echo off

MD build > nul 2>&1
CD build
cmake ..

PAUSE