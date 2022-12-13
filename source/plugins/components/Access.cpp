/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Access.cpp
 * Author: rlcancian
 * 
 * Created on 11 de Setembro de 2019, 13:14
 */

#include "Access.h"

#include "../../kernel/simulator/Model.h"

#ifdef PLUGINCONNECT_DYNAMIC

extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &Access::GetPluginInformation;
}
#endif

ModelDataDefinition* Access::NewInstance(Model* model, std::string name) {
	return new Access(model, name);
}

Access::Access(Model* model, std::string name) : ModelComponent(model, Util::TypeOf<Access>(), name) {
}

std::string Access::show() {
	return ModelComponent::show() + "";
}

ModelComponent* Access::LoadInstance(Model* model, PersistenceRecord *fields) {
	Access* newComponent = new Access(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

void Access::_onDispatchEvent(Entity* entity, unsigned int inputPortNumber) {
	_parentModel->getTracer()->trace("I'm just a dummy model and I'll just send the entity forward");
	this->_parentModel->sendEntityToComponent(entity, this->getConnections()->getFrontConnection());
}

bool Access::_loadInstance(PersistenceRecord *fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		// @TODO: not implemented yet
	}
	return res;
}

//void Access::_initBetweenReplications() {}

void Access::_saveInstance(PersistenceRecord *fields, bool saveDefaultValues) {
	ModelComponent::_saveInstance(fields, saveDefaultValues);
	// @TODO: not implemented yet
}

bool Access::_check(std::string* errorMessage) {
	bool resultAll = true;
	// @TODO: not implemented yet
	*errorMessage += "";
	return resultAll;
}

PluginInformation* Access::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Access>(), &Access::LoadInstance, &Access::NewInstance);
	info->setCategory("Material Handling");
	info->setDescriptionHelp("//@TODO");
	// ...
	return info;
}


