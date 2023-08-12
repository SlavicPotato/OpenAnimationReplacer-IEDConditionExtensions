#pragma once

#include "PluginInterfaceBase.h"

#include "EventSink.h"

struct SDSPlayerShieldOnBackSwitchEvent
{
	bool isOnBack;
};

class PluginInterfaceSDS :
	public PluginInterfaceBase
{
public:
	static constexpr std::uint64_t UNIQUE_ID = 0x3180B30EFCC0DB62;
	static constexpr const char* PLUGIN_DLL = "SimpleDualSheath.dll";

	virtual std::uint32_t GetPluginVersion() const override;
	virtual const char* GetPluginName() const override;
	virtual std::uint32_t GetInterfaceVersion() const override;
	virtual const char* GetInterfaceName() const override;
	virtual std::uint64_t GetUniqueID() const override;

	//

	virtual bool GetShieldOnBackEnabled(RE::Actor* a_actor) const;
	virtual void RegisterForPlayerShieldOnBackEvent(::Events::EventSink<SDSPlayerShieldOnBackSwitchEvent>* a_sink);
	virtual bool IsWeaponNodeSharingDisabled() const;
};