#include "Globals.h"

bool Globals::Setup()
{
	this->Util = new Utility();

	this->GUIManager = new GUI();

	if (!this->GUIManager->Setup())
		return this->Delete();

	this->D3DManager = new DirectXHolder();

	if (!this->D3DManager->Setup(this->GUIManager->hWindow))
	{
		this->GUIManager->Destroy();
		return this->Delete();
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

	this->Delete();
}

bool Globals::Delete()
{
	DeletePtr(this->FeatureManager);
	DeletePtr(this->GUIManager);
	DeletePtr(this->D3DManager);
	DeletePtr(this->Util);

	return false;
}
