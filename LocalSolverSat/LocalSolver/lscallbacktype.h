// Copyright (C) 2016 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_CALLBACKTYPE_H
#define LS_CALLBACKTYPE_H

namespace localsolver {
    /**
     * List of types that can be used with callbacks.
     * Each type corresponds to a specific event that can occur during the resolution.
     * 
     * @since 4.0
     */
    enum LSCallbackType {
        /**
         * Event that occurs when a phase is started.
         */
        CT_PhaseStarted,

        /**
         * Event that occurs when a phase ends.
         */
        CT_PhaseEnded,
        
        /**
         * Event that occurs regularly before, after and during the search
         * to display useful information about the model and the resolution.
         * The time between two such events can be tuned with the
         * timeBetweenDisplays parameter.
         * @see LSParam#setTimeBetweenDisplays(int)
         */
        CT_Display,
        
        /**
         * Event that occurs regularly during the resolution. 
         * The time between two such events can be tuned with the 
         * timeBetweenTicks parameter.
         * @see LSParam#setTimeBetweenTicks(int)
         * @since 6.0
         */
        CT_TimeTicked,
        
        /**
         * Event that occurs regularly during the resolution.
         * The number of iterations between two such events can be tuned
         * with the iterationsBetweenTicked parameter.
         * @see LSParam#setIterationBetweenTicks(long long)
         * @since 6.0
         */
        CT_IterationTicked
    };
}

#endif
