/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime.cpp
 * Author: rlcancian
 * 
 * Created on 3 de Setembro de 2019, 18:34
 */

#include "Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime.h"

// you have to included need libs

// GEnSyS Simulator
#include "../../../../kernel/simulator/Simulator.h"

// Model Components
#include "../../../../plugins/components/Create.h"
#include "../../../../plugins/components/Assign.h"
#include "../../../../plugins/data/Station.h"
#include "../../../../plugins/components/Dispose.h"

Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime::Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime::main(int argc, char** argv) {
	Simulator* genesys = new Simulator();
	this->setDefaultTraceHandlers(genesys->getTracer());
	this->insertFakePluginsByHand(genesys);
	// crete model
	Model* model = genesys->getModels()->newModel();
	PluginManager* plugins = genesys->getPlugins();
	
        Station* station = plugins->newInstance<Create>(model);
        station->setName("Back Entrance");
        //como usar esse station?
        
        Assign* assign2 = plugins->newInstance<Assign>(model);
        //assign2->
        
	Dispose* dispose1 = plugins->newInstance<Dispose>(model);
	
        
        
        // connect model components to create a "workflow"
	station->getConnections()->insert(assign2);
	dummy1->getConnections()->insert(dispose1);
	
        // set options, save and simulate
	model->getSimulation()->setReplicationLength(60, Util::TimeUnit::second);
	model->save("./models/Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime.gen");
	model->getSimulation()->start();
        
        for(int i = 0; i < 1e9; i++);
	delete genesys;
	return 0;
};
