/*
 * Core.hpp
 *
 *  Created on: Jun 17, 2016
 *      Author: kjaron
 */

#ifndef INCLUDE_CORE_H_
#define INCLUDE_CORE_H_

class Core {
	public:
		Core(SimulationSetting simulation_setting);
		virtual ~Core();

	private:
		World world;
};

#endif /* INCLUDE_CORE_H_ */
