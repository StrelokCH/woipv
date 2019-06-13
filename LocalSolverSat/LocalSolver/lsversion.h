// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#ifndef LS_LSVERSION_H
#define LS_LSVERSION_H

#include "entrypoint.h"
#include <string>
#include <sstream>

namespace localsolver {

    /**
     * Version, license and copyright info.
     */
    class LSVersion {
    public:

        /** 
         * Returns %LocalSolver's major version number. 
         * This number is an integer in the range [0, 2999]. 
         * 
         * @return Major version number.
         */
        static int getMajorVersionNumber() {
            return ls_major_version();
        }

        /** 
         * Returns %LocalSolver's minor version number. 
         * This number is an integer in the range [0, 9].
         * 
         * @return Minor version number.
         */
        static int getMinorVersionNumber() {
            return ls_minor_version();
        }

        /** 
         * Returns %LocalSolver's build date. This number is an integer YYYYMMDD 
         * with YYYY the year, MM the month, DD the day. 
         * 
         * @return Build date.
         */
        static int getBuildDate() {
            return ls_build_date();
        }
        
        /**
         * Returns the platform of the %LocalSolver's binary as a string.
         * The format of the string is PPPPXX where PPPP is the platform (Win, Linux or MacOS)
         * and XX is the architecture (32 or 64 bits).
         * 
         * @return Platform as string.
         * @since 3.0
         */
        static std::string getPlatform() {
            int size = ls_platform(NULL, 0);
            std::string str(size, '\0');
            ls_platform(&str[0], size);
            return str;
        }

        /** 
         * Returns %LocalSolver's version as a string. The format of the string is: MA.MI.YYYYMMDD-PPPPXX with 
         * MA the major number version, MI the minor number version, YYYY the year, MM the month, DD the day,
         * PPPP the platform (Win, Linux or MacOS) and XX the architecture.
         * 
         * @return Version as string "MA.MI.YYYYMMDD-PPPPXX".
         */
        static std::string getVersion() {
            int size = ls_version(NULL, 0);
            std::string str(size, '\0');
            ls_version(&str[0], size);
            return str;
        }

        /** 
         * Returns %LocalSolver's version code as an integer in the range [0, 29999].
         * The version code is computed as follows: MA * 10 + MI with MA the major number version, 
         * MI the minor number version. Note that the version code can be encoded as a 16-bit integer.
         * 
         * @return Version code.
         */
        static int getVersionCode() {
            return ls_version_code();
        }
        
        /** 
         * Returns %LocalSolver's copyright as a string. 
         *
         * @return Copyright notice. 
         */
        static std::string getCopyright() {
            int size = ls_copyright(NULL, 0);
            std::string str(size, '\0');
            ls_copyright(&str[0], size);
            return str;
        }

        /** 
         * Returns version and copyright info as a string.
         * 
         * @return Version and copyright info. 
         */
        static std::string getInfo() {
            int size = ls_info(NULL, 0);
            std::string str(size, '\0');
            ls_info(&str[0], size);
            return str;
        }
        
        /**
         * Returns the license path of %LocalSolver.
         * By default, LocalSolver looks for a license file in the following locations,
         * in that order:
         * -# A file specified by a call to {@link #setLicensePath}.
         * -# The LS_LICENSE_PATH environment variable.
         * -# A license.dat file in the current directory.
         * -# In the default location (c:\\localsolver_X_Y\\license.dat on Windows,
         * /opt/localsolver_X_Y/license.dat on Linux and Mac OS).
         * 
         * **For windows users**: If you used the installer to setup LocalSolver, the
         * LS_LICENSE_PATH environment variable is already defined and points to the
         * license file present in the installation folder.
         * 
         * @return Path to the license
         * @see #setLicensePath
         * @see #getLicenseContent
         * @see #setLicenseContent
         * @since 3.0
         */
        static std::string getLicensePath() {
            int size = ls_license_path(NULL, 0);
            std::string str(size, '\0');
            ls_license_path(&str[0], size);
            return str;
        }
        
        /**
         * Change the license path used by %LocalSolver.
         * 
         * @param licensePath new path for the license
         * @see #getLicensePath
         * @see #setLicenseContent
         * @see #getLicenseContent
         * @since 3.0
         */
        static void setLicensePath(const std::string& licensePath) {
            ls_set_license_path(licensePath.c_str());
        }
        
        
        /**
         * Returns the license content previously set with {@link #setLicenseContent}.
         * 
         * @see #setLicenseContent
         * @see #getLicensePath
         * @see #setLicensePath
         * @since 5.0
         * @return The content of the license or an empty string if the content was not set.
         */
        static std::string getLicenseContent() {
            int size = ls_license_content(NULL, 0);
            std::string str(size, '\0');
            ls_license_content(&str[0], size);
            return str;
        }
        
        /**
         * Change the license content. By default, %LocalSolver tries to get the license
         * from the path specified in setLicensePath or in a default location.
         * Use this function to set the license content directly, without using a file.
         * 
         * @param licenseContent content of the license to use.
         * @see #getLicenseContent
         * @see #setLicensePath
         * @see #getLicensePath
         * @since 5.0
         */
        static void setLicenseContent(const std::string& licenseContent) {
            ls_set_license_content(licenseContent.c_str());
        }
        
    private:
        LSVersion();
    };
}

#endif
