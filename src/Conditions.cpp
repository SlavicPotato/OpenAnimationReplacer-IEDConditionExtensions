#include "Conditions.h"
#include "Conditions.h"

#include "Interface.h"

namespace Conditions
{
	IEDNodePlacementCondition::IEDNodePlacementCondition()
	{
		gearNodeIDComponent = static_cast<INumericConditionComponent*>(AddBaseComponent(
			ConditionComponentType::kNumeric, "Gear node ID"));
		comparisonComponent = static_cast<IComparisonConditionComponent*>(AddBaseComponent(
			ConditionComponentType::kComparison, "Comparison"));
		weaponPlacementIDComponent = static_cast<INumericConditionComponent*>(AddBaseComponent(
			ConditionComponentType::kNumeric, "Weapon placement ID"));
	}

	RE::BSString IEDNodePlacementCondition::GetArgument() const
	{
		const auto gearNodeIdArgument = gearNodeIDComponent->GetArgument();
		const auto comparisonArgument = comparisonComponent->GetArgument();
		const auto weaponPlacementIdArgument = weaponPlacementIDComponent->GetArgument();
		return std::format("GetPlacementHintForGearNode({}) {} {}", gearNodeIdArgument.data(), comparisonArgument.data(), weaponPlacementIdArgument.data()).data();
	}

	RE::BSString IEDNodePlacementCondition::GetCurrent(RE::TESObjectREFR* a_refr) const
	{
		if (a_refr) {
			const auto gearNodeID = static_cast<GearNodeID>(gearNodeIDComponent->GetNumericValue(a_refr));
			const auto placementID = g_interfaceIED->GetPlacementHintForGearNode(a_refr, gearNodeID);
			return std::to_string(stl::to_underlying(placementID)).data();
		}

		return "";
	}

	bool IEDNodePlacementCondition::EvaluateImpl([[maybe_unused]] RE::TESObjectREFR* a_refr,
		[[maybe_unused]] RE::hkbClipGenerator* a_clipGenerator)
		const
	{
		const auto gearNodeID = static_cast<GearNodeID>(gearNodeIDComponent->GetNumericValue(a_refr));
		const auto placementID = g_interfaceIED->GetPlacementHintForGearNode(a_refr, gearNodeID);
		const auto valuePlacementID = static_cast<WeaponPlacementID>(weaponPlacementIDComponent->GetNumericValue(a_refr));

		return comparisonComponent->GetComparisonResult(static_cast<float>(placementID), static_cast<float>(valuePlacementID));
	}


	IEDNodeParentNameCondition::IEDNodeParentNameCondition()
	{
		gearNodeIDComponent = static_cast<INumericConditionComponent*>(AddBaseComponent(
			ConditionComponentType::kNumeric, "Gear node ID"));
		matchTextComponent = static_cast<ITextConditionComponent*>(AddBaseComponent(
			ConditionComponentType::kText, "Node name"));
	}

	RE::BSString IEDNodeParentNameCondition::GetArgument() const
	{
		const auto gearNodeIdArgument = gearNodeIDComponent->GetArgument();
		const auto matchTextArgument = matchTextComponent->GetArgument();
		return std::format("GetGearNodeParentName({}) == {}", gearNodeIdArgument.data(), matchTextArgument.data()).data();
	}

	RE::BSString IEDNodeParentNameCondition::GetCurrent(RE::TESObjectREFR* a_refr) const
	{
		if (a_refr) {
			const auto gearNodeID = static_cast<GearNodeID>(gearNodeIDComponent->GetNumericValue(a_refr));
			return g_interfaceIED->GetGearNodeParentName(a_refr, gearNodeID);
		}

		return "";
	}

	bool IEDNodeParentNameCondition::EvaluateImpl([[maybe_unused]] RE::TESObjectREFR* a_refr,
		[[maybe_unused]] RE::hkbClipGenerator* a_clipGenerator)
		const
	{
		const auto gearNodeID = static_cast<GearNodeID>(gearNodeIDComponent->GetNumericValue(a_refr));
		const auto parentName = g_interfaceIED->GetGearNodeParentName(a_refr, gearNodeID);

		return parentName == matchTextComponent->GetTextValue();
	}


	RE::BSString SDSShieldOnBackEnabledCondition::GetArgument() const
	{
		return "IsShieldOnBackEnabled() == true";
	}

	RE::BSString SDSShieldOnBackEnabledCondition::GetCurrent(RE::TESObjectREFR* a_refr) const
	{
		if (a_refr) {
			if (auto actor = a_refr->As<RE::Actor>()) {
				const auto shieldOnBack = g_interfaceSDS->GetShieldOnBackEnabled(actor);
				return shieldOnBack ? "true" : "false";
			}
		}

		return "false";
	}

	bool SDSShieldOnBackEnabledCondition::EvaluateImpl([[maybe_unused]] RE::TESObjectREFR* a_refr,
		[[maybe_unused]] RE::hkbClipGenerator* a_clipGenerator)
		const
	{
		auto actor = a_refr ? a_refr->As<RE::Actor>() : nullptr;
		return actor ? g_interfaceSDS->GetShieldOnBackEnabled(actor) : false;
	}

}
