#ifndef PLANNING_VALUE_TYPES_H
#define PLANNING_VALUE_TYPES_H

namespace goap
{
    /* Possible types for ValueType */
    enum class Type
    {
        NONE,
        INT,
        FLOAT,
        BOOL
    };

    /* The value union for ValueType */
    union Value
    {
        int iValue;
        bool bValue;
        float fValue;
    };

    /* Base class for ValueType */
    class ValueType
    {
    public:

        ValueType()
        {
            mValueType = Type::NONE;
        }

        /* Virtual operator== so extra logic for comparing the types can be avoided */
        virtual bool operator==(const ValueType& other) const
        {
            return false;
        }

        /* Accessor for getType */
        Type getType() const
        {
            return mValueType;
        }

        /* Accessor for value */
        Value getValue() const
        {
            return mValue;
        }

    protected:

        /* The type of this value */
        Type mValueType;

        /* This value */
        Value mValue;

    };

    /* Int interface for ValueType */
    class IntValue
        : public ValueType
    {
    public:

        /* Constructs an int ValueType */
        IntValue(int value)
        {
            mValueType = Type::INT;
            mValue.iValue = value;
        }

        /* Compares int ValueTypes, returns false for comparisons with other ValueTypes */
        bool operator==(const ValueType& other) const override
        {
            return other.getType() == mValueType && other.getValue().iValue == mValue.iValue;
        }

    };

    /* Float interface for ValueType */
    class FloatValue
        : public ValueType
    {
    public:

        /* Constructs a float ValueType */
        FloatValue(float value)
        {
            mValueType = Type::FLOAT;
            mValue.fValue = value;
        }

        /* Compares float ValueTypes, returns false for comparisons with other ValueTypes */
        bool operator==(const ValueType& other) const override
        {
            return other.getType() == mValueType && other.getValue().fValue == mValue.fValue;
        }

    };

    /* Bool interface for ValueType */
    class BoolValue
        : public ValueType
    {
    public:

        /* Constructs a bool ValueType */
        BoolValue(bool value)
        {
            mValueType = Type::BOOL;
            mValue.bValue = value;
        }

        /* Compares bool ValueTypes, returns false for comparisons with other ValueTypes */
        bool operator==(const ValueType& other) const override
        {
            return other.getType() == mValueType && other.getValue().bValue == mValue.bValue;
        }

    };
}

#endif /* PLANNING_VALUE_TYPES_H */