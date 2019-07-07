@echo off
call mvn clean
call mvn compile
call mvn verify -Dcucumber.options="--tags @Vehicule"
pause
