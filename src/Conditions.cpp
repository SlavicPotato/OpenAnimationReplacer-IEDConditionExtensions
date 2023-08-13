#include "Conditions.h"

#include "Interface.h"

namespace Conditions
{
	IEDNodePlacementCondition::IEDNodePlacementCondition()
	{
		gearNodeIDComponent        = static_cast<INumericConditionComponent*>(AddBaseComponent(
            ConditionComponentType::kNumeric,
            "Gear node ID"));
		comparisonComponent        = static_cast<IComparisonConditionComponent*>(AddBaseComponent(
            ConditionComponentType::kComparison,
            "Comparison"));
		weaponPlacementIDComponent = static_cast<INumericConditionComponent*>(AddBaseComponent(
			ConditionComponentType::kNumeric,
			"Weapon placement ID"));
	}

	RE::BSString IEDNodePlacementCondition::GetArgument() const
	{
		const auto gearNodeIdArgument        = gearNodeIDComponent->GetArgument();
		const auto comparisonArgument        = comparisonComponent->GetArgument();
		const auto weaponPlacementIdArgument = weaponPlacementIDComponent->GetArgument();

		return std::format(
				   "GetPlacementHintForGearNode({}) {} {}",
				   gearNodeIdArgument.data(),
				   comparisonArgument.data(),
				   weaponPlacementIdArgument.data())
		    .data();
	}

	RE::BSString IEDNodePlacementCondition::GetCurrent(RE::TESObjectREFR* a_refr) const
	{
		if (a_refr)
		{
			const auto gearNodeID  = static_cast<GearNodeID>(gearNodeIDComponent->GetNumericValue(a_refr));
			const auto placementID = g_interfaceIED->GetPlacementHintForGearNode(a_refr, gearNodeID);
			return std::to_string(stl::to_underlying(placementID)).data();
		}

		return ""sv;
	}

	bool IEDNodePlacementCondition::EvaluateImpl(
		RE::TESObjectREFR*                     a_refr,
		[[maybe_unused]] RE::hkbClipGenerator* a_clipGenerator)
		const
	{
		const auto gearNodeID       = static_cast<GearNodeID>(gearNodeIDComponent->GetNumericValue(a_refr));
		const auto placementID      = g_interfaceIED->GetPlacementHintForGearNode(a_refr, gearNodeID);
		const auto valuePlacementID = static_cast<WeaponPlacementID>(weaponPlacementIDComponent->GetNumericValue(a_refr));

		return comparisonComponent->GetComparisonResult(
			static_cast<float>(placementID),
			static_cast<float>(valuePlacementID));
	}

	IEDNodeEquippedPlacementCondition::IEDNodeEquippedPlacementCondition()
	{
		isLeftHandComponent        = static_cast<IBoolConditionComponent*>(AddBaseComponent(
            ConditionComponentType::kBool,
            "Left hand"));
		comparisonComponent        = static_cast<IComparisonConditionComponent*>(AddBaseComponent(
            ConditionComponentType::kComparison,
            "Comparison"));
		weaponPlacementIDComponent = static_cast<INumericConditionComponent*>(AddBaseComponent(
			ConditionComponentType::kNumeric,
			"Weapon placement ID"));
	}

	RE::BSString IEDNodeEquippedPlacementCondition::GetArgument() const
	{
		const auto isLeftHandArgument        = isLeftHandComponent->GetArgument();
		const auto comparisonArgument        = comparisonComponent->GetArgument();
		const auto weaponPlacementIdArgument = weaponPlacementIDComponent->GetArgument();

		return std::format(
				   "GetPlacementHintForEquippedWeapon({}) {} {}",
				   isLeftHandArgument.data(),
				   comparisonArgument.data(),
				   weaponPlacementIdArgument.data())
		    .data();
	}

	RE::BSString IEDNodeEquippedPlacementCondition::GetCurrent(RE::TESObjectREFR* a_refr) const
	{
		if (a_refr)
		{
			const auto isLeftHand  = isLeftHandComponent->GetBoolValue();
			const auto placementID = g_interfaceIED->GetPlacementHintForEquippedWeapon(a_refr, isLeftHand);
			return std::to_string(stl::to_underlying(placementID)).data();
		}

		return ""sv;
	}

	bool IEDNodeEquippedPlacementCondition::EvaluateImpl(
		RE::TESObjectREFR*                     a_refr,
		[[maybe_unused]] RE::hkbClipGenerator* a_clipGenerator)
		const
	{
		const auto isLeftHand       = isLeftHandComponent->GetBoolValue();
		const auto placementID      = g_interfaceIED->GetPlacementHintForEquippedWeapon(a_refr, isLeftHand);
		const auto valuePlacementID = static_cast<WeaponPlacementID>(weaponPlacementIDComponent->GetNumericValue(a_refr));

		return comparisonComponent->GetComparisonResult(
			static_cast<float>(placementID),
			static_cast<float>(valuePlacementID));
	}

	IEDNodeParentNameCondition::IEDNodeParentNameCondition()
	{
		gearNodeIDComponent = static_cast<INumericConditionComponent*>(AddBaseComponent(
			ConditionComponentType::kNumeric,
			"Gear node ID"));
		matchTextComponent  = static_cast<ITextConditionComponent*>(AddBaseComponent(
            ConditionComponentType::kText,
            "Node name"));
	}

	RE::BSString IEDNodeParentNameCondition::GetArgument() const
	{
		const auto gearNodeIdArgument = gearNodeIDComponent->GetArgument();
		const auto matchTextArgument  = matchTextComponent->GetArgument();

		return std::format(
				   "GetGearNodeParentName({}) == \"{}\"",
				   gearNodeIdArgument.data(),
				   matchTextArgument.data())
		    .data();
	}

	RE::BSString IEDNodeParentNameCondition::GetCurrent(RE::TESObjectREFR* a_refr) const
	{
		if (a_refr)
		{
			const auto gearNodeID = static_cast<GearNodeID>(gearNodeIDComponent->GetNumericValue(a_refr));
			return g_interfaceIED->GetGearNodeParentName(a_refr, gearNodeID);
		}

		return "";
	}

	bool IEDNodeParentNameCondition::EvaluateImpl(RE::TESObjectREFR* a_refr, [[maybe_unused]] RE::hkbClipGenerator* a_clipGenerator)
		const
	{
		const auto gearNodeID = static_cast<GearNodeID>(gearNodeIDComponent->GetNumericValue(a_refr));
		const auto parentName = g_interfaceIED->GetGearNodeParentName(a_refr, gearNodeID);

		return parentName == matchTextComponent->GetTextValue();
	}

	IEDHasEquipmentSlot::IEDHasEquipmentSlot()
	{
		isLeftHandComponent = static_cast<IBoolConditionComponent*>(AddBaseComponent(
			ConditionComponentType::kBool,
			"Left hand"));
		matchFormComponent  = static_cast<IFormConditionComponent*>(AddBaseComponent(
            ConditionComponentType::kForm,
            "Equipment slot"));
	}

	RE::BSString IEDHasEquipmentSlot::GetArgument() const
	{
		const auto isLeftHandArgument = isLeftHandComponent->GetArgument();
		const auto formArgument       = matchFormComponent->GetArgument();

		return std::format(
				   "GetSlotForEquippedItem({}) == {}",
				   isLeftHandArgument.data(),
				   formArgument.data())
		    .data();
	}

	RE::BSString IEDHasEquipmentSlot::GetCurrent(RE::TESObjectREFR* a_refr) const
	{
		if (a_refr)
		{
			const auto isLeftHand = isLeftHandComponent->GetBoolValue();
			const auto equipSlot  = GetEquipSlotForEquippedItem(a_refr, isLeftHand);
			return equipSlot ? std::format("0x{:X}", equipSlot->formID).data() : "";
		}

		return ""sv;
	}

	bool IEDHasEquipmentSlot::EvaluateImpl(
		RE::TESObjectREFR*                     a_refr,
		[[maybe_unused]] RE::hkbClipGenerator* a_clipGenerator)
		const
	{
		const auto matchForm = matchFormComponent->GetTESFormValue();
		if (!matchForm)
		{
			return false;
		}

		const auto isLeftHand = isLeftHandComponent->GetBoolValue();
		const auto equipSlot  = GetEquipSlotForEquippedItem(a_refr, isLeftHand);

		return equipSlot && equipSlot == matchForm;
	}

	RE::BGSEquipSlot* IEDHasEquipmentSlot::GetEquipSlotForEquippedItem(
		RE::TESObjectREFR* a_refr,
		bool               a_leftHand)
	{
		if (!a_refr)
		{
			return nullptr;
		}

		const auto actor = a_refr->As<RE::Actor>();
		if (!actor)
		{
			return nullptr;
		}

		const auto equippedObject = actor->GetEquippedObject(a_leftHand);
		const auto equipType      = equippedObject ? equippedObject->As<RE::BGSEquipType>() : nullptr;

		return equipType ? equipType->equipSlot : nullptr;
	}

	RE::BSString SDSShieldOnBackEnabledCondition::GetArgument() const
	{
		return "IsShieldOnBackEnabled() == true"sv;
	}

	RE::BSString SDSShieldOnBackEnabledCondition::GetCurrent(RE::TESObjectREFR* a_refr) const
	{
		if (a_refr)
		{
			if (auto actor = a_refr->As<RE::Actor>())
			{
				if (g_interfaceSDS->GetShieldOnBackEnabled(actor))
				{
					return "true"sv;
				}
			}
		}

		return "false"sv;
	}

	bool SDSShieldOnBackEnabledCondition::EvaluateImpl(
		RE::TESObjectREFR*                     a_refr,
		[[maybe_unused]] RE::hkbClipGenerator* a_clipGenerator)
		const
	{
		auto actor = a_refr ? a_refr->As<RE::Actor>() : nullptr;
		return actor ? g_interfaceSDS->GetShieldOnBackEnabled(actor) : false;
	}

	IEDPluginOptionCondition::IEDPluginOptionCondition()
	{
		optionKeyComponent = static_cast<INumericConditionComponent*>(AddBaseComponent(
			ConditionComponentType::kNumeric,
			"Key"));

		comparisonComponent = static_cast<IComparisonConditionComponent*>(AddBaseComponent(
			ConditionComponentType::kComparison,
			"Comparison"));

		matchValueComponent = static_cast<INumericConditionComponent*>(AddBaseComponent(
			ConditionComponentType::kNumeric,
			"Match value"));
	}

	RE::BSString IEDPluginOptionCondition::GetArgument() const
	{
		const auto keyArgument        = optionKeyComponent->GetArgument();
		const auto comparisonArgument = comparisonComponent->GetArgument();
		const auto valueArgument      = matchValueComponent->GetArgument();

		return std::format(
				   "GetPluginOption({}) {} {}",
				   keyArgument.data(),
				   comparisonArgument.data(),
				   valueArgument.data())
		    .data();
	}

	RE::BSString IEDPluginOptionCondition::GetCurrent(RE::TESObjectREFR* a_refr) const
	{
		const auto key   = static_cast<PluginOptionKey>(optionKeyComponent->GetNumericValue(a_refr));
		const auto value = g_interfaceIED->GetPluginOption(key);
		return std::format("{}", static_cast<std::int32_t>(value)).data();
	}

	bool IEDPluginOptionCondition::EvaluateImpl(
		RE::TESObjectREFR*                     a_refr,
		[[maybe_unused]] RE::hkbClipGenerator* a_clipGenerator) const
	{
		const auto key        = static_cast<PluginOptionKey>(optionKeyComponent->GetNumericValue(a_refr));
		const auto value      = g_interfaceIED->GetPluginOption(key);
		const auto matchValue = static_cast<PluginOptionKey>(matchValueComponent->GetNumericValue(a_refr));

		return comparisonComponent->GetComparisonResult(
			static_cast<float>(value),
			static_cast<float>(matchValue));
	}

}
