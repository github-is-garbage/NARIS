#include "Globals.h"

bool Globals::Setup()
{
	this->GUIManager = new GUI();

	if (!this->GUIManager->Setup())
		return false;

	this->D3DManager = new DirectXHolder();

	if (!this->D3DManager->Setup(this->GUIManager->hWindow))
	{
		this->GUIManager->Destroy();
		return false;
	}

	this->FeatureManager = new Features();
	this->FeatureManager->Setup();

	return true;
}

void Globals::Destroy()
{
	this->FeatureManager->Destroy();
	this->GUIManager->Destroy();
	this->D3DManager->Destroy();
}
