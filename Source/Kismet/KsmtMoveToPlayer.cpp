#include "KsmtMoveToPlayer.h"

#include "Player.h"

#include "Saturn/Physics/PhysicsCharacterController.h"

#include "Saturn/AI/BehaviourTree/AssetViewer/BehaviourTreeNodeEditor.h"
#include "Saturn/AI/BehaviourTree/AssetViewer/Nodes/BehaviourTreeNodeBase.h"

KsmtMoveToPlayer::KsmtMoveToPlayer()
{
}

KsmtMoveToPlayer::~KsmtMoveToPlayer()
{
	if( m_pPath )
	{
		m_pActiveScene->GetNavigationSystem().DestroyStraightPath( m_pPath );
		m_pPath = nullptr;
	}

	m_pActiveScene = nullptr;
}

void KsmtMoveToPlayer::InitialiseTask( NodeEditorTaskHandler* pHandler, NodeEditorBase* pEditor, NodeEditorNodeBase* pNode )
{
	BehaviourTreeNodeEditor* BT = dynamic_cast< BehaviourTreeNodeEditor* >( pEditor );

	m_Agent = BT->GetTargetAgent();
	m_NodeID = pNode->ID;

	auto players = m_Agent->GetScene()->GetAllEntitiesWithClass<Player>();
	for( auto& rPlayer : players )
	{
		m_pTarget = rPlayer.Get();
	}

	m_PathRetargetDelay = m_PathRetargetDelayInterval;

	// HACK!
	m_pActiveScene = g_ActiveScene;
}

Saturn::NodeEditorTaskState KsmtMoveToPlayer::Tick( Timestep ts )
{
	auto& movementComp = m_Agent->GetComponent<CharacterMovementComponent>();
	if( movementComp.CharacterMovement->IsGrounded() )
	{
		movementComp.CharacterMovement->Move( m_NextPosition );
	}
	else
	{
		movementComp.CharacterMovement->Move( m_LastPosition * ts.Seconds() );
	}

	if( m_pPath == nullptr )
	{
		m_CurrentState = InitPath();
		return m_CurrentState;
	}

	m_CurrentState = WalkToNextPoint( ts );

	m_PathRetargetDelay -= ts.Seconds();
	if( m_PathRetargetDelay <= 0.0f )
	{
		if( m_pTarget )
		{
			m_pPath->RetargetPath( m_Agent->GetLocalPosition(), m_pTarget->GetLocalPosition() );
		}

		m_PathRetargetDelay = m_PathRetargetDelayInterval;
	}

	return m_CurrentState;
}

void KsmtMoveToPlayer::Reset()
{
	m_NextPosition = m_NextPosition;
	m_LastPosition = m_NextPosition;
}

Saturn::NodeEditorTaskState KsmtMoveToPlayer::InitPath()
{
	if( !m_pTarget )
		return NodeEditorTaskState::Failed;

	m_pPath = m_pActiveScene->GetNavigationSystem().CreateStraightPath( m_Agent->GetLocalPosition(), m_pTarget->GetLocalPosition() );
	m_pPath->CreatePath();

	return NodeEditorTaskState::Starting;
}

Saturn::NodeEditorTaskState KsmtMoveToPlayer::WalkToNextPoint( Timestep ts )
{
	if( !m_pPath->IsLive() )
		return NodeEditorTaskState::Failed;

	auto& tc = m_Agent->GetComponent<TransformComponent>();
	auto& movementComp = m_Agent->GetComponent<CharacterMovementComponent>();

	glm::vec3 rCurrentPosition = tc.Position;
	const auto& rCurrentWaypoint = m_pPath->GetCurrentWaypoint();

	glm::vec3 diff = rCurrentWaypoint - rCurrentPosition;

	// If the physics engine says we are grounded then we are grounded,
	// so we cannot move below what the position is until we are no longer
	// grounded.
	if( movementComp.CharacterMovement->IsGrounded() )
	{
		diff.y = 0.0f;
	}

	const float distance = glm::length( diff );

	if( distance <= 0.2f )
	{
		// Move on to the next waypoint.
		m_pPath->NextWaypoint();

		// If its zero after the NextWaypoint call we know that we have reached the end.
		if( m_pPath->GetCurrentWaypointIndex() == 0 )
		{
			return NodeEditorTaskState::Completed;
		}

		return NodeEditorTaskState::Running;
	}

	// Walk.
	const glm::vec3 dir = diff / distance;
	const float step = std::min( 5.0f * ts.Seconds(), distance );

	m_NextPosition = dir * step;
	m_LastPosition = m_NextPosition / ts.Seconds();

	return NodeEditorTaskState::Running;
}

