/**
 * \file retqss_object.hh
 */

#ifndef _RETQSS_OBJECT_H_
#define _RETQSS_OBJECT_H_

#include <mutex>
#include <string>

#include "retqss_cgal_main_types.hh"
#include "retqss_types.hh"
#include "retqss_object_properties.hh"
#include "retqss_exceptions.hh"


namespace retQSS
{

/**
 * \brief Implementation of retQSS objects
 *
 * This class provides an implementation of the common behavior underlying
 * retQSS objects, mainly related to property management (see
 * retQSS::ObjectProperties). In order to ensure thread-safety (for parallel
 * simulations), a mutex protects concurrent access to the internal instance
 * that handles property queries.
 */
class Object
{
protected:
	ObjectID id;
	std::string name;

	retQSS::ObjectProperties props; //!< This object manages property access
	                                //!< and definition.

	std::mutex props_mutex; //!< Mutex to protect concurrent access to
	                        //!< properties (only meaningful for parallel
	                        //!< simulations).

public:
	/**
	 * \brief Object default constructor
	 *
	 * Constructs a retQSS object with the given ID.
	 *
	 * \note Default name is 'retQSS object'.
	 */
	Object(ObjectID id) : id(id), name("retQSS object") {};

	/**
	 * \brief Object constructor
	 *
	 * Constructs a retQSS object with the given ID and name.
	 */
	Object(ObjectID id, const std::string &name) :
		id(id),
		name(name) {};

	virtual ~Object() {};

	/**
	 * \brief Retrieve object name
	 *
	 * Retrieves the name of this retQSS object.
	 */
	inline const std::string &get_name() const
	{
		return this->name;
	}

	/**
	 * \brief Retrieve object ID
	 *
	 * Retrieves the ID assigned to this retQSS object.
	 *
	 * \note ID may not be unique among objects of different types.
	 */
	inline ObjectID get_ID() const
	{
		return this->id;
	}

	/**
	 * \brief Check if a property is defined
	 *
	 * Check if a given property is defined in this object.
	 */
	inline bool has_property(const std::string &tag)
	{
		retQSS::Lock lock(this->props_mutex);
		return this->props.has(tag);
	}

	/**
	 * \brief Retrieve value associated with a property tag
	 *
	 * Retrieves the value associated with a given property tag. In case a
	 * property has a sequence of values assigned, the first one is returned.
	 */
	inline double get_property(const std::string &tag)
	{
		retQSS::Lock lock(this->props_mutex);

		if(!this->props.has(tag))
			throw retQSS::UndefinedPropertyException(this, tag);

		return this->props.get(tag);
	}

	/**
	 * \brief Retrieve sequence of values of a property
	 *
	 * Retrieves the full sequence of values associated with a given property
	 * tag. In case a single value is associated (as in fact happens with usual
	 * properties), an array of length 1 is returned.
	 */
	inline const std::vector<double> &get_property_array(const std::string &tag)
	{
		retQSS::Lock lock(this->props_mutex);

		if(!this->props.has(tag))
			throw retQSS::UndefinedPropertyException(this, tag);

		return this->props.get_array(tag);
	}

	/**
	 * \brief Set object property.
	 *
	 * Sets or updates a property for this retQSS object.
	 */
	inline void set_property(const std::string &tag, double value)
	{
		retQSS::Lock lock(this->props_mutex);
		this->props.set(tag, value);
	}

	/**
	 * \brief Set object property.
	 *
	 * Sets or updates a property for this retQSS object.
	 */
	inline void set_property(
			const std::string &tag,
			const std::vector<double> &values)
	{
		retQSS::Lock lock(this->props_mutex);
		this->props.set(tag, values);
	}

	/**
	 * \brief Set object property.
	 *
	 * Sets or updates a property for this retQSS object.
	 */
	inline void set_property(const std::string &tag, const Vector_3 &vector)
	{
		retQSS::Lock lock(this->props_mutex);
		this->props.set(tag, vector);
	}

	/**
	 * \brief Set object property.
	 *
	 * Sets or updates a property for this retQSS object.
	 */
	inline void set_property(const std::string &tag, const Point_3 &point)
	{
		retQSS::Lock lock(this->props_mutex);
		this->props.set(tag, point);
	}

	/**
	 * \brief Append new value to sequence of property values
	 *
	 * Append a new value to the end of the sequence of values associated with
	 * the given property tag.
	 *
	 * \pre Property tag is defined.
	 */
	inline void property_append(const std::string &tag, double value)
	{
		retQSS::Lock lock(this->props_mutex);
		this->props.append(tag, value);
	}

	/**
	 * \brief Remove value from sequence of property values
	 *
	 * Remove all occurrences of the given value from the sequence of values
	 * associated with the given property tag.
	 *
	 * \pre Property tag is defined.
	 */
	inline void property_remove(const std::string &tag, double value)
	{
		retQSS::Lock lock(this->props_mutex);
		this->props.remove(tag, value);
	}
};

}

#endif
