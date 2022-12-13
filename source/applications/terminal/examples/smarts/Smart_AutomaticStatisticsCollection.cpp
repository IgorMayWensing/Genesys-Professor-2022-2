/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Smart_AutomaticStatisticsCollection.cpp
 * Authors: Igor May Wensing
 *          Isac Campos
 * 
 * Created on 11 de Dezembro de 2022, 11:14
 */

#include "Smart_AutomaticStatisticsCollection.h"

// you have to included need libs

// GEnSyS Simulator
#include "../../../../kernel/simulator/Simulator.h"

// Model Components
#include "../../../../plugins/components/Create.h"
#include "../../../../plugins/components/Process.h"
#include "../../../../plugins/components/Dispose.h"

Smart_AutomaticStatisticsCollection::Smart_AutomaticStatisticsCollection() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Smart_AutomaticStatisticsCollection::main(int argc, char** argv) {
	Simulator* genesys = new Simulator();
	this->setDefaultTraceHandlers(genesys->getTracer());
	this->insertFakePluginsByHand(genesys);
	
        // create model
	Model* model = genesys->getModels()->newModel();
	PluginManager* plugins = genesys->getPlugins();
	
        Create* create = plugins->newInstance<Create>(model);
        create->setName("Equipment Arrives");
        create->setTimeBetweenCreationsExpression("norm(8,9)");
        create->setTimeUnit(Util::TimeUnit::minute);
        create->setEntitiesPerCreation(1);  // Entities per arrival do arena?
        create->setFirstCreation(0);
        //create->setMaxCreations(INFINITY);
        
        //Resource
        Resource* processor = new Resource(model, "processor");
        processor->setCapacity(1);
        processor->setCostBusyHour(5.25);
        processor->isReportStatistics();
        
        Queue* queueProcessor = new Queue(model, "Fila_alocacao_processor");// n sei se eh necessario
        
        Process* process = plugins->newInstance<Process>(model);
        process->setName("Equipment is Processed");
        process->setPriority(1);
	//process->getSeizeRequests()->insert(new SeizableItem(plugins->newInstance<Resource>(model)));
	process->getSeizeRequests()->insert(new SeizableItem(processor));
        process->setAllocationType(1);
	process->setQueueableItem(new QueueableItem(queueProcessor)); //plugins->newInstance<Queue>(model)
        process->setDelayTimeUnit(Util::TimeUnit::hour);
	process->setDelayExpression("tria(1,2,3)");
                
	Dispose* dispose = plugins->newInstance<Dispose>(model);
	
        // connect model components to create a "workflow"
        create->getConnections()->insert(process);
	process->getConnections()->insert(dispose);
	
        // set options, save and simulate
        ModelSimulation* sim = model->getSimulation();
	
        //seguindo enunciado do professor
        sim->setReplicationLength(480);//tem q variar
        sim->setReplicationLengthTimeUnit(Util::TimeUnit::minute);
	sim->setTerminatingCondition("count(Dispose_1.CountNumberIn)>1000");
        sim->setNumberOfReplications(300);
        sim->setWarmUpPeriod(sim->getReplicationLength()*0.05); //5% de 480 = 24
        sim->setWarmUpPeriodTimeUnit(Util::TimeUnit::minute);
        
        sim->start();
        
        model->save("./models/Smart_AutomaticStatisticsCollection.gen");
        
        for(int i = 0; i < 1e9; i++);
	delete genesys;
	return 0;
};