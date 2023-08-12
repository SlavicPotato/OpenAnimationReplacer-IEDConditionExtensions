#pragma once

namespace Conditions
{
    class ICondition;
    class IConditionComponent;

    using ConditionFactory = ICondition* (*)();
    using ConditionComponentFactory = IConditionComponent* (*)(const ICondition* a_parentCondition, const char* a_name, const char* a_description);

    enum class ConditionComponentType : uint8_t
    {
        kMulti,
        kForm,
        kNumeric,
        kNiPoint3,
        kKeyword,
        kText,
        kBool,
        kComparison,
        kRandom,

        kCustom
    };

    enum class ComparisonOperator : uint8_t
    {
        kEqual,
        kNotEqual,
        kGreater,
        kGreaterEqual,
        kLess,
        kLessEqual,

        kInvalid
    };

    enum class ActorValueType : int
    {
        kActorValue,
        kBase,
        kMax,
        kPercentage
    };

    enum class GraphVariableType : int
    {
        kFloat,
        kInt,
        kBool
    };

    // the parent class of all conditions
    // some arguments are kept as void* pointers to avoid including rapidjson headers. You most likely don't need to include it in your project if you're not creating a custom condition component
    // some functions return a RE::BSString because std::string is unreliable over DLL boundaries
    class ICondition
    {
    public:
        virtual ~ICondition() = default;

        [[nodiscard]] virtual bool Evaluate(RE::TESObjectREFR* a_refr, RE::hkbClipGenerator* a_clipGenerator) const = 0;

        virtual void Initialize(void* a_value) = 0; // rapidjson::Value* a_value
        virtual void InitializeLegacy([[maybe_unused]] const char* a_argument) {}
        virtual void Serialize(void* a_value, void* a_allocator, ICondition* a_outerCustomCondition = nullptr) = 0; // rapidjson::Value* a_value, rapidjson::Document::AllocatorType* a_allocator

        virtual void PreInitialize() {}
        virtual void PostInitialize() {}

        [[nodiscard]] virtual RE::BSString GetArgument() const = 0;
        [[nodiscard]] virtual RE::BSString GetCurrent(RE::TESObjectREFR* a_refr) const = 0;

        [[nodiscard]] virtual RE::BSString GetName() const = 0;
        [[nodiscard]] virtual RE::BSString GetDescription() const = 0;
        [[nodiscard]] virtual REL::Version GetRequiredVersion() const = 0;
        [[nodiscard]] virtual RE::BSString GetRequiredPluginName() const = 0;
        [[nodiscard]] virtual RE::BSString GetRequiredPluginAuthor() const = 0;

        [[nodiscard]] virtual bool IsDisabled() const = 0;
        virtual void SetDisabled(bool a_bDisabled) = 0;
        [[nodiscard]] virtual bool IsNegated() const = 0;
        virtual void SetNegated(bool a_bNegated) = 0;
        [[nodiscard]] virtual bool IsValid() const { return true; }

        [[nodiscard]] virtual uint32_t GetNumComponents() const = 0;
        [[nodiscard]] virtual IConditionComponent* GetComponent(uint32_t a_index) const = 0;
        virtual IConditionComponent* AddComponent(ConditionComponentFactory a_factory, const char* a_name, const char* a_description = "") = 0;

        [[nodiscard]] virtual bool IsCustomCondition() const = 0;
        [[nodiscard]] virtual ICondition* GetWrappedCondition() const = 0;

		[[nodiscard]] virtual bool IsDeprecated() const { return false; }
        [[nodiscard]] virtual RE::TESObjectREFR* GetRefrToEvaluate(RE::TESObjectREFR* a_refr) const { return a_refr; }

		[[nodiscard]] class ConditionSet* GetParentConditionSet() const { return _parentConditionSet; }
		void SetParentConditionSet(ConditionSet* a_conditionSet) { _parentConditionSet = a_conditionSet; }

    protected:
        virtual bool EvaluateImpl(RE::TESObjectREFR* a_refr, RE::hkbClipGenerator* a_clipGenerator) const = 0;

		ConditionSet* _parentConditionSet = nullptr;
    };

    // a condition can have many condition components
    // these are the configurable variables inside a condition (e.g. a form, a number, a keyword, etc.)
    // the existing components should be enough for most conditions
    // however you can add your own custom condition components by inheriting from ICustomConditionComponent
    // this will require you to do the serialization yourself however, so it's a bit complex and possibly not safe across DLL boundaries with different compilers, I'm not sure how safe rapidjson is in such cases
    // check the math plugin (https://github.com/ersh1/OpenAnimationReplacer-Math) for an example
    class IConditionComponent
    {
    public:
        IConditionComponent(const ICondition* a_parentCondition, const char* a_name, const char* a_description = "") :
			_parentCondition(a_parentCondition),
            _name(a_name),
            _description(a_description) {}

        virtual ~IConditionComponent() = default;

        virtual void InitializeComponent(void* a_value) = 0;                   // rapidjson::Value* a_value
        virtual void SerializeComponent(void* a_value, void* a_allocator) = 0; // rapidjson::Value* a_value, rapidjson::Document::AllocatorType* a_allocator

        virtual void PostInitialize() {}

        virtual bool DisplayInUI(bool a_bEditable, float a_firstColumnWidthPercent) = 0;

        [[nodiscard]] virtual ConditionComponentType GetType() const = 0;
        [[nodiscard]] virtual RE::BSString GetArgument() const = 0;
        [[nodiscard]] virtual RE::BSString GetName() const { return _name.data(); }

        [[nodiscard]] virtual RE::BSString GetDescription() const
        {
            if (_description.empty()) {
                return GetDefaultDescription();
            }
            return _description.data();
        }

        [[nodiscard]] virtual RE::BSString GetDefaultDescription() const = 0;
        [[nodiscard]] virtual bool IsValid() const = 0;

		[[nodiscard]] const ICondition* GetParentCondition() const { return _parentCondition; }

    protected:
		const ICondition* _parentCondition = nullptr;

        const std::string _name;
        const std::string _description;
    };

    class IMultiConditionComponent : public IConditionComponent
    {
    public:
        inline static constexpr auto CONDITION_COMPONENT_TYPE = ConditionComponentType::kMulti;
        inline static constexpr auto DEFAULT_DESCRIPTION = "A list of child conditions."sv;

        IMultiConditionComponent(const ICondition* a_parentCondition, const char* a_name, const char* a_description = "") :
			IConditionComponent(a_parentCondition, a_name, a_description) {}

        [[nodiscard]] ConditionComponentType GetType() const override { return CONDITION_COMPONENT_TYPE; }
        [[nodiscard]] RE::BSString GetDefaultDescription() const override { return DEFAULT_DESCRIPTION; }

        [[nodiscard]] virtual ConditionSet* GetConditions() const = 0;
    };

    class IFormConditionComponent : public IConditionComponent
    {
    public:
        inline static constexpr auto CONDITION_COMPONENT_TYPE = ConditionComponentType::kForm;
        inline static constexpr auto DEFAULT_DESCRIPTION = "A reference to a form."sv;

        IFormConditionComponent(const ICondition* a_parentCondition, const char* a_name, const char* a_description = "") :
			IConditionComponent(a_parentCondition, a_name, a_description) {}

        [[nodiscard]] ConditionComponentType GetType() const override { return CONDITION_COMPONENT_TYPE; }
        [[nodiscard]] RE::BSString GetDefaultDescription() const override { return DEFAULT_DESCRIPTION; }

        [[nodiscard]] virtual RE::TESForm* GetTESFormValue() const = 0;
        virtual void SetTESFormValue(RE::TESForm* a_form) = 0;
    };

    class INumericConditionComponent : public IConditionComponent
    {
    public:
        inline static constexpr auto CONDITION_COMPONENT_TYPE = ConditionComponentType::kNumeric;
        inline static constexpr auto DEFAULT_DESCRIPTION = "A numeric value - a static value, a reference to a global variable, an Actor Value, or a behavior graph variable."sv;

        INumericConditionComponent(const ICondition* a_parentCondition, const char* a_name, const char* a_description = "") :
			IConditionComponent(a_parentCondition, a_name, a_description) {}

        [[nodiscard]] ConditionComponentType GetType() const override { return CONDITION_COMPONENT_TYPE; }
        [[nodiscard]] RE::BSString GetDefaultDescription() const override { return DEFAULT_DESCRIPTION; }

        [[nodiscard]] virtual float GetNumericValue(RE::TESObjectREFR* a_refr) const = 0;
        virtual void SetStaticValue(float a_value) = 0;
        virtual void SetGlobalVariable(RE::TESGlobal* a_global) = 0;
        virtual void SetActorValue(RE::ActorValue a_actorValue, ActorValueType a_valueType) = 0;
        virtual void SetGraphVariable(const char* a_graphVariableName, GraphVariableType a_valueType) = 0;
    };

    class INiPoint3ConditionComponent : public IConditionComponent
    {
    public:
        inline static constexpr auto CONDITION_COMPONENT_TYPE = ConditionComponentType::kNiPoint3;
        inline static constexpr auto DEFAULT_DESCRIPTION = "A user defined static NiPoint3 value (x, y, z vector)."sv;

        INiPoint3ConditionComponent(const ICondition* a_parentCondition, const char* a_name, const char* a_description = "") :
			IConditionComponent(a_parentCondition, a_name, a_description) {}

        [[nodiscard]] ConditionComponentType GetType() const override { return CONDITION_COMPONENT_TYPE; }
        [[nodiscard]] RE::BSString GetDefaultDescription() const override { return DEFAULT_DESCRIPTION; }

        [[nodiscard]] virtual const RE::NiPoint3& GetNiPoint3Value() const = 0;
        virtual void SetNiPoint3Value(const RE::NiPoint3& a_point) = 0;
    };

    class IKeywordConditionComponent : public IConditionComponent
    {
    public:
        inline static constexpr auto CONDITION_COMPONENT_TYPE = ConditionComponentType::kKeyword;
        inline static constexpr auto DEFAULT_DESCRIPTION = "A keyword value - a reference to a BGSKeyword form, or a literal EditorID of the keyword."sv;

        IKeywordConditionComponent(const ICondition* a_parentCondition, const char* a_name, const char* a_description = "") :
			IConditionComponent(a_parentCondition, a_name, a_description) {}

        [[nodiscard]] ConditionComponentType GetType() const override { return CONDITION_COMPONENT_TYPE; }
        [[nodiscard]] RE::BSString GetDefaultDescription() const override { return DEFAULT_DESCRIPTION; }

        [[nodiscard]] virtual bool HasKeyword(const RE::BGSKeywordForm* a_form) const = 0;
        virtual void SetKeyword(RE::BGSKeyword* a_keyword) = 0;
        virtual void SetLiteral(const char* a_literal) = 0;
    };

    class ITextConditionComponent : public IConditionComponent
    {
    public:
        inline static constexpr auto CONDITION_COMPONENT_TYPE = ConditionComponentType::kText;
        inline static constexpr auto DEFAULT_DESCRIPTION = "A text value."sv;

        ITextConditionComponent(const ICondition* a_parentCondition, const char* a_name, const char* a_description = "") :
			IConditionComponent(a_parentCondition, a_name, a_description) {}

        [[nodiscard]] ConditionComponentType GetType() const override { return CONDITION_COMPONENT_TYPE; }
        [[nodiscard]] RE::BSString GetDefaultDescription() const override { return DEFAULT_DESCRIPTION; }

        [[nodiscard]] virtual RE::BSString GetTextValue() const = 0;
        virtual void SetTextValue(const char* a_text) = 0;
        virtual void SetAllowSpaces(bool a_bAllowSpaces) = 0;
    };

    class IBoolConditionComponent : public IConditionComponent
    {
    public:
        inline static constexpr auto CONDITION_COMPONENT_TYPE = ConditionComponentType::kBool;
        inline static constexpr auto DEFAULT_DESCRIPTION = "A boolean value."sv;

        IBoolConditionComponent(const ICondition* a_parentCondition, const char* a_name, const char* a_description = "") :
			IConditionComponent(a_parentCondition, a_name, a_description) {}

        [[nodiscard]] ConditionComponentType GetType() const override { return CONDITION_COMPONENT_TYPE; }
        [[nodiscard]] RE::BSString GetDefaultDescription() const override { return DEFAULT_DESCRIPTION; }

        [[nodiscard]] virtual bool GetBoolValue() const = 0;
        virtual void SetBoolValue(bool a_value) = 0;
    };

    class IComparisonConditionComponent : public IConditionComponent
    {
    public:
        inline static constexpr auto CONDITION_COMPONENT_TYPE = ConditionComponentType::kComparison;
        inline static constexpr auto DEFAULT_DESCRIPTION = "A comparison operator."sv;

        IComparisonConditionComponent(const ICondition* a_parentCondition, const char* a_name, const char* a_description = "") :
			IConditionComponent(a_parentCondition, a_name, a_description) {}

        [[nodiscard]] ConditionComponentType GetType() const override { return CONDITION_COMPONENT_TYPE; }
        [[nodiscard]] RE::BSString GetDefaultDescription() const override { return DEFAULT_DESCRIPTION; }

        [[nodiscard]] virtual bool GetComparisonResult(float a_valueA, float a_valueB) const = 0;
        [[nodiscard]] virtual ComparisonOperator GetComparisonOperator() const = 0;
        virtual void SetComparisonOperator(ComparisonOperator a_operator) = 0;

        [[nodiscard]] virtual RE::BSString GetComparisonOperatorFullName() const = 0;
    };

    class IRandomConditionComponent : public IConditionComponent
    {
    public:
        inline static constexpr auto CONDITION_COMPONENT_TYPE = ConditionComponentType::kRandom;
        inline static constexpr auto DEFAULT_DESCRIPTION = "A random value."sv;

        IRandomConditionComponent(const ICondition* a_parentCondition, const char* a_name, const char* a_description = "") :
			IConditionComponent(a_parentCondition, a_name, a_description) {}

        [[nodiscard]] ConditionComponentType GetType() const override { return CONDITION_COMPONENT_TYPE; }
        [[nodiscard]] RE::BSString GetDefaultDescription() const override { return DEFAULT_DESCRIPTION; }

        // Returns true if the given clip generator is active. This means that the random float is going to be stable for this clip and won't be randomized again on each evaluation until explicitly reset
        virtual bool GetRandomFloat(RE::hkbClipGenerator* a_clipGenerator, float& a_outFloat) const = 0;
        [[nodiscard]] virtual float GetMinValue() const = 0;
        [[nodiscard]] virtual float GetMaxValue() const = 0;
        virtual void SetMinValue(float a_min) = 0;
        virtual void SetMaxValue(float a_max) = 0;
    };

    class ICustomConditionComponent : public IConditionComponent
    {
    public:
        inline static constexpr auto CONDITION_COMPONENT_TYPE = ConditionComponentType::kCustom;

        ICustomConditionComponent(const ICondition* a_parentCondition, const char* a_name, const char* a_description = "") :
			IConditionComponent(a_parentCondition, a_name, a_description) {}

        [[nodiscard]] ConditionComponentType GetType() const override { return CONDITION_COMPONENT_TYPE; }
    };

    // use this class as your parent class for a custom condition
    // this one has a wrapped condition that will be internally used for serialization and internal things like that, which don't concern you
    // you just need to implement the few other functions
    // this approach should save a lot of headaches and should be safe across DLL boundaries with different compilers
    // you can also not use this class and implement ICondition yourself, but then you need to reimplement all the serialization stuff yourself while keeping it compatible with Open Animation Replacer, which could potentially have a breaking change in the future
    // I don't think there's a reason not to use this class as a parent class for your conditions
    // check the example plugin (https://github.com/ersh1/OpenAnimationReplacer-ExamplePlugin) for simple examples and a much more complicated one in the math plugin (https://github.com/ersh1/OpenAnimationReplacer-Math)
    class CustomCondition : public ICondition
    {
    public:
        CustomCondition();

        bool Evaluate(RE::TESObjectREFR* a_refr, RE::hkbClipGenerator* a_clipGenerator) const override;

        void Initialize(void* a_value) override { return _wrappedCondition->Initialize(a_value); }
        void Serialize(void* a_value, void* a_allocator, [[maybe_unused]] ICondition* a_outerCondition) override { _wrappedCondition->Serialize(a_value, a_allocator, this); }

        void PreInitialize() override { _wrappedCondition->PostInitialize(); }
        void PostInitialize() override { _wrappedCondition->PostInitialize(); }

        [[nodiscard]] RE::BSString GetArgument() const override { return _wrappedCondition->GetArgument(); }
        [[nodiscard]] RE::BSString GetCurrent(RE::TESObjectREFR* a_refr) const override { return _wrappedCondition->GetCurrent(a_refr); }

        [[nodiscard]] RE::BSString GetRequiredPluginName() const override { return SKSE::PluginDeclaration::GetSingleton()->GetName().data(); }
        [[nodiscard]] RE::BSString GetRequiredPluginAuthor() const override { return SKSE::PluginDeclaration::GetSingleton()->GetAuthor().data(); }

        [[nodiscard]] bool IsDisabled() const override { return _wrappedCondition->IsDisabled(); }
        void SetDisabled(bool a_bDisabled) override { _wrappedCondition->SetDisabled(a_bDisabled); }
        [[nodiscard]] bool IsNegated() const override { return _wrappedCondition->IsNegated(); }
        void SetNegated(bool a_bNegated) override { _wrappedCondition->SetNegated(a_bNegated); }

        [[nodiscard]] uint32_t GetNumComponents() const override { return _wrappedCondition->GetNumComponents(); }
        [[nodiscard]] IConditionComponent* GetComponent(uint32_t a_index) const override { return _wrappedCondition->GetComponent(a_index); }
        IConditionComponent* AddComponent(ConditionComponentFactory a_factory, const char* a_name, const char* a_description = "") override { return _wrappedCondition->AddComponent(a_factory, a_name, a_description); }

        [[nodiscard]] bool IsCustomCondition() const override { return true; }
        [[nodiscard]] ICondition* GetWrappedCondition() const override { return _wrappedCondition.get(); }

        IConditionComponent* AddBaseComponent(ConditionComponentType a_componentType, const char* a_name, const char* a_description = "");

        template <typename T>
        static ICondition* CreateCondition()
        {
            return new T();
        }

        template <typename T>
        static ConditionFactory GetFactory()
        {
            return &CreateCondition<T>;
        }

    protected:
        std::unique_ptr<ICondition> _wrappedCondition = nullptr;
    };
}
