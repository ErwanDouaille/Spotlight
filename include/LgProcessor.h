#pragma once

/*!
 * \file LgProcessor.h
 * \brief File containing the Processor class, the mother class of any Node that need to modify the Groups.
 * \author Bremard Nicolas
 * \version 0.2
 * \date 02 july 2014
 */

#ifndef LGPROCESSOR_H_
#define LGPROCESSOR_H_

/*!
* \brief Type of Node for Processors
*/
#define LG_NODE_PROCESSOR "LG_NODE_PROCESSOR"

#include "LgNode.h"

/*! 
 * \namespace lg
 * \brief Namespace containing everything for managing libGina Environment, Node and Groups.
*/
namespace lg
{
	/*!
	* \brief Mother class of any Node that need to modify the Groups.
	* A Processor is a Node that modify the Groups or that generate data from them.
	* These can be, by example, some kind of filters or pointing (generating 2D data from 3D data).
	*/
	class Processor : public Node {
		public:

		/*!
		 * \brief Constructor for creating a new Processor
		 * Create a new Processor with a name and a type
		 * \param[in] name : a name for the instance of this Node
		 * \param[in] name : the type of this Processor
		 */
		Processor(string name) : Node(name,LG_NODE_PROCESSOR){}
		
		/*!
		 * \brief Constructor by copy
		 * \param[in] copy : another instance of this Node from which copy the details (name, id...)
		 */
		Processor(const Processor &copy) : Node(copy) {}
		
		/*!
		 * \brief Destructor
		 */
		virtual ~Processor(void){}
	};
}

#endif /* LGPROCESSOR_H_ */