#pragma once

#include "API/OpenAnimationReplacerAPI-Conditions.h"

namespace Conditions
{
	class IEDNodePlacementCondition : public CustomCondition
	{
		using GearNodeID        = PluginInterfaceIED::GearNodeID;
		using WeaponPlacementID = PluginInterfaceIED::WeaponPlacementID;

	public:
		constexpr static inline std::string_view CONDITION_NAME = "IED_GearNodePlacementHint"sv;

		IEDNodePlacementCondition();

		RE::BSString GetName() const override { return CONDITION_NAME.data(); }

		RE::BSString GetDescription() const override
		{
			return "Checks if the gear node's current placement hint matches the 'Weapon placement ID' parameter according to the selected 'Comparison' operator. The gear node is specified by the 'Gear node ID' parameter."sv
			    .data();
		}

		constexpr REL::Version GetRequiredVersion() const override { return { 1, 0, 0 }; }

		RE::BSString GetArgument() const override;

		RE::BSString GetCurrent(RE::TESObjectREFR* a_refr) const override;

	protected:
		bool EvaluateImpl(RE::TESObjectREFR* a_refr, RE::hkbClipGenerator* a_clipGenerator) const override;

		IComparisonConditionComponent* comparisonComponent;
		INumericConditionComponent*    gearNodeIDComponent;
		INumericConditionComponent*    weaponPlacementIDComponent;
	};

	class IEDNodeEquippedPlacementCondition : public CustomCondition
	{
		using GearNodeID        = PluginInterfaceIED::GearNodeID;
		using WeaponPlacementID = PluginInterfaceIED::WeaponPlacementID;

	public:
		constexpr static inline std::string_view CONDITION_NAME = "IED_GearNodeEquippedPlacementHint"sv;

		IEDNodeEquippedPlacementCondition();

		RE::BSString GetName() const override { return CONDITION_NAME.data(); }

		RE::BSString GetDescription() const override
		{
			return "Same as IED_GearNodePlacementHint except that it determines the gear node from the equipped weapon type."sv
			    .data();
		}

		constexpr REL::Version GetRequiredVersion() const override { return { 1, 0, 0 }; }

		RE::BSString GetArgument() const override;

		RE::BSString GetCurrent(RE::TESObjectREFR* a_refr) const override;

	protected:
		bool EvaluateImpl(RE::TESObjectREFR* a_refr, RE::hkbClipGenerator* a_clipGenerator) const override;

		IBoolConditionComponent*       isLeftHandComponent;
		IComparisonConditionComponent* comparisonComponent;
		INumericConditionComponent*    weaponPlacementIDComponent;
	};

	class IEDNodeParentNameCondition : public CustomCondition
	{
		using GearNodeID = PluginInterfaceIED::GearNodeID;

	public:
		constexpr static inline std::string_view CONDITION_NAME = "IED_GearNodeParentName"sv;

		IEDNodeParentNameCondition();

		RE::BSString GetName() const override { return CONDITION_NAME.data(); }

		RE::BSString GetDescription() const override
		{
			return "Checks if the gear node's current parent node name equals the text in 'Node name' parameter. The gear node is specified by the 'Gear node ID' parameter. Case insensitive."sv
			    .data();
		}

		constexpr REL::Version GetRequiredVersion() const override { return { 1, 0, 0 }; }

		RE::BSString GetArgument() const override;

		RE::BSString GetCurrent(RE::TESObjectREFR* a_refr) const override;

	protected:
		bool EvaluateImpl(RE::TESObjectREFR* a_refr, RE::hkbClipGenerator* a_clipGenerator) const override;

		INumericConditionComponent* gearNodeIDComponent;
		ITextConditionComponent*    matchTextComponent;
	};

	class IEDHasEquipmentSlot : public CustomCondition
	{
	public:
		constexpr static inline std::string_view CONDITION_NAME = "IED_HasEquipSlot"sv;

		IEDHasEquipmentSlot();

		RE::BSString GetName() const override { return CONDITION_NAME.data(); }

		RE::BSString GetDescription() const override
		{
			return "Checks if an item equipped in the target ref's hand has the specified equip slot configured."sv
			    .data();
		}

		constexpr REL::Version GetRequiredVersion() const override { return { 1, 0, 0 }; }

		RE::BSString GetArgument() const override;

		RE::BSString GetCurrent(RE::TESObjectREFR* a_refr) const override;

	protected:
		bool EvaluateImpl(RE::TESObjectREFR* a_refr, RE::hkbClipGenerator* a_clipGenerator) const override;

		static RE::BGSEquipSlot* GetEquipSlotForEquippedItem(RE::TESObjectREFR* a_refr, bool a_leftHand);

		IBoolConditionComponent* isLeftHandComponent;
		IFormConditionComponent* matchFormComponent;
	};
	
	class IEDIsBoundWeaponEquipped : public CustomCondition
	{
	public:
		constexpr static inline std::string_view CONDITION_NAME = "IED_IsBoundWeaponEquipped"sv;

		IEDIsBoundWeaponEquipped();

		RE::BSString GetName() const override { return CONDITION_NAME.data(); }

		RE::BSString GetDescription() const override
		{
			return "Checks if an item equipped in the target ref's hand is a bound weapon."sv
			    .data();
		}

		constexpr REL::Version GetRequiredVersion() const override { return { 1, 0, 0 }; }

		RE::BSString GetArgument() const override;

		RE::BSString GetCurrent(RE::TESObjectREFR* a_refr) const override;

	protected:
		bool EvaluateImpl(RE::TESObjectREFR* a_refr, RE::hkbClipGenerator* a_clipGenerator) const override;

		static bool IsBoundWeaponEquipped(RE::TESObjectREFR* a_refr, bool a_leftHand);

		IBoolConditionComponent* isLeftHandComponent;
	};

	class IEDPluginOptionCondition : public CustomCondition
	{
		using PluginOptionKey = PluginInterfaceIED::PluginOptionKey;

	public:
		constexpr static inline std::string_view CONDITION_NAME = "IED_PluginOption"sv;

		IEDPluginOptionCondition();

		RE::BSString GetName() const override { return CONDITION_NAME.data(); }

		RE::BSString GetDescription() const override
		{
			return "Checks various IED settings."sv
			    .data();
		}

		constexpr REL::Version GetRequiredVersion() const override { return { 1, 0, 0 }; }

		RE::BSString GetArgument() const override;

		RE::BSString GetCurrent(RE::TESObjectREFR* a_refr) const override;

	protected:
		bool EvaluateImpl(RE::TESObjectREFR* a_refr, RE::hkbClipGenerator* a_clipGenerator) const override;

		INumericConditionComponent*    optionKeyComponent;
		IComparisonConditionComponent* comparisonComponent;
		INumericConditionComponent*    matchValueComponent;
	};

	class SDSShieldOnBackEnabledCondition : public CustomCondition
	{
	public:
		constexpr static inline std::string_view CONDITION_NAME = "SDS_IsShieldOnBackEnabled"sv;

		RE::BSString GetName() const override { return CONDITION_NAME.data(); }

		RE::BSString GetDescription() const override
		{
			return "Checks if the target ref has shield on back enabled in Simple Dual Sheath."sv
			    .data();
		}

		constexpr REL::Version GetRequiredVersion() const override { return { 1, 0, 0 }; }

		RE::BSString GetArgument() const override;

		RE::BSString GetCurrent(RE::TESObjectREFR* a_refr) const override;

	protected:
		bool EvaluateImpl(RE::TESObjectREFR* a_refr, RE::hkbClipGenerator* a_clipGenerator) const override;
	};
}
