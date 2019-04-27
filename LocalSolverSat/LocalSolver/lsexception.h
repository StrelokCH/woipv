// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_EXCEPTION_H
#define LS_EXCEPTION_H

#include "lserrorcode.h"
#include <exception>
#include <string>
#include <sstream>

namespace localsolver {

    /**
     * LocalSolver exception. If an error occurred, LocalSolver throws 
     * a specific exception which contains useful info for debugging purposes. 
     */
    class LSException : public std::exception {
    public:

        /** 
         * Constructor. 
         * 
         * @param errorCode The code associated to this exception.
         * @param fileName Name of the %LocalSolver source file where the exception is thrown.
         * @param lineNumber Line number in the %LocalSolver source file where the exception is thrown.
         * @param functionName Name of the function where the exception is thrown.
         * @param explanation An explanation about the exception.
         */
        LSException(LSErrorCode errorCode, const std::string& fileName, int lineNumber,
                const std::string& functionName, const std::string& explanation) {
            this->errorCode = errorCode;
            this->fileName = fileName;
            this->lineNumber = lineNumber;
            this->functionName = functionName;
            this->explanation = explanation;
            
            std::stringstream s;
            s << "ERROR [function " << functionName << "]: " << explanation;
            this->message = s.str();
        }

        /** 
         * Destructor. 
         */
        virtual ~LSException() throw () {}

        /** 
         * Gets the name of the %LocalSolver source file where the exception is thrown.
         * Only useful for %LocalSolver's maintenance team.
         * 
         * @return File name.
         */
        const std::string& getFileName() const {
            return fileName;
        }

        /** 
         * Gets the line number in the %LocalSolver source file where the exception is thrown.
         * Only useful for %LocalSolver's maintenance team.
         * 
         * @return Line number.
         */
        int getLineNumber() const {
            return lineNumber;
        }

        /** 
         * Gets the name of the function where the exception is thrown.
         * 
         * @return Function name.
         */
        const std::string& getFunctionName() const {
            return functionName;
        }

        /** 
         * Gets the explanation about the exception.
         * 
         * @return Explanation.
         */
        const std::string& getExplanation() const {
            return explanation;
        }

        /** 
         * Returns a string message with all useful info about the current exception.
         * 
         * @return Exception message.
         */
        const std::string& getMessage() const {
            return message;
        }
        
        /**
         * Returns the error code associated to this exception.
         * 
         * @return Error code.
         */
        LSErrorCode getErrorCode() const {
            return errorCode;
        }

        /** 
         * Returns a C-style string explanation about the current exception.
         *
         * @return Exception message.
         */
        const char* what() const throw () {
            return explanation.c_str();
        }

        /** 
         * Same as getMessage().
         */
        const std::string& toString() const {
            return message;
        }

    private:

        LSErrorCode errorCode;
        std::string fileName;
        std::string functionName;
        std::string explanation;
        std::string message;
        int lineNumber;
    };
}

#endif
