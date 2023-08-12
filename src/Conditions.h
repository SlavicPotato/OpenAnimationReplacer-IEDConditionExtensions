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
			return "Returns true if the gear node's current placement hint matches the 'Weapon placement ID' parameter according to the selected 'Comparison' operator. The gear node is specified by the 'Gear node ID' parameter."sv
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

	class IEDNodeParentNameCondition : public CustomCondition
	{
		using GearNodeID = PluginInterfaceIED::GearNodeID;

	public:
		constexpr static inline std::string_view CONDITION_NAME = "IED_GearNodeParentName"sv;

		IEDNodeParentNameCondition();

		RE::BSString GetName() const override { return CONDITION_NAME.data(); }

		RE::BSString GetDescription() const override
		{
			return "Returns true if the gear node's current parent node name equals the text in 'Node name' parameter. The gear node is specified by the 'Gear node ID' parameter. Case insensitive."sv
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

	class SDSShieldOnBackEnabledCondition : public CustomCondition
	{
	public:
		constexpr static inline std::string_view CONDITION_NAME = "SDS_IsShieldOnBackEnabled"sv;

		RE::BSString GetName() const override { return CONDITION_NAME.data(); }

		RE::BSString GetDescription() const override
		{
			return "Returns true if shield on back is enabled for the actor in Simple Dual Sheath."sv
			    .data();
		}

		constexpr REL::Version GetRequiredVersion() const override { return { 1, 0, 0 }; }

		RE::BSString GetArgument() const override;

		RE::BSString GetCurrent(RE::TESObjectREFR* a_refr) const override;

	protected:
		bool EvaluateImpl(RE::TESObjectREFR* a_refr, RE::hkbClipGenerator* a_clipGenerator) const override;
	};
}
