/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Smart_EvaluatingConditionsBeforeEnteringQueue.cpp
 * Author: rlcancian
 * 
 * Created on 3 de Setembro de 2019, 18:34
 */

#include <cmath>

#include "Smart_EvaluatingConditionsBeforeEnteringQueue.h"

// you have to included need libs

// GEnSyS Simulator
#include "../../../../kernel/simulator/Simulator.h"

// Model Components
#include "../../../../plugins/components/Create.h"
#include "../../../../plugins/components/Assign.h"
#include "../../../../plugins/components/Decide.h"
#include "../../../../plugins/components/Process.h"
#include "../../../../plugins/components/Dispose.h"

Smart_EvaluatingConditionsBeforeEnteringQueue::Smart_EvaluatingConditionsBeforeEnteringQueue() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Smart_EvaluatingConditionsBeforeEnteringQueue::main(int argc, char** argv) {
	Simulator* genesys = new Simulator();
	this->setDefaultTraceHandlers(genesys->getTracer());
	this->insertFakePluginsByHand(genesys);
	
        
        // crete model
	Model* model = genesys->getModels()->newModel();
	PluginManager* plugins = genesys->getPlugins();
        
        Variable* procStartTime = plugins->newInstance<Variable>(model, "procstarttime");
        Variable* procTime      = plugins->newInstance<Variable>(model, "proctime");
	
        EntityType* entityType = plugins->newInstance<EntityType>(model, "Entity 1");
        
        Create* create = plugins->newInstance<Create>(model, "Create Entity");
        create->setEntityType(entityType);
        create->setTimeBetweenCreationsExpression("60");
        create->setTimeUnit(Util::TimeUnit::minute);
        create->setEntitiesPerCreation(1);
        create->setFirstCreation(0);
//        create->setMaxCreations(INFINITY);
        
        Attribute* timeIn = plugins->newInstance<Attribute>(model, "Time_In");
        
        Assign* assign1        = plugins->newInstance<Assign>(model, "Assign attribute time in equal to current time variable TNOW");
        assign1->getAssignments()->insert(new Assignment(model, "Time_In", "TNOW"));
        
        std::string timeGoneBy        = "(TNOW - procstarttime)";
        std::string remainingProcTime = "mx(0, proctime - " + timeGoneBy + ")";
        Decide* decide = plugins->newInstance<Decide>(model, "Is the remaining process time less than 20?");
        decide->getConditions()->insert(remainingProcTime + " < 20");
        
        Process* process1 = plugins->newInstance<Process>(model, "Begin processing by seizing resource");
        
        Assign* assign2        = plugins->newInstance<Assign>(model, "Assign variables Procstarttime and Proctime");
        assign2->getAssignments()->insert(new Assignment(model, "procstarttime", "TNOW", false));
        assign2->getAssignments()->insert(new Assignment(model, "proctime", "expo(480)", false));
        
//        Process* process2 = plugins->newInstance<Process>(model, "Begin processing delay then release resource");
//	
//	Dispose* disposeTrue  = plugins->newInstance<Dispose>(model, "Dispose of entity");
//        Dispose* disposeFalse = plugins->newInstance<Dispose>(model, "Dispose of entity if condition not met");
	
        
        // connect model components to create a "workflow"
//	create1->getConnections()->insert(dummy1);
//	dummy1->getConnections()->insert(dispose1);
	
        
        // set options, save and simulate
	model->getSimulation()->setReplicationLength(60, Util::TimeUnit::second);
	model->save("./models/Smart_EvaluatingConditionsBeforeEnteringQueue.gen");
	model->getSimulation()->start();
        
        for(int i = 0; i < 1e9; i++);
	delete genesys;
	return 0;
};

