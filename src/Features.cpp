#include "Features.h"

#include "Features/Clients.h"

Features::Features()
{
	this->mapFeatures = std::unordered_map<std::type_index, BaseFeature*>();
}

void Features::Setup()
{
	this->AddFeature<Clients>(new Clients());

	for (const auto& Entry : this->mapFeatures)
		Entry.second->Setup();
}

void Features::Destroy()
{
	for (const auto& Entry : this->mapFeatures)
		Entry.second->Destroy();
}
