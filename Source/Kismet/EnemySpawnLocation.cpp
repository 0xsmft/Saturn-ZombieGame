#include "sppch.h"
#include "EnemySpawnLocation.h"

#include "Saturn/Core/Random.h"
#include "Saturn/Asset/AssetManager.h"
#include "Saturn/Asset/Prefab.h"

EnemySpawnLocation::EnemySpawnLocation()
{
}

EnemySpawnLocation::~EnemySpawnLocation()
{
}

void EnemySpawnLocation::BeginPlay()
{
	Super::BeginPlay();

	const auto centre = GetLocalPosition();

	constexpr float Spacing = 1.5f;

	std::vector<glm::vec3> offsets = 
	{
		{ 0.0f, 0.0f, 0.0f }, // centre (0,0,0)
		{ 0.0f, 0.0f, 1.0f }, // north Z+
		{ 1.0f, 0.0f, 0.0f }, // east X+
		{ 0.0f, 0.0f, -1.0f }, // south Z-
		{ -1.0f, 0.0f, 0.0f }, // west X-
	};

	for( const auto& rLocation : offsets )
	{
		m_SpawingLocations.push_back( centre + rLocation * Spacing );
	}
}

void EnemySpawnLocation::OnUpdate( Saturn::Timestep ts )
{
}

void EnemySpawnLocation::Spawn()
{
	Ref<Asset> asset = AssetManager::Get().FindAsset( 5113922268661067256llu );
	Ref<Prefab> prefabAsset = AssetManager::Get().GetAssetAs<Prefab>( asset->ID );

	for( uint32_t i = 0; i < m_NumberToSpawn; ++i )
	{
		CreateEntityParameters entityParameters;
		entityParameters.Position = m_SpawingLocations[ Random::RandomElementInRange( 0, m_SpawingLocations.size() - 1 ) ];

		GetScene()->CreatePrefab( prefabAsset, entityParameters );
	}
}
