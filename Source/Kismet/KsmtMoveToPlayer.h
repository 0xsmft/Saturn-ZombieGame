#pragma once

#include "Saturn/Core/Timestep.h"
#include "Saturn/GameFramework/Core/GameScript.h"

#include "Saturn/AI/BehaviourTree/Tasks/BehaviourTreeBaseTask.h"
#include "Saturn/AI/Navigation/StraightNavPath.h"
#include "Saturn/AI/AIAgentEntity.h"

#include "KsmtMoveToPlayer.Gen.h"

using namespace Saturn;

// Kismet Player
class Player;

namespace Saturn {
	class Scene;
}

/**
 * Enemy AI
 * 
 * Move to player (MoveToEntity really...)
 */
SCLASS( VisibleInEditor )
class KsmtMoveToPlayer : public BehaviourTreeBaseTask
{
	GENERATED_BODY()
public:
	KsmtMoveToPlayer();
	virtual ~KsmtMoveToPlayer();

#if !defined(SAT_DIST)
	virtual void PreInitialiseTask( NodeEditor* pEditor, NodeEditorNodeBase* pNode ) override;
#endif
	virtual void InitialiseTaskWithOther( NodeEditorTaskHandler* pHandler, NodeEditorTaskBase* pOther ) override;

	virtual NodeEditorTaskState Tick( Timestep ts ) override;
	virtual void Reset() override;

#if !defined(SAT_DIST)
	[[nodiscard]] virtual bool IsSpawnableNode() const { return true; }
	virtual const char* GetTaskName() const { return "Move To Athena Player"; }
#endif

private:
	NodeEditorTaskState InitPath();
	NodeEditorTaskState WalkToNextPoint( Timestep ts );

private:
	// #ReplaceRawPtrOrRefWithWeakRef, for now it's a raw ptr
	AIAgentEntity* m_Agent = nullptr;

	Player* m_pTarget = nullptr;
	Scene* m_pActiveScene = nullptr;

	StraightNavPath* m_pPath = nullptr;
	glm::vec3 m_NextPosition{};
	glm::vec3 m_LastPosition{};

	// Time in seconds.
	float m_PathRetargetDelay = 0.0f;
	float m_PathRetargetDelayInterval = 1.0f;
};
