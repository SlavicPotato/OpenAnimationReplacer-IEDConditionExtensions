#pragma once

#include "PluginInterfaceBase.h"

class PluginInterfaceIED :
	public PluginInterfaceBase
{
public:
	static constexpr std::uint64_t UNIQUE_ID = 0xBD869D3E87EF7D51;
	static constexpr const char* PLUGIN_DLL = "ImmersiveEquipmentDisplays.dll";

	enum class WeaponPlacementID : std::uint8_t
	{
		None = 0,
		Default = 1,
		OnBack = 2,
		OnBackHip = 3,
		Ankle = 4,
		AtHip = 5,
		Frostfall = 6,
		BowShoulder = 7,
		AxeReverse = 8,
	};

	enum class GearNodeID : std::uint32_t
	{
		None = 0,

		k1HSword = 1,
		k1HSwordLeft = 2,
		k1HAxe = 3,
		k1HAxeLeft = 4,
		kTwoHanded = 5,
		kTwoHandedAxeMace = 6,
		kDagger = 7,
		kDaggerLeft = 8,
		kMace = 9,
		kMaceLeft = 10,
		kStaff = 11,
		kStaffLeft = 12,
		kBow = 13,
		kCrossBow = 14,
		kShield = 15,
		kQuiver = 16,
	};

	virtual std::uint32_t GetPluginVersion() const override;
	virtual const char* GetPluginName() const override;
	virtual std::uint32_t GetInterfaceVersion() const override;
	virtual const char* GetInterfaceName() const override;
	virtual std::uint64_t GetUniqueID() const override;

	//

	virtual WeaponPlacementID GetPlacementHintForGearNode(RE::TESObjectREFR* a_actor, GearNodeID a_id) const;
	virtual RE::BSString GetGearNodeParentName(RE::TESObjectREFR* a_refr, GearNodeID a_id) const;
};