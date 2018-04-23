/*
 * main.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 *      CS 381 INFO:
 *      Author: Grant Hooks with Gary Situ
 *      Date: 3/27/18
 *      For: Assignment 5 CS 381 Spring 2018
 */

#include <Engine.h>

int main(int argc, char *argv[]){

	Engine *engine = new Engine();

	engine->Init();
	engine->Run();
	engine->Cleanup();

	return 0;
}


