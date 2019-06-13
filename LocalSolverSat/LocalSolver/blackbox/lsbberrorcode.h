// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_BLACKBOXERRORCODE_H
#define LS_BLACKBOXERRORCODE_H

namespace localsolverblackbox {

    /**
     * List of error codes used by LSBBException. 
     */
    enum LSBBErrorCode {
        /**
         * Code used for errors related to API functions. 
         * Used when you call a function in a wrong way or with inappropriate parameters.
         * 
         * Examples of error messages:
         * <ul>
         *   <li>Argument 'argName' cannot be null.</li>
         *   <li>Argument 'argName' does not belong to this instance of LocalSolverBlackBox.</li>
         *   <li>Argument 'argName' must be positive.</li>
         *   <li>Argument 'argName' cannot be an empty string.</li>
         *   <li>This method is only allowed in state Modeling (the model must not be closed).</li>
         * </ul>
         */
        EC_BB_Api,

        /**
         * Code used when an error related to input/output operations occurs.
         * 
         * Examples of error messages:
         * <ul>
         *   <li>File doesn't exist.</li>
         *   <li>File is corrupted.</li>
         *   <li>Cannot open file.</li>
         *   <li>File format is not recognized.</li>
         * </ul>
         */
        EC_BB_File,


        /**
         * Code used when a problem related to the structure of the model occurs.
         * 
         * Examples of error messages:
         * <ul>
         *   <li>At least one objective is required in the model.</li>
         *   <li>A cycle of length 'n' was detected in the model.</li>
         *   <li>Number of operands ('n') is too small: at least 'm' operands are expected for 'operator'.</li>
         *   <li>Operand 'n' of 'operator' must be 'type'. Type provided: 'otherType'.</li>
         * </ul>
         */
        EC_BB_Model,

        /** 
         * Code used when an error is encountered in a user callback.
         */
        EC_BB_Callback,

        /**
         * Code used when a problem related to licensing occurs.
         * That could be a problem with the license itself (expiration, hardware signature, ...),
         * or a problem related to input/output or networking operations.
         * 
         * Examples of error messages:
         * <ul>
         *   <li>Incorrect license number.</li>
         *   <li>This license key is not compatible with the current hardware. Please contact your reseller.</li>
         *   <li>Property <i>prop</i> is missing in file [<i>file</i>]</li>
         *   <li>Fail to contact the token server. Check your connection.</li>
         *   <li>This license is not compatible with the current hardware.</li>
         *   <li>No token available. All tokens are currently in use.</li>
         * </ul>
         */
        EC_BB_License,

        /**
         * Code used when a problem occurs during the resolution such a division by zero
         * or an index out of bounds. 
         * 
         * Keep in mind that, during the search variables are likely to take values that
         * do not satisfy the constraints (for instance in the feasibility stage).
         * Consequently when an division by zero or array overflow occurs in your model,
         * it probably means that the denominator of a modulo or the index of a array
         * can take invalid values. You can fix this using min and max expressions for instance:
         * <code>z &lt;- x % y</code> can be replaced by <code>z&lt;-x % max(1,y)</code>.
         * 
         * Examples of error messages:
         * <ul>
         *   <li>Division by zero.</li>
         *   <li>Index out of bounds for 'at' operator (index: 'indexId', array size: 'n')</li>
         * </ul>
         */
        EC_BB_Solver,


        /**
         * Internal LocalSolverBlackBox error.
         */
        EC_BB_Internal,
        
        
        /**
         * Code used when an error is encountered in the modeler.
         */
        EC_BB_Modeler
    };
}

#endif
