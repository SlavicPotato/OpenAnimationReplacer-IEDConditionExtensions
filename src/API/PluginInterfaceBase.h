#pragma once

#include <cstdint>
#include <type_traits>

enum class PluginInterfaceQueryErrorState : std::uint32_t
{
	kNone                = 0,
	kInvalidArgument     = 1,
	kDllNotLoaded        = 2,
	kEntryPointNotFound  = 3,
	kUniqueIDCheckFailed = 4,
	kSuccess             = 5
};

class PluginInterfaceBase
{
private:
	template <class T>
	struct query_result
	{
		T*                             intfc{ nullptr };
		PluginInterfaceQueryErrorState error{ PluginInterfaceQueryErrorState::kNone };

		constexpr explicit operator bool() const noexcept
		{
			return static_cast<bool>(intfc);
		}
	};

public:
	static constexpr const char* PLUGIN_DLL = nullptr;

	template <class T>
	static constexpr query_result<T> query_interface(const char* a_dll = T::PLUGIN_DLL)  //
		requires std::is_base_of_v<PluginInterfaceBase, T>
	{
		query_result<T> result;

		if (a_dll == nullptr)
		{
			result.error = PluginInterfaceQueryErrorState::kInvalidArgument;

			return result;
		}

		auto handle = GetModuleHandle(a_dll);
		if (!handle)
		{
			result.error = PluginInterfaceQueryErrorState::kDllNotLoaded;

			return result;
		}

		using func_t = T* (*)();

		func_t func = reinterpret_cast<func_t>(GetProcAddress(handle, "SKMP_GetPluginInterface"));

		auto intfc = func();
		if (!intfc)
		{
			result.error = PluginInterfaceQueryErrorState::kEntryPointNotFound;

			return result;
		}

		if (intfc->GetUniqueID() != T::UNIQUE_ID)
		{
			result.error = PluginInterfaceQueryErrorState::kUniqueIDCheckFailed;

			return result;
		}

		result.intfc = intfc;
		result.error = PluginInterfaceQueryErrorState::kSuccess;

		return result;
	}

	static constexpr const char* get_error_string(
		PluginInterfaceQueryErrorState a_error) noexcept
	{
		switch (a_error)
		{
		case PluginInterfaceQueryErrorState::kNone:
			return "No error";
		case PluginInterfaceQueryErrorState::kInvalidArgument:
			return "Invalid argument";
		case PluginInterfaceQueryErrorState::kDllNotLoaded:
			return "DLL not loaded";
		case PluginInterfaceQueryErrorState::kEntryPointNotFound:
			return "Entry point not found";
		case PluginInterfaceQueryErrorState::kUniqueIDCheckFailed:
			return "Unique ID check failed";
		case PluginInterfaceQueryErrorState::kSuccess:
			return "Success";
		default:
			return "Unknown error";
		}
	}

	virtual std::uint32_t GetPluginVersion() const    = 0;
	virtual const char*   GetPluginName() const       = 0;
	virtual std::uint32_t GetInterfaceVersion() const = 0;
	virtual const char*   GetInterfaceName() const    = 0;
	virtual std::uint64_t GetUniqueID() const         = 0;
};