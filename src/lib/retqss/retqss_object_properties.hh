/**
 * \file retqss_object_properties.hh
 * \brief Object properties
 */

#ifndef _RETQSS_OBJECT_PROPERTIES_H_
#define _RETQSS_OBJECT_PROPERTIES_H_

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

#include "retqss_types.hh"
#include "retqss_cgal_main_types.hh"


namespace retQSS
{

/**
 * \brief Properties of retQSS objects
 *
 * This class provides support for property management in retQSS objects. A
 * \em property is a quantity describing a relevant attribute of the object
 * at hand. When defining a property, a value is associated to a tag through
 * which the property can be later referenced. Every retQSS object contains
 * an instance of this class, where properties are stored and queried
 * throughout the simulation.
 */
class ObjectProperties
{
private:
	/**
	 * \brief Property lookup table
	 *
	 * A property tag is associated with a sequence of values. However, most
	 * properties usually demand only one such value. In those cases, the
	 * associated sequence will only contain one element.
	 */
	std::unordered_map<
		std::string,
		std::vector<double>> props;

public:
	ObjectProperties() {};

	~ObjectProperties() {};

	/**
	 * \brief Check if property is defined.
	 */
	inline bool has(const std::string &tag)
	{
		return this->props.find(tag) != this->props.end();
	}

	/**
	 * \brief Retrieve sequence of values for a property tag
	 *
	 * Retrieve sequence of values associated with a property tag.
	 *
	 * \pre Property tag is defined.
	 */
	inline std::vector<double> &get_array(const std::string &tag)
	{
		assert(this->has(tag));
		return this->props[tag];
	}

	/**
	 * \brief Retrieve value associated with a property tag
	 *
	 * Retrieve first value of the sequence associated with the given tag.
	 *
	 * \pre Property tag is defined.
	 */
	inline double get(const std::string &tag)
	{
		return this->get_array(tag)[0];
	}

	/**
	 * \brief Set value for a property tag
	 *
	 * Defines a new property or overrides its value in case it is already
	 * defined.
	 */
	inline void set(const std::string &tag, double value)
	{
		this->props[tag] = {value};
	}

	/**
	 * \brief Set value for a property tag
	 *
	 * Defines a new property or overrides its value in case it is already
	 * defined.
	 */
	inline void set(const std::string &tag, const std::vector<double> &values)
	{
		this->props[tag] = values;
	}

	/**
	 * \brief Set value for a property tag
	 *
	 * Defines a new property or overrides its value in case it is already
	 * defined.
	 */
	inline void set(const std::string &tag, const Vector_3 &vector)
	{
		std::vector<double> values = {vector.x(), vector.y(), vector.z()};
		this->props[tag] = values;
	}

	/**
	 * \brief Set value for a property tag
	 *
	 * Defines a new property or overrides its value in case it is already
	 * defined.
	 */
	inline void set(const std::string &tag, const Point_3 &point)
	{
		std::vector<double> values = {point.x(), point.y(), point.z()};
		this->props[tag] = values;
	}

	/**
	 * \brief Append new value to sequence of property values
	 *
	 * Append a new value to the end of the sequence of values associated with
	 * the given property tag.
	 *
	 * \pre Property tag is defined.
	 */
	inline void append(const std::string &tag, const double &value)
	{
		this->get_array(tag).push_back(value);
	}

	/**
	 * \brief Remove value from sequence of property values
	 *
	 * Remove all occurrences of the given value from the sequence of values
	 * associated with the given property tag.
	 *
	 * \pre Property tag is defined.
	 */
	inline void remove(const std::string &tag, const double &value)
	{
		auto &array = this->get_array(tag);
		array.erase(
				std::remove_if(
						array.begin(), array.end(),
						[&value](const double &v) { return v == value; }),
				array.end());
	}
};

}

#endif
