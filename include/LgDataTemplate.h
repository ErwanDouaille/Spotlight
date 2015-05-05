#pragma once

/*!
 * \file LgDataTemplate.h
 * \brief File containing the template class for creating OrientedPoint 3D,2D and 1D
 * \author Bremard Nicolas
 * \version 0.2
 * \date 02 july 2014
 */

#ifndef LGDATATEMPLATE_H_
#define LGDATATEMPLATE_H_

#include <string>

#include "LgMetaData.h"

using namespace std;

/*! 
 * \namespace lg
 * \brief Namespace containing everything for managing libGina Environment, Node and Groups.
*/
namespace lg
{
	/*!
	 * \brief Template class for creating Orientedpoint
	 * An OrientedPoint has a position, an orientation, a confidence, an intensity and some MetaData
	 * The MetaData are added by extending the MetaData class.
	 * Position and Orientation differs of type depending if they are 3D,2D or 1D
	*/
	template <typename P,typename O> class DataTemplate : public MetaData
	{
	protected:
		//! Position 
		P _position;

		//! Orientation
		O _orientation;

		//! Confidence (between 0.0 and 1.0)
		float _confidence;

		//! Intensity
		float _intensity;
	public:
		/*!
		 * \brief Default constructor
		 */
		DataTemplate() : MetaData() {}

		/*!
		 * \brief Constructor by copy, copy all the stored data from another DataTemplate<P,O> class instance
		 * \param[in] copy : another DataTemplate<P,O> from which copy the values
		 */
		DataTemplate(const DataTemplate<P,O> &copy) : MetaData(copy) 
		{
			_position = copy.getPosition();
			_orientation = copy.getOrientation();
			_confidence = copy.getConfidence();
			_intensity = copy.getIntensity();
		}

		/*!
		 * \brief Constructor with an initial value
		 * \param[in] position : its position
		 * \param[in] orientation : an orientation (degrees or radians)
		 * \param[in] confidence : its confidence (between 0.0 and 1.0)
		 * \param[in] intensity : its intensity
		 */
		DataTemplate(P position, O orientation, float confidence, float intensity)
		{
			_position = position;
			_orientation = orientation;
			_confidence = confidence;
			_intensity = intensity;
		}

		/*!
		 * \brief Destructor
		 */
		virtual ~DataTemplate(void) {}

		/*!
		 * \brief Get the position of this DataTemplate
		 * \return The position of this DataTemplate
		 */
		inline P getPosition() const {return _position;}

		/*!
		 * \brief Get the orientation of this DataTemplate
		 * \return The orientation of this DataTemplate
		 */
		inline O getOrientation() const {return _orientation;}

		/*!
		 * \brief Get the confidence of this DataTemplate
		 * \return The confidence of this DataTemplate
		 */
		inline float getConfidence() const {return _confidence;}

		/*!
		 * \brief Get the intensity of this DataTemplate
		 * \return The intensity of this DataTemplate
		 */
		inline float getIntensity() const {return _intensity;}

		/*!
		 * \brief Indicates if the data has values
		 * Check if all values are still set to their default values.
		 * \return true if all attributes of the DataTemplate are set to their default values, false otherwise
		 */
		bool isNull(){return ((_position == P())&&(_orientation == O())&&(_confidence==0.0)&&(_intensity==0.0));}
	};

}

#endif /* LGDATATEMPLATE_H_ */