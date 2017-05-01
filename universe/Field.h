#ifndef _Field_h_
#define _Field_h_

#include "UniverseObject.h"

#include "../util/Export.h"

namespace Effect {
    class EffectsGroup;
}

/** a class representing a region of space */
class FO_COMMON_API Field : public UniverseObject {
public:
    /** \name Accessors */ //@{
    std::set<std::string> Tags() const override;

    bool HasTag(const std::string& name) const override;

    UniverseObjectType ObjectType() const override;

    std::string Dump() const override;

    int ContainerObjectID() const override;

    bool ContainedBy(int object_id) const override;

    std::shared_ptr<UniverseObject> Accept(const UniverseObjectVisitor& visitor) const override;

    const std::string& PublicName(int empire_id) const override;

    void ClampMeters() override;

    const std::string&          FieldTypeName() const { return m_type_name; }

    bool InField(std::shared_ptr<const UniverseObject> obj) const;

    bool                        InField(double x, double y) const;
    //@}

    /** \name Mutators */ //@{
    void Copy(std::shared_ptr<const UniverseObject> copied_object, int empire_id = ALL_EMPIRES) override;

    void ResetTargetMaxUnpairedMeters() override;
    //@}

protected:
    friend class Universe;
    /** \name Structors */ //@{
    Field();

    Field(const std::string& field_type, double x, double y, double radius);

    template <typename T> friend void UniverseObjectDeleter(T*);
    template <class T> friend void boost::python::detail::value_destroyer<false>::execute(T const volatile* p);

public:
    ~Field();

protected:
    /** Returns new copy of this Field. */
    Field* Clone(int empire_id = ALL_EMPIRES) const override;
    //@}

private:
    std::string     m_type_name;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version);
};


/** A specification for a type of field. */
class FO_COMMON_API FieldType {
public:
    /** \name Structors */ //@{
    FieldType(const std::string& name, const std::string& description,
              float stealth, const std::set<std::string>& tags,
              const std::vector<std::shared_ptr<Effect::EffectsGroup>>& effects,
              const std::string& graphic);
    ~FieldType();
    //@}

    /** \name Accessors */ //@{
    const std::string&              Name() const            { return m_name; }          ///< returns the unique name for this type of field
    const std::string&              Description() const     { return m_description; }   ///< returns a text description of this type of building
    std::string                     Dump() const;                                       ///< returns a data file format representation of this object
    float                           Stealth() const         { return m_stealth; }       ///< returns stealth of field type
    const std::set<std::string>&    Tags() const            { return m_tags; }

    /** Returns the EffectsGroups that encapsulate the effects of thisi
        FieldType. */
    const std::vector<std::shared_ptr<Effect::EffectsGroup>>& Effects() const
    { return m_effects; }

    const std::string&              Graphic() const         { return m_graphic; }       ///< returns the name of the grapic file for this field type

    /** Returns a number, calculated from the contained data, which should be
      * different for different contained data, and must be the same for
      * the same contained data, and must be the same on different platforms
      * and executions of the program and the function. Useful to verify that
      * the parsed content is consistent without sending it all between
      * clients and server. */
    unsigned int                    GetCheckSum() const;
    //@}

private:
    std::string                                         m_name;
    std::string                                         m_description;
    float                                               m_stealth;
    std::set<std::string>                               m_tags;
    std::vector<std::shared_ptr<Effect::EffectsGroup>>  m_effects;
    std::string                                         m_graphic;
};


class FieldTypeManager {
public:
    typedef std::map<std::string, FieldType*>::const_iterator iterator;

    /** \name Accessors */ //@{
    /** returns the field type with the name \a name; you should use the
      * free function GetFieldType(...) instead, mainly to save some typing. */
    const FieldType*            GetFieldType(const std::string& name) const;

    /** iterator to the first field type */
    iterator                    begin() const   { return m_field_types.begin(); }

    /** iterator to the last + 1th field type */
    iterator                    end() const     { return m_field_types.end(); }

    /** returns the instance of this singleton class; you should use the free
      * function GetFieldTypeManager() instead */
    static FieldTypeManager&    GetFieldTypeManager();

    /** Returns a number, calculated from the contained data, which should be
      * different for different contained data, and must be the same for
      * the same contained data, and must be the same on different platforms
      * and executions of the program and the function. Useful to verify that
      * the parsed content is consistent without sending it all between
      * clients and server. */
    unsigned int                GetCheckSum() const;
    //@}
private:
    FieldTypeManager();
    ~FieldTypeManager();
    std::map<std::string, FieldType*>   m_field_types;
    static FieldTypeManager*            s_instance;
};

/** returns the singleton field type manager */
FO_COMMON_API FieldTypeManager& GetFieldTypeManager();

/** Returns the BuildingType specification object for a field of
  * type \a name.  If no such FieldType exists, 0 is returned instead. */
FO_COMMON_API const FieldType* GetFieldType(const std::string& name);


#endif // _Ship_h_
