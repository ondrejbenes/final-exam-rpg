#include "AiComponent.h"
#include "AiIdle.h"
#include "EntityManager.h"
#include "VectorUtilities.h"
#include "AiAttack.h"
#include "Logger.h"
#include "Tilemap.h"


AiIdle::AiIdle(AiComponent* component, float attackRadius) :
AiState(component),
_attackRadius(attackRadius)
{

}

void AiIdle::update()
{
	if(_timer.getElapsedTime().asSeconds() < UPDATE_FREQUENCY)
		return;

	auto playerPos = EntityManager::getInstance()->getLocalPlayer()->getPosition();
	auto aiPos = _aiComponent->getParent().getPosition();

	if(VectorUtilities::calculateDistance(playerPos, aiPos) < _attackRadius)
	{
		LOG_D("Going from Idle to Attack ");
		_aiComponent->ChangeState(new AiAttack(_aiComponent, aiPos, Tilemap::MAP_WIDTH));
	}
}

float AiIdle::UPDATE_FREQUENCY = 1;